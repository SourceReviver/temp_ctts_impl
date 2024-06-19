#pragma once
#include "service.h"
#include <QtNetwork>
#include <QtMultimedia/QtMultimedia>

class Service_azure : public Service
{
    Q_OBJECT

public:
    Service_azure(QObject* parent, QNetworkAccessManager* networkAccessManager);
    [[nodiscard]] std::optional<std::string> speak(QUtf8StringView s) override;
    [[nodiscard]] ServiceConfigUI* getConfigWidget(QWidget*) override;

    ~Service_azure() override;

private:
    QNetworkAccessManager* networkAccessManager;
    QNetworkReply* reply = nullptr;
    QMediaPlayer* player;
    QNetworkRequest* request;

private slots:
    void slotError(QNetworkReply::NetworkError e);
    void slotSslErrors();

    void mediaErrorOccur(QMediaPlayer::Error error, const QString& errorString);
    void mediaStatus(QMediaPlayer::MediaStatus status);
};
