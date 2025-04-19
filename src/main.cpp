#include "mbed.h"
#include "includes/NerfBlaster.h"

#define TRIG PC_0
#define ECHO PC_1  



Timer timer;
DigitalOut trigger(TRIG);
DigitalIn echo(ECHO);
std::chrono::microseconds duration;
PwmOut myServo(PA_0);
int main()
{
    //NerfBlaster *blaster = new NerfBlaster();
    rtos::Queue<float, 16> *messageQueue = new rtos::Queue<float, 16>();
    PinName trigPin = TRIG;
    PinName echoPin = ECHO;
    HCSR04 *sensor = new HCSR04();
    sensor->init(messageQueue, trigPin, echoPin);


    //blaster->init(messageQueue, trigPin, echoPin);

    // myServo.period_ms(20);
    // myServo.pulsewidth_us(1500); //NB in microseconds

    while(true) 
    {
        



    }
    

    //blaster now takes control
    //nothing to do in main.  
    // while (true) 

    // {
    //     timer.reset();
    //     trigger = 1;
    //     ThisThread::sleep_for(0.01);
    //     trigger = 0;
    //     timer.start();
    //     while(echo);
    //     timer.stop();
    //     duration = timer.elapsed_time();

    //     Queue<float, 32> messageQueue;
    //     //printf(duration);
    // }
}

