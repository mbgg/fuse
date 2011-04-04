#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include <string.h>
#include <arpa/inet.h>
#include <signal.h> 

#define PORT 9111

void ignore(int sig) {
        printf("Signal %d ignored\n", sig);
}

int main(void) {
	struct sockaddr_in saddr;
	int lsd = socket(AF_INET, SOCK_STREAM, 0);
 
	(void) signal(SIGINT|SIGPIPE, ignore);

	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(PORT);
	saddr.sin_addr.s_addr = htons(INADDR_ANY);
 
	bind(lsd, (struct sockaddr*) &saddr, sizeof(saddr));
 
	if (listen(lsd, 10) == -1)
		printf("error listening on socket %d\n", lsd);
	else
		printf("listen on socket %d\n", lsd);
 
	while(1) {
		struct sockaddr_in clientaddr;
		char buffer[500];
		bzero(buffer, sizeof(buffer));
 
		socklen_t clen = sizeof(clientaddr);

		int csd = accept(lsd, (struct sockaddr*) &clientaddr, &clen);
		if (csd < 0) {
			printf("error accepting\n");
			continue;
		}
 
		//int bytes = recv(csd, buffer, sizeof(buffer), 0);
		int bytes = read(csd, buffer, sizeof(buffer));
		if (bytes) {
			printf("%s", buffer);
		}
		else {
			printf("#");
		}

		close(csd);
	}

	return EXIT_SUCCESS;
 
}
