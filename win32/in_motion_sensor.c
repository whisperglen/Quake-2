#include <freespace/freespace.h>
#include "in_motion_sensor.h"


#include <stdio.h>
#include <string.h>
#include <math.h>

#include "../client/client.h"

#ifdef WIN32
#include <windows.h>

void IN_MotionSensor_Thread(void);

int qlibfreespaceInitialized = 0 ;


static int CtrlHandler(DWORD fdwCtrlType) {
    return 1;
}

void addControlHandler() {
    if (!SetConsoleCtrlHandler( (PHANDLER_ROUTINE) CtrlHandler, TRUE )) {
        printf("Could not install control handler\n");
    }
}


FreespaceDeviceId device;

void IN_MotionSensor_Init(void)
{
	int rc;
	int numIds;
	struct freespace_message message;

	LPDWORD dwThreadId=0;
	int i;
	

	addControlHandler();
	
	// Initialize the freespace library
    rc = freespace_init();
    if (rc != FREESPACE_SUCCESS) {
        printf("Initialization error. rc=%d\n", rc);
		return;
	}

	 /** --- START EXAMPLE INITIALIZATION OF DEVICE -- **/
    rc = freespace_getDeviceList(&device, 1, &numIds);
    if (numIds == 0) {
        printf("freespaceInputThread: Didn't find any devices.\n");
        return;
    }

    rc = freespace_openDevice(device);
    if (rc != FREESPACE_SUCCESS) {
        printf("freespaceInputThread: Error opening device: %d\n", rc);
        return;
    }

    rc = freespace_flush(device);
    if (rc != FREESPACE_SUCCESS) {
        printf("freespaceInputThread: Error flushing device: %d\n", rc);
        return;
    }

	 memset(&message, 0, sizeof(message));
    if ( freespace_isNewDevice(device)) {
        message.messageType = FREESPACE_MESSAGE_DATAMODECONTROLV2REQUEST;
        message.dataModeControlV2Request.packetSelect = 3;
        message.dataModeControlV2Request.modeAndStatus |= 0 << 1;
    } else {
        message.messageType = FREESPACE_MESSAGE_DATAMODEREQUEST;
        message.dataModeRequest.enableUserPosition = 1;
        message.dataModeRequest.inhibitPowerManager = 1;
    }

    rc = freespace_sendMessage(device, &message);
    if (rc != FREESPACE_SUCCESS) {
        printf("freespaceInputThread: Could not send message: %d.\n", rc);
    }

	
	CreateThread(NULL, //Choose default security
				0, //Default stack size
				(LPTHREAD_START_ROUTINE)&IN_MotionSensor_Thread,
				//Routine to execute
				(LPVOID) &i, //Thread parameter
				0, //Immediately run the thread
				dwThreadId //Thread Id
	);

	qlibfreespaceInitialized = 1;
}

struct Vec3f {
    float x;
    float y;
    float z;
};

struct Quaternion {
    float x;
    float y;
    float z;
    float w;
};

float q_lengthSq(const struct Quaternion* q) {
    return ((q->w * q->w) + (q->x * q->x) + (q->y * q->y) + (q->z * q->z));
}


float q_length(const struct Quaternion* q) {
    return sqrtf(q_lengthSq(q));
}

void q_scale(struct Quaternion* out,
             const struct Quaternion* q,
             float scale) {
    out->w = q->w * scale;
    out->x = q->x * scale;
    out->y = q->y * scale;
    out->z = q->z * scale;
}

void q_normalize(struct Quaternion* out, const struct Quaternion* q) {
    float len = q_length(q);
    q_scale(out, q, 1.0f / len);
}

void q_quatFromUserFrame(struct Quaternion* quat,
                         const struct freespace_UserFrame* userFrame) {
    quat->w = userFrame->angularPosA;
    quat->x = userFrame->angularPosB;
    quat->y = userFrame->angularPosC;
    quat->z = userFrame->angularPosD;

    q_normalize(quat, quat);
}

void q_conjugate(struct Quaternion* out,
                 const struct Quaternion* q) {
    out->w = q->w;
    out->x = -q->x;
    out->y = -q->y;
    out->z = -q->z;
}








void q_toEulerAngles(struct Vec3f* out, const struct Quaternion* q) {
    float m11 = (2.0f * q->w * q->w) + (2.0f * q->x * q->x) - 1.0f;
    float m12 = (2.0f * q->x * q->y) + (2.0f * q->w * q->z);
    float m13 = (2.0f * q->x * q->z) - (2.0f * q->w * q->y);
    float m23 = (2.0f * q->y * q->z) + (2.0f * q->w * q->x);
    float m33 = (2.0f * q->w * q->w) + (2.0f * q->z * q->z) - 1.0f;

    out->x = atan2f(m23, m33);
    out->y = asinf(-m13);
    out->z = atan2f(m12, m11);
}

static void getEulerAnglesFromUserFrame(const struct freespace_UserFrame* user,
                                        struct Vec3f* eulerAngles) {
    struct Quaternion q;
    q_quatFromUserFrame(&q, user);

    // The Freespace quaternion gives the rotation in terms of
    // rotating the world around the object. We take the conjugate to
    // get the rotation in the object's reference frame.
    q_conjugate(&q, &q);

    // Convert quaternion to Euler angles
    q_toEulerAngles(eulerAngles, &q);
}

float motionSensorAngles[3];										
#define RADIANS_TO_DEGREES(rad) ((float) rad * (float) (180.0 / M_PI))
void IN_MotionSensor_Thread()
{
	int rc;
	struct freespace_message message;
	struct Vec3f eulerAngles;

	if (!qlibfreespaceInitialized)
		return ;

	while(1)
	{
		

		rc = freespace_readMessage(device, &message, 1/* 1 second timeout */);

		
         
		if (message.messageType == FREESPACE_MESSAGE_USERFRAME) 
		{
			getEulerAnglesFromUserFrame(&message.userFrame,&eulerAngles);

			motionSensorAngles[ROLL] =    RADIANS_TO_DEGREES(eulerAngles.x);
			motionSensorAngles[PITCH] = - RADIANS_TO_DEGREES(eulerAngles.y);
			motionSensorAngles[YAW] =   - RADIANS_TO_DEGREES(eulerAngles.z);
		
			}

	}
}


void IN_MotionSensor_Move(void)
{
	cl.viewangles[ROLL]  = motionSensorAngles[ROLL];
	cl.viewangles[PITCH] = motionSensorAngles[PITCH];
	cl.viewangles[YAW]   = motionSensorAngles[YAW];
}

#else
void IN_MotionSensor_Init(void){}
void IN_MotionSensor_Move(void){}
#endif
