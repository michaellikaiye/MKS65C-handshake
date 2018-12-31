#include "pipe_networking.h"

void server_handshakeA(int *from_client){
  mkfifo("wkp", 0644);
  *from_client = open("wkp", O_RDONLY);
}

int server_handshakeB(int *to_client, int *from_client) {
  char name[HANDSHAKE_BUFFER_SIZE];
  char msg[HANDSHAKE_BUFFER_SIZE];

  read(*from_client, name, sizeof(name));
  printf("Server read \"%s\"\n", name);

  *to_client = open(name, O_WRONLY);

  write(*to_client, ACK, sizeof(ACK));
  printf("Server wrote \"%s\"\n", ACK);

  read(*from_client, msg, sizeof(msg));
  printf("Server read \"%s\"\n", msg);
  printf("connected\n---------------\n");

  return *from_client;
}

int client_handshake(int *to_server) {
  char pid[HANDSHAKE_BUFFER_SIZE];
  char msg[HANDSHAKE_BUFFER_SIZE];

  sprintf(pid, "%d", getpid());

  mkfifo(pid, 0644);

  *to_server = open("wkp", O_WRONLY);

  write(*to_server, pid, sizeof(pid));
  printf("Client wrote \"%s\"\n", pid);

  int from_server = open(pid, O_RDONLY);

  read(from_server, msg, sizeof(msg));
  printf("Client read \"%s\"\n", msg);

  remove(pid);

  write(*to_server, ACK, sizeof(ACK));
  printf("Client wrote \"%s\"\n", ACK);

  printf("connected\n---------------\n");

  return from_server;
}
