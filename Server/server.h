#ifndef PAS_SERVER_H
#define PAS_SERVER_H

#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "../Common/common.h"

void send_nok();

/**
 * @fn void send_ok(pid_t sender, int mailbox)
 * @brief Send a OK message to the sender
 * @param sender: pid of the sender
 * @param mailbox: id of the message queue
 */
void send_ok(pid_t sender, int mailbox);

/**
 * @fn void ask_areas(pid_t sender, int mailbox)
 * @brief Send the list of areas to the sender
 * @param sender: pid of the sender
 * @param mailbox: id of the message queue
 */
void list_areas(pid_t sender, int mailbox);

/**
 * @fn void del_area(int area_segm_id)
 * @brief Delete an area and remove it from the list
 * @param area_segm_id: id of the shared memory segment
 */
void del_area(int area_segm_id);

#endif //PAS_SERVER_H
