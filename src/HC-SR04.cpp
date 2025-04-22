/*
*   @file HC-SR04.cpp
*
*   @author Carl Negrescu
*   @date 04/05/2025
*/
#include "includes/HC-SR04.h"
#include "os_tick.h"


HCSR04::HCSR04()
{
    _tickcounterStart = 0;
    _tickcounterStart = 0;
}

Result HCSR04::init(rtos::Queue<uint32_t, 16>* messageQueue, PinName trigPin, PinName echoPin)
{
    osStatus status = osOK;
    
    _trigPin = new DigitalOut(trigPin, PushPullPullUp);
    _messageQueue = messageQueue; 
     _echoPin = new InterruptIn(echoPin, PullUp);
    _echoPin->rise(callback(this, &HCSR04::echoRise));
    _echoPin->fall(callback(this, &HCSR04::echoFall));

    thread = new Thread(osPriorityNormal, OS_STACK_SIZE, nullptr, "DistanceSensor");
    status = thread->start(callback(this, &HCSR04::measurementThread));

    return status == osOK ? RESULT_OK : RESULT_ERROR;
}

void HCSR04::measurementThread()
{
    timer.reset();
    timer.start();
    startMeasure();
    while(true)
    {
        if(_measurementReady)
        {
            _measurementReady = false;
            _duration = _tickcounterEnd -  _tickcounterStart;
            _distance = (_duration * 0.0343f) / 2.0f;
            //printf("start: %u, end: %u, delta: %u, distance: %u cm\n", _tickcounterStart, _tickcounterEnd,  _tickcounterEnd -  _tickcounterStart, _distance);
            _messageQueue->try_put(&_distance);
            startMeasure();
        }
        ThisThread::sleep_for(500ms);
    }
}

void HCSR04::startMeasure()
{
    *_trigPin = 0;
    wait_us(2);     ///Small Wait to indicate state change
    *_trigPin = 1;
    wait_us(10);   ///Pulse duration to start the distance measurment
    *_trigPin = 0;
    wait_us(3);    /// Small 
}

void HCSR04::echoRise()
{
    _tickcounterStart = timer.read_us();
}

void HCSR04::echoFall()
{
    _tickcounterEnd =  timer.read_us();   
    _measurementReady = true;
}