/*
*   @file NerfBlaster.h
*
*   
*
*   @date 04/11/2025
*/
#include "Result.h"
#include "includes/HC-SR04.h"




class NerfBlaster
{
    public:
        NerfBlaster();
        ~NerfBlaster();
        virtual Result init(rtos::Queue<uint32_t, 16>* messageQueue, PinName trigPin, PinName echoPin,
                            DigitalOut *redPin, DigitalOut *yellowPin, DigitalOut *greenPin, DigitalOut *_nerfMotor,
                            PwmOut *servoMotor);

        virtual void stateMachine(uint32_t distance);

    private:
        virtual void executionThread();
        //virtual Result fireBlaster();
        //virtual Result targetFar();
        
        
    private:
       rtos::Queue<uint32_t, 16>* _messageQueue = nullptr;
        Thread *thread                          = nullptr;
        HCSR04 *_distanceSensor                 = nullptr;
        DigitalOut *_redPin                      = nullptr;
        DigitalOut *_yellowPin                   = nullptr;
        DigitalOut *_greenPin                    = nullptr;
        DigitalOut *_nerfMotor                   = nullptr;
        PwmOut *_servoMotor                      = nullptr;
        uint32_t *_distance;
};