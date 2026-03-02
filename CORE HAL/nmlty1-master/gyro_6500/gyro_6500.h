#ifndef gyro_6500_H
#define gyro_6500_H
#include <stm32f1xx_hal.h>

// Cac thanh ghi cua MPU 6500
#define WHO_AM_I      0x75
#define PWR_MGMT_1    0x6B
#define SMPLRT_DIV    0x19
#define CONFIG        0x1A
#define GYRO_CONFIG   0x1B
#define GYRO_XOUT_H   0x43


void MPU_Init(void);
void MPU_ReadGyroRaw(void);
void MPU_Calibrate(void);
void MPU_Update(void);
void MPU_Reset(void);
float Real_Angle(void);
float Real_Ang_Increment(void);

#endif 