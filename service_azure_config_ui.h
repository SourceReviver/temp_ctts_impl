#pragma once
#include "service.h"
#include "service_azure.h"
#include "fmt/format.h"
#include <QPushButton>
#include <QHBoxLayout>

class AzureConfigUI : public ServiceConfigUI
{
    Q_OBJECT

public:
    explicit AzureConfigUI(QWidget* parent): ServiceConfigUI(parent)
    {
        auto* hbox = new QHBoxLayout(this);
        auto* btn = new QPushButton("AzureConfig", this);

        this->setLayout(hbox);

        hbox->addWidget(btn);
    };

    std::optional<std::string> save() override
    {
        fmt::println("configureSaved");
        return {};
    };
};
