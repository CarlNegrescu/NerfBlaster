/*
*   @file HC-SR04.cpp
*
*   @author Carl Negrescu
*   @date 04/05/2025
*/
#include "includes/HC-SR04.h"


HCSR04::HCSR04()
{
}

Result HCSR04::init(rtos::Queue<float, 16>* messageQueue, PinName trigPin, PinName echoPin)
{
    osStatus status = osOK;

    _trigPin = new DigitalOut(trigPin);
    _messageQueue = messageQueue; 
    _echoPin = new InterruptIn(echoPin);

    _echoPin->rise(callback(this, &HCSR04::echoRise));
    _echoPin->fall(callback(this, &HCSR04::echoFall));

    thread = new Thread(osPriorityNormal, OS_STACK_SIZE, nullptr, "DistanceSensor");
    status = thread->start(callback(this, &HCSR04::measurementThread));

    return status == osOK ? RESULT_OK : RESULT_ERROR;
}

void HCSR04::measurementThread()
{
    while(true)
    {
        if(_measurementReady)
        {
            _measurementReady = false;
            _messageQueue->try_put(&_distance);
        }
        startMeasure();
        ThisThread::sleep_for(100ms);
    }
}

void HCSR04::startMeasure()
{
    *_trigPin = 1;
    wait_us(10);
    *_trigPin = 0;
}

void HCSR04::echoRise()
{
    timer.reset();
    timer.start();
}

void HCSR04::echoFall()
{
    timer.stop();
    duration = timer.elapsed_time();
    _distance = (duration.count() * 0.0343f) / 2.0f;
    _measurementReady = true;
}