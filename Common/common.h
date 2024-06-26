#ifndef COMMON_H
#define COMMON_H

#include <unistd.h>
#include <stdlib.h> 
#include <stdio.h> 
#include <string.h>

#include <pthread.h>
#include <sys/shm.h>
#include <errno.h>

#include "protocol.h"
#include "linked_list.h"

#define MESSAGE_KEY 69 //FIXME: config.txt

#define LENGTH_NAME_AREA 16

#define bool char 
#define true 1
#define false 0

/**
 * @def DEBUG
 * @brief Debug macro (useful...)
 */
#define DEBUG(...) printf(__VA_ARGS__); getchar();

/**
 * @brief Read a config file and return the value of a key
 * @param path_name: path to the config file
 * @param searched_key: key to search
 * @param found_value: value to return
 * @return value of the key
 */
char* read_config(char* path_name, char* searched_key, char* found_value);

/**
 * @enum type
 * @brief Area type
 */
enum type {
	DESK,
	MEETING_ROOM,
};

/**
 * @struct area
 * @brief Area structure
 */
struct area {
	char name[LENGTH_NAME_AREA]; // name of the area
	enum type type; // desk or meeting room
	pthread_mutex_t mutex; // to prevent any conflicts
	int shared_memory; // user pid or null
	char path[120]; // path to shared memory (unique name of the shared_memory)
};

/**
 * @typedef area_t
 * @brief Typedef for area structure
 */
typedef struct area area_t;

#endif //COMMON_H