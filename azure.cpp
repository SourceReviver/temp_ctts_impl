#include "azure.h"
#include <QFormLayout>
#include <QLabel>
#include <QNetworkReply>
#include <QString>
#include <fmt/format.h>

std::optional<azure_config> azure_config::loadFromFile(const QString& p)
{
    if (!QFileInfo::exists(p)) {
        return {};
    }

    QFile f(p);

    if (!f.open(QFile::ReadOnly)) {
        return {};
    };

    QString apiKey;
    QString region;

    auto json = QJsonDocument::fromJson(f.readAll());

    if (json.isObject()) {
        QJsonObject o = json.object();

        if (const QJsonValue v = o["apikey"]; v.isString()) {
            apiKey = v.toString();
        } else {
            apiKey = "";
        }

        if (const QJsonValue v = o["region"]; v.isString()) {
            region = v.toString();
        } else {
            region = "";
        }
    }

    return { { apiKey, region } };
}

bool azure_config::saveToFile(const azure_config& c)
{
    QJsonDocument doc(QJsonObject({ { "region", c.region }, { "apikey", c.apiKey } }));

    QSaveFile f("./azure.json");
    f.open(QSaveFile::WriteOnly);
    f.write(doc.toJson(QJsonDocument::Indented));
    return f.commit();
}

AzureConfigWidget::AzureConfigWidget(QWidget* parent)
    : ServiceConfigWidget(parent)
{
    auto* form = new QFormLayout(this);

    auto config = azure_config::loadFromFile("./azure.json");

    if (!config.has_value()) {
        region = new QLineEdit("eastus", this);
        apiKey = new QLineEdit("b9885138792d4403a8ccf1a34553351d", this);
    } else {
        region = new QLineEdit(config->region, this);
        apiKey = new QLineEdit(config->apiKey, this);
    }

    auto* title = new QLabel("<b>Azure config</b>", this);

    title->setAlignment(Qt::AlignCenter);
    form->addRow(title);
    form->addRow("Location/Region", region);
    form->addRow("API Key", apiKey);

    form->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
    apiKey->setMinimumWidth(400);

    this->setLayout(form);
}

std::optional<std::string> AzureConfigWidget::save()
{
    if (!azure_config::saveToFile({ .apiKey = apiKey->text().simplified(), .region = region->text().simplified() })) {
        return { "sth is wrong" };
    } else {
        return {};
    }
}

bool Service_azure::initalize(QObject* parent, QNetworkAccessManager* networkAccessManager)
{
    this->networkAccessManager = networkAccessManager;
    this->setParent(parent);

    auto ret_config = azure_config::loadFromFile("./azure.json");
    if (!ret_config.has_value()) {
        if (!azure_config::saveToFile({ .apiKey = "b9885138792d4403a8ccf1a34553351d", .region = "eastus" })) {
            exit(1);
        };
    }

    request = new QNetworkRequest();
    request->setUrl(QUrl(QString("https://%1.tts.speech.microsoft.com/cognitiveservices/v1").arg(ret_config->region)));
    request->setRawHeader("User-Agent", "WhatEver");
    request->setRawHeader("Ocp-Apim-Subscription-Key", ret_config->apiKey.toLatin1());
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

Service_azure* Service_azure::Construct(QObject* parent, QNetworkAccessManager* networkAccessManager)
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
)",
        s.data());

    reply = networkAccessManager->post(*request, y.data());
    connect(reply, &QNetworkReply::finished, [this]() {
        player->setSourceDevice(reply);
        player->play();
    });

    connect(reply, &QNetworkReply::errorOccurred, this, &Service_azure::slotError);
    connect(reply, &QNetworkReply::sslErrors, this, &Service_azure::slotSslErrors);
    return std::nullopt;
}

ServiceConfigWidget* Service_azure::getConfigWidget(QWidget* HostWiget) { return new AzureConfigWidget(HostWiget); }

Service_azure::~Service_azure() = default;

void Service_azure::slotError(QNetworkReply::NetworkError e) { qDebug() << e; }

void Service_azure::slotSslErrors() { qDebug() << "ssl error"; }

void Service_azure::mediaErrorOccur(QMediaPlayer::Error error, const QString& errorString)
{
    qDebug() << "media " << error << errorString;
}

void Service_azure::mediaStatus(QMediaPlayer::MediaStatus status) { qDebug() << "status " << status; }