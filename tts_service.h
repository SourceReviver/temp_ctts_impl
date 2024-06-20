#pragma once

#include <QString>
#include <QWidget>
#include <optional>

/*
 *
 * Do not add anything new to this header.
 * So that we can ensure maximum decouping between different services.
 *
 * Things needed by Services, should be added to specific services.
 *
 * If something is needed by multiple services,
 * it should be implemented as a component that
 * can be plugged into needed services.
 *
 */

class TextToSpeechConfigWidget : public QWidget {
public:
    explicit TextToSpeechConfigWidget(QWidget* parent)
        : QWidget(parent)
    {
    }
    /// Ask the service to save it's config.
    /// @return if failed, return a string that contains Error message.
    virtual std::optional<std::string> save() { return {}; };
};

class TextToSpeechService : public QObject {
public:
    ///
    /// @return If failed, return a string that contains Error message.
    virtual std::optional<std::string> speak(QUtf8StringView s) { return {}; };

    /// Used in config dialog, or anywhere that may contains widdget.
    /// @param HostWiget The returned Widget will set its parent to HostWidget, so that its life time doesn't need much
    /// consideration, the `ServiceConfigWidget` should be written in a way that completly decoupled with the Service
    /// @return configWidget with a single save() method to call
    virtual TextToSpeechConfigWidget* getConfigWidget(QWidget* HostWiget) { return {}; };
};
