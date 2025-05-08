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

        ///@brief Starts the NerfBlaster thread and initializes the pins
        ///
        /// @param Queue messageQueue 
        /// @param PinName trigPin Trigger pin for the distance sensor
        /// @param PinName echoPin Echo pin for the distance measurment
        /// @param DigitalOut redPin Red LED 
        /// @param DigitalOut yellowPin Yellow LED
        /// @param DigitalOut greenPin Green LED
        /// @param DigitalOut nerfMotor Activates the nerf blaster
        /// @param PwmOut servo motor pin
        ///
        /// @retval Result enum indicating return status 
        virtual Result init(rtos::Queue<uint32_t, 16>* messageQueue, PinName trigPin, PinName echoPin,
                            DigitalOut *redPin, DigitalOut *yellowPin, DigitalOut *greenPin, DigitalOut *_nerfMotor,
                            PwmOut *servoMotor);

        
    private:
        /// @brief Main thread that takes in messages from the message queue
        ///         Sends the distance to the StateMachine
        ///
        /// @param none
        ///
        /// @retval none
        virtual void executionThread();

        ///@brief Takes in the new measurement and dictates the action that should be taken
        ///
        /// @param uint32_t distance measurement
        ///
        /// @retval none
        virtual void stateMachine(uint32_t distance);
        
        
    private:
       rtos::Queue<uint32_t, 16>* _messageQueue = nullptr;
        Thread *thread                          = nullptr;
        HCSR04 *_distanceSensor                 = nullptr;
        DigitalOut *_redPin                     = nullptr;
        DigitalOut *_yellowPin                  = nullptr;
        DigitalOut *_greenPin                   = nullptr;
        DigitalOut *_nerfMotor                  = nullptr;
        PwmOut *_servoMotor                     = nullptr;
        uint32_t *_distance                     = 0;
};