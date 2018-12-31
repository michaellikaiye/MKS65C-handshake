#include "pipe_networking.h"


int main() {
  int to_server;
  int from_server;
  from_server = client_handshake(&to_server);
  char buf[BUFFER_SIZE];
  while(1) {
    printf("Send something to the server:\n");
    fgets(buf, 100, stdin);
    write(to_server, buf, sizeof(buf));
    printf("waiting for response...\n");
    read(from_server, buf, sizeof(buf));
    printf("server message: %s\n\n", buf);
  }
}
