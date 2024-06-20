#pragma once

#include <QMainWindow>
#include <QPlainTextEdit>
#include <QPointer>
#include <QPushButton>

#include "tts_service.h"

class ctts_mainwindow : public QMainWindow {
    Q_OBJECT

public:
    explicit ctts_mainwindow();
    std::unique_ptr<TextToSpeechService> currentService;
    QPointer<QPlainTextEdit> textEdit;
    QPointer<QPushButton> speakButton;
    QPointer<QPushButton> configButton;
};