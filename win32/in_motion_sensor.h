#ifndef IN_MOTION_SENSOR
#define IN_MOTION_SENSOR

#include "../client/client.h"
#include "winquake.h"


void IN_MotionSensor_Init(void);
void IN_MotionSensor_Move(usercmd_t *cmd);


#endif