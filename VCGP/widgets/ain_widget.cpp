/*******************************************************
 * File: ain_widget.cpp
 *******************************************************/

#include "ain_widget.h"

#include <QHBoxLayout>
#include <QSlider>
#include <QLabel>

#include "ain_device.h"

AinWidget::AinWidget(AinDevice *device,
                     QWidget *parent)
    : QWidget(parent)
{
    m_device = device;

    m_label = new QLabel(this);

    m_valueLabel = new QLabel(this);

    m_slider = new QSlider(Qt::Horizontal,
                           this);

    m_slider->setRange(0, 4095);

    QHBoxLayout *layout =
            new QHBoxLayout(this);

    layout->addWidget(m_label);

    layout->addWidget(m_slider);

    layout->addWidget(m_valueLabel);

    setLayout(layout);

    connect(m_slider,
            &QSlider::valueChanged,
            this,
            &AinWidget::slotSliderChanged);

    connect(m_device,
            &AinDevice::sigValueChanged,
            this,
            &AinWidget::slotValueChanged);

    updateUI();
}

void AinWidget::slotSliderChanged(int value)
{
    m_device->setValue(value);
}

void AinWidget::slotValueChanged(int)
{
    updateUI();
}

void AinWidget::updateUI()
{
    m_label->setText(
                QString("AIN%1")
                .arg(m_device->index() + 1));

    m_slider->blockSignals(true);

    m_slider->setValue(m_device->value());

    m_slider->blockSignals(false);

    m_valueLabel->setText(
                QString::number(
                    m_device->value()));
}