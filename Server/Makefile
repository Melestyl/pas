CC?=gcc
RM?=rm -f

FILE_NAME=Server

COMMON=../Common/common.o

all: common $(FILE_NAME).exe

$(FILE_NAME).exe: $(FILE_NAME).c
	$(CC) -o $(FILE_NAME).exe $(FILE_NAME).c $(COMMON) -lpthread

common: $(COMMON)
	cd ../Common && $(MAKE)

clean:
	$(RM) *.o *.exe
	cd ../Common && $(MAKE) clean
