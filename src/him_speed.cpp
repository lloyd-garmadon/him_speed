/***********************************************************************
* FILENAME:
*       him_speed.cpp
*
* DESCRIPTION:
*       Him Arduino Speed Sensor Routines.
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

#define HIM_LIBRARY_COMPILE

#include "Arduino.h"

#include "him_speed.h"


int HimSpeedHallSensor::s_instance_index  = 0;
int HimSpeedHallSensor::s_instance_max    = HimSpeedHallSensorMax;
HimSpeedHallSensor* HimSpeedHallSensor::s_instance_table[HimSpeedHallSensorMax]  = {0};

HimSpeedHallSensor::HimSpeedHallSensor(unsigned int pinIRQ, unsigned int pinB, bool invert = false) :
    m_invert(invert),
    m_pinIRQ(pinIRQ),
    m_pinB(pinB),
    m_increment(0),
    m_time(0)
{
    switch(pinIRQ) {
        case 2:
        case 3:
            m_initalized = true;
            break;
        default:
            m_initalized = false;
    }

    if(m_initalized) {
        if( s_instance_index >= s_instance_max ) {
            m_initalized = false;
        }
    }

    if(m_initalized) {
        pinMode(pinIRQ, INPUT);
        pinMode(pinB, INPUT);

        m_time = s_time = micros();

        s_instance_table[s_instance_index] = this;
        switch (s_instance_index) {
            case 0: attachInterrupt(digitalPinToInterrupt(pinIRQ), s_instance_0_irq_func, FALLING); break;
            case 1: attachInterrupt(digitalPinToInterrupt(pinIRQ), s_instance_1_irq_func, FALLING); break;
        }

        s_instance_index++;
    }
}


HimSpeedHallSensor::~HimSpeedHallSensor()
{
}


void HimSpeedHallSensor::irq_func()
{
    if(m_invert == !digitalRead(m_pinB)) {
        m_increment++;
    } else {
        m_increment--;
    }
}
void HimSpeedHallSensor::s_instance_0_irq_func() { s_instance_table[0]->irq_func(); }
void HimSpeedHallSensor::s_instance_1_irq_func() { s_instance_table[1]->irq_func(); }


bool HimSpeedHallSensor::getIncrement(int &increment, bool lock)
{
    if (!m_initalized) {
        return false;
    } else {

        if (lock) cli();
            increment = m_increment;
            m_increment = 0;
        if (lock) sei();

        return true;
    }
}


bool HimSpeedHallSensor::getValue(unsigned int &interval_us, int &increment)
{
    if (!m_initalized) {
        return false;
    } else {
        unsigned long int time = micros();

        cli();
            increment = m_increment;
            m_increment = 0;

            interval_us = time - m_time;
            m_time = time;
        sei();

        return true;
    }
}

unsigned long int HimSpeedHallSensor::s_time = 0;
bool HimSpeedHallSensor::getValues(unsigned int &interval_us, int (&increment)[HimSpeedHallSensorMax], HimSpeedHallSensor (&instance)[HimSpeedHallSensorMax], unsigned int count)
{
    bool res = true;
    unsigned long int time = micros();;

    cli();
        for(int i=0; i<count; i++) {
            res = res & instance[i].getIncrement( increment[i], false);
        }

        interval_us = time - s_time;
        s_time = time;
    sei();

    return res;
}

