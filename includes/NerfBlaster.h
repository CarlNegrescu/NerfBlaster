/*
*   @file NerfBlaster.h
*
*   
*
*   @date 04/11/2025
*/
#include "Result.h"
#include "includes/HC-SR04.h"

#define VERY_FAR   150
#define FAR        100
#define MED        75
#define CLOSE      50
#define VERY_CLOSE 20
#define ATTACK     10


class NerfBlaster
{
    public:
        NerfBlaster();
        ~NerfBlaster();
        virtual Result init(rtos::Queue<float, 32>* messageQueue, PinName trigPin, PinName echoPin);

        virtual void stateMachine(float distance);

    private:
        virtual void executionThread();
        
    private:
        rtos::Queue<float, 32> *_messageQueue = nullptr;
        Thread *thread = nullptr;
        HCSR04 *_distanceSensor = nullptr;
        float *_distance;
};