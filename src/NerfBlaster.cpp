/*
*   @file NerfBlaster.cpp
*
*
*
*
*   @date 04/11/2025
*/
#include "includes/NerfBlaster.h"
#include <iostream>

NerfBlaster::NerfBlaster()
:_distance(nullptr)
{
    //DigitalOut redPin = new DigitalOut();
}

NerfBlaster::~NerfBlaster() {}

Result NerfBlaster::init(rtos::Queue<uint32_t, 16>* messageQueue, PinName trigPin, PinName echoPin,
                            DigitalOut *redPin, DigitalOut *yellowPin, DigitalOut *greenPin, DigitalOut *nerfMotor,
                            PwmOut *servoMotor)
{
    osStatus status = osOK;
    Result result = RESULT_OK;

    _redPin = redPin;
    _yellowPin = yellowPin;
    _greenPin = greenPin;
    _nerfMotor = nerfMotor;
    _servoMotor = servoMotor;

    *_redPin = 1;
    wait_us(90000);
    *_yellowPin = 1;
    wait_us(90000);
    *_greenPin = 1;
    wait_us(90000);
    *_nerfMotor = 1;
    wait_us(1000000);
    *_redPin    = 0;
    *_yellowPin = 0;
    *_greenPin  = 0;
    *_nerfMotor = 0;

    _distanceSensor = new HCSR04();
    _messageQueue = messageQueue;
    result = _distanceSensor->init(_messageQueue, trigPin, echoPin);
    
    if(result != RESULT_OK)
        return result;
    thread = new Thread(osPriorityNormal, OS_STACK_SIZE, nullptr, "NerfBlaster");
    status = thread->start(callback(this, &NerfBlaster::executionThread));

    return status == osOK ? RESULT_OK : RESULT_ERROR;
}

void NerfBlaster::executionThread()
{
    _servoMotor->period(0.02f);
    _servoMotor->pulsewidth(0.002f);
    ThisThread::sleep_for(400ms); 
    _servoMotor->pulsewidth(0.001f);
    uint32_t* distancePtr = nullptr;
    while(true)
    {
        if(_messageQueue->try_get(&distancePtr))
        {
            uint32_t distance = *distancePtr;
            stateMachine(distance);
        }
    }

}


void NerfBlaster::stateMachine(uint32_t distance)
{
    uint32_t nextDistance = distance;

   if(distance >= FAR)
   {
        *_yellowPin = 0;
        *_redPin = 0;
        *_greenPin = 1;
        *_nerfMotor = 0;
       ///Do Nothing
       printf("\nSubject is too FAR you can continue: %u cm\n", distance);
       return;
   }
    ///Between MED and FAR, FAR LOGIC
   if(distance >= MED && distance < FAR)
   {
        *_yellowPin = 0;
        *_redPin = 0;
        *_greenPin = 1;
        *_nerfMotor = 0;
       ///Do Something
       printf("\nSubject is FAR: %u cm\n", distance);
       return;
   }
   ///Between CLOSE and MED, MED logic
   if(distance >= CLOSE && distance < MED)
   {        
        *_yellowPin = 1;
        *_redPin = 0;
        *_greenPin = 0;
        *_nerfMotor = 0;
       ///Do Something
       printf("\nSubject is Medium: %u cm\n", distance);
       return;
   }
   ///Between VERY_CLOSE and CLOSE, CLOSE logic
   if(distance >= VERY_CLOSE && distance < CLOSE)
   {
        *_yellowPin = 1;
        *_redPin = 1;
        *_greenPin = 0;
        *_nerfMotor = 0;
       ///Do Something
       printf("\nSubject is Close: %u cm\n", distance);
       return;
   }
   ///Between ATTACK and VERY_CLOSE, VERY_CLOSE logic
   if(distance >= ATTACK && distance < VERY_CLOSE)
   {
       //Only Red LED should be on and nerfMotor should be started up
       // Preparing to shoot if subject gets closer
        *_yellowPin = 0;
        *_redPin = 1;
        *_greenPin = 0;
        *_nerfMotor = 1;
       ///Do Something
       printf("\nSubject is Very Close: %u cm\n", distance);
       return;
   }
    ///ATTACK!!!
    if(distance <= ATTACK)
    {
        *_yellowPin = 0;
        *_redPin = 1;
        *_greenPin = 0;
        *_nerfMotor = 1;
        _servoMotor->period(0.02f);
        _servoMotor->pulsewidth(0.002f);
        ThisThread::sleep_for(400ms); 
        _servoMotor->pulsewidth(0.001f);
        ///ATTACK!!!
        printf("\nATTACK: %u cm\n", distance);
        return;
    }
}
