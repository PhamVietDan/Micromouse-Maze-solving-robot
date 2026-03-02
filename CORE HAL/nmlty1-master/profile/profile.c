#include "profile.h"
#include "encoder.h"
#include <stdbool.h>
#include <math.h>

void Reset(Profile *p)
{
		__disable_irq();
    p->position = 0;
    p->speed = 0;
    p->target_speed = 0;
    p->state = IDLE;
    __enable_irq();
}

void Start(Profile *p, float distance, float top_speed, float final_speed, float acceleration)
{
	p->sign =(distance < 0) ? -1 : 1;
	p->acceleration = acceleration;
	p->one_per_acc = 1/p->acceleration;
	if(distance < 0) 
	{
		distance = -distance;
	}
	if(final_speed > top_speed)
	{
		final_speed = top_speed;
	}
	
		p->position = 0;                                  //vi tri bat dau là 0
    p->final_position = distance;                     //vi tri ket thuc
    p->acceleration = fabsf(acceleration);
    p->target_speed = p->sign * fabsf(top_speed);
    p->final_speed  = p->sign * fabsf(final_speed);
    p->state = ACCEL;                                 //bat dau tang toc
		
	  
}



void Update(Profile *p)
{
    if(p->state == IDLE)          // Neu IDLE thi khong doc
		{
        return;
		}
		
    float delta_v = p->acceleration * 0.001f;     // v - v0 = a * (t-t0)
    float remaining = fabsf(p->final_position) - fabsf(p->position);   // khoang cach con phai di

    if(p->state == ACCEL)    // neu dang tang toc 
    {
        if(remaining <= Get_Braking_Distance(p)) // bat dau den khoang giam toc
        {
            p->state = BRAKE;
					if(p->final_speed == 0)
					{
						p->target_speed = p->sign * 0.1f * p->target_speed;         // Tranh bi dung qua nhanh
					}
					else
					{
						p->target_speed = p->final_speed;
					}
        }
    }

    if(p->speed < p->target_speed)          // tang toc
    {
        p->speed += delta_v;
        if(p->speed > p->target_speed)
				{
            p->speed = p->target_speed;
				}
    }

    if(p->speed > p->target_speed)          // giam toc
    {
        p->speed -= delta_v;
        if(p->speed < p->target_speed)
				{
            p->speed = p->target_speed;
				}
    }

    p->position += p->speed * 0.001f;     // cap nhat vi tri moi: s - s0 = v* dt
		
    if(remaining < 0.125f)       // dung som
    {
        p->state = FINISHED;
    }
}

void Stop(Profile *p)
{
	p->target_speed = 0;
	p->speed = p->target_speed;
	p->state = FINISHED;
}

bool is_finished(Profile *p) 
{ 
return p->state == FINISHED;
}

float Get_Braking_Distance(Profile *p) // tinh toan khoang cach phanh
{
	return fabsf((p->speed * p->speed - p->final_speed * p->final_speed) * 0.5f * p->one_per_acc);
}

float position(Profile *p)  // lay gia tri position hien tai
{
	float pos;
	pos = p->position;
	return pos;
}

float speed(Profile *p)   // lay gia tri speed hien tai
{
	float speed;
	speed = p->speed;
	return speed;
}
float acceleration(Profile *p) // lay gia tri acceleration hien tai
{
    float acc;
    acc = p->acceleration;  
		return acc;
}

float increment(Profile *p)   // tinh quang duong di chuyen dc trong 1ms
{
	float inc;
	inc = p->speed * 0.001f;
	return inc;
}

void adjust_position(Profile *p, float adjustment)  // bu sai lech khi di thang
{
   p->position += adjustment; 
}


void set_state(Profile *p, ProfileState State)   // thay doi trang thai thang hoac quay
{
	p->state = State;
}

 void set_speed(Profile *p, float speed)    // thay doi toc do hien tai
{
      p->speed = speed;
}

void set_target_speed(Profile *p, float speed)    // thay doi toc do muc tieu
{
      p->target_speed = speed;
}

void set_position(Profile *p, float position)     // thay doi position hien tai
{
	p->position = position; 
}




