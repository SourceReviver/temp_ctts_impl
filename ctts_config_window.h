#pragma once

#include "azure.h"
#include <QDialogButtonBox>
#include <QWidget>

class ctts_config_window : public QWidget {
    Q_OBJECT

public:
    explicit ctts_config_window(QWidget* parent);

private:
    QDialogButtonBox* buttonBox;
    ServiceConfigWidget* serviceConfigUI;
    QLineEdit* previewLineEdit;
    QPushButton* previewButton;
};