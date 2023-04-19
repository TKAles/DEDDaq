#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_GooeyDeadDaque.h"
#include <iostream>
#include <qmessagebox.h>
class GooeyDeadDaque : public QMainWindow
{
    Q_OBJECT

public:
    GooeyDeadDaque(QWidget *parent = nullptr);
    ~GooeyDeadDaque();
    void PointsToAcquireClick();
    void AcquisitionFrequencyChanged();
    void DetectClockClick();
    void ClockSelectChanged();

    void LookForMonocamsClick();
    void SelectedMonocamChanged();

    void Xi400SerialChanged();
    void Xi400ToggleClick();
    void Xi400TempRangeChanged();



private:
    Ui::GooeyDeadDaqueClass ui;
};
