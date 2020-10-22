#ifndef _GYRO_H
#define _GYRO_H

#include "esp_types.h"
#include "stdio.h"

extern uint8_t Gyro_Msg[9];
extern Gyro_Info_t Gyro_Infomation;

int Gyro_Data_validation(void);

int Gyro_Temperature_validation();

void Gyro_Info(void);

#endif
