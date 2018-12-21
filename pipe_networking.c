#include "pipe_networking.h"
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

char *WKP = "WKP";
int bSize = 1024;

/*=========================
  server_handshake
  args: int * to_client

  Performs the client side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe.

  returns the file descriptor for the upstream pipe.
  =========================*/
int server_handshake(int *to_client) {
  char buf[bSize];
  int sread;
  int swrite;
  if(mkfifo(WKP, 0644) < 0) {
    perror("failed to make wkp\n");
    return -1;
  }
  printf("waiting for connection...\n");
  sread = open(WKP, O_RDONLY);
  if(read < 0) {
    perror("failed to open wkp\n");
    return -1;
  }
  read(sread, buf, bSize);
  printf("received client message, opening pipe |%s|\n", buf);
  remove(WKP);
  swrite = open(buf, O_WRONLY);
  if(swrite < 0) {
    perror("failed to open pipe\n");
    return -1;
  }
  printf("connected, sending server message\n");
  write(swrite, "hi boi", 7);

  //to do
  
  printf("waiting for response...\n");
  read(sread, buf, bSize);
  if (strncmp(buf, "i see uuu", 10) != 0) {
    printf("wrong message\n");
    return -1;
  }
  printf("connected\n");
  printf("-------------------\n");
  while(1) {
    printf("waiting for response...\n");
    read(sread, buf, bSize);
    printf("message: %s\n", buf);
    int mSize = strlen(buf);
    char stuff[bSize];
    sprintf(stuff, "%d", mSize - 1); 
    write(swrite, stuff, sizeof(stuff));
    printf("sending processed data, %s\n", stuff);
  }
  *to_client = swrite;
  return sread;
}


/*=========================
  client_handshake
  args: int * to_server

  Performs the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.

  returns the file descriptor for the downstream pipe.
  =========================*/
int client_handshake(int *to_server) {
  char buf[bSize];
  char *pp = "private";
  int cread;
  int cwrite;
  if(mkfifo(pp, 0644) < 0) {
    perror("failed to make pp\n");
    return -1;
  }
  printf("sending |%s| to server...\n", pp);
  cwrite = open(WKP, O_WRONLY);
  if (cwrite < 0) {
    perror("failed open server pipe");
    return -1;
  }
  write(cwrite, pp, strlen(pp) + 1);
  cread = open(pp, O_RDONLY);
  if(cread < 0) {
    perror("failed to open pp\n");
    return -1;
  }
  printf("waiting for response...\n");
  read(cread, buf, 7);
  if(strncmp(buf, "hi boi", 7) != 0) {
    printf("wrong message\n");
    return -1;
  }
  printf("got server message, sending response...\n");
  remove(pp);
  write(cwrite,  "i see uuu", 10);
  printf("-------------------\n");
  while(1) {
    printf("Send something to the server:\n");
    fgets(buf, 100, stdin);
    write(cwrite, buf, sizeof(buf));
    printf("waiting for response...\n");
    read(cread, buf, bSize);
    printf("server message: word count is %s\n\n", buf);
  }
  *to_server = cwrite;
  return cread;
}
