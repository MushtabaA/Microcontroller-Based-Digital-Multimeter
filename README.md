# Multimeter Project Using PIC24F16KA101 Microcontroller

This project demonstrates the design and implementation of a multimeter that reads and outputs **voltage**, **resistance**, and **capacitance** values. Utilizing a **PIC24F16KA101** microcontroller, the device is programmed to display measurements on a serial monitor. The project is built on a breadboard and programmed with **MPLAB X IDE** using **PICkit 4** as the in-circuit debugger. Readings are outputted via **UART2** to the **Tera Term** terminal application.

<p float="left">
    <img src="https://github.com/user-attachments/assets/4334f520-179e-4b27-acb5-562d87c9e302" width="40%" />
    <img src="https://github.com/user-attachments/assets/d1b3fbbb-4d11-403d-99c4-af486004daa9" width="40%" />
    <img src="https://github.com/user-attachments/assets/0c25de30-1d48-4138-bb6e-d3c715fd31cf" width="48%" />
    <img src="https://github.com/user-attachments/assets/85548441-6c01-4575-aacb-3280c960834a" width="20%" />
</p>

## Features
- Voltage Measurement: Reads and displays voltage on a connected component.
- Resistance Measurement: Calculates and outputs the resistance of a component.
- Capacitance Measurement: Measures capacitance values using a CTMU (Charge Time Measurement Unit) approach.

## Code Overview
The project’s code includes several critical files:

- **main.c:** Initializes the microcontroller, including clock and peripheral settings, and handles the measurement loops.
- **UART2.c:** Manages UART2 communication functions, including data transmission and interrupt handling.
- **IOs.c:** Handles input/output configurations, such as pushbuttons for triggering different measurement modes.
- **ADC.c:** Reads and processes the analog-to-digital conversions necessary for voltage and resistance measurements.
- **CTMU.c:** Configures and utilizes the CTMU module for capacitance measurement by timing charge and discharge cycles on the capacitor under test.

## Demonstration Video
Click below to watch a demonstration of the project


[![Demonstration Video](https://github.com/user-attachments/assets/90aa8bca-837c-4dd6-8958-179c4961deab)](https://www.youtube.com/watch?v=2IUto0paSR4)
