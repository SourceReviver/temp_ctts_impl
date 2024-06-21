#include "azure.h"

#include "ctts_config_window.h"
#include "ctts_current_service.h"
#include "dummy.h"

#include <QDialogButtonBox>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>

ctts_config_window::ctts_config_window(QWidget* parent)
    : QWidget(parent, Qt::Window)
{
    setWindowTitle("Service Config");

    MainLayout = new QGridLayout(this);

    configPane = new QGroupBox("Service Config", this);
    auto* previewPane = new QGroupBox("Audio Preview", this);

    configPane->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    previewPane->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::MinimumExpanding);

    configPane->setLayout(new QVBoxLayout());
    previewPane->setLayout(new QVBoxLayout());

    auto* serviceSelectLayout = new QHBoxLayout(nullptr);
    auto* serviceLabel = new QLabel("Select service", this);
    serviceSelector = new QComboBox();
    serviceSelector->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);

    serviceSelectLayout->addWidget(serviceLabel);
    serviceSelectLayout->addWidget(serviceSelector);

    serviceSelector->addItem("Azure Text to Speech", QStringLiteral("azure"));
    serviceSelector->addItem("Dummy", QStringLiteral("dummy"));

    this->currentService = GlobalCurrentService->retrive();

    if (auto i = serviceSelector->findData(this->currentService); i != -1) {
        serviceSelector->setCurrentIndex(i);
    }

    previewLineEdit = new QLineEdit(this);
    previewButton = new QPushButton("Preview", this);

    previewPane->layout()->addWidget(previewLineEdit);
    previewPane->layout()->addWidget(previewButton);
    qobject_cast<QVBoxLayout*>(previewPane->layout())->addStretch();

    connect(previewButton, &QPushButton::clicked, this, [this] {
        this->serviceConfigUI->save();

        QScopedPointer<TextToSpeechService> newService;

        if (currentService == "azure") {
            newService.reset(Azure::Service::Construct("./azure.json"));
        } else {
            newService.reset(new dummy::Service());
        }

        if (newService != nullptr) {
            auto _ = newService->speak(previewLineEdit->text().toUtf8());
        } else {
            exit(1);
        }
    });

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel | QDialogButtonBox::Help, this);

    MainLayout->addLayout(serviceSelectLayout, 0, 0, 1, 2);
    MainLayout->addWidget(configPane, 1, 0, 1, 1);
    MainLayout->addWidget(previewPane, 1, 1, 1, 1);
    MainLayout->addWidget(buttonBox, 2, 0, 1, 2);

    updateConfigPaneBasedOnCurrentService();

    connect(
        serviceSelector, &QComboBox::currentIndexChanged, this, [this] { updateConfigPaneBasedOnCurrentService(); });

    connect(buttonBox, &QDialogButtonBox::accepted, this, [this]() {
        qDebug() << "accept";
        this->serviceConfigUI->save();
        GlobalCurrentService->save(this->serviceSelector->currentData().toByteArray());

        emit this->finished_config();
        this->deleteLater();
    });

    connect(buttonBox, &QDialogButtonBox::rejected, this, [this]() {
        qDebug() << "rejected";
        this->deleteLater();
    });

    connect(buttonBox->button(QDialogButtonBox::Help), &QPushButton::clicked, this, [this]() { qDebug() << "help"; });
}

void ctts_config_window::updateConfigPaneBasedOnCurrentService()
{
    if (serviceSelector->currentData() == "azure") {
        serviceConfigUI.reset(new Azure::ConfigWidget(this));
    } else {
        serviceConfigUI.reset(new dummy::ConfigWidget(this));
    }
    configPane->layout()->addWidget(serviceConfigUI.get());
}