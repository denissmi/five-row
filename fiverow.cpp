#include "fiverow.h"
#include <string.h>
#include <iostream>

FiveRow::FiveRow() {   }

bool FiveRow::isVictory(char** cellValue, int horizontalSize, int x, int y)
{
    char ch = cellValue[x][y];
    bool vertical = checkVertical(cellValue, x, y, ch);
    bool horizontal = checkHorizontal(cellValue, horizontalSize, x, y, ch);
    bool mainDiagonal = checkMainDiagonal(cellValue, horizontalSize, x, y, ch);
    bool secondaryDiagonal = checkSecondaryDiagonal(cellValue, horizontalSize, x, y, ch);
    return false || vertical || horizontal || mainDiagonal || secondaryDiagonal;
}

bool FiveRow::checkVertical(char** cellValue, int x, int y, char ch)
{
    int victoryCondition = 1;
    int limit = y-5 > -1 ? y-5 : -1;
    for (int i=y-1; i>limit; i--) {
        if (cellValue[x][i] == ch) {
            victoryCondition++;
        } else break;
    }
    int verticalSize = strlen(cellValue[0]);
    limit = y+5 < verticalSize ? y+5 : verticalSize;
    for (int i=y+1; i<limit; i++) {
        if (cellValue[x][i] == ch) {
            victoryCondition++;
        } else break;
    }
    if (victoryCondition >= 5) {
        return true;
    }
    return false;
}

bool FiveRow::checkHorizontal(char** cellValue, int horizontalSize, int x, int y, char ch)
{
    int victoryCondition = 1;
    int limit = x-5 > -1 ? x-5 : -1;
    for (int i=x-1; i>limit; i--) {
        if (cellValue[i][y] == ch) {
            victoryCondition++;
        } else break;
    }
    limit = x+5 < horizontalSize ? x+5 : horizontalSize;
    for (int i=x+1; i<limit; i++) {
        if (cellValue[i][y] == ch) {
            victoryCondition++;
        } else break;
    }
    if (victoryCondition >= 5) {
        return true;
    }
    return false;
}

bool FiveRow::checkMainDiagonal(char** cellValue, int horizontalSize, int x, int y, char ch)
{
    int victoryCondition = 1;
    int limit = x+1 < y+1 ? x+1 : y+1;
    limit = limit < 5 ? limit : 5;
    for (int i=1; i<limit; ++i) {
        if (cellValue[x-i][y-i] == ch) {
            victoryCondition++;
        } else break;
    }
    int verticalSize = strlen(cellValue[0]);
    limit = horizontalSize-x < verticalSize-y ? horizontalSize-x : verticalSize-y;
    limit = limit < 5 ? limit : 5;
    for (int i=1; i<limit; ++i) {
        if (cellValue[x+i][y+i] == ch) {
            victoryCondition++;
        } else break;
    }
    if (victoryCondition >= 5) {
        return true;
    }
    return false;
}

bool FiveRow::checkSecondaryDiagonal(char** cellValue, int horizontalSize, int x, int y, char ch)
{
    int victoryCondition = 1;
    int limit = horizontalSize-x < y+1 ? horizontalSize-x : y+1;
    limit = limit < 5 ? limit : 5;
    for (int i=1; i<limit; ++i) {
        if (cellValue[x+i][y-i] == ch) {
            victoryCondition++;
        } else break;
    }
    int verticalSize = strlen(cellValue[0]);
    limit = verticalSize-y < x+1 ? verticalSize-y : x+1;
    limit = limit < 5 ? limit : 5;
    for (int i=1; i<limit; ++i) {
        if (cellValue[x-i][y+i] == ch) {
            victoryCondition++;
        } else break;
    }
    if (victoryCondition >= 5) {
        return true;
    }
    return false;
}

int FiveRow::getCellNumber(char **cellValue, int horizontalSize)
{
    int cellNumber;
    int verticalSize = strlen(cellValue[0]);
    int *lossLevelCell = new int[horizontalSize*verticalSize];
    int *winLevelCell = new int[horizontalSize*verticalSize];
    lossLevelCell = getCellLevelArray(cellValue, horizontalSize, verticalSize, 'x');
    winLevelCell = getCellLevelArray(cellValue, horizontalSize, verticalSize, 'o');
    int maxNumber = getMax(lossLevelCell, horizontalSize*verticalSize);
    for (int i=0; i<horizontalSize*verticalSize; ++i) {
        if(lossLevelCell[i] != maxNumber) {
            lossLevelCell[i] = -1;
        }
        else {
            lossLevelCell[i] += winLevelCell[i];
        }
    }
    cellNumber = getIndexOfMax(lossLevelCell, horizontalSize*verticalSize);
    delete [] lossLevelCell;
    delete [] winLevelCell;
    return cellNumber;
}

int* FiveRow::getCellLevelArray(char **cellValue, int horizontalSize, int verticalSize, char ch) {
    int *levelCell = new int[horizontalSize*verticalSize];
    int index = 0;
    for (int i=0; i<horizontalSize; ++i) {
        for (int j=0; j<verticalSize; ++j) {
            if (cellValue[i][j] == ' ') {
                levelCell[index] = 0;
                levelCell[index] += verticalLevelCell(cellValue, i, j, ch);
                levelCell[index] += horizontalLevelCell(cellValue, horizontalSize, i, j, ch);
                levelCell[index] += mainDiagonalLevelCell(cellValue, horizontalSize, i, j, ch);
                levelCell[index++] += secondaryDiagonalLevelCell(cellValue, horizontalSize, i, j, ch);
            }
            else if (cellValue[i][j] == 'o') {
                levelCell[index++] = -1;
            }
            else {
                levelCell[index++] = -2;
            }
        }
    }
    return levelCell;
}

int FiveRow::verticalLevelCell(char **cellValue, int x, int y, char ch)
{
    int verticalSize = strlen(cellValue[0]);
    return getCellLevel(cellValue[x], verticalSize, y, ch);
}

int FiveRow::horizontalLevelCell(char **cellValue, int horizontalSize, int x, int y, char ch)
{
    int cellLevel = 0;
    char *horizontalCellValue = new char[horizontalSize+1];
    for (int i=0; i<horizontalSize; ++i) {
        horizontalCellValue[i] = cellValue[i][y];
    }
    horizontalCellValue[horizontalSize] = '\0';
    cellLevel = getCellLevel(horizontalCellValue, horizontalSize, x, ch);
    delete [] horizontalCellValue;
    return cellLevel;
}

int FiveRow::mainDiagonalLevelCell(char **cellValue, int horizontalSize, int x, int y, char ch)
{
    int cellLevel = 0;
    int verticalSize = strlen(cellValue[0]);
    int upSize = x < y ? x : y;
    int downSize = verticalSize-y < horizontalSize-x ? verticalSize-y : horizontalSize-x;
    int size = upSize + downSize;
    if (size < 5) {
       return 0;
    }
    int index = x < y ? x : y;
    char *mainDiagonalCellValue = new char[size+1];
    for (int i=0; i<size; ++i) {
        mainDiagonalCellValue[i] = cellValue[x-index+i][y-index+i];
    }
    mainDiagonalCellValue[size] = '\0';
    cellLevel = getCellLevel(mainDiagonalCellValue, size, index, ch);
    delete [] mainDiagonalCellValue;
    return cellLevel;
}

int FiveRow::secondaryDiagonalLevelCell(char **cellValue, int horizontalSize, int x, int y, char ch)
{
    int cellLevel = 0;
    int verticalSize = strlen(cellValue[0]);
    int downSize = y < horizontalSize-x-1 ? y : horizontalSize-x-1;
    int upSize = x < verticalSize-y-1 ? x : verticalSize-y-1;
    int size = upSize + downSize + 1;
    if (size < 5) {
       return 0;
    }
    int index = y < horizontalSize-x-1 ? y : horizontalSize-x-1;
    char *secondaryDiagonalCellValue = new char[size+1];
    for (int i=0; i<size; ++i) {
        secondaryDiagonalCellValue[i] = cellValue[x+index-i][y-index+i];
    }
    secondaryDiagonalCellValue[size] = '\0';
    cellLevel = getCellLevel(secondaryDiagonalCellValue, size, index, ch);
    delete [] secondaryDiagonalCellValue;
    return cellLevel;
}

int FiveRow::getCellLevel(char *cellValue, int size, int index, char ch)
{
    int cellLevel = 0;
    int numFreeCell = 0;
    int distancePoint = 0;
    int numNecessaryCell = 0;
    int limit = index-5 > -1 ? index-5 : -1;
    for (int i=index-1; i>limit; i--) {
        if (cellValue[i] == ' ') {
            numFreeCell++;
        }
        else if (cellValue[i] == ch) {
            numFreeCell++;
            numNecessaryCell++;
            distancePoint += i-limit;
        }
        else break;
    }
    limit = index+5 < size ? index+5 : size;
    for (int i=index+1; i<limit; i++) {
        if (cellValue[i] == ' ') {
            numFreeCell++;
        }
        else if (cellValue[i] == ch) {
            numFreeCell++;
            numNecessaryCell++;
            distancePoint += limit-i;
        }
        else break;
    }
    if (numFreeCell >= 4) {
        cellLevel += numNecessaryCell*numNecessaryCell+distancePoint;
    }
    return cellLevel;
}

int FiveRow::getIndexOfMax(int *nums, int size)
{
    int index = 0;
    int max = getMax(nums, size);
    for (int i=0; i<size; ++i) {
        if(max == nums[i]) {
            index = i;
            break;
        }
    }
    return index;
}

int FiveRow::getMax(int *nums, int size)
{
    int max = nums[0];
    for (int i=1; i<size; ++i) {
        if(max < nums[i]) {
            max = nums[i];
        }
    }
    return max;
}

bool FiveRow::existTurn(char **cellValue, int horizontalSize)
{
    int verticalSize = strlen(cellValue[0]);
    for (int i=0; i<horizontalSize; ++i) {
        for (int j=0; j<verticalSize; ++j) {
            if (cellValue[i][j] == ' ') {
                return true;
            }
        }
    }
    return false;
}

FiveRow::~FiveRow() {   }
