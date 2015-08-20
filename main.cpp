#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlComponent>
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QQmlApplicationEngine engine;

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    QObject *rootObject = engine.rootObjects().first();
    QObject *tabViewObject = rootObject->findChild<QObject*>("tb");
    QQmlComponent component(&engine, QUrl(QStringLiteral("qrc:/Tab.qml")));


    ifstream file("C:/Swetha/CPUInfo.txt");

    if( file.is_open() )
    {
        int count = 0;
        string line="";
        QObject* tabObject = NULL;
        QObject* tabContent = NULL;
        QVariant returnedValue;
        QString fullText = "";

        while( getline( file, line ) )
        {
            stringstream ss(line);
            string word;     

            while( getline(ss, word, ' ') )
            {
                if( word == "processor" )
                {
                    if(tabContent && !fullText.isEmpty())
                    {
                        tabContent->setProperty("text", fullText);
                        fullText = "";
                    }
                    QString procCount = QString::number(count);
                    QVariant title = "processor " + procCount ;
                    QMetaObject::invokeMethod(tabViewObject, "addTab",
                    Q_RETURN_ARG(QVariant, returnedValue),
                    Q_ARG(QVariant, title),
                    Q_ARG(QVariant, QVariant::fromValue(&component)));
                    tabObject = qvariant_cast<QObject *>(returnedValue);
                    tabObject->setProperty("active", "true");
                    tabContent = tabObject->findChild<QObject*>("tb1");
                    count++;
                }
            }

            fullText += "<br>";
            fullText += QString::fromStdString(line);
        }

        if(tabContent && !fullText.isEmpty())
        {
            tabContent->setProperty("text", fullText);
        }


        file.close();
    }

    return app.exec();
}
