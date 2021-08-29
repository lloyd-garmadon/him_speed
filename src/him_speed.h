/***********************************************************************
* FILENAME:
*       him_speed.h
*
* DESCRIPTION:
*       Him Arduino Rotary Encoder Routines.
*
* NOTES:
*       This library was written to design just for fun.
*
* AUTHOR:
*       Sven Himstedt
*
* COPYRIGHT: 
*       Sven Himstedt 2021, All rights reserved.
*
* LICENSE:
*       This library is free software.
*       You can redistribute it and/or modify it.
*       But give credit or you are an asshole.
*
* START DATE:
*       2021/30/01
*
* CHANGES:
*
***********************************************************************/

#ifndef _HIM_SPEED_H_
#define _HIM_SPEED_H_


#define HimSpeedHallSensorMax 2

class HimSpeedHallSensor
{
public:
    HimSpeedHallSensor(unsigned int pinIRQ, unsigned int pinB, bool invert = false);
    ~HimSpeedHallSensor();

    bool getIncrement(int &increment, bool lock);
    bool getValue(unsigned int &interval_us, int &increment);

public:
    static bool getValues(unsigned int &interval_us, int (&increment)[HimSpeedHallSensorMax], HimSpeedHallSensor (&instance)[HimSpeedHallSensorMax], unsigned int count);
    static unsigned long int s_time;

    static int s_instance_index;
    static int s_instance_max;
    static HimSpeedHallSensor* s_instance_table[HimSpeedHallSensorMax];

    static void s_instance_0_irq_func();
    static void s_instance_1_irq_func();

private:
    void irq_func();

    bool m_initalized;

    bool m_invert;
    unsigned int m_pinIRQ;
    unsigned int m_pinB;

    unsigned long int m_time;

    volatile int m_increment;
};

#endif /* _HIM_SPEED_H_ */
