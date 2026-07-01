#include "MainWindow.h"
#include "Minesweeper.h"
#include "NewGameDialog.h"

#include <QApplication>
#include <QMenuBar>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QDialog>
#include <QStatusBar>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setWindowTitle("Minesweeper");

    setupMenu();
    createGameLayout();
    setupStatusBar();

    layout()->setSizeConstraint(QLayout::SetFixedSize);
}

MainWindow::~MainWindow()
{

}

void MainWindow::setupMenu() {
    m_fileMenu = menuBar()->addMenu("&File");

    m_newGameAction = new QAction("&New Game", this);
    m_newGameAction->setShortcut(QKeySequence::New);
    connect(m_newGameAction, &QAction::triggered, this, &MainWindow::newGame);

    m_resetGameAction = new QAction("&Restart Game", this);
    m_resetGameAction->setShortcut(QKeySequence::Refresh);
    connect(m_resetGameAction, &QAction::triggered, this, &MainWindow::restartGame);

    m_exitAction = new QAction("&Exit", this);
    m_exitAction->setShortcut(QKeySequence::Quit);
    connect(m_exitAction, &QAction::triggered, this, &QApplication::quit);

    m_fileMenu->addAction(m_newGameAction);
    m_fileMenu->addAction(m_resetGameAction);
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_exitAction);

    m_helpMenu = menuBar()->addMenu("&Help");

    m_aboutAction = new QAction("&About", this);
    connect(m_aboutAction, &QAction::triggered, this, &MainWindow::about);

    m_helpMenu->addAction(m_aboutAction);
}

void MainWindow::newGame()
{
    NewGameDialog dialog(this);

    if (dialog.exec() == QDialog::Accepted) {
        int fieldRows = dialog.selectedFieldRows();
        int fieldCols = dialog.selectedFieldCols();
        int mineCount = dialog.selectedMineCount();

        m_gameScreen->startNewGame(fieldRows, fieldCols, mineCount);
    }
}

void MainWindow::restartGame()
{
    emit restartRequested();
}

void MainWindow::about()
{
    QMessageBox::aboutQt(this);
}

void MainWindow::createGameLayout()
{
    m_gameWrapper = new QWidget(this);

    m_mainLayout = new QVBoxLayout(m_gameWrapper);
    m_mainLayout->setSizeConstraint(QLayout::SetFixedSize);

    m_gameScreen = new Minesweeper(15, 30, 70, this);
    connect(this, &MainWindow::restartRequested, m_gameScreen, &Minesweeper::handleRestart);

    m_mainLayout->addWidget(m_gameScreen);

    setCentralWidget(m_gameWrapper);
}

void MainWindow::setupStatusBar()
{
    m_gameStatusLabel = new QLabel(QString("Field Size: 30x15 - Mines Marked: 0 - Mines Total: 70"), this);

    connect(m_gameScreen, &Minesweeper::gameStateUpdated, this, &MainWindow::updateStatusBar);

    statusBar()->setSizeGripEnabled(false);
    statusBar()->addWidget(m_gameStatusLabel, 1);
}

void MainWindow::updateStatusBar(int cols, int rows, int mineCount, int markedCount)
{
    m_gameStatusLabel->setText(QString("Field Size: %0x%1 - Mines Marked: %2 - Mines Total: %3")
        .arg(cols)
        .arg(rows)
        .arg(markedCount)
        .arg(mineCount));
}
