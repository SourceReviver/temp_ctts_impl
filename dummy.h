#pragma once
#include "tts_service.h"
#include <QLabel>
#include <QLayout>
#include <QVBoxLayout>

namespace dummy {
class Service : public TextToSpeechService {
    std::optional<std::string> speak(QUtf8StringView s) override
    {
        qDebug() << "dummy speaks" << s;
        return {};
    };
};

class ConfigWidget : public TextToSpeechConfigWidget {
    Q_OBJECT
public:
    explicit ConfigWidget(QWidget* parent)
        : TextToSpeechConfigWidget(parent)
    {
        this->setLayout(new QVBoxLayout);
        this->layout()->addWidget(new QLabel("dummy", parent));
    }

    std::optional<std::string> save() override { return {}; };
};

}
