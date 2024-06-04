CC?=gcc
RM?=rm -f

COMMUNICATION=shared_memory mailbox

all: common.o linked_list.o communication

common.o: common.c common.h
	$(CC) -c common.c

linked_list.o: linked_list.c linked_list.h
	$(CC) -c linked_list.c

communication:
	touch $(COMMUNICATION)

clean:
	$(RM) *.o
	$(RM) $(COMMUNICATION)