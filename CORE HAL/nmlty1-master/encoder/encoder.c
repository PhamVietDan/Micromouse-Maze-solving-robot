#include "encoder.h"
#include <stdint.h>
#include <stm32f1xx_hal.h>

extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim4;

static volatile float real_position = 0;
static volatile float real_pos_increment = 0;

void Encoder_Init()
{
    HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);
    HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_ALL);
}

void Reset_Encoder()
{
    __HAL_TIM_SET_COUNTER(&htim2, 0);
    __HAL_TIM_SET_COUNTER(&htim4, 0);
}

static int16_t Get_Delta_Encoder_Left(void)
{
    static uint16_t encoder_left_last = 0;
    uint16_t encoder_left_now = __HAL_TIM_GetCounter(&htim2);
    int16_t delta = (int16_t)(encoder_left_now - encoder_left_last); 
    encoder_left_last = encoder_left_now;
    return delta;
}

static int16_t Get_Delta_Encoder_Right(void)
{
    static uint16_t encoder_right_last = 0;
    uint16_t encoder_right_now = __HAL_TIM_GetCounter(&htim4); // ? htim4
    int16_t delta = (int16_t)(encoder_right_now - encoder_right_last); 
    encoder_right_last = encoder_right_now;
    return delta;
}

void Update_Encoder()
{
    // Cap nhat pos_increment
    real_pos_increment = (Get_Delta_Encoder_Left() + Get_Delta_Encoder_Right()) * MM_PER_CNT_ENCODER * 0.5f;
    // Cap nhat position
    real_position += real_pos_increment;
}

float Real_Pos_Increment(void)
{
    return real_pos_increment;
}

float Real_Position(void)
{
    return real_position;
}