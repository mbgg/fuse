#include "logc.h"
#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#define __USE_GNU
#include <fcntl.h>

int fd;

void wr(char * str) {
	int i;

	for(i=0; i<100; i++)
		write(fd, str, strlen(str));
}

int main()
{
	pid_t pid, apid;
	int i;
	struct timeval tv;
	gettimeofday(&tv, NULL);
	printf("seconds past = %d.%d\n", tv.tv_sec, tv.tv_usec);

/*	log_open_fifo();
	log_log("hallo\n");
	log_log("n\n");
	log_log("ljkdjajslfjasdlfjañsdjflajsdfljsadlfjsldjflsajfljslfjslj\n");
	log_log("the end\n");

	gettimeofday(&tv, NULL);
	printf("seconds past = %d.%d\n", tv.tv_sec, tv.tv_usec);
*/

	fd = open("scalus.tmp", O_CREAT|O_RDWR|O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);//|O_DIRECT);
//	chmod("scalus.tmp", S_IRUSR|S_IXUSR|S_IWUSR);

	for(i=0; i<10000; i++) {
		write(fd, (char*)&i, 1);
	}
	fsync(fd);

	pid = fork();
	if (pid == -1) { perror("fork"); exit(EXIT_FAILURE); }
	if (pid == 0) {
		apid = getpid();
		printf("child writes - pid %d\n", apid);
		wr("child");
		sleep(1);
		wr("child");
		printf("child ends\n");
		close(fd);
	} else {
		apid = getpid();
		printf("parent writes - pid %d\n", apid);
		wr("parent");
		usleep(500*1000);
		wr("parent");
		printf("parent ends\n");
		close(fd);
	}

	return 0;
}
