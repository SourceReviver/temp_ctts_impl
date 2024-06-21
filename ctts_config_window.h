#pragma once

#include "azure.h"
#include <QDialogButtonBox>
#include <QGridLayout>
#include <QGroupBox>
#include <QWidget>

class ctts_config_window : public QWidget {
    Q_OBJECT

public:
    explicit ctts_config_window(QWidget* parent);

signals:
    void finished_config();

private:
    QGridLayout* MainLayout;
    QGroupBox* configPane;

    QDialogButtonBox* buttonBox;
    QLineEdit* previewLineEdit;
    QPushButton* previewButton;

    QString currentService;

    QComboBox* serviceSelector;
    QScopedPointer<TextToSpeechConfigWidget> serviceConfigUI;

private slots:
    void updateConfigPaneBasedOnCurrentService();
};
