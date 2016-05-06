#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QFrame>
#include <QList>
#include <QTextEdit>
#include <QLabel>
#include <QSignalMapper>
#include <QDebug>
#include <fiverow.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void handleNewGameButton();
    void handleCell(int);

private:
    Ui::MainWindow *ui;
    QFrame *gameField;
    QList<QPushButton*> cellList;
    QTextEdit *width;
    QTextEdit *height;
    QPushButton *newGameButton;
    QLabel *message;
    void drawWindow(int, int);
    char **cellValue;
    FiveRow *fr;
};

#endif // MAINWINDOW_H
