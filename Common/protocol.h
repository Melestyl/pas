#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <unistd.h>

/**
 * @def MAX_LENGTH_DATA
 * @brief Maximum length of data in a message
 */
#define MAX_LENGTH_DATA 200

/**
 * @def message
 * @param mtype: type of the message
 * @param sender: pid sender
 * @param code: request code
 * @param data: request data
 * @brief Structure of a message used in a mailbox
 */
struct message{
    long mtype;
    pid_t sender;
    char code;
    char data[MAX_LENGTH_DATA];
};

/**
 * @def OK
 * @brief OK code
 */
#define OK 1

/**
 * @def NOK
 * @brief NOK code
 */
#define NOK 2

/**
 * @def ASK_AREAS
 * @brief Ask the list of areas 
 */
#define ASK_AREAS 3

/**
 * @def LIST_AREAS
 * @brief Return the list of areas 
 */
#define LIST_AREAS 4

/**
 * @def CREATE_AREA
 * @brief Create an area with a name and a type
 */
#define CREATE_AREA 5

/**
 * @def DEL_AREA
 * @brief Delete an area with its name
 */
#define DEL_AREA 6

#endif