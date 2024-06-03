CC?=gcc
RM?=rm -f

FILE_NAME=user

COMMON=../Common/common.o ../Common/linked_list.o

all: common $(AREAS) $(FILE_NAME).exe

$(FILE_NAME).exe: $(FILE_NAME).c
	$(CC) -o $(FILE_NAME).exe $(FILE_NAME).c $(COMMON) -lpthread

common: $(COMMON)
	cd ../Common && $(MAKE)

clean:
	$(RM) *.o *.exe
	cd ../Common && $(MAKE) clean
