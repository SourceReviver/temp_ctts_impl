#pragma once

#include "tts_service.h"

#include <QComboBox>
#include <QLineEdit>
#include <QString>
#include <QtMultimedia/QtMultimedia>
#include <QtNetwork>

#include <optional>

namespace Azure {

static const char* hostUrlBody = "tts.speech.microsoft.com/cognitiveservices";

struct AzureConfig {
    QString apiKey;
    QString region;
    QString voiceShortName;

    [[nodiscard]] static std::optional<AzureConfig> loadFromFile(const QString&);
    [[nodiscard]] static bool saveToFile(const AzureConfig&);
};

class ConfigWidget : public TextToSpeechConfigWidget {
    Q_OBJECT

public:
    explicit ConfigWidget(QWidget* parent);

    std::optional<std::string> save() override;

private:
    QLineEdit* region;
    QLineEdit* apiKey;
    QScopedPointer<QNetworkRequest> voiceListRequest;
    QScopedPointer<QNetworkReply> voiceListReply;

    QComboBox* voiceList;

    void asyncVoiceListPopulating(const QString& autoSelectThisName);
};

class Service : public TextToSpeechService {
    Q_OBJECT

public:
    static Service* Construct(QObject* parent, const QString& configFilePath);
    [[nodiscard]] std::optional<std::string> speak(QUtf8StringView s) override;
    [[nodiscard]] TextToSpeechConfigWidget* getConfigWidget(QWidget*) override;

    ~Service() override;

private:
    Service() = default;
    bool initalize(QObject* parent, const QString& configFilePath);
    QNetworkReply* reply;
    QMediaPlayer* player;
    QNetworkRequest* request;
    std::string voiceShortName;

private slots:
    void slotError(QNetworkReply::NetworkError e);
    void slotSslErrors();

    void mediaErrorOccur(QMediaPlayer::Error error, const QString& errorString);
    void mediaStatus(QMediaPlayer::MediaStatus status);
};
}