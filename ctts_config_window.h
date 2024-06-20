#pragma once

#include <QWidget>
#include <QPushButton>
#include <QDialogButtonBox>
#include "service_azure_config_ui.h"

class ctts_config_window : public QWidget {
    Q_OBJECT

public:
    explicit ctts_config_window(QWidget* parent);

private:
    QDialogButtonBox* buttonBox;
    ServiceConfigUI* serviceConfigUI;
    QLineEdit* previewLineEdit;
    QPushButton* previewButton;
};