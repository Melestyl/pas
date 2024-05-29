#include <unistd.h> 
#include <stdlib.h> 
#include <stdio.h> 
#include <string.h>

#include <sys/shm.h>
#include <errno.h>

struct area {
    pthread_mutex_t mutex;
    int shared_memory; // user pid or null
};
