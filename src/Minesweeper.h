#pragma once

#include <QWidget>
#include <QGridLayout>
#include "MinesweeperCell.h"
#include <QMainWindow>

class Minesweeper : public QWidget
{
    Q_OBJECT

public:
    explicit Minesweeper(int rows, int cols, int mineCount, QWidget *parent);
    ~Minesweeper();

    void startNewGame(int rows, int cols, int mineCount);
    void resetGame();

signals:
    void gameStateUpdated(int cols, int rows, int mineCount, int markedCount);

public slots:
    void handleRestart();

private slots:
    void handleLeftClick();
    void handleRightClick();

private:
    QVBoxLayout *m_mainLayout;
    QGridLayout *m_gridLayout;
    MinesweeperCell ***m_buttons;

    bool m_firstStart;
    int m_rows;
    int m_cols;
    int m_mineCount;
    bool **m_mineField;
    bool **m_revealed;
    bool **m_marked;
    int **m_adjacentMines;
    int m_revealedCount;
    int m_markedCount;
    bool m_gameActive;

    void initializeGame();
    void setupUI();
    void clearGame();

    void placeMines(int firstClickRow, int firstClickCol);
    void calculateAdjacentMines();
    int countAdjacentMines(int row, int col);

    void revealCell(int row, int col);
    void toggleMark(int row, int col);
    void revealAdjacentCells(int row, int col);
    void gameOver(bool won);
    bool isValidPosition(int row, int col) const;

    QVector<QPair<int, int>> getSurroundingPositions(int row, int col);
};
