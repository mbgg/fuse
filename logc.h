#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <signal.h>
 
int log_connect(char* addr, int port);

int log_send(char* data);

void log_close(void);

int log_log(char *data, int pid);

int log_open_fifo(void);
