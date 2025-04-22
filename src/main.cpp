#include "mbed.h"
#include "includes/NerfBlaster.h"
#include <cstdint>

#define TRIG PC_0
#define ECHO PC_1  



int main()
{
    printf("Initializing\n");

    NerfBlaster *blaster = new NerfBlaster();
    rtos::Queue<uint32_t, 16> *messageQueue = new rtos::Queue<uint32_t, 16>();
    PinName trigPin = TRIG;
    PinName echoPin = ECHO;
    DigitalOut *redPin = new DigitalOut(RED_PIN);
    DigitalOut *yellowPin = new DigitalOut(YELLOW_PIN);
    DigitalOut *greenPin = new DigitalOut(GREEN_PIN);
    DigitalOut *nerfMotor = new DigitalOut(MOTOR_PIN);
    PwmOut *servoMotor = new PwmOut(SERVO_PIN);
    blaster->init(messageQueue, trigPin, echoPin, redPin, yellowPin, greenPin, nerfMotor, servoMotor);

    //Nothing to do here 
    while(true)
    {
        // servoMotor->period(0.02f);
        // servoMotor->pulsewidth(0.002f);
        // ThisThread::sleep_for(1000ms); 
        // servoMotor->pulsewidth(0.001f);
        ThisThread::sleep_for(1000ms); 
    
    }
}

