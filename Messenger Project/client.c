#include "openssl/bio.h"
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

bool b_exit = false;
char rbuffer[256];
char wbuffer[256];
SSL *ssl;
int sockfd;
int n;
void *read_handler(void *threadid) {
  while (1) {
    if (b_exit) {
      pthread_exit(NULL);
    }

    bzero(rbuffer, 256);
    n = SSL_read(ssl, rbuffer, 255);
    if (n < 0) {
      printf("Error getting messages from server");
    } else {
      printf("\n%s", rbuffer);
    }
  }
}
void *write_handler(void *sock) {

  while (1) {
    if (b_exit) {
      pthread_exit(NULL);
    }
    printf("\nPlease enter message: ");

    bzero(wbuffer, 256);
    fgets(wbuffer, 255, stdin);

    n = SSL_write(ssl, wbuffer, strlen(wbuffer));
  }
}

SSL_CTX *InitCTX(void) {
  SSL_METHOD *method;
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

int main() {

  struct sockaddr_in serv_addr;
  struct hostent *host;

  SSL_CTX *ctx;

  X509 *cert;

  int server;
  int ret;

  pthread_t read_thread;
  pthread_t write_thread;

  ctx = InitCTX();    // initialize context
  ssl = SSL_new(ctx); // setup layer

  // server = OpenConnection(hostname, atoi(portnum));

  // get host
  host = gethostbyname("localhost");

  // configure socket file descriptor
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0)
    printf("Error opening socket.\n");

  bzero((char *)&serv_addr, sizeof(serv_addr));

  // set server address and port
  serv_addr.sin_family = AF_INET;
  // serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  serv_addr.sin_addr.s_addr = *(long *)(host->h_addr);
  serv_addr.sin_port = htons(8888);

  // connect socket to server
  if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    printf("Error connecting.\n");
    return 0;
  } else {
    printf("Successfully connect to server.\n");
  }

  // bind layer and socket together
  ret = SSL_set_fd(ssl, sockfd);
  if (ret == 0) {
    ERR_print_errors_fp(stderr);
    abort();
  }

  // connect client layer to the server's layer
  ret = SSL_connect(ssl);
  if (ret != 1) {
    ERR_print_errors_fp(stderr);
    abort();
  }

  printf("Connected with %s encryption\n", SSL_get_cipher(ssl));

  cert = SSL_get_peer_certificate(ssl);
  if (cert != NULL) {
    char *line;
    printf("Server certificates:\n");
    line = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0);
    printf("Subject: %s\n", line);
    free(line);
    line = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0);
    printf("Issuer: %s\n", line);
    free(line);
    X509_free(cert);
  } else {
    printf("No certificates.\n");
  }

  pthread_create(&read_thread, NULL, read_handler, NULL);
  pthread_create(&write_thread, NULL, write_handler, (void *)&sockfd);
  while (1) {

    if (strcmp(rbuffer, "-1\n") == 0) {
      b_exit = true;
      break;
    }
  }

  return 0;
}
