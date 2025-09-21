# Nerf Sentry Turret - Autonomous Nerf Blaster

This project is the firmware for an autonomous Nerf sentry turret built on the Mbed OS platform. The system uses an HC-SR04 ultrasonic distance sensor to detect targets within its range. Based on the target's proximity, it uses a state machine to provide visual feedback via LEDs and automatically fires the blaster at targets that get too close.

The project is designed with a multi-threaded RTOS architecture to ensure real-time responsiveness and modularity.

---

## Features

* **Autonomous Target Detection:** Uses an HC-SR04 ultrasonic sensor to measure target distance in real-time.
* **State Machine Logic:** Implements a multi-stage state machine (Safe, Caution, Warning, Attack) based on target proximity.
* **Automatic Firing:** Revs up the flywheel motor and actuates a servo to fire darts at targets within the "Attack" range.
* **Visual Status Indicators:** Uses Green, Yellow, and Red LEDs to provide clear visual feedback on the turret's current state.
* **Multi-Threaded RTOS Design:** Decouples the sensor reading from the main application logic using two separate RTOS threads and a message queue for communication.
* **Modular Hardware Drivers:** The HC-SR04 sensor logic is encapsulated in its own class, making the code clean and reusable.

---

## Hardware Required

* **Microcontroller:** An Mbed-enabled board (e.g., STM32 Nucleo, NXP FRDM).
* **Nerf Blaster:** An electronic, flywheel-based blaster that can be controlled via GPIO.
* **Ultrasonic Sensor:** 1x HC-SR04 Distance Sensor.
* **Status LEDs:** 1x Red, 1x Yellow, 1x Green LED.
* **Resistors:** 3x appropriate current-limiting resistors for the LEDs (e.g., 220Ω).
* **Motor Control:**
    * A logic-level MOSFET or motor driver to control the high-current flywheel motor.
    * A standard servo motor for the dart-pusher mechanism.
* **Power Supply:** Separate power supplies for the microcontroller and the motors to prevent brownouts.
* **Breadboard and Wires.**

---

## Software and Dependencies

* **Mbed OS:** The project is built on the Mbed real-time operating system.
* **IDE:** Mbed Studio, Keil Studio, or Mbed CLI with an Mbed-compatible toolchain (e.g., ARM GCC).

---

## How It Works

The system's architecture is based on two independent RTOS threads that communicate through a message queue. This design prevents the main logic from being blocked while waiting for sensor readings.



1.  **Sensor Thread (`HCSR04`)**
    * This thread is dedicated entirely to managing the HC-SR04 sensor.
    * It sends a 10µs pulse to the sensor's `TRIG` pin to start a measurement.
    * It uses hardware interrupts (`InterruptIn`) on the `ECHO` pin to precisely measure the duration of the return pulse.
    * The duration is converted into distance (in cm).
    * The final distance value is placed into an RTOS message queue.

2.  **Main Logic Thread (`NerfBlaster`)**
    * This thread waits for a message to arrive in the queue.
    * Upon receiving a new distance measurement, it passes the value to a state machine.
    * The **State Machine** then determines the turret's action based on predefined distance thresholds:
        * **FAR (> 120cm):** Green LED on. System is safe.
        * **MEDIUM (80-120cm):** Yellow LED on. A target is detected.
        * **CLOSE (50-80cm):** Yellow and Red LEDs on. Target is getting close.
        * **VERY CLOSE (30-50cm):** Red LED on. The flywheel motor spins up in preparation to fire.
        * **ATTACK (< 30cm):** Red LED on, motor is running. The servo is actuated to push a dart and fire.

---

## Setup and Installation

1.  **Clone the Repository:**

2.  **Import Project:**
    * Import the cloned repository into Mbed Studio or your preferred Mbed IDE.
    * The IDE should automatically detect the `mbed-os.lib` file and import the Mbed OS dependency.

3.  **Hardware Wiring:**
    * Connect the hardware components to your microcontroller according to the pin definitions in `Result.h`.
    * **(It is highly recommended to create and add a wiring diagram here.)**

4.  **Compile and Flash:**
    * Select your target Mbed board in the IDE.
    * Compile the project and flash the resulting binary to your board.

---

## Configuration

All key parameters can be easily configured in the **`Result.h`** file:

* **Pin Assignments:** Change the pin definitions (`SERVO_PIN`, `RED_PIN`, `MOTOR_PIN`, etc.) to match your hardware wiring.
* **Distance Thresholds:** Adjust the values for `FAR`, `MED`, `CLOSE`, `ATTACK`, etc., to tune the turret's sensitivity and engagement range.