#include "ctts_config_window.h"
#include "service_azure_config_ui.h"
#include <QLabel>
#include <QDialogButtonBox>

ctts_config_window::ctts_config_window(QWidget* parent)
    : QWidget(parent, Qt::Window)
{
    this->setBaseSize(100, 100);
    auto* layout = new QVBoxLayout();
    serviceConfigUI = new AzureConfigUI(this);
    layout->addWidget(serviceConfigUI);

    this->setLayout(layout);

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
        | QDialogButtonBox::Cancel | QDialogButtonBox::Help, this);

    layout->addWidget(buttonBox);

    connect(buttonBox, &QDialogButtonBox::accepted, this, [this]() {
        qDebug() << "accept";
        this->serviceConfigUI->save();
        this->deleteLater();
    });

    connect(buttonBox, &QDialogButtonBox::rejected, this, [this]() {
        qDebug() << "rejected";
        this->deleteLater();
    });

}