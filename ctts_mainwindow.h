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
    QPointer<Service> currentService;
    QPointer<QPlainTextEdit> textEdit;
    QPointer<QPushButton> speakButton;
    QPointer<QPushButton> configButton;
};