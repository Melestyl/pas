CC?=gcc
RM?=rm -f

all: common.o shared_memory mailbox

common.o: common.c common.h
	$(CC) -c common.c

shared_memory:
	touch shared_memory
mailbox:
	touch mailbox

clean:
	$(RM) *.o