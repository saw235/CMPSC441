#include "guihandler.h"

GuiHandler::GuiHandler(QObject *parent) :
    QObject(parent)
{
    b_exit = false;
    connected = false;
    ctx = InitCTX();    // initialize context
    ssl = SSL_new(ctx); // setup layer

    /*
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        emit errorMsg("Error opening socket");
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, NULL, sizeof(int)) < 0)
      {printf("setsockopt(SO_REUSEADDR) failed");}
    */
    bzero((char *)&serv_addr, sizeof(serv_addr));
}


void GuiHandler::connectToServer(QString ip, int port){

    b_exit = false;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
       { emit errorMsg("Error opening socket");return;}
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, NULL, sizeof(int)) < 0)
      {printf("setsockopt(SO_REUSEADDR) failed");}

    bzero((char *)&serv_addr, sizeof(serv_addr));



    // set server address and port
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(ip.toUtf8().constData());
    //serv_addr.sin_addr.s_addr = *(long *)(host->h_addr); //change to inet_addr(ipaadress_string);  such as "168.0.1.2"
    serv_addr.sin_port = htons(port);     // change this to a variable to get from GUI

    // connect socket to server
    if (::connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        //printf("Error connecting.\n");
        qDebug() << "Error connecting";
        emit errorMsg("Error connecting");
        //return;
    } else {
        //printf("Successfully connect to server.\n");
        qDebug() << "Successfull connection";
        emit errorMsg("Successfully connect to server");


    // bind layer and socket together
    ret = SSL_set_fd(ssl, sockfd);
    if (ret == 0) {
     ERR_print_errors_fp(stderr);
     return;
    }

    // connect client layer to the server's layer
    ret = SSL_connect(ssl);
    if (ret != 1) {
     ERR_print_errors_fp(stderr);
     return;
    }

    // printf("Connected with %s encryption\n", SSL_get_cipher(ssl));

    connected = true;
    pthread_create(&read_thread, NULL, read_handler, (void*)this);
    //pthread_create(&write_thread, NULL, write_handler, (void *)this);
    }
}

void GuiHandler::disconnect(){
    sendMsg("-1\n");  // HEHEHEHE
    SSL_shutdown(ssl);
    //close(sockfd);
    connected = false;
    b_exit = true;
    emit errorMsg("Disconnected from server");
}

void GuiHandler::sendMsg(QString msg){

    strcpy(wbuffer, msg.toUtf8().constData());
    n = SSL_write(ssl, wbuffer, strlen(wbuffer));
}

void *GuiHandler::read_handler(void *threadid) {
   GuiHandler *g = (GuiHandler *) threadid;
   while (1) {
    if (g->b_exit) {
      pthread_exit(NULL);
    }

    bzero(g->rbuffer, 256);
    g->n = SSL_read(g->ssl, g->rbuffer, 255);
    if (g->n < 0) {
      printf("Error getting messages from server");
      emit g->errorMsg("Error getting messages from server");
      pthread_exit(NULL);
    } else {
      //printf("\n%s", g->rbuffer);
      emit g->newMsg(g->rbuffer); // send the message to the GUI
    }
  }
}

//void *GuiHandler::write_handler(void *threadid) {
//  GuiHandler *g = (GuiHandler *) threadid;
//  while (1) {
//    if (g->b_exit) {
//      pthread_exit(NULL);
//    }
//    printf("\nPlease enter message: ");

//    bzero(g->wbuffer, 256);
//    fgets(g->wbuffer, 255, stdin);

//    g->n = SSL_write(g->ssl, g->wbuffer, strlen(g->wbuffer));
//  }
//}


SSL_CTX *GuiHandler::InitCTX(void) {
  const SSL_METHOD *method;
  SSL_CTX *ctx;

  // load cryptos, et.al.
  SSL_library_init();
  // Bring in and register error
  SSL_load_error_strings();
  // Create new client-method instance
  method = TLSv1_2_client_method();
  // Create new context
  ctx = SSL_CTX_new(method);

  if (ctx == NULL) {
    ERR_print_errors_fp(stderr);
    abort();
  }

  // return SSL context
  return ctx;
}

void GuiHandler::getConState(){
    emit this->connectionState(this->connected);
}
