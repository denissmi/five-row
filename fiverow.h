#ifndef FIVEROW_H
#define FIVEROW_H


class FiveRow
{

public:
    FiveRow();
    bool isVictory(char**, int, int, int);
    int getCellNumber(char**, int);
    bool existTurn(char**, int);
    ~FiveRow();

private:
    bool checkVertical(char**, int, int, char);
    bool checkHorizontal(char**, int, int, int, char);
    bool checkMainDiagonal(char**, int, int, int, char);
    bool checkSecondaryDiagonal(char**, int, int, int, char);
    int getIndexOfMax(int*, int);
    int getMax(int*, int);
    int verticalLevelCell(char**, int, int, char);
    int horizontalLevelCell(char**, int, int, int, char);
    int mainDiagonalLevelCell(char**, int, int, int, char);
    int secondaryDiagonalLevelCell(char**, int, int, int, char);
    int getCellLevel(char*, int, int, char);
    int* getCellLevelArray(char**, int, int, char);
};

#endif // FIVEROW_H
