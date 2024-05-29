#ifndef PAS_SERVER_H
#define PAS_SERVER_H

#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "../Common/common.h"

void send_nok();
void ask_areas();
void del_area();

#endif //PAS_SERVER_H
