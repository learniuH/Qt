/*******************************************************
 * File: din_widget.cpp
 *******************************************************/

#include "din_widget.h"

#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>

#include "din_device.h"

DinWidget::DinWidget(DinDevice *device,
                     QWidget *parent)
    : QWidget(parent)
{
    m_device = device;

    m_label = new QLabel(this);

    m_btn = new QPushButton(this);

    QHBoxLayout *layout =
            new QHBoxLayout(this);

    layout->addWidget(m_label);

    layout->addStretch();

    layout->addWidget(m_btn);

    setLayout(layout);

    connect(m_btn,
            &QPushButton::clicked,
            this,
            &DinWidget::slotButtonClicked);

    connect(m_device,
            &DinDevice::sigValueChanged,
            this,
            &DinWidget::slotValueChanged);

    updateUI();
}

void DinWidget::slotButtonClicked()
{
    int value = m_device->value();

    value = !value;

    m_device->setValue(value);
}

void DinWidget::slotValueChanged(int)
{
    updateUI();
}

void DinWidget::updateUI()
{
    m_label->setText(
                QString("DIN%1")
                .arg(m_device->index() + 1));

    if (m_device->value())
    {
        m_btn->setText("ON");
    }
    else
    {
        m_btn->setText("OFF");
    }
}