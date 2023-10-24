#include "debug/tge_QtDebug.hpp"

namespace tge {  

    TgeDebug* TgeDebug::instance = nullptr;

    TgeDebug::~TgeDebug() {}

    void TgeDebug::staticMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
        Q_UNUSED(context);

        QByteArray localMsg = msg.toLocal8Bit();

        if(TgeDebug::instance && TgeDebug::instance->textEditConsole) {

            switch (type) {
                case QtDebugMsg:
                    instance->textEditConsole->append(QString("Debug: %1").arg(localMsg.constData()));
                    break;
                case QtInfoMsg:
                    instance->textEditConsole->append(QString("Info: %1").arg(localMsg.constData()));
                    break;
                case QtWarningMsg:
                    instance->textEditConsole->append(QString("Warning: %1").arg(localMsg.constData()));
                    break;
                case QtCriticalMsg:
                    instance->textEditConsole->append(QString("Critical: %1").arg(localMsg.constData()));
                    break;
                case QtFatalMsg:
                    instance->textEditConsole->append(QString("Fatal: %1").arg(localMsg.constData()));
                    abort();
            }
        } 
    }
}