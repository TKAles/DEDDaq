#include "GooeyDeadDaque.h"

GooeyDeadDaque::GooeyDeadDaque(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    // UI Connection Functions
    QObject::connect(ui.PBDetectClock, &QPushButton::released,
        this, &GooeyDeadDaque::DetectClockClick);
    QObject::connect(ui.PBMonoCamDetect, &QPushButton::released,
        this, &GooeyDeadDaque::LookForMonocamsClick);
    QObject::connect(ui.CBoxAvailableClocks, &QComboBox::currentIndexChanged,
        this, &GooeyDeadDaque::ClockSelectChanged);
    QObject::connect(ui.PBToggleIRConnect, &QPushButton::released,
        this, &GooeyDeadDaque::Xi400ToggleClick);
    QObject::connect(ui.CBoxFrequency, &QComboBox::currentIndexChanged,
        this, &GooeyDeadDaque::AcquisitionFrequencyChanged);

    // Add some test entries to the Cboxes
    ui.CBoxAvailableClocks->addItem("Test 1");
    ui.CBoxAvailableClocks->addItem("Test 2");
    ui.CBoxFrequency->addItem("Test F 1");
    ui.CBoxFrequency->addItem("Test F 2");
    ui.CBoxXi400Temp->addItem("Test T 1");
    ui.CBoxXi400Temp->addItem("Test T 2");
}


GooeyDeadDaque::~GooeyDeadDaque()
{
    // Destructor
}

void GooeyDeadDaque::PointsToAcquireClick()
{
    
}

void GooeyDeadDaque::AcquisitionFrequencyChanged()
{
    QMessageBox TempBox = QMessageBox();
    TempBox.setText(QString("Selected Acq Frequency changed!"));
    TempBox.exec();
}

void GooeyDeadDaque::DetectClockClick()
{
    QMessageBox TempBox = QMessageBox();
    TempBox.setText(QString("Detect Clock Click"));
    TempBox.exec();
}

void GooeyDeadDaque::ClockSelectChanged()
{
    QMessageBox TempBox = QMessageBox();
    TempBox.setText(QString("Clock Selection Changed!"));
    TempBox.exec();
}

void GooeyDeadDaque::LookForMonocamsClick()
{
    QMessageBox TempBox = QMessageBox();
    TempBox.setText(QString("Monocam Detect Click"));
    TempBox.exec();
}

void GooeyDeadDaque::SelectedMonocamChanged()
{

}

void GooeyDeadDaque::Xi400SerialChanged()
{

}

void GooeyDeadDaque::Xi400ToggleClick()
{
    QMessageBox TempBox = QMessageBox();
    TempBox.setText(QString("Xi400 Connection Toggle Clicked!"));
    TempBox.exec();
}

void GooeyDeadDaque::Xi400TempRangeChanged()
{
    QMessageBox TempBox = QMessageBox();
    TempBox.setText(QString("Xi400 Temperature Changed"));
    TempBox.exec();
}

