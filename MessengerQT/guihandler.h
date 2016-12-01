#ifndef GUIHANDLER_H
#define GUIHANDLER_H

#include <QObject>

class GuiHandler
{
public:
    GuiHandler();

signals:
    void errorMsg(QString e);
    void newMsg(QString msg);

public slots:
    void connect(QString ip, QString port);
    void disconnect();
    void sendMsg(QString msg);
};

#endif // GUIHANDLER_H
