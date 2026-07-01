#include "NewGameDialog.h"

#include <QDialogButtonBox>

NewGameDialog::NewGameDialog(QWidget *parent)
    : QDialog(parent)
    , m_fieldSizeRows(30)
    , m_fieldSizeCols(15)
    , m_mineCount(70)
{
    setupUI();
    setWindowTitle("New Game");
    setModal(true);
}

void NewGameDialog::setupUI()
{
    m_mainLayout = new QVBoxLayout(this);

    m_selectLayout = new QHBoxLayout();

    m_fieldSizeLabel = new QLabel("Field Size:");
    m_fieldSizeCombo = new QComboBox();
    m_fieldSizeCombo->addItem("Small", (int)FieldSize::SMALL);
    m_fieldSizeCombo->addItem("Medium", (int)FieldSize::MEDIUM);
    m_fieldSizeCombo->addItem("Large", (int)FieldSize::LARGE);
    m_fieldSizeCombo->addItem("Extra Large", (int)FieldSize::EXTRA_LARGE);
    m_fieldSizeCombo->setCurrentIndex(1);

    m_mineCountLabel = new QLabel("Difficulty:");
    m_mineCountCombo = new QComboBox();
    m_mineCountCombo->addItem("Easy", (int)MineCount::EASY);
    m_mineCountCombo->addItem("Medium", (int)MineCount::MEDIUM);
    m_mineCountCombo->addItem("Hard", (int)MineCount::HARD);
    m_mineCountCombo->addItem("Expert", (int)MineCount::EXPERT);
    m_mineCountCombo->setCurrentIndex(1);

    m_selectLayout->addWidget(m_fieldSizeLabel);
    m_selectLayout->addWidget(m_fieldSizeCombo);
    m_selectLayout->addWidget(m_mineCountLabel);
    m_selectLayout->addWidget(m_mineCountCombo);

    m_confirmButton = new QPushButton("Start New Game");
    m_confirmButton->setDefault(true);

    connect(m_confirmButton, &QPushButton::clicked, this, &NewGameDialog::onConfirm);

    m_mainLayout->addLayout(m_selectLayout);
    m_mainLayout->addWidget(m_confirmButton);

    setLayout(m_mainLayout);
}

void NewGameDialog::onConfirm()
{
    switch (m_fieldSizeCombo->currentData().toInt()) {
        case (int)FieldSize::SMALL:
            m_fieldSizeRows = 15;
            m_fieldSizeCols = 15;
            break;
        case (int)FieldSize::MEDIUM:
            m_fieldSizeRows = 15;
            m_fieldSizeCols = 30;
            break;
        case (int)FieldSize::LARGE:
            m_fieldSizeRows = 20;
            m_fieldSizeCols = 35;
            break;
        default:
            m_fieldSizeRows = 25;
            m_fieldSizeCols = 45;
            break;
    }

    switch (m_mineCountCombo->currentData().toInt()) {
        case (int)MineCount::EASY:
            m_mineCount = int((m_fieldSizeRows * m_fieldSizeCols) / 8.0f);
            break;
        case (int)MineCount::MEDIUM:
            m_mineCount = int((m_fieldSizeRows * m_fieldSizeCols) / 6.0f);
            break;
        case (int)MineCount::HARD:
            m_mineCount = int((m_fieldSizeRows * m_fieldSizeCols) / 5.0f);
            break;
        default:
            m_mineCount = int((m_fieldSizeRows * m_fieldSizeCols) / 3.0f);
            break;
    }

    accept();
}

int NewGameDialog::selectedFieldRows() const
{
    return m_fieldSizeRows;
}

int NewGameDialog::selectedFieldCols() const
{
    return m_fieldSizeCols;
}

int NewGameDialog::selectedMineCount() const
{
    return m_mineCount;
}
