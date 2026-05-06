/*******************************************************
 * File: din_widget.h
 *******************************************************/

#ifndef DIN_WIDGET_H
#define DIN_WIDGET_H

#include <QWidget>

class QLabel;
class QPushButton;

class DinDevice;

class DinWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DinWidget(DinDevice *device,
                       QWidget *parent = nullptr);

private slots:
    void slotButtonClicked();

    void slotValueChanged(int value);

private:
    void updateUI();

private:
    DinDevice *m_device;

    QLabel *m_label;

    QPushButton *m_btn;
};

#endif // DIN_WIDGET_H