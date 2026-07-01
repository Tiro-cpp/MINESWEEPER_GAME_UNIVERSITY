#pragma once

#include <QDialog>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>

enum class FieldSize {
    SMALL,
    MEDIUM,
    LARGE,
    EXTRA_LARGE
};

enum class MineCount {
    EASY,
    MEDIUM,
    HARD,
    EXPERT,
};

class NewGameDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewGameDialog(QWidget *parent = nullptr);

    int selectedFieldRows() const;
    int selectedFieldCols() const;
    int selectedMineCount() const;

private slots:
    void onConfirm();

private:
    void setupUI();

    QVBoxLayout *m_mainLayout;
    QHBoxLayout *m_selectLayout;

    QLabel *m_fieldSizeLabel;
    QLabel *m_mineCountLabel;

    QComboBox *m_fieldSizeCombo;
    QComboBox *m_mineCountCombo;
    QPushButton *m_confirmButton;

    int m_fieldSizeRows;
    int m_fieldSizeCols;
    int m_mineCount;
};
