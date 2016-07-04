#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // Инициализация UI элементов
    gameField = new QFrame(this);
    gameField->setFrameStyle(QFrame::Box);
    gameField->setLineWidth(1);
    width = new QTextEdit(this);
    height = new QTextEdit(this);
    newGameButton = new QPushButton("New Game", this);
    message = new QLabel(this);

    // Начальная отрисовка окна с игровым полем 5x5
    drawWindow(5, 5);

    // Соединение сигнала нажатия кнопки "New Game" с функцией обработки этой кнопки handleNewGameButton()
    connect(newGameButton, SIGNAL(clicked()), this, SLOT (handleNewGameButton()));
    // Инициализация объекта класса FiveRow
    fr = new FiveRow();
}

void MainWindow::drawWindow(int horizontalCellNumber, int verticalCellNumber)
{
    // Размер игровой ячейки в пикселях
    const int cellSize = 20;
    // Размеры игрового поля
    int horizontalFieldSize = cellSize*horizontalCellNumber;
    int verticalFieldSize = cellSize*verticalCellNumber;

    // Очистка игрового поля и массива значений игровых ячеек при переотрисовке игрового окна
    if (cellList.length() != 0) {
        for (int i=0; i<cellList.length(); ++i) {
            delete cellList.at(i);
        }
        cellList.clear();
        for (int i=0; i<cellList.length()/verticalCellNumber; ++i) {
            delete [] cellValue[i];
        }
        delete [] cellValue;
    }

    // Выставление начальной координаты и размеров игрового поля
    gameField->setGeometry(QRect(QPoint(10, 20), QSize(horizontalFieldSize+1, verticalFieldSize+1)));
    if (!gameField->isEnabled()) {
        gameField->setEnabled(true);
    }

    // Инициализация  (и добавление их в список cellList) и массива значений игровых ячеек
    QPushButton *cell;
    cellValue = new char* [horizontalCellNumber];
    for (int i=0; i<horizontalCellNumber; ++i) {
        cellValue[i] = new char [verticalCellNumber+1];
        for (int j=0; j<verticalCellNumber; ++j) {
            cell = new QPushButton(" ");
            cellValue[i][j] = ' ';
            cell->setGeometry(QRect(QPoint(cellSize*i, cellSize*j), QSize(cellSize+1, cellSize+1)));
            cellList.append(cell);
        }
        cellValue[i][verticalCellNumber] = '\0';
    }

    // Соединение сигнала нажатия кнопок (ячеек) игрового поля с функцией обработки этих кнопок handleCell(int)
    QSignalMapper* signalMapper = new QSignalMapper(this);
    for (int i=0; i<cellList.length(); ++i) {
        cellList.at(i)->setParent(gameField);
        cellList.at(i)->show();
        connect(cellList.at(i), SIGNAL(clicked()), signalMapper, SLOT(map()));
        signalMapper -> setMapping (cellList.at(i), i);
    }
    connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(handleCell(int)));

    // Выставление начальной координаты, размеров и текста для текстового поля ширины
    width->setGeometry(QRect(QPoint(10, verticalFieldSize+30), QSize(41, 31)));
    width->setText(QString::number(horizontalCellNumber));

    // Выставление начальной координаты, размеров и текста для текстового поля высоты
    height->setGeometry(QRect(QPoint(60, verticalFieldSize+30), QSize(41, 31)));
    height->setText(QString::number(verticalCellNumber));

    // Выставление начальной координаты и размеров кнопки "New Game"
    newGameButton->setGeometry(QRect(QPoint(110, verticalFieldSize+30), QSize(61, 31)));

    // Выставление начальной координаты, размеров и текста для сообщения
    message->setGeometry(QRect(QPoint(10, verticalFieldSize+60), QSize(101, 31)));
    message->setText("");

    // Выставление фиксированного размеры для главного окна приложения и его обновление
    int horizontalWindowSize = horizontalFieldSize+21 > 181 ? horizontalFieldSize+21 : 181;
    this->setFixedSize(QSize(horizontalWindowSize, verticalFieldSize+91));
    this->update();
}

void MainWindow::handleNewGameButton()
{
    // Значения, ограничивающие ширину и высоту игрового поля
    int minCellNumber = 5;
    int maxCellNumber = 25;
    // Получение значений ширины и высоты игрового поля из UI
    int horizontalCellNumber = width->toPlainText().toInt();
    int verticalCellNumber = height->toPlainText().toInt();
    if (horizontalCellNumber >=minCellNumber && horizontalCellNumber <=maxCellNumber &&
        verticalCellNumber >=minCellNumber && verticalCellNumber <=maxCellNumber)
    {
        // Переотрисовка игрового окна
        drawWindow(horizontalCellNumber, verticalCellNumber);
    }
    else {
        message->setText("Min = " + QString::number(minCellNumber) +
                         "; Max = " + QString::number(maxCellNumber));
    }
}

void MainWindow::handleCell(int cellNumber)
{
    if (!cellList.at(cellNumber)->text().compare(" ")) {
        // Получение значений ширины и высоты игрового поля из UI
        int horizontalCellNumber = width->toPlainText().toInt();
        int verticalCellNumber = height->toPlainText().toInt();
        // Остаток от деления для получения индексов элемента двумерного массива игровых ячеек из индекса элемента из списка игровых ячеек
        int remainder = cellNumber%verticalCellNumber;
        // Установка "x" в нажатой ячейке
        cellList.at(cellNumber)->setText("x");
        // Изменение соответствующего значения элемента двумерного массива игровых ячеек на 'x'
        cellValue[(cellNumber-remainder)/verticalCellNumber][remainder] = 'x';
        // Проверка на победу 'x'
        if (fr->isVictory(cellValue, horizontalCellNumber, (cellNumber-remainder)/verticalCellNumber, remainder)) {
            cellList.at(cellNumber)->setStyleSheet("font: bold 14px");
            message->setText("You win!");
            gameField->setEnabled(false);
        }
        // Проверка на существование хода
        else if (fr->existTurn(cellValue, horizontalCellNumber)) {
            // Получение индекса элемента из списка игровых ячеек для хода компьютера
            int computerCellNumber = fr->getCellNumber(cellValue, horizontalCellNumber);
            // Ход компьютера
            remainder = computerCellNumber%verticalCellNumber;
            cellList.at(computerCellNumber)->setText("o");
            cellValue[(computerCellNumber-remainder)/verticalCellNumber][remainder] = 'o';
            // Проверка на победу 'o'
            if (fr->isVictory(cellValue, horizontalCellNumber, (computerCellNumber-remainder)/verticalCellNumber, remainder)) {
                cellList.at(computerCellNumber)->setStyleSheet("font: bold 14px");
                message->setText("You lose!");
                gameField->setEnabled(false);
            }
        }
        // Ничья (все ячейки заполнены)
        else {
            message->setText("Dead head!");
            gameField->setEnabled(false);
        }
    }
}

MainWindow::~MainWindow()
{
    // Очистка игрового поля
    for (int i=0; i<cellList.length()/height->toPlainText().toInt(); ++i) {
        delete [] cellValue[i];
    }
    delete [] cellValue;
    delete ui;
}
