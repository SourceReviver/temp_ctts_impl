#include "ctts_config_window.h"
#include <QVBoxLayout>
#include <QLabel>

ctts_config_window::ctts_config_window(QWidget* parent)
    : QWidget(parent,Qt::Window)
{
    this->setBaseSize(100, 100);
    auto* layout = new QVBoxLayout();
    layout->addWidget(new QLabel("ok", this));
    this->setLayout(layout);
}