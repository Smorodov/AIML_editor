#include <QApplication>
#include <QFile>
#include <QTextStream>
#include "mainwindow.h"

/*
int main(int argv, char **args)
{
    QApplication app(argv, args);

    QFile f(":qdarkstyle/style.qss");

    if (!f.exists())
    {
        printf("Unable to set stylesheet, file not found\n");
    }
    else
    {
        f.open(QFile::ReadOnly | QFile::Text);
        QTextStream ts(&f);
        app.setStyleSheet(ts.readAll());
    }

    CodeEditor editor;
    editor.setWindowTitle(QObject::tr("AIML Editor"));
    editor.show();

    return app.exec();
}
*/

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    
    QFile f(":qdarkstyle/style.qss");

    if (!f.exists())
    {
        printf("Unable to set stylesheet, file not found\n");
    }
    else
    {
        f.open(QFile::ReadOnly | QFile::Text);
        QTextStream ts(&f);
        app.setStyleSheet(ts.readAll());
    }

    MainWindow window;
    //window.resize(1024, 768);
    window.show();
    return app.exec();
}

