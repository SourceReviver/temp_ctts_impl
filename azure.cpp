#include "azure.h"
#include "global_network_access_manager.h"
#include <QFormLayout>
#include <QLabel>
#include <fmt/format.h>

namespace Azure {

bool Service::initalize(QObject* parent, const QString& configFilePath)
{
    this->setParent(parent);

    auto ret_config = AzureConfig::loadFromFile(configFilePath);
    if (!ret_config.has_value()) {
        if (!AzureConfig::saveToFile({ .apiKey = "b9885138792d4403a8ccf1a34553351d",
                .region = "eastus",
                .voiceShortName = "en-CA-ClaraNeural" })) {
            return false;
        };
        ret_config = AzureConfig::loadFromFile(configFilePath);
    }

    voiceShortName = ret_config->voiceShortName.toStdString();

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

    connect(player, &QMediaPlayer::errorOccurred, this, &Service::mediaErrorOccur);
    connect(player, &QMediaPlayer::mediaStatusChanged, this, &Service::mediaStatus);

    return true;
}

Service* Service::Construct(QObject* parent, const QString& configFilePath)
{
    auto azure = new Service();
    if (azure->initalize(parent, configFilePath)) {
        return azure;
    }
    return nullptr;
};

[[nodiscard]] std::optional<std::string> Service::speak(QUtf8StringView s)
{
    std::string y = fmt::format(R"(<speak version='1.0' xml:lang='en-US'>
    <voice name='{}'>
        {}
    </voice>
</speak>)",
        voiceShortName, s.data());

    reply = globalNetworkAccessManager->post(*request, y.data());
    connect(reply, &QNetworkReply::finished, this, [this]() {
        player->setSourceDevice(reply);
        player->play();
    });

    connect(reply, &QNetworkReply::errorOccurred, this, &Service::slotError);
    connect(reply, &QNetworkReply::sslErrors, this, &Service::slotSslErrors);
    return std::nullopt;
}

TextToSpeechConfigWidget* Service::getConfigWidget(QWidget* HostWiget) { return new ConfigWidget(HostWiget); }

Service::~Service() = default;

void Service::slotError(QNetworkReply::NetworkError e) { qDebug() << e; }

void Service::slotSslErrors() { qDebug() << "ssl error"; }

void Service::mediaErrorOccur(QMediaPlayer::Error error, const QString& errorString)
{
    qDebug() << "media " << error << errorString;
}

void Service::mediaStatus(QMediaPlayer::MediaStatus status) { qDebug() << "status " << status; }

std::optional<AzureConfig> AzureConfig::loadFromFile(const QString& p)
{
    if (!QFileInfo::exists(p)) {
        return {};
    }

    QFile f(p);

    if (!f.open(QFile::ReadOnly)) {
        return {};
    };

    AzureConfig ret {};

    auto json = QJsonDocument::fromJson(f.readAll());

    if (json.isObject()) {
        QJsonObject o = json.object();

        if (const QJsonValue v = o["apikey"]; v.isString()) {
            ret.apiKey = v.toString();
        } else {
            ret.apiKey = "";
        }

        if (const QJsonValue v = o["region"]; v.isString()) {
            ret.region = v.toString();
        } else {
            ret.region = "";
        }

        if (const QJsonValue v = o["voiceShortName"]; v.isString()) {
            ret.voiceShortName = v.toString();
        } else {
            ret.voiceShortName = "";
        }
    }

    return { ret };
}

bool Azure::AzureConfig::saveToFile(const AzureConfig& c)
{
    QJsonDocument doc(
        QJsonObject({ { "region", c.region }, { "apikey", c.apiKey }, { "voiceShortName", c.voiceShortName } }));

    QSaveFile f("./azure.json");
    f.open(QSaveFile::WriteOnly);
    f.write(doc.toJson(QJsonDocument::Indented));
    return f.commit();
}

ConfigWidget::ConfigWidget(QWidget* parent)
    : TextToSpeechConfigWidget(parent)
{
    auto* form = new QFormLayout(this);

    auto config = AzureConfig::loadFromFile("./azure.json");

    voiceList = new QComboBox(this);

    if (!config.has_value()) {
        region = new QLineEdit("eastus", this);
        apiKey = new QLineEdit("b9885138792d4403a8ccf1a34553351d", this);
    } else {
        region = new QLineEdit(config->region, this);
        apiKey = new QLineEdit(config->apiKey, this);
    }

    this->asyncVoiceListPopulating(config->voiceShortName);

    auto* title = new QLabel("<b>Azure config</b>", this);

    title->setAlignment(Qt::AlignCenter);
    form->addRow(title);
    form->addRow("Location/Region", region);
    form->addRow("API Key", apiKey);
    form->addRow("Voice", voiceList);
    voiceList->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    form->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
    apiKey->setMinimumWidth(400);

    this->setLayout(form);
}

std::optional<std::string> ConfigWidget::save()
{
    if (!AzureConfig::saveToFile({ .apiKey = apiKey->text().simplified(),
            .region = region->text().simplified(),
            .voiceShortName = voiceList->currentText() })) {
        return { "sth is wrong" };
    } else {
        return {};
    }
}

void ConfigWidget::asyncVoiceListPopulating(const QString& autoSelectThisName)
{

    voiceListRequest.reset(new QNetworkRequest());
    voiceListRequest->setRawHeader("User-Agent", "WhatEver");
    voiceListRequest->setUrl(QUrl(QString("https://%1.%2/voices/list").arg(this->region->text(), Azure::hostUrlBody)));
    voiceListRequest->setRawHeader("Ocp-Apim-Subscription-Key", this->apiKey->text().toLatin1());

    voiceListReply.reset(globalNetworkAccessManager->get(*voiceListRequest));

    connect(voiceListReply.get(), &QNetworkReply::finished, this, [this, autoSelectThisName]() {
        voiceList->clear();
        auto json = QJsonDocument::fromJson(this->voiceListReply->readAll());
        if (json.isArray()) {
            for (auto&& o : json.array()) {
                if (o.isObject()) {
                    if (auto r = o.toObject()["ShortName"]; r.isString()) {
                        if (auto s = r.toString(); !s.isNull()) {
                            voiceList->addItem(s);
                        }
                    }
                }
            }
        }
        if (auto i = voiceList->findText(autoSelectThisName); i != -1) {
            voiceList->setCurrentIndex(i);
        }
    });

    connect(voiceListReply.get(), &QNetworkReply::errorOccurred, this, [this](QNetworkReply::NetworkError e) {
        qDebug() << "f";
        this->voiceList->clear();
        this->voiceList->addItem("Failed to retrive voice list: " + QString::number(e));
    });
}
}