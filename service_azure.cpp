#include "service_azure.h"
#include "service_azure_config_ui.h"
#include <QNetworkReply>
#include <fmt/format.h>

bool Service_azure::initalize(QObject* parent, QNetworkAccessManager* networkAccessManager)
{
    this->networkAccessManager = networkAccessManager;
    this->setParent(parent);
    if (!QFileInfo::exists("./azure.json")) {
        QFile f("./azure.json");
        f.open(QFile::WriteOnly);
        f.write(R"(
{
    "apikey": "b9885138792d4403a8ccf1a34553351d",
    "region": "eastus"
}
)");
        f.close();
    }
    QFile f("./azure.json");

    if (!f.open(QFile::ReadOnly)) {
        return false;
    };

    QString apiKey;
    QString region;

    auto json = QJsonDocument::fromJson(f.readAll());

    if (json.isObject()) {
        QJsonObject o = json.object();

        if (const QJsonValue v = o["apikey"]; v.isString()) {
            apiKey = v.toString();
        } else {
            return false;
        }

        if (const QJsonValue v = o["region"]; v.isString()) {
            region = v.toString();
        } else {
            return false;
        }
    }

    request = new QNetworkRequest();
    request->setUrl(QUrl(QString("https://%1.tts.speech.microsoft.com/cognitiveservices/v1").arg(region)));
    request->setRawHeader("User-Agent", "WhatEver");
    request->setRawHeader("Ocp-Apim-Subscription-Key", apiKey.toLatin1());
    request->setRawHeader("Content-Type", "application/ssml+xml");
    request->setRawHeader("X-Microsoft-OutputFormat", "ogg-48khz-16bit-mono-opus");

    player = new QMediaPlayer();
    auto* audioOutput = new QAudioOutput;
    audioOutput->setVolume(50);
    player->setAudioOutput(audioOutput);

    connect(player, &QMediaPlayer::errorOccurred, this, &Service_azure::mediaErrorOccur);
    connect(player, &QMediaPlayer::mediaStatusChanged, this, &Service_azure::mediaStatus);

    return true;
}

Service_azure* Service_azure::Construct(QObject* parent,
    QNetworkAccessManager* networkAccessManager)
{
    auto azure = new Service_azure();
    if (azure->initalize(parent, networkAccessManager)) {
        return azure;
    }
    return nullptr;

};


[[nodiscard]] std::optional<std::string> Service_azure::speak(QUtf8StringView s)
{
    std::string y = fmt::format(R"(
<speak version='1.0' xml:lang='en-US'>
    <voice name='en-US-LunaNeural'>
        {}
    </voice>
</speak>
)", s.data());

    reply = networkAccessManager->post(*request, y.data());
    connect(reply, &QNetworkReply::finished, [this]() {
        player->setSourceDevice(reply);
        player->play();
    });

    connect(reply, &QNetworkReply::errorOccurred, this, &Service_azure::slotError);
    connect(reply, &QNetworkReply::sslErrors, this, &Service_azure::slotSslErrors);
    return std::nullopt;
}

ServiceConfigUI* Service_azure::getConfigWidget(QWidget* HostWiget)
{
    return new AzureConfigUI(HostWiget);
}

Service_azure::~Service_azure()
= default;


void Service_azure::slotError(QNetworkReply::NetworkError e)
{
    qDebug() << e;
}

void Service_azure::slotSslErrors()
{
    qDebug() << "ssl error";
}

void Service_azure::mediaErrorOccur(QMediaPlayer::Error error, const QString& errorString)
{
    qDebug() << "media " << error << errorString;
}

void Service_azure::mediaStatus(QMediaPlayer::MediaStatus status)
{
    qDebug() << "status " << status;
}