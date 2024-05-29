CC?=gcc
RM?=rm -f

FILE_NAME=server

AREAS=areas.o

COMMON=../Common/common.o

all: common $(AREAS) $(FILE_NAME).exe

areas.o: areas.c
	$(CC) -c areas.c

$(FILE_NAME).exe: $(FILE_NAME).c
	$(CC) -o $(FILE_NAME).exe $(FILE_NAME).c $(COMMON) $(AREAS) -lpthread

common: $(COMMON)
	cd ../Common && $(MAKE)

clean:
	$(RM) *.o *.exe
	cd ../Common && $(MAKE) clean
