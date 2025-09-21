/*
* @file HC-SR04.cpp
* @brief Driver for the HC-SR04 ultrasonic distance sensor.
*
* This class handles the timing and logic for measuring distance. It runs on its
* own RTOS thread, continuously triggers measurements, listens for the echo
* pulse using interrupts, calculates the distance, and sends the result to the
* main application via a message queue.
*
* @author Carl Negrescu
* @date 04/05/2025
*/
#include "includes/HC-SR04.h"
#include "os_tick.h"

/**
 * @brief Constructor for the HCSR04 sensor driver.
 */
HCSR04::HCSR04()
{
    _tickcounterStart = 0;
    _tickcounterStart = 0;
}

/**
 * @brief Initializes the sensor, configures pins, and starts the measurement thread.
 *
 * @param messageQueue Pointer to the RTOS queue for sending distance results.
 * @param trigPin The GPIO pin connected to the sensor's TRIG pin.
 * @param echoPin The GPIO pin connected to the sensor's ECHO pin.
 * 
 * @retval RESULT_ERROR on failure
 * @retval RESULT_OK on success
 */
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

/// @brief The main loop for the sensor's RTOS thread.
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


/// @brief Sends a 10-microsecond pulse on the trigger pin to start a measurement.
void HCSR04::startMeasure()
{
    *_trigPin = 0;
    wait_us(2);     ///Small Wait to indicate state change
    *_trigPin = 1;
    wait_us(10);   ///Pulse duration to start the distance measurment
    *_trigPin = 0;
    wait_us(3);    /// Small 
}

/// @brief Interrupt Service Routine (ISR) called on the rising edge of the echo pin.
/// This marks the start time of the echo pulse.
void HCSR04::echoRise()
{
    _tickcounterStart = timer.read_us();
}

/// @brief Interrupt Service Routine (ISR) called on the falling edge of the echo pin.
/// This marks the end time of the echo pulse and signals that a measurement is ready.
void HCSR04::echoFall()
{
    _tickcounterEnd =  timer.read_us();   
    _measurementReady = true;
}