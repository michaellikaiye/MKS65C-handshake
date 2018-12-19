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
  int read;
  int write;
  if(mkfifo(WKP, 0644) < 0) {
    perror("failed to make wkp\n");
    return -1;
  }
  printf("waiting...\n");
  read = open(WKP, O_RDONLY);
  if(read < 0) {
    perror("failed to open wkp\n");
    return -1;
  }
  read(read, buf, bSize);
  printf("received client message, opening pipe %s\n", buf);
  remove(WKP);
  write = open(buf, O_WRONLY);
  if(write < 0) {
    perror("failed to open pipe"\n);
    return -1;
  }
  printf("connected, sending message\n");
  write(write, "hi boi", 7);
  printf("waiting for response...\n");
  read(read, buf, bSize);
  if (
}


/*=========================
  client_handshake
  args: int * to_server

  Performs the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.

  returns the file descriptor for the downstream pipe.
  =========================*/
int client_handshake(int *to_server) {
  mkfifo("private",0644);
  printf("pipe created\n");
  fd = open ("private", O_RDONLY);
  printf("pipe opened: %d\n", fd);
  return 0;
}



  #include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "pipe_networking.h"

char *WKP = "pkfreeze";

/*=========================
  server_handshake
  args: int * to_client

  Performs the client side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe.

  returns the file descriptor for the upstream pipe.
  =========================*/
int server_handshake(int *to_client)
{
    char buf[256];
    int read_end, write_end;

    if (mkfifo(WKP, 0644) < 0 && errno != EEXIST)
    {
        perror("Failed to create known named pipe");
        return -1;
    }

    printf("Waiting for connection...\n");
    read_end = open(WKP, O_RDONLY);
    if (read_end < 0)
    {
        perror("Failed to open known named pipe you just created???");
        return -1;
    }

    read(read_end, buf, 256);

    printf("Received message from a client, opening pipe \"%s\"...\n", buf);
    remove(WKP);
    write_end = open(buf, O_WRONLY);
    if (write_end < 0)
    {
        perror("Could not open pipe, handshake aborted");
        return -1;
    }

    printf("Communication established, sending message to client...\n");
    write(write_end, "WhiteAlbum", 10);

    printf("Waiting for response from client...\n");
    read(read_end, buf, 256);
    if (strncmp(buf, "WeepsGently", 11) != 0)
    {
        printf("Unknown message received, aborting handshake.\n");
        return -1;
    }
    printf("Handshake successful!\n");

    *to_client = write_end;
    return read_end;
}


/*=========================
  client_handshake
  args: int * to_server

  Performs the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.

  returns the file descriptor for the downstream pipe.
  =========================*/
int client_handshake(int *to_server)
{
    char buf[255];
    char *pipename = "pkfire";
    int read_end, write_end;
    if (mkfifo(pipename, 0644) < 0 && errno != EEXIST)
    {
        perror("Failed to create named pipe");
        return -1;
    }

    printf("Attempting connection, sending private pipe name to server...\n");
    write_end = open(WKP, O_WRONLY);
    if (write_end < 0)
    {
        perror("Failed open the server's pipe");
        return -1;
    }
    write(write_end, pipename, strlen(pipename) + 1);

    read_end = open(pipename, O_RDONLY);
    if (read_end < 0)
    {
        perror("Failed to open known named pipe you just created???");
        return -1;
    }

    printf("Waiting for response from server...\n");
    read(read_end, buf, 10);
    if (strncmp(buf, "WhiteAlbum", 10) != 0)
    {
        printf("Unknown message received, aborting handshake.");
        return -1;
    }

    printf("Received response from server, sending response...\n");
    remove(pipename);
    write(write_end, "WeepsGently", 11);

    *to_server = write_end;
    return read_end;
}
