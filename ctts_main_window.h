#pragma once

#include <QMainWindow>
#include <QPointer>
#include <QPlainTextEdit>
#include <QPushButton>

#include "service.h"

class ctts_mainwindow : public QMainWindow {
    Q_OBJECT

public:
    explicit ctts_mainwindow();
    std::unique_ptr<Service> currentService;
    QPointer<QPlainTextEdit> textEdit;
    QPointer<QPushButton> speakButton;
    QPointer<QPushButton> configButton;
};