#pragma once
#include <QString>
#include <optional>

struct azure_config {
    QString apiKey;
    QString region;

    [[nodiscard]] static std::optional<azure_config> loadFromFile(const QString&);
    [[nodiscard]] static bool saveToFile(const azure_config&);
};
