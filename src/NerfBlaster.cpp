/*
* @file NerfBlaster.cpp
* @brief Main application logic for the Nerf Sentry Turret.
*
* This class coordinates all hardware components based on distance data received
* from the HC-SR04 sensor. It runs a state machine on its own RTOS thread that
* controls the LEDs, flywheel motor, and dart-pusher servo to track and fire
* at targets that get too close.
*
* @author Carl Negrescu
* @date 04/11/2025
*/
#include "includes/NerfBlaster.h"
#include <iostream>

#define INIT_TIME = 90000

NerfBlaster::NerfBlaster()
:_distance(nullptr)
{
    //DigitalOut redPin = new DigitalOut();
}

NerfBlaster::~NerfBlaster() {}

 /// @brief Initializes all hardware, the distance sensor, and starts the main execution thread.
 ///
 /// @param messageQueue Pointer to the RTOS queue for receiving distance data.
 /// @param trigPin The GPIO pin for the sensor's TRIG.
 /// @param echoPin The GPIO pin for the sensor's ECHO.
 /// @param redPin DigitalOut for the red LED.
 /// @param yellowPin DigitalOut for the yellow LED.
 /// @param greenPin DigitalOut for the green LED.
 /// @param nerfMotor DigitalOut for the flywheel motor.
 /// @param servoMotor PwmOut for the dart-pusher servo.
 /// @return RESULT_OK on success, RESULT_ERROR on failure.
Result NerfBlaster::init(rtos::Queue<uint32_t, 16>* messageQueue, PinName trigPin, PinName echoPin,
                            DigitalOut *redPin, DigitalOut *yellowPin, DigitalOut *greenPin, DigitalOut *nerfMotor,
                            PwmOut *servoMotor)
{
    osStatus status = osOK;
    Result result = RESULT_OK;

    _redPin =       redPin;
    _yellowPin =    yellowPin;
    _greenPin =     greenPin;
    _nerfMotor =    nerfMotor;
    _servoMotor =   servoMotor;
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


/// @brief The main loop for the NerfBlaster's RTOS thread.
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

/// @brief Controls the blaster's LEDs and motors based on the measured distance.
///
/// @param distance The distance to the target in centimeters. 
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
       //Preparing to shoot if subject gets closer
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
