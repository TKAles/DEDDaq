#include "GooeyDeadDaque.h"

GooeyDeadDaque::GooeyDeadDaque(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    // UI Connection Functions

}


GooeyDeadDaque::~GooeyDeadDaque()
{}

void GooeyDeadDaque::PointsToAcquireClick()
{
    
}

void GooeyDeadDaque::AcquisitionFrequencyChanged()
{

}

void GooeyDeadDaque::DetectClockClick()
{
    QMessageBox TempBox = QMessageBox();
    TempBox.setText(QString("Detect Clock Click"));
    TempBox.exec();
}

void GooeyDeadDaque::ClockSelectChanged()
{

}

void GooeyDeadDaque::LookForMonocamsClick()
{

}

void GooeyDeadDaque::SelectedMonocamChanged()
{

}

void GooeyDeadDaque::Xi400SerialChanged()
{

}

void GooeyDeadDaque::Xi400ToggleClick()
{

}

void GooeyDeadDaque::Xi400TempRangeChanged()
{

}

