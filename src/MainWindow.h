#pragma once

#include "Minesweeper.h"

#include <QMainWindow>
#include <QMenu>
#include <QAction>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QLabel>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

signals:
    void restartRequested();

private slots:
    void updateStatusBar(int cols, int rows, int mineCount, int markedCount);
    void newGame();
    void restartGame();
    void about();

private:
    void setupMenu();
    void createGameLayout();
    void setupStatusBar();

    QMenu *m_fileMenu;
    QMenu *m_helpMenu;

    QAction *m_newGameAction;
    QAction *m_resetGameAction;
    QAction *m_exitAction;
    QAction *m_aboutAction;

    QVBoxLayout *m_mainLayout;
    QWidget *m_gameWrapper;
    Minesweeper *m_gameScreen;

    QLabel *m_gameStatusLabel;
};
