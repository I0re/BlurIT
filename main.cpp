#include "mainwindow.h"
#include <QApplication>
#include <QVBoxLayout>
#include <QFile>
#include <memory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    std::unique_ptr<MainWindow> w = std::make_unique<MainWindow>();

    w->setWindowIcon(QIcon(":/Assets/Assets/logo.ico"));

    //QSS stylesheet
    QFile styleFile(":/Assets/Styles/SyNet.qss");
    styleFile.open(QFile::ReadOnly);
    QString style = QLatin1String(styleFile.readAll());
    w->setStyleSheet(style);

    w->setMinimumSize(750, 750);
    w->show();

    return a.exec();
}
