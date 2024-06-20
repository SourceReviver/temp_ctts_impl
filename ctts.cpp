#include "azure.h"
#include "ctts_main_window.h"
#include <QApplication>

int main(int argc, char* argv[])
{

    QApplication gui(argc, argv);
    (new ctts_mainwindow())->show();
    return QApplication::exec();
}
