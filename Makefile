project: grep.o source.o buffer.o
	cc -Wall grep.o source.o buffer.o -o project
grep.o: grep.c source.h buffer.h header.h
	cc -c grep.c
buffer.o: buffer.c buffer.h
	cc -c buffer.c
source.o: source.c source.h
	cc -c source.c
