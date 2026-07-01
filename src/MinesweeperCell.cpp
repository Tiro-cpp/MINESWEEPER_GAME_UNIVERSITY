#include "MinesweeperCell.h"

void MinesweeperCell::mouseReleaseEvent(QMouseEvent *event) {
    QPushButton::mouseReleaseEvent(event);

    if (event->button() == Qt::LeftButton) {
        emit leftClicked();
    } else if (event->button() == Qt::RightButton) {
        emit rightClicked();
    }
}
