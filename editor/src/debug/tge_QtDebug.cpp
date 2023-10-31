#include "debug/tge_QtDebug.hpp"
#include <QScrollBar>

namespace tge {  

    TgeDebug* TgeDebug::instance = nullptr;

    TgeDebug::~TgeDebug() {}

    void TgeDebug::staticMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
        Q_UNUSED(context);

        if(TgeDebug::instance && TgeDebug::instance->textEditConsole) {
            QString formattedMessage;

            switch (type) {
                case QtDebugMsg:
                    formattedMessage = QString("Debug: %1").arg(msg);
                    break;
                case QtInfoMsg:
                    formattedMessage = QString("Info: %1").arg(msg);
                    break;
                case QtWarningMsg:
                    formattedMessage = QString("Warning: %1").arg(msg);
                    break;
                case QtCriticalMsg:
                    formattedMessage = QString("Critical: %1").arg(msg);
                    break;
                case QtFatalMsg:
                    formattedMessage = QString("Fatal: %1").arg(msg);
                    abort();
            }

            // Ensure that newline characters are correctly interpreted
            formattedMessage.replace("\\n", "\n");

            instance->textEditConsole->moveCursor(QTextCursor::End);
            instance->textEditConsole->insertPlainText(formattedMessage);
            instance->textEditConsole->append(""); // Move to a new line
            instance->textEditConsole->moveCursor(QTextCursor::End);

            // Ensure the latest text is visible
            QScrollBar *scrollBar = instance->textEditConsole->verticalScrollBar();
            scrollBar->setValue(scrollBar->maximum());
        } 
    }
}
