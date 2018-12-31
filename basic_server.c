#include "pipe_networking.h"

static void sighandler(int signo) {
  if(signo == SIGINT) {
    remove("wkp");
    exit(0);
  }
}

int main() {
  signal(SIGINT, sighandler);
  int to_client;
  int from_client;
  char buf[BUFFER_SIZE];
  char stuff[BUFFER_SIZE];

  while(1){
    server_handshakeA(&from_client);
    int client_pid = fork();
    if(client_pid){
      close(from_client);
      remove("wkp");
    }
    else{
      server_handshakeB(&to_client, &from_client);
      while(read(from_client, buf, sizeof(buf))){
          printf("client %d message: %s", getpid(), buf);
          int mSize = strlen(buf);
          sprintf(stuff, "%d", mSize - 1);
          write(to_client, stuff, sizeof(stuff));
          printf("sending processed data: %s\n\n", buf);
      }
    }
  }
}
