#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    gameField = new QFrame(this);
    gameField->setFrameStyle(QFrame::Box);
    gameField->setLineWidth(1);
    width = new QTextEdit(this);
    height = new QTextEdit(this);
    newGameButton = new QPushButton("New Game", this);
    message = new QLabel(this);

    drawWindow(5, 5);

    connect(newGameButton, SIGNAL(clicked()), this, SLOT (handleNewGameButton()));
    fr = new FiveRow();
}

void MainWindow::drawWindow(int horizontalCellNumber, int verticalCellNumber)
{
    const int cellSize = 20;
    int horizontalFieldSize = cellSize*horizontalCellNumber;
    int verticalFieldSize = cellSize*verticalCellNumber;

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

    gameField->setGeometry(QRect(QPoint(10, 20), QSize(horizontalFieldSize+1, verticalFieldSize+1)));
    if (!gameField->isEnabled()) {
        gameField->setEnabled(true);
    }

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

    QSignalMapper* signalMapper = new QSignalMapper(this);
    for (int i=0; i<cellList.length(); ++i) {
        cellList.at(i)->setParent(gameField);
        cellList.at(i)->show();
        connect(cellList.at(i), SIGNAL(clicked()), signalMapper, SLOT(map()));
        signalMapper -> setMapping (cellList.at(i), i);
    }
    connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(handleCell(int)));

    width->setGeometry(QRect(QPoint(10, verticalFieldSize+30), QSize(41, 31)));
    width->setText(QString::number(horizontalCellNumber));

    height->setGeometry(QRect(QPoint(60, verticalFieldSize+30), QSize(41, 31)));
    height->setText(QString::number(verticalCellNumber));

    newGameButton->setGeometry(QRect(QPoint(110, verticalFieldSize+30), QSize(61, 31)));

    message->setGeometry(QRect(QPoint(10, verticalFieldSize+60), QSize(101, 31)));
    message->setText("");

    int horizontalWindowSize = horizontalFieldSize+21 > 181 ? horizontalFieldSize+21 : 181;
    this->setFixedSize(QSize(horizontalWindowSize, verticalFieldSize+91));
    this->update();
}

void MainWindow::handleNewGameButton()
{
    int minCellNumber = 5;
    int maxCellNumber = 25;
    int horizontalCellNumber = width->toPlainText().toInt();
    int verticalCellNumber = height->toPlainText().toInt();
    if (horizontalCellNumber >=minCellNumber && horizontalCellNumber <=maxCellNumber &&
        verticalCellNumber >=minCellNumber && verticalCellNumber <=maxCellNumber)
    {
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
        int horizontalCellNumber = width->toPlainText().toInt();
        int verticalCellNumber = height->toPlainText().toInt();
        int remainder = cellNumber%verticalCellNumber;
        cellList.at(cellNumber)->setText("x");
        cellValue[(cellNumber-remainder)/verticalCellNumber][remainder] = 'x';
        if (fr->isVictory(cellValue, horizontalCellNumber, (cellNumber-remainder)/verticalCellNumber, remainder)) {
            cellList.at(cellNumber)->setStyleSheet("font: bold 14px");
            message->setText("You win!");
            gameField->setEnabled(false);
        }
        else if (fr->existTurn(cellValue, horizontalCellNumber)) {
            int computerCellNumber = fr->getCellNumber(cellValue, horizontalCellNumber);
            remainder = computerCellNumber%verticalCellNumber;
            cellList.at(computerCellNumber)->setText("o");
            cellValue[(computerCellNumber-remainder)/verticalCellNumber][remainder] = 'o';
            if (fr->isVictory(cellValue, horizontalCellNumber, (computerCellNumber-remainder)/verticalCellNumber, remainder)) {
                cellList.at(computerCellNumber)->setStyleSheet("font: bold 14px");
                message->setText("You lose!");
                gameField->setEnabled(false);
            }
        }
        else {
            message->setText("Dead head!");
            gameField->setEnabled(false);
        }
    }
}

MainWindow::~MainWindow()
{
    for (int i=0; i<cellList.length()/height->toPlainText().toInt(); ++i) {
        delete [] cellValue[i];
    }
    delete [] cellValue;
    delete ui;
}
