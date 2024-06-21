#pragma once

#include "azure.h"
#include "dummy.h"
#include <QFileInfo>
#include <QSaveFile>

#define CURRENT_SERVICE_FILE "./currentService.txt"

struct current_service {

    TextToSpeechService* getServiceAccordingToConfigInDisk()
    {
        TextToSpeechService* newService;
        if (this->retrive() == "azure") {
            newService = Azure::Service::Construct("./azure.json");
        } else {
            newService = new dummy::Service();
        }
        return newService;
    };

    QString retrive()
    {
        if (!QFileInfo::exists(CURRENT_SERVICE_FILE)) {
            save("azure");
        }
        QFile f(CURRENT_SERVICE_FILE);
        if (!f.open(QFile::ReadOnly)) {
            exit(1);
        }
        return f.readAll();
    }

    void save(const QUtf8StringView s)
    {
        QSaveFile f(CURRENT_SERVICE_FILE);
        if (!f.open(QFile::WriteOnly)) {
            exit(1);
        }
        f.write(s.data(), s.length());
        f.commit();
    }
};

Q_GLOBAL_STATIC(current_service, GlobalCurrentService)
