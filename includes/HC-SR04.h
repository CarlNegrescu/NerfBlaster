/*
* @file HC-SR04.h
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

        /// @brief starts the thread to start running the distance sensor
        ///
        /// @param none
        ///
        /// @retval Result enum indicating the return status
        Result init(rtos::Queue<uint32_t, 16>* messageQueue, PinName trigPin, PinName echoPin);
    
    private:
        /// @brief helper method to measure the distance
        ///
        /// @param none
        ///
        /// @retval none
        void startMeasure();

        /// @brief Main execution thread, triggers the sensor for 10 microseconds, 
        ///        throws an interrupt when the echo is active,
        ///        then calculates the distances and puts the value into a messageQueue
        ///
        /// @param none
        ///
        /// @retval none 
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
        DigitalOut *_trigPin = nullptr;
        InterruptIn *_echoPin = nullptr;
        Thread *thread = nullptr;
        Timer timer;
        std::chrono::microseconds startTime;
        std::chrono::microseconds endTime;
        std::chrono::microseconds duration;
        uint32_t _tickcounterStart;
        uint32_t _tickcounterEnd;
        uint32_t _distance = 0;
        uint32_t _duration = 0;
        volatile bool _measurementReady = false;

     };