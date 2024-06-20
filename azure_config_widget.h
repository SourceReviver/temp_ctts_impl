#pragma once
#include "service.h"
#include <QLineEdit>

class AzureConfigWidget : public ServiceConfigWidget {
    Q_OBJECT

public:
    explicit AzureConfigWidget(QWidget* parent);

    std::optional<std::string> save() override;

private:
    QLineEdit* region;
    QLineEdit* apiKey;
};
