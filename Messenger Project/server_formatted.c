#include <arpa/inet.h>
#include <netinet/in.h>
#include <openssl/err.h>
#include <openssl/ssl.h>
#include <pthread.h>
#include <resolv.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX_SIZE 50
#define NUM_CLIENT 50

#define FAIL -1

int n;
int nclient = 0;
int sock_desc, clilen;
int clientsock_list[MAX_SIZE];
SSL *ssl_list[MAX_SIZE];
char shared_buffer[256];
bool event = false;
pthread_mutex_t shared_buffer_mutex;

// bool killserv = false;

struct sockaddr_in serv_addr;

struct connection_thread_data {
  pthread_t threadid;
  int threadnum;
  char buffer[256];
  int newsockfd;
  SSL *ssl;
  struct sockaddr_in cli_addr;
};

void *connection_handler(void *threaddata) {

  struct connection_thread_data *data =
      (struct connection_thread_data *)threaddata;
  printf("Connection %d accepted with authentication.\n", data->threadnum);
  // once accepted connection, waits for message from client
  while (1) {

    bzero(data->buffer, 256);
    n = SSL_read(data->ssl, data->buffer, 255); // blocking read
    pthread_mutex_lock(&shared_buffer_mutex);
    if (n < 0) {
      ERR_print_errors_fp(stderr);
      ssl_list[data->threadnum] = NULL;
      clientsock_list[data->threadnum] = -1;
      nclient--;
      sprintf(shared_buffer, "User %d has left the chat.", data->threadnum);
      break;
    } else {

      bzero(shared_buffer, 256);
      event = true;
      printf("User %d : %s\n", data->threadnum, data->buffer);

      if (strcmp(data->buffer, "-1\n") == 0) {
        n = SSL_write(data->ssl, "-1\n", 3);
        ssl_list[data->threadnum] = NULL;
        clientsock_list[data->threadnum] = -1;
        nclient--;
        sprintf(shared_buffer, "User %d has left the chat.", data->threadnum);
        break;
      } else {
        sprintf(shared_buffer, "User %d : %s\n", data->threadnum, data->buffer);
      }
      pthread_mutex_unlock(&shared_buffer_mutex);
    }
  }

  pthread_mutex_unlock(&shared_buffer_mutex);
  SSL_free(data->ssl);
  close(data->newsockfd);
  printf("Exiting connection %d\n", data->threadnum);
  free(data);
  pthread_exit(NULL);
}

void *write_handler(void *threaddata) {
  while (1) {
    pthread_mutex_lock(&shared_buffer_mutex);
    if (event) {
      for (int i = 0; i < NUM_CLIENT; i++) {
        if (ssl_list[i] != NULL) {
          n = SSL_write(ssl_list[i], shared_buffer, strlen(shared_buffer));
          if (n < 0)
            printf("Error writting to client socket.\n");
        }
      }

      bzero(shared_buffer, 256);
      event = false;
    }
    pthread_mutex_unlock(&shared_buffer_mutex);
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
  method = TLSv1_2_server_method();
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
  SSL_CTX *ctx;
  X509 *cert; // pointer for certificate
  pthread_mutex_init(&shared_buffer_mutex, NULL);
  pthread_t write_thread;
  int newsockfd;

  ctx = InitCTX();
  if (SSL_CTX_use_certificate_file(ctx, "public.pem", SSL_FILETYPE_PEM) <= 0) {
    printf("Error setting the certificate file.\n");
    exit(0);
  }

  if (SSL_CTX_use_PrivateKey_file(ctx, "private.pem", SSL_FILETYPE_PEM) <= 0) {
    printf("Error setting the private key file.\n");
    exit(0);
  }

  if (!SSL_CTX_check_private_key(ctx)) {
    printf("Private key does not match!");
    exit(0);
  }

  for (int i = 0; i < MAX_SIZE; i++) {
    clientsock_list[i] = -1;
  }
  if ((sock_desc = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    printf("Failed creating socket.\n");

  if (setsockopt(sock_desc, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) <
      0)
    printf("setsockopt(SO_REUSEADDR) failed");
  bzero((char *)&serv_addr, sizeof(serv_addr));

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  serv_addr.sin_port = htons(8888);

  // bind address and port to the socket
  if (bind(sock_desc, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    printf("ERROR on binding.\n");

  listen(sock_desc, MAX_SIZE); // start listening on the server socket end.

  pthread_create(&(write_thread), NULL, write_handler, NULL);
  // int i = 0;
  while (1) {

    struct connection_thread_data *thread_data;
    SSL *ssl;

    // allocates memory for thread_data
    thread_data = malloc(sizeof(struct connection_thread_data));
    newsockfd = accept(sock_desc, (struct sockaddr *)&(thread_data->cli_addr),
                       &clilen); // blocking accept

    if (newsockfd < 0) {
      printf("ERROR on accepting connection.\n");

      // if there is an error, there is no need for the thread_data so free it
      free(thread_data);
    } else {

      // else pass value to the thread_data and use it to create a new thread
      // for handling connections

      ssl = SSL_new(ctx);
      SSL_set_fd(ssl, newsockfd);

      thread_data->threadnum = nclient;
      thread_data->newsockfd = newsockfd;
      thread_data->ssl = ssl;

      int ret;

      ret = SSL_accept(ssl);

      if (ret == FAIL) {
        ERR_print_errors_fp(stderr);
      }

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

      pthread_mutex_lock(&shared_buffer_mutex);
      clientsock_list[nclient] = newsockfd;
      ssl_list[nclient] = ssl;
      nclient++;
      pthread_mutex_unlock(&shared_buffer_mutex);

      pthread_create(&(thread_data->threadid), NULL, connection_handler,
                     (void *)thread_data); // dostuff
    }
  }

  return 0;
}
