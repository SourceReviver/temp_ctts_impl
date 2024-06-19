#include "ctts_mainwindow.h"
#include "service_azure.h"
#include "ctts_config_window.h"
#include <QVBoxLayout>

ctts_mainwindow::ctts_mainwindow()
{
    auto* networkManager = new QNetworkAccessManager();

    auto* vbox = new QVBoxLayout(nullptr);

    auto* mainWidget = new QWidget(this);

    currentService = new Service_azure(this, networkManager);
    textEdit = new QPlainTextEdit(this);
    textEdit->setPlainText("hello");
    speakButton = new QPushButton("Speak", this);
    configButton = new QPushButton("Config", this);

    vbox->addWidget(textEdit.get());
    vbox->addWidget(speakButton.get());
    vbox->addWidget(configButton.get());

    connect(speakButton.get(), &QPushButton::clicked, this, [this] {
        currentService->speak(textEdit->toPlainText().toStdString());
    });

    connect(configButton.get(), &QPushButton::clicked, this, [this] {
        auto* cfgWindow = new ctts_config_window(this);
        cfgWindow->setWindowModality(Qt::WindowModal);
        cfgWindow->show();

    });

    mainWidget->setLayout(vbox);
    this->setCentralWidget(mainWidget);

}