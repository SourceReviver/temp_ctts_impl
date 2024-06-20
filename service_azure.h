#pragma once
#include "service.h"
#include <QtNetwork>
#include <QtMultimedia/QtMultimedia>
#include <memory>

class Service_azure : public Service
{
    Q_OBJECT

public:
    Service_azure() = default;
    bool initalize(QObject* parent, QNetworkAccessManager* networkAccessManager);
    static Service_azure* Construct(QObject* parent,
                                    QNetworkAccessManager* networkAccessManager);
    [[nodiscard]] std::optional<std::string> speak(QUtf8StringView s) override;
    [[nodiscard]] ServiceConfigUI* getConfigWidget(QWidget*) override;

    ~Service_azure() override;

protected:
    QNetworkAccessManager* networkAccessManager;
    QNetworkReply* reply;
    QMediaPlayer* player;
    QNetworkRequest* request;

private slots:
    void slotError(QNetworkReply::NetworkError e);
    void slotSslErrors();

    void mediaErrorOccur(QMediaPlayer::Error error, const QString& errorString);
    void mediaStatus(QMediaPlayer::MediaStatus status);
};
