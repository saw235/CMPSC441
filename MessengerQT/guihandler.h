#ifndef GUIHANDLER_H
#define GUIHANDLER_H

#include <QObject>
#include "openssl/err.h"
#include "openssl/ssl.h"
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <resolv.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <QDebug>
#include "arpa/inet.h"

class GuiHandler : public QObject
{
    Q_OBJECT

public:
    explicit GuiHandler(QObject *parent = 0);

signals:
    void errorMsg(QVariant e);
    void newMsg(QVariant msg);

public slots:
    void connectToServer(QString ip, int port);
    void disconnect();
    void sendMsg(QString msg);

private:
    static void  *read_handler(void *threadid);
    //static void  *write_handler(void *threadid);
    SSL_CTX *InitCTX(void);

    struct sockaddr_in serv_addr;
    struct hostent *host;
    SSL_CTX *ctx;
    X509 *cert;
    int server;
    int ret;
    pthread_t read_thread;
    pthread_t write_thread;
    bool b_exit;
    char rbuffer[256];
    char wbuffer[256];
    SSL *ssl;
    int sockfd;
        int n;


};

#endif // GUIHANDLER_H
