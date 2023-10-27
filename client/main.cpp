#include "loginwindow.h"
#include <QApplication>
#include <QSettings>
#include <QLoggingCategory>



void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QSettings *settings = new QSettings("config.ini", QSettings::IniFormat);
    qInstallMessageHandler(messageHandler);
    Client *client = new Client(settings);
    LoginWindow *logWin = new LoginWindow(client);
    logWin->show();
    return a.exec();
}

void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    static QString logFileName;
    static QtMsgType logLevel;

    if(logFileName.isEmpty() || logLevel == 0)
    {
        QSettings settings("config.ini", QSettings::IniFormat);
        logFileName = settings.value("logFile").toString();
        QString level = settings.value("logLevel").toString();
        if (level == "info") {
            logLevel = QtInfoMsg;
        } else if (level == "warning") {
            logLevel = QtWarningMsg;
        } else if (level == "critical") {
            logLevel = QtCriticalMsg;
        } else {
            logLevel = QtDebugMsg;
        }
    }
    if(type == logLevel)
    {
        QFile logFile(logFileName);
        if(logFile.open(QIODevice::WriteOnly | QIODevice::Append))
        {
            QTextStream out(&logFile);
            out << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz ");
            switch (type)
            {
            case QtInfoMsg:     out << "INFO "; break;
            case QtDebugMsg:    out << "DEBUG"; break;
            case QtWarningMsg:  out << "WARNING"; break;
            case QtCriticalMsg: out << "CRITICAL"; break;
            }
            out << context.function << ": " << context.line << ": "
                << msg << "\n";
            out.flush();
            logFile.close();

        }
    }
}
