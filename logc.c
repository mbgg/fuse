#include "logc.h"
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/time.h>
#include <unistd.h>

#define PORT 9111

int socks = -1;
int fd = 0;
struct timeval otv;

int log_connect(char* name, int port) 
{
	int con;
	struct sockaddr_in serveraddr;
	unsigned long addr;

	bzero(&serveraddr, sizeof(serveraddr));

	if (socks >= 0) {
		printf("dreckige scheisse!\n");
		return EXIT_FAILURE;
	}

	/* Socket erstellen */
	socks = socket(AF_INET, SOCK_STREAM, 0);
 
	printf("socks is %d!\n", socks);

	addr = inet_addr("127.0.0.1");
	memcpy( (char *)&serveraddr.sin_addr, &addr, sizeof(addr));
	serveraddr.sin_port = htons(PORT);
	serveraddr.sin_family = AF_INET;
 
	do { 
		con =connect(socks, (struct sockaddr*) &serveraddr, sizeof(serveraddr));
	} while (con != 0);
 
	return EXIT_SUCCESS;
}

int log_send(char *data) 
{
//	char *buffer;
	int len, bytes;

//	buffer = malloc(strlen(data));
//	strncpy(buffer, data, strlen(data));	
	len = strlen(data);

	bytes = send(socks , data, len, 0);
 	if (bytes != len) {
		printf("send() hat eine andere Anzahl von Bytes versendet als erwartet !!!!");
	} else {
//		printf("%d Bytes gesendet - ", bytes);
//		printf("Sende String: %s", buffer);
	}

//	free(buffer);

	return bytes;
}


void log_close() 
{
	close(socks);
	socks = -1;
}
 
int log_time_diff(struct timeval ntv, struct timeval *dtv)
{
	if(ntv.tv_sec < otv.tv_sec) 
		return -1;
	else if(ntv.tv_sec == otv.tv_sec && ntv.tv_usec >= otv.tv_usec) {
			dtv->tv_sec = 0;
			dtv->tv_usec = ntv.tv_usec - otv.tv_usec;
	} else { // ntv->tv_sec > otv.tv_sec
		if(otv.tv_usec > ntv.tv_usec) {
			dtv->tv_sec = ntv.tv_sec - 1 - otv.tv_sec;
			dtv->tv_usec = ntv.tv_usec + 1000000 - otv.tv_usec;
		} else {
			dtv->tv_sec = ntv.tv_sec - otv.tv_sec;
			dtv->tv_usec = ntv.tv_usec - otv.tv_usec;
		}
	}
	return 0;
}

#define FUSE_LOG "/tmp/fuse-log-fifo"

int log_open_fifo()
{
	gettimeofday(&otv, NULL);

	printf("log_open_fifo\n");
	fd = open(FUSE_LOG, O_WRONLY|O_CREAT, S_IRWXU|S_IRWXG );
	printf("fd is %d\n", fd);
	return 0;
}

int log_log(char *data, int pid)
{
	int ret = 0;
	char *string = malloc(strlen(data)+100);
	struct timeval ntv, difftv;

	gettimeofday(&ntv, NULL);

	if(log_time_diff(ntv, &difftv))
			sprintf(string, "not able to calc tdiff: %d.%.6d - %d.%.6d\n", otv.tv_sec, otv.tv_usec, ntv.tv_sec, ntv.tv_usec);

	sprintf(string, "%d:%d.%.6d:%s", pid, difftv.tv_sec, difftv.tv_usec, data);
	//sprintf(string, "%d@%d.%.6d # %d.%.6d - %d.%.6d\n", pid, difftv.tv_sec, difftv.tv_usec, ntv.tv_sec, ntv.tv_usec, otv.tv_sec, otv.tv_usec);

	ret = write(fd, string, strlen(string));
/*
	log_connect(NULL, -1);
	ret = log_send(data);
	log_close();
*/
	return ret;
}
