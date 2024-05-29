CC?=gcc
RM?=rm -f

COMMUNICATION=shared_memory mailbox

all: common.o communication

common.o: common.c common.h
	$(CC) -c common.c

communication:
	touch $(COMMUNICATION)

clean:
	$(RM) *.o
	$(RM) $(COMMUNICATION)