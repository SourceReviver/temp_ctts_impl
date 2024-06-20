#pragma once
#include "service.h"
#include "fmt/format.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QSaveFile>
#include <QFormLayout>
#include <QLineEdit>
#include <QLabel>


class AzureConfigUI : public ServiceConfigUI {
    Q_OBJECT

public:
    explicit AzureConfigUI(QWidget* parent): ServiceConfigUI(parent)
    {

        auto* form = new QFormLayout(this);
        region = new QLineEdit("eastus", this);
        apiKey = new QLineEdit("b9885138792d4403a8ccf1a34553351d", this);
        auto* title = new QLabel("<b>Azure config</b>", this);

        title->setAlignment(Qt::AlignCenter);
        form->addRow(title);
        form->addRow("Location/Region", region);
        form->addRow("API Key", apiKey);

        form->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
        apiKey->setMinimumWidth(400);

        this->setLayout(form);

    };

    std::optional<std::string> save() override
    {

        QJsonDocument doc(QJsonObject({
            { "region", region->text().simplified() },
            { "apikey", apiKey->text().simplified() }

        }));

        QSaveFile f("./azure.json");
        f.open(QSaveFile::WriteOnly);
        f.write(doc.toJson(QJsonDocument::Indented));
        f.commit();
        return {};
    };

private:
    QLineEdit* region;
    QLineEdit* apiKey;
};