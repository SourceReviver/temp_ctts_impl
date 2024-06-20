#pragma once

#include <QString>
#include <QWidget>
#include <optional>

/*
 *
 * Do not add anything new in the code below to ensure maximum decouping between different services.
 *
 */

class ServiceConfigWidget : public QWidget {
public:
    explicit ServiceConfigWidget(QWidget* parent)
        : QWidget(parent)
    {
    }
    /// Ask the service to save it's config.
    /// @return if failed, return a string that contains Error message.
    virtual std::optional<std::string> save() { return {}; };
};

class Service : public QObject {
public:
    ///
    /// @return If failed, return a string that contains Error message.
    virtual std::optional<std::string> speak(QUtf8StringView s) { return {}; };

    /// Used in config dialog, or anywhere that may contains widdget.
    /// @param HostWiget The returned Widget will set its parent to HostWidget, so that its life time doesn't need much
    /// consideration, the `ServiceConfigWidget` should be written in a way that completly decoupled with the Service
    /// @return configWidget with a single save() method to call
    virtual ServiceConfigWidget* getConfigWidget(QWidget* HostWiget) { return {}; };
};
