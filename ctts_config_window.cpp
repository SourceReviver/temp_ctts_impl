#include "azure.h"

#include "ctts_config_window.h"
#include <QDialogButtonBox>
#include <QGridLayout>
#include <QGroupBox>
#include <QPushButton>

ctts_config_window::ctts_config_window(QWidget* parent)
    : QWidget(parent, Qt::Window)
{
    auto* MainLayout = new QGridLayout(this);

    auto* configPane = new QGroupBox("Service Config", this);
    auto* previewPane = new QGroupBox("Audio Preview", this);

    configPane->setLayout(new QVBoxLayout());
    previewPane->setLayout(new QVBoxLayout());

    serviceConfigUI = new Azure::ConfigWidget(this);
    configPane->layout()->addWidget(serviceConfigUI);

    previewLineEdit = new QLineEdit(this);
    previewButton = new QPushButton("Save config and Preview", this);

    previewPane->layout()->addWidget(previewLineEdit);
    previewPane->layout()->addWidget(previewButton);

    connect(previewButton, &QPushButton::clicked, this, [this] {
        this->serviceConfigUI->save();
        auto newService = Azure::Service::Construct(this,"./azure.json");
        if (newService != nullptr) {
            auto _ = newService->speak(previewLineEdit->text().toUtf8());
        } else {
            qDebug() << "Failed to get Azure Service";
            exit(1);
        }
    });

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel | QDialogButtonBox::Help, this);

    MainLayout->addWidget(configPane, 0, 0, 1, 1);
    MainLayout->addWidget(previewPane, 0, 1, 1, 1);
    MainLayout->addWidget(buttonBox, 1, 0, 1, 2);

    connect(buttonBox, &QDialogButtonBox::accepted, this, [this]() {
        qDebug() << "accept";
        this->serviceConfigUI->save();
        emit this->finished_config();
        this->deleteLater();
    });

    connect(buttonBox, &QDialogButtonBox::rejected, this, [this]() {
        qDebug() << "rejected";
        this->deleteLater();
    });

    connect(buttonBox->button(QDialogButtonBox::Help), &QPushButton::clicked, this, [this]() { qDebug() << "help"; });
}