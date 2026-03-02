#ifndef encoder_H
#define encoder_H
#include <stdint.h>
#define MM_PER_CNT_ENCODER 0.7659   //1 vòng 820 xung encoder , duong kinh banh 20mm

void Encoder_Init();
void Reset_Encoder();
void Update_Encoder();

float Real_Position();
float Real_Pos_Increment();

#endif