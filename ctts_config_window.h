#pragma once

#include "azure.h"
#include <QDialogButtonBox>
#include <QWidget>

class ctts_config_window : public QWidget {
    Q_OBJECT

public:
    explicit ctts_config_window(QWidget* parent);

signals:
    void finished_config();
private:
    QDialogButtonBox* buttonBox;
    TextToSpeechConfigWidget* serviceConfigUI;
    QLineEdit* previewLineEdit;
    QPushButton* previewButton;
};