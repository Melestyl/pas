#ifndef PAS_SERVER_H
#define PAS_SERVER_H

#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "../Common/common.h"

void send_nok();

/**
 * @fn void send_ok(pid_t sender, int num)
 * @brief Send a OK message to the sender
 * @param sender: pid of the sender
 * @param num: id of the message queue
 */
void send_ok(pid_t sender, int num);
void ask_areas();

/**
 * @fn void del_area(int area_segm_id)
 * @brief Delete an area and remove it from the list
 * @param area_segm_id: id of the shared memory segment
 */
void del_area(int area_segm_id);

#endif //PAS_SERVER_H
