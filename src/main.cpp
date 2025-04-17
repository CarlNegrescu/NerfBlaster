#include "mbed.h"
#include "includes/NerfBlaster.h"

#define TRIG PC_0
#define ECHO PC_1



Timer timer;
DigitalOut trigger(TRIG);
DigitalIn echo(ECHO);
std::chrono::microseconds duration;
// main() runs in its own thread in the OS
int main()
{
    NerfBlaster *blaster = new NerfBlaster();
    rtos::Queue<float, 32> *messageQueue;
    PinName trigPin = TRIG;
    PinName echoPin = ECHO;


    blaster->init(messageQueue, trigPin, echoPin);

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

