/*******************************************************
 * File: ain_widget.h
 *******************************************************/

#ifndef AIN_WIDGET_H
#define AIN_WIDGET_H

#include <QWidget>

class QLabel;
class QSlider;

class AinDevice;

class AinWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AinWidget(AinDevice *device,
                       QWidget *parent = nullptr);

private slots:
    void slotSliderChanged(int value);

    void slotValueChanged(int value);

private:
    void updateUI();

private:
    AinDevice *m_device;

    QLabel *m_label;

    QLabel *m_valueLabel;

    QSlider *m_slider;
};

#endif // AIN_WIDGET_H