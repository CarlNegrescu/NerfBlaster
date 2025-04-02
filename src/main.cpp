#include "mbed.h"

#define TRIG PC_0
#define ECHO PC_1



Timer timer;
DigitalOut trigger(TRIG);
DigitalIn echo(ECHO);
std::chrono::microseconds duration;
// main() runs in its own thread in the OS
int main()
{
    while (true) 
    {
        timer.reset();
        trigger = 1;
        ThisThread::sleep_for(0.01);
        trigger = 0;
        timer.start();
        while(echo);
        timer.stop();
        duration = timer.elapsed_time();
        
        printf(duration);
    }
}

