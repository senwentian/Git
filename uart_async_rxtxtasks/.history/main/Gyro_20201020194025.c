#include "Gyro.h"
#define Gyro_scale_factor    4.701932

uint8_t Gyro_Msg[9];
uint8_t Gyro_validation[7];
int32_t Gyro_Message;
uint16_t Gyro_Temperature;
double Angular_velocity_z;

uint8_t test;

int Gyro_Data_validation(void)
{
    Gyro_validation[0] = ((Gyro_Msg[0] & 0x40) ^ (Gyro_Msg[1] & 0x40) ^ (Gyro_Msg[2] & 0x40) ^ (Gyro_Msg[3] & 0x40) ^ (Gyro_Msg[4] & 0x40));
    Gyro_validation[1] = ((Gyro_Msg[0] & 0x20) ^ (Gyro_Msg[1] & 0x20) ^ (Gyro_Msg[2] & 0x20) ^ (Gyro_Msg[3] & 0x20) ^ (Gyro_Msg[4] & 0x20));
    Gyro_validation[2] = ((Gyro_Msg[0] & 0x10) ^ (Gyro_Msg[1] & 0x10) ^ (Gyro_Msg[2] & 0x10) ^ (Gyro_Msg[3] & 0x10) ^ (Gyro_Msg[4] & 0x10));
    Gyro_validation[3] = ((Gyro_Msg[0] & 0x08) ^ (Gyro_Msg[1] & 0x08) ^ (Gyro_Msg[2] & 0x08) ^ (Gyro_Msg[3] & 0x08) ^ (Gyro_Msg[4] & 0x08));
    Gyro_validation[4] = ((Gyro_Msg[0] & 0x04) ^ (Gyro_Msg[1] & 0x04) ^ (Gyro_Msg[2] & 0x04) ^ (Gyro_Msg[3] & 0x04) ^ (Gyro_Msg[4] & 0x04));
    Gyro_validation[5] = ((Gyro_Msg[0] & 0x02) ^ (Gyro_Msg[1] & 0x02) ^ (Gyro_Msg[2] & 0x02) ^ (Gyro_Msg[3] & 0x02) ^ (Gyro_Msg[4] & 0x02));
    Gyro_validation[6] = ((Gyro_Msg[0] & 0x01) ^ (Gyro_Msg[1] & 0x01) ^ (Gyro_Msg[2] & 0x01) ^ (Gyro_Msg[3] & 0x01) ^ (Gyro_Msg[4] & 0x01));
    test = ((Gyro_validation[0] | Gyro_validation[1] | Gyro_validation[2] | Gyro_validation[3] | Gyro_validation[4] | Gyro_validation[5] | Gyro_validation[6]) & 0x7F);
    printf("data validation is %d\n", test);
    if(test == Gyro_Msg[5])
        return 1;
    else
        return 0;
}

int Gyro_Temperature_validation()
{
    Gyro_validation[0] = ((Gyro_Msg[6] & 0x40) ^ (Gyro_Msg[7] & 0x40));
    Gyro_validation[1] = ((Gyro_Msg[6] & 0x20) ^ (Gyro_Msg[7] & 0x20));
    Gyro_validation[2] = ((Gyro_Msg[6] & 0x10) ^ (Gyro_Msg[7] & 0x10));
    Gyro_validation[3] = ((Gyro_Msg[6] & 0x08) ^ (Gyro_Msg[7] & 0x08));
    Gyro_validation[4] = ((Gyro_Msg[6] & 0x04) ^ (Gyro_Msg[7] & 0x04));
    Gyro_validation[5] = ((Gyro_Msg[6] & 0x02) ^ (Gyro_Msg[7] & 0x02));
    Gyro_validation[6] = ((Gyro_Msg[6] & 0x01) ^ (Gyro_Msg[7] & 0x01));
    test = ((Gyro_validation[0] | Gyro_validation[1] | Gyro_validation[2] | Gyro_validation[3] | Gyro_validation[4] | Gyro_validation[5] | Gyro_validation[6]) & 0x7F);
    printf("Temperature validation is %d\n", test);
    if(test == Gyro_Msg[8])
        return 1;
    else
        return 0;
}

void Gyro_Info(void)
{
    if((Gyro_Msg[4] & 0x08) == 0x08) {
        //negative
        Gyro_Message = (int32_t)(~((Gyro_Msg[0]  |  Gyro_Msg[1] << 7  |  Gyro_Msg[2] << 14  |  Gyro_Msg[3] << 21  |  Gyro_Msg[4] << 28) & 0xFFFF)) + 1;
    } else {
        //positive
        Gyro_Message = ((Gyro_Msg[0]  |  Gyro_Msg[1] << 7  |  Gyro_Msg[2] << 14  |  Gyro_Msg[3] << 21  |  Gyro_Msg[4] << 28) & 0xFFFF);
    }
    Angular_velocity_z = Gyro_Message / Gyro_scale_factor / 10000;
    Gyro_Temperature = ((Gyro_Msg[6]  |  Gyro_Msg[7] << 7) & 0x3FFF);
    printf("the Gyrp message is %d\nthe angular velocity of 'z' is %lf\nthe Gyro Temperature is %d\n", Gyro_Message, Angular_velocity_z, Gyro_Temperature);
}
