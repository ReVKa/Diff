#include "mainwindow.h"
#include <QApplication>

#include "itercore.h"

#include <QSplashScreen>
#include <QTime>
#include <QLabel>
#include <QColor>

//#define LOAD_SCREEN

#ifdef LOAD_SCREEN
static const int LOAD_TIME = 3 * 1000;
#endif

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
#ifdef LOAD_SCREEN
    QSplashScreen *splash = new QSplashScreen;
    splash->setPixmap(QPixmap("D:/icons/diff.png"));
    splash->show();

    qApp->processEvents();

    QTime time;
    time.start();
    while (time.elapsed() < LOAD_TIME)
    {
        const int progress = static_cast< double >( time.elapsed() ) / LOAD_TIME * 100.0;
        splash->showMessage(
                   QString("Загружено: %1%").arg( progress ),
                    Qt::AlignBottom | Qt::AlignRight
                );
        qApp->processEvents();
    }
#endif
    MainWindow w;
#ifdef LOAD_SCREEN
    splash->finish( &w);
#endif
    w.showMaximized();

    return a.exec();
}
