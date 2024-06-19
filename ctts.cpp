#include "ctts_mainwindow.h"

#include <QApplication>
#include <QDialog>
#include <QPushButton>
#include <QVBoxLayout>


int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    auto* mainwindow = new ctts_mainwindow();
    mainwindow->show();

    /*
    auto* configWindow = new QWidget();
    auto* layout = new QVBoxLayout(configWindow);
    auto* cur_config = ctts->getConfigWidget(configWindow);

    auto* acceptBtn = new QPushButton("accept", configWindow);

    layout->addWidget(cur_config);
    layout->addWidget(acceptBtn);

    configWindow->setLayout(layout);

    QObject::connect(acceptBtn, &QPushButton::clicked, configWindow, [&configWindow,&cur_config]
    {
        configWindow->close();
        cur_config->save();
    });

    configWindow->show();
    */

    return QApplication::exec();
}
