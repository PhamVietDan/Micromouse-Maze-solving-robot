#include "gyro_6500.h"
#include <stdint.h>
#include <stm32f1xx_hal.h>

extern SPI_HandleTypeDef hspi2;

volatile float angle_x, angle_y, angle_z;
float Gx,Gy,Gz;
float offset_x, offset_y, offset_z;
float gx,gy,gz;


static void MPU_Write(uint8_t reg, uint8_t data)
{
    uint8_t tx[2];
    tx[0] = reg & 0x7F;   // write
    tx[1] = data;

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, 0);
    HAL_SPI_Transmit(&hspi2, tx, 2, 100);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, 1);
}
static void MPU_Read(uint8_t reg, uint8_t *data, uint8_t len)
{
    uint8_t addr = reg | 0x80; // read

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, 0);
    HAL_SPI_Transmit(&hspi2, &addr, 1, 100);
    HAL_SPI_Receive(&hspi2, data, len, 100);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, 1);
}
void MPU_Init(void)
{
	  uint8_t check;

    // Reset
    MPU_Write(PWR_MGMT_1, 0x80);
    HAL_Delay(100);
    // Check WHO_AM_I
    MPU_Read(WHO_AM_I, &check, 1);

    if(check == 0x71)
		{
    // Clock source = gyro X
    MPU_Write(PWR_MGMT_1, 0x01);

    // Sample rate = 1kHz
    MPU_Write(SMPLRT_DIV, 0x00);

    // DLPF config (optional)
    MPU_Write(CONFIG, 0x03);

    // Gyro ±1000 dps (FS_SEL = 2)
    MPU_Write(GYRO_CONFIG, 0x10);
		}
    

}
void MPU_ReadGyroRaw(void)
{
		uint8_t buffer[6];
		
	  
    MPU_Read(GYRO_XOUT_H, buffer, 6);

    gx = (int16_t)(buffer[0] << 8 | buffer[1]);
    gy = (int16_t)(buffer[2] << 8 | buffer[3]);
    gz = (int16_t)(buffer[4] << 8 | buffer[5]);
}

void MPU_Calibrate(void)
{
    float sumx = 0;
		float sumy = 0;
		float sumz = 0;

    for(int i = 0; i < 1000; i++)
    {
			  MPU_ReadGyroRaw();
        sumx += gx;
				sumy += gy;
				sumz += gz;
        HAL_Delay(1);
    }
		
	  offset_x= sumx/1000;
		offset_y= sumy/1000;
		offset_z= sumz/1000;
    
}

void MPU_Update()
{
	MPU_ReadGyroRaw();
	Gx = (gx-offset_x)/131.0f;
	Gy = (gy-offset_y)/131.0f;
	Gz = (gz-offset_z)/131.0f;
	
	angle_x += Gx * 0.001f;
	angle_y += Gy * 0.001f;
	angle_z += Gz * 0.001f;
}

void MPU_Reset()
{
	angle_x=0;
	angle_y=0;
	angle_z=0;
}

float Real_Angle(void)
{
    return angle_x;
}

float Real_Ang_Increment(void)
{
	return (Gx*0.001f);
}
