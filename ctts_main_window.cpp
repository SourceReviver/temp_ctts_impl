#include "ctts_main_window.h"
#include "azure.h"
#include "ctts_config_window.h"
#include <QGroupBox>
#include <QVBoxLayout>

ctts_mainwindow::ctts_mainwindow()
{
    auto* vbox = new QVBoxLayout(nullptr);

    auto* mainWidget = new QWidget(this);

    auto newService = Azure::Service::Construct(this, "./azure.json");

    if (newService != nullptr) {
        // has_value checks if a type-value is returned
        currentService.reset(newService);
    } else {
        qDebug() << "Failed to get Azure Service";
        exit(1);
    }

    textEdit = new QPlainTextEdit(this);
    textEdit->setPlainText("hello");
    speakButton = new QPushButton("Speak", this);
    configButton = new QPushButton("Config", this);

    vbox->addWidget(textEdit.get());
    vbox->addWidget(speakButton.get());
    vbox->addWidget(configButton.get());

    connect(speakButton.get(), &QPushButton::clicked, this,
        [this] { currentService->speak(textEdit->toPlainText().toStdString()); });

    connect(configButton.get(), &QPushButton::clicked, this, [this] {
        auto* cfgWindow = new ctts_config_window(this);
        cfgWindow->setWindowModality(Qt::WindowModal);
        connect(cfgWindow, &ctts_config_window::finished_config, this, [this] {
            auto newService = Azure::Service::Construct(this, "./azure.json");

            if (newService != nullptr) {
                // has_value checks if a type-value is returned
                this->currentService.reset(newService);
            } else {
                qDebug() << "Failed to get Azure Service";
                exit(1);
            }
        });
        cfgWindow->show();
    });

    mainWidget->setLayout(vbox);
    this->setCentralWidget(mainWidget);
}