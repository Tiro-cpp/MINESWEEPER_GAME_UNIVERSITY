#include "Minesweeper.h"

#include <QMessageBox>
#include <QMenuBar>
#include <QMenu>
#include <QVBoxLayout>
#include <QDebug>
#include <random>

Minesweeper::Minesweeper(int rows, int cols, int mineCount, QWidget *parent)
    : QWidget(parent)
    , m_firstStart(true)
    , m_gameActive(false)
    , m_rows(rows)
    , m_cols(cols)
    , m_mineCount(mineCount)
    , m_mainLayout(nullptr)
    , m_gridLayout(nullptr)
    , m_buttons(nullptr)
    , m_mineField(nullptr)
    , m_revealed(nullptr)
    , m_marked(nullptr)
    , m_revealedCount(0)
    , m_markedCount(0)
    , m_adjacentMines(nullptr)
{
    this->setMinimumSize(100, 100);
    initializeGame();
    setupUI();
}

Minesweeper::~Minesweeper()
{
    clearGame();
}

void Minesweeper::initializeGame()
{
    m_revealedCount = 0;
    m_markedCount = 0;
    m_gameActive = true;

    m_buttons = new MinesweeperCell ** [m_rows];
    m_mineField = new bool * [m_rows];
    m_revealed = new bool * [m_rows];
    m_marked = new bool * [m_rows];
    m_adjacentMines = new int * [m_rows];

    for (int i = 0; i < m_rows; ++i) {
        m_buttons[i] = new MinesweeperCell * [m_cols];
        m_mineField[i] = new bool [m_cols];
        m_revealed[i] = new bool [m_cols];
        m_marked[i] = new bool [m_cols];
        m_adjacentMines[i] = new int [m_cols];

        for (int j = 0; j < m_cols; ++j) {
            m_buttons[i][j] = nullptr;
            m_mineField[i][j] = false;
            m_revealed[i][j] = false;
            m_marked[i][j] = false;
            m_adjacentMines[i][j] = 0;
        }
    }
}

void Minesweeper::setupUI()
{
    if (m_firstStart) {
        m_gridLayout = new QGridLayout();
        m_gridLayout->setSpacing(1);
        m_gridLayout->setSizeConstraint(QLayout::SetFixedSize);
    }

    for (int row = 0; row < m_rows; ++row) {
        for (int col = 0; col < m_cols; ++col) {
            auto *button = new MinesweeperCell();
            button->setFixedSize(30, 30);
            button->setProperty("row", row);
            button->setProperty("col", col);

            connect(button, &MinesweeperCell::leftClicked, this, &Minesweeper::handleLeftClick);
            connect(button, &MinesweeperCell::rightClicked, this, &Minesweeper::handleRightClick);

            m_buttons[row][col] = button;
            m_gridLayout->addWidget(button, row, col);
        }
    }

    if (m_firstStart) {
        m_mainLayout = new QVBoxLayout(this);
        m_mainLayout->setSizeConstraint(QLayout::SetFixedSize);

        m_mainLayout->addLayout(m_gridLayout);
        setLayout(m_mainLayout);

        m_firstStart = false;
    }

    setWindowTitle(QString("Minesweeper - %1 bombs (%2x%3)").arg(m_mineCount).arg(m_rows).arg(m_cols));
}

void Minesweeper::handleLeftClick()
{
    if (!m_gameActive) return;

    auto *clickedButton = qobject_cast<MinesweeperCell*>(sender());
    if (!clickedButton) return;

    int row = clickedButton->property("row").toInt();
    int col = clickedButton->property("col").toInt();

    if (m_revealedCount == 0) {
        placeMines(row, col);
        calculateAdjacentMines();
    }

    if (m_marked[row][col]) {
        return;
    }

    if (m_mineField[row][col]) {
        gameOver(false);
        return;
    }

    revealCell(row, col);

    if (m_revealedCount == (m_rows * m_cols - m_mineCount)) {
        gameOver(true);
    }
}

void Minesweeper::handleRightClick()
{
    if (!m_gameActive) return;

    auto *clickedButton = qobject_cast<MinesweeperCell*>(sender());
    if (!clickedButton) return;

    int row = clickedButton->property("row").toInt();
    int col = clickedButton->property("col").toInt();

    if (m_revealed[row][col]) {
        return;
    }

    toggleMark(row, col);
    emit gameStateUpdated(m_cols, m_rows, m_mineCount, m_markedCount);
}

void Minesweeper::placeMines(int firstClickRow, int firstClickCol)
{
    int minesPlaced = 0;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> rowDist(0, m_rows - 1);
    std::uniform_int_distribution<> colDist(0, m_cols - 1);

    while (minesPlaced < m_mineCount) {
        int row = rowDist(gen);
        int col = colDist(gen);

        bool isNearFirstClick = (abs(row - firstClickRow) <= 1 && abs(col - firstClickCol) <= 1);

        if (!m_mineField[row][col] && !isNearFirstClick) {
            m_mineField[row][col] = true;
            minesPlaced++;
        }
    }
}

void Minesweeper::calculateAdjacentMines()
{
    for (int row = 0; row < m_rows; ++row) {
        for (int col = 0; col < m_cols; ++col) {
            m_adjacentMines[row][col] = countAdjacentMines(row, col);
        }
    }
}

int Minesweeper::countAdjacentMines(int row, int col)
{
    int count = 0;
    QVector<QPair<int, int>> surrounding = getSurroundingPositions(row, col);

    for (const auto& pos: surrounding) {
        if (m_mineField[pos.first][pos.second]) {
            count++;
        }
    }

    return count;
}

void Minesweeper::toggleMark(int row, int col)
{
    if (!isValidPosition(row, col) || m_revealed[row][col]) {
        return;
    }

    MinesweeperCell *button = m_buttons[row][col];

    if (m_marked[row][col]) {
        button->setText("");
        button->setStyleSheet("");
        m_marked[row][col] = false;
        m_markedCount--;
    } else {
        if (m_markedCount + 1 > m_mineCount) {
            QMessageBox::information(this, "Can't do that!", "You already marked enough cells to cover all mines!");
            return;
        }

        button->setText("?");
        button->setStyleSheet("color: yellow; background-color: #755; font-weight: bold;");
        m_marked[row][col] = true;
        m_markedCount++;
    }
}

void Minesweeper::revealCell(int row, int col)
{
    if (!isValidPosition(row, col) || m_revealed[row][col] || m_marked[row][col]) {
        return;
    }

    m_revealed[row][col] = true;
    m_revealedCount++;

    MinesweeperCell *button = m_buttons[row][col];
    int adjacent = m_adjacentMines[row][col];

    if (adjacent > 0) {
        button->setText(QString::number(adjacent));

        switch (adjacent) {
            case 1: button->setStyleSheet("color: green; background-color: #aaa; font-weight: bold;"); break;
            case 2: button->setStyleSheet("color: blue; background-color: #aaa; font-weight: bold;"); break;
            case 3: button->setStyleSheet("color: red; background-color: #aaa; font-weight: bold;"); break;
            default: button->setStyleSheet("color: purple; background-color: #aaa; font-weight: bold;"); break;
        }

        button->setEnabled(false);
    } else {
        button->setText("");
        button->setStyleSheet("background-color: #aaa;");
        button->setEnabled(false);

        revealAdjacentCells(row, col);
    }
}

void Minesweeper::revealAdjacentCells(int row, int col)
{
    QVector<QPair<int, int>> surrounding = getSurroundingPositions(row, col);

    for (const auto& pos: surrounding) {
        if (!m_revealed[pos.first][pos.second] && !m_mineField[pos.first][pos.second]) {
            revealCell(pos.first, pos.second);
        }
    }
}

void Minesweeper::gameOver(bool won)
{
    m_gameActive = false;

    for (int row = 0; row < m_rows; ++row) {
        for (int col = 0; col < m_cols; ++col) {
            if (m_mineField[row][col]) {
                m_buttons[row][col]->setText("X");
                m_buttons[row][col]->setStyleSheet("background-color: red;");
            }
        }
    }

    QMessageBox msgBox;
    msgBox.setText(won ? "Congratulations!" : "Game Over");
    msgBox.setInformativeText(won ? "You won the game!" : "You hit a mine!");
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);

    msgBox.exec();
    resetGame();
}

QVector<QPair<int, int>> Minesweeper::getSurroundingPositions(int row, int col)
{
    QVector<QPair<int, int>> positions;

    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            if (i == 0 && j == 0) continue;

            int newRow = row + i;
            int newCol = col + j;

            if (isValidPosition(newRow, newCol)) {
                positions.append(qMakePair(newRow, newCol));
            }
        }
    }

    return positions;
}

bool Minesweeper::isValidPosition(int row, int col) const
{
    return (row >= 0 && row < m_rows && col >= 0 && col < m_cols);
}

void Minesweeper::startNewGame(int rows, int cols, int mineCount)
{
    clearGame();

    m_rows = rows;
    m_cols = cols;
    m_mineCount = mineCount;

    initializeGame();

    QLayoutItem *item;
    while ((item = m_gridLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    setupUI();
    emit gameStateUpdated(m_cols, m_rows, m_mineCount, m_markedCount);
}

void Minesweeper::handleRestart()
{
    resetGame();
}

void Minesweeper::resetGame()
{
    startNewGame(m_rows, m_cols, m_mineCount);
}

void Minesweeper::clearGame()
{
    if (m_buttons) {
        for (int i = 0; i < m_rows; ++i) {
            for (int j = 0; j < m_cols; ++j) {
                delete m_buttons[i][j];
            }

            delete[] m_buttons[i];
        }

        delete[] m_buttons;
    }

    if (m_mineField) {
        for (int i = 0; i < m_rows; ++i) {
            delete[] m_mineField[i];
        }

        delete[] m_mineField;
    }

    if (m_revealed) {
        for (int i = 0; i < m_rows; ++i) {
            delete[] m_revealed[i];
        }

        delete[] m_revealed;
    }

    if (m_adjacentMines) {
        for (int i = 0; i < m_rows; ++i) {
            delete[] m_adjacentMines[i];
        }

        delete[] m_adjacentMines;
    }
}
