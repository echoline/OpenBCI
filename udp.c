#include "udp.h"

int init_udp_listen(int port) {
	struct sockaddr_in inet_addr;
	int one = 1;
	int listener;

	if ((listener = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("socket");
		return -1;
	}
	if (setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(int)) == -1) {
		perror("setsockopt");
		return -1;
	}
	inet_addr.sin_family = AF_INET ;
	inet_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	inet_addr.sin_port = htons(port);
	if (bind(listener, (struct sockaddr*)&inet_addr, sizeof(struct sockaddr)) < 0) {
		perror("bind") ;
		return -1;
	}

	return listener;
}
