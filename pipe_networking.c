#include "pipe_networking.h"

/*=========================
  server_handshake
  args: int * to_client

  Performs the client side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe.

  returns the file descriptor for the upstream pipe.
  =========================*/
int server_handshake(int *to_client) {
  mkfifo(WKP, 0644);
  printf("Reading from client...\n");
  int from_client = open(WKP, O_RDONLY);
  char received[100] = {'\0'};
  int i = read(from_client, received, sizeof(received));
  if (i){
    printf("Receive success: %s\n", received);
  }
  else {
    printf("Error: %s\n", strerror(errno));
  }
  remove(WKP);
  printf("Sending to client...\n");
  *to_client = open(received, O_WRONLY);
  write(*to_client, ACK, strlen(ACK));
  printf("Reading from client again...\n");
  i = read(from_client, received, sizeof(received));
  if (i){
    printf("Received message: %s\n", received);
  }
  else {
    printf("Error: %s\n", strerror(errno));
  }
  return from_client;
}


/*=========================
  client_handshake
  args: int * to_server
x
  Performs the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.

  returns the file descriptor for the downstream pipe.
  =========================*/
int client_handshake(int *to_server) {
  int pidNum = getpid();
  char pid[100];
  char message[100] = {'\0'};
  sprintf(pid, "%d", pidNum);
  mkfifo(pid, 0644);
  printf("PID: %d\n", pidNum);
  *to_server = open(WKP, O_WRONLY);
  printf("Writing to server...\n");
  write(*to_server, pid, strlen(pid));
  printf("Reading from server...\n");
  int from_server = open(pid, O_RDONLY);
  int i = read(from_server, message, sizeof(message));
  if (i){
    printf("Received message: %s\n", message);
  }
  else {
    printf("Error: %s\n", strerror(errno));
  }
  remove(pid);
  printf("Writing to server again...\n");
  write(*to_server, ACK, strlen(ACK));
  return from_server;
}
