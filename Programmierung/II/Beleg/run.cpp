#include "mainw.h"
#include <QApplication>
#include <QFile>
#include <QStyleFactory>
#include <QDebug>

// method to add a custom QSS theme to the application
static void loadQssTheme(QApplication &app, const QString &resourcePath) {
    // open file
    QFile f(resourcePath);
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Could not load theme:" << resourcePath;
        return;
    }
    // set style sheet
    app.setStyleSheet(QString::fromUtf8(f.readAll()));
}

// method to run the UI
int run_ui(int argc, char *argv[]) {
    Q_INIT_RESOURCE(ressources);
    QApplication app(argc, argv);

    // modern base style
    app.setStyle(QStyleFactory::create("Fusion"));
    // custom material QSS
    loadQssTheme(app, ":/ui/material.qss");

    MainWindow w;
    w.show();
    return app.exec();
}

int main(int argc, char *argv[]) {
    return run_ui(argc, argv);
}