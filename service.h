#pragma once

#include <QString>
#include <QWidget>
#include <optional>
#include <QtMultimedia/QAudioOutput>

class ServiceConfigUI : public QWidget
{
public:
    explicit ServiceConfigUI(QWidget* parent): QWidget(parent)
    {
    }

    virtual std::optional<std::string> save() { return {}; };
};


class Service : public QObject
{
public:
    virtual std::optional<std::string> speak(QUtf8StringView s)
    {
        return {};
    };

    virtual ServiceConfigUI* getConfigWidget(QWidget* HostWiget)
    {
        return {};
    };
};
