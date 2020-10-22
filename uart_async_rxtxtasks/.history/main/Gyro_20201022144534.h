#ifndef _GYRO_H
#define _GYRO_H

#include "esp_types.h"
#include "stdio.h"

typedef struct
{
    int32_t Gyro_Message;
    float Gyro_Temperature;
    double Angular_velocity_z;
} Gyro_Info_t;

extern uint8_t Gyro_Msg[9];

int Gyro_Data_validation(void);

int Gyro_Temperature_validation();

void Gyro_Info(void);

#endif
