#pragma once

#include <QDebug>
#include <QTextEdit>

namespace tge {
    class TgeDebug {
        public:
            TgeDebug() { instance = this; }
            ~TgeDebug();

            TgeDebug(const TgeDebug &) = delete;
	        		TgeDebug &operator=(const TgeDebug &) = delete;

            static void staticMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg); 

            void setTextEdit(QTextEdit* textEdit) {
                textEditConsole = textEdit;
            }

        private:
            static TgeDebug* instance;
            QTextEdit* textEditConsole;  
    };
}