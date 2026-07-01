#pragma once

#include <QPushButton>
#include <QMouseEvent>

class MinesweeperCell : public QPushButton
{
    Q_OBJECT

public:
    using QPushButton::QPushButton;

signals:
    void leftClicked();
    void rightClicked();

protected:
    void mouseReleaseEvent(QMouseEvent *event) override;
};
