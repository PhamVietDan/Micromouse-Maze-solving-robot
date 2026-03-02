#ifndef PROFILE_H
#define PROFILE_H

#include "main.h"
#include <stdint.h>
#include <stdbool.h>

typedef enum {
    IDLE = 0,
    ACCEL,
    BRAKE,
    FINISHED
} ProfileState;

typedef struct
{
    volatile ProfileState state; // Trang thai hien tai
    volatile float speed;   // toc do hien tai
		volatile float target_speed;  // toc do huong toi  
    int8_t sign;   // xac dinh chieu di chuyen tien hay lui
    float acceleration;  // gia toc hien tai
    float final_speed;   // toc do cuoi
    float final_position; // vi tri can dat
		volatile float position; // vi tri hien tai
	  float one_per_acc;
} Profile;

// Cac ham chinh
void ProReset(Profile *p);
void Start(Profile *p,
           float m_distance,
           float m_top_speed,
           float m_final_speed,
           float m_acceleration);
void Stop(Profile *p);
void Update(Profile *p);
bool is_finished(Profile *p);
								 
// Cac ham tinh toan & cap nhat				 
float Get_Braking_Distance(Profile *p);										 
float position(Profile *p);
float speed(Profile *p);									 
float acceleration(Profile *p);									 
float increment(Profile *p);									 
void adjust_position(Profile *p, float adjustment);									 
									 
// Cac ham setting									 
void set_state(Profile *p, ProfileState State);									 
void set_speed(Profile *p, float speed);									 
void set_target_speed(Profile *p, float speed);									 
void set_position(Profile *p, float position);									 
									 
									 

#endif