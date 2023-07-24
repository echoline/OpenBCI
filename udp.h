#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <netdb.h>
#include <sys/socket.h>

int init_udp_listen(int);
int accept_connection(int);
