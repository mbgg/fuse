all:
	gcc -c -o logc.o logc.c
	gcc -o logd logd.c
	gcc -o test-log test-log.c logc.c
	gcc -o ttest ttest.c
	
	gcc -DHAVE_CONFIG_H -I. -I../fuse-2.8.5/include  -I../fuse-2.8.5/include -D_FILE_OFFSET_BITS=64 -D_REENTRANT   -Wall -W -Wno-sign-compare -Wstrict-prototypes -Wmissing-declarations -Wwrite-strings -g -O2 -fno-strict-aliasing -MT fusexmp.o -MD -MP -MF .deps/fusexmp.Tpo -c -o fusexmp.o fusexmp.c
	/bin/sh ../fuse-2.8.5/libtool --tag=CC   --mode=link gcc  -Wall -W -Wno-sign-compare -Wstrict-prototypes -Wmissing-declarations -Wwrite-strings -g -O2 -fno-strict-aliasing   -o fusexmp fusexmp.o logc.o ../lib/libfuse.la -pthread -lrt -ldl 

clean:
	rm fusexmp *.o logd test-log ttest
