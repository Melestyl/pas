#ifndef PAS_SERVER_H
#define PAS_SERVER_H

#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "../Common/common.h"

/**
 * @fn void send_nok(pid_t sender, int num)
 * @brief Send a NOK message to the sender
 * @param sender: pid of the sender
 * @param num: id of the message queue
 */
void send_nok(pid_t sender, int num);

void ask_areas();
void del_area();

#endif //PAS_SERVER_H
