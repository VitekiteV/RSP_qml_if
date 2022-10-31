#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "rsp_actionhandler.h"

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    RSP_ActionHandler handler;

    auto root_objects = engine.rootObjects();
    if (!root_objects.empty())
    {
        auto main_window = root_objects.first();
        if (main_window != nullptr)
        {
            QObject::connect(main_window, SIGNAL(signalQuit()), &handler, SLOT(onQuit()));
            QObject::connect(main_window, SIGNAL(signalSaveOptions(bool)), &handler, SLOT(onSaveOptions(bool)));
            QObject::connect(main_window, SIGNAL(updStatisticsReq()), &handler, SLOT(updStatisticsReq()));
            QObject::connect(&handler, SIGNAL(updStatisticsReply(int,int,int)), main_window, SIGNAL(updStatisticsReply(int,int,int)));
            QObject::connect(&handler, SIGNAL(signalGameResult(QString,QString,QString,QString,QString)), main_window, SIGNAL(gameRoundResult(QString,QString,QString,QString,QString)));
        }

        auto rock_button = root_objects.first()->findChild<QObject*>("RockButton");
        if (rock_button != nullptr)
            QObject::connect(rock_button, SIGNAL(buttonClicked(int)), &handler, SLOT(onGameButtonClicked(int)));

        auto scissors_button = root_objects.first()->findChild<QObject*>("ScissorsButton");
        if (scissors_button != nullptr)
            QObject::connect(scissors_button, SIGNAL(buttonClicked(int)), &handler, SLOT(onGameButtonClicked(int)));

        auto paper_button = root_objects.first()->findChild<QObject*>("PaperButton");
        if (paper_button != nullptr)
            QObject::connect(paper_button, SIGNAL(buttonClicked(int)), &handler, SLOT(onGameButtonClicked(int)));
    }

    QObject::connect(&app, &QGuiApplication::aboutToQuit, &handler, &RSP_ActionHandler::aboutToQuit);

    return app.exec();
}
