/*
* @file HC-SR04.h
*
* @date 01/04/2025
* @author Carl Negrescu
*/
#include "Result.h"
#pragma once 



class HCSR04 
{
    public:

        HCSR04();
        /// @brief constructor, initalizes the pins
        ///
        /// @param messageQueue Takes in the messageQueue to output to
        /// @param trigPin triger pin to start the measurment
        /// @param echoPin input pin to read the distance
        ///
        /// @retval none 
        HCSR04(rtos::Queue<float, 32>* messageQueue, PinName trigPin, PinName echoPin);

        ~HCSR04();

        /// @brief starts the thread to start running the distance sensor
        ///
        /// @param none
        ///
        /// @retval Result enum indicating the return status
        Result init(rtos::Queue<float, 16>* messageQueue, PinName trigPin, PinName echoPin);
    
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
        rtos::Queue<float, 16> *_messageQueue = nullptr;
        DigitalOut *_trigPin = nullptr;
        InterruptIn *_echoPin = nullptr;
        Thread *thread = nullptr;
        Timer timer;
        std::chrono::microseconds startTime;
        std::chrono::microseconds endTime;
        std::chrono::microseconds duration;
        float _distance = 0;
        bool _measurementReady = false;

     };