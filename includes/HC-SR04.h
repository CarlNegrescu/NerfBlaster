/*
* @file HC-SR04.h
* @brief Header file for the HCSR04 ultrasonic distance sensor driver.
*
* @date 01/04/2025
* @author Carl Negrescu
*/
#include "Result.h"
#include <iostream>
#pragma once 

class HCSR04 
{
    public:

        HCSR04();


        ~HCSR04();

         /// @brief Initializes the driver, configures GPIO pins, and starts the measurement thread.
         /// @param messageQueue Pointer to the RTOS queue used to send distance results.
         /// @param trigPin The GPIO pin connected to the sensor's TRIG input.
         /// @param echoPin The GPIO pin connected to the sensor's ECHO output.
         /// @return Result enum indicating success (RESULT_OK) or failure.
         ///
        Result init(rtos::Queue<uint32_t, 16>* messageQueue, PinName trigPin, PinName echoPin);
    
    private:
       
         /// @brief Sends the required 10-microsecond pulse to the TRIG pin to start a measurement.
        void startMeasure();

        /// @brief Main execution thread, triggers the sensor for 10 microseconds, 
        ///        throws an interrupt when the echo is active,
        ///        then calculates the distances and puts the value into a messageQueue
        void measurementThread();
        
        /// @brief interrupt when trigger starts
        ///
        /// @param none
        ///
        /// @retval none
        void echoRise();

        /// @brief interrupt when trigger stops
        ///
        /// @param none
        ///
        /// @retval none
        void echoFall();

    private:
        rtos::Queue<uint32_t, 16>* _messageQueue = nullptr;
        DigitalOut *_trigPin                     = nullptr;
        InterruptIn *_echoPin                    = nullptr;
        Thread *thread                           = nullptr;
        volatile bool _measurementReady          = false;
        Timer timer;
        std::chrono::microseconds startTime;
        std::chrono::microseconds endTime;
        std::chrono::microseconds duration;
        uint32_t _tickcounterStart;
        uint32_t _tickcounterEnd;
        uint32_t _distance = 0;
        uint32_t _duration = 0;
     };