#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>

struct timeval otv, ntv, difftv;

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

void nt(int t) {
	usleep(t);
	gettimeofday(&ntv, NULL);
	log_time_diff(ntv, &difftv);
	printf("%d.%.6d - %d.%.6d = %d.%.6d\n", ntv.tv_sec, ntv.tv_usec, otv.tv_sec, otv.tv_usec, difftv.tv_sec, difftv.tv_usec);

}

void main()
{
	gettimeofday(&otv, NULL);

	nt(3*1000);
	nt(3*1000);
	nt(3*1000);
	nt(3*1000);
	nt(3*1000);
	nt(500*1000);
	nt(1000*1000);
	nt(100*1000);
	nt(100*1000);
	nt(100*1000);
	nt(100*1000);
	nt(100*1000);
	nt(100*1000);
	nt(100*1000);
	nt(100*1000);
	nt(100*1000);
}	
