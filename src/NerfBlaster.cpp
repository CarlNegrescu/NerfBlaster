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

}

NerfBlaster::~NerfBlaster() {}

Result NerfBlaster::init(rtos::Queue<float, 16>* messageQueue, PinName trigPin, PinName echoPin)
{
    osStatus status = osOK;
    Result result = RESULT_OK;

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
    float* distancePtr = nullptr;
    while(true)
    {
        if(_messageQueue->try_get(&distancePtr))
        {
            float distance = *distancePtr;
            //std::printf("Distance is: %f \n", distance);
            stateMachine(distance);
        }
    }

}


void NerfBlaster::stateMachine(float distance)
{
   if(distance >= VERY_FAR)
   {
       ///Do Nothing
       return;
   }
    ///Between FAR and VERY_FAR, Nothing to do? 
   if(distance >= FAR && distance < VERY_FAR)
   {
       ///Do Something
       return;
   }
    ///Between MED and FAR, FAR LOGIC
   if(distance >= MED && distance < FAR)
   {
       ///Do Something
       return;
   }
   ///Between CLOSE and MED, MED logic
   if(distance >= CLOSE && distance < MED)
   {
       ///Do Something
       return;
   }
   ///Between VERY_CLOSE and CLOSE, CLOSE logic
   if(distance >= VERY_CLOSE && distance < CLOSE)
   {
       ///Do Something
       return;
   }
   ///Between ATTACK and VERY_CLOSE, VERY_CLOSE logic
   if(distance >= ATTACK && distance < VERY_CLOSE)
   {
       ///Do Something
       return;
   }
    ///ATTACK!!!
    if(distance <= ATTACK)
    {
        ///ATTACK!!!
        return;
    }
}


