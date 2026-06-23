# IoT_Based_Patient_Health_Monitoring_System

## Project Overview

The IoT-Based Patient Health Monitoring System is a smart healthcare solution designed to monitor a patient's vital health parameters in real time. The system uses an ESP32 microcontroller and a MAX30100 Pulse Oximeter Sensor to measure Heart Rate (BPM) and Blood Oxygen Saturation (SpO₂). The collected data is displayed locally on an OLED display and remotely through a web-based dashboard hosted on the ESP32.

This project demonstrates the integration of embedded systems, IoT connectivity, sensor interfacing, and web-based monitoring for healthcare applications.

## Features

* Real-time Heart Rate Monitoring (BPM)
* Real-time Blood Oxygen (SpO₂) Monitoring
* ESP32-Based Wi-Fi Connectivity
* Built-in Web Dashboard for Remote Monitoring
* OLED Display for Local Data Visualization
* Heartbeat Detection Indicator
* Health Alerts for Abnormal Heart Rate and SpO₂ Levels
* Automatic Dashboard Refresh for Live Data Updates

## Hardware Components

* ESP32 DevKit V1
* MAX30100 Pulse Oximeter Sensor
* OLED Display (128x64, I2C)
* Buzzer Module
* LED Indicator
* PCB (Printed Circuit Board)


## Software and Tools Used

### Development Environment

* Arduino IDE

### Programming Language

* Embedded C / Arduino C++

### Libraries

• WiFi.h                     – Enables ESP32 to connect to a Wi-Fi network for IoT communication.

• WebServer.h                – Creates a local web server and hosts the real-time health monitoring dashboard.

• Wire.h                     – Provides I2C communication between ESP32 and peripheral devices.

• MAX30100_PulseOximeter.h   – Reads and processes Heart Rate (BPM) and SpO₂ data from the MAX30100 sensor.

• Adafruit_GFX.h             – Provides graphics functions for displaying text, shapes, and animations on the OLED.

• Adafruit_SSD1306.h         – Controls the SSD1306 OLED display and manages screen output.

## Communication Protocols

* Wi-Fi
* HTTP
* I2C

### Wi-Fi Communication

The ESP32 connects to a Wi-Fi network using the WiFi library. This enables wireless communication and allows users to monitor patient health parameters remotely through a web browser.

### HTTP Communication

The ESP32 operates as a local HTTP web server. It generates and serves a real-time dashboard displaying Heart Rate (BPM) and Blood Oxygen Saturation (SpO₂) values. Users can access the dashboard using the ESP32 IP address.

### I2C Communication

The MAX30100 sensor and OLED display communicate with the ESP32 using the I2C protocol. The sensor sends health data to the ESP32, while the OLED receives processed information for local display. Both devices share the same SDA and SCL communication lines, reducing wiring complexity.


## System Architecture

MAX30100 Sensor → ESP32 → OLED Display
                     ↓
              Web Dashboard
                     ↓
              Remote Monitoring

## Working Principle

1. The MAX30100 sensor continuously measures Heart Rate (BPM) and Blood Oxygen Saturation (SpO₂).
2. ESP32 reads and processes the sensor data through the I2C protocol.
3. The measured values are displayed on the OLED display.
4. ESP32 hosts a local web server and creates a web dashboard.
5. Users can access real-time health data through a web browser connected to the same Wi-Fi network.
6. If abnormal health conditions are detected, alert notifications are displayed on the dashboard and OLED screen.


## Applications

* Remote Patient Monitoring
* Healthcare IoT Systems
* Home Health Monitoring
* Elderly Care Systems
* Smart Healthcare Solutions


## Future Improvements

* Cloud Data Storage
* Mobile Application Integration
* Email and SMS Alerts
* MQTT-Based Remote Monitoring
* AI-Based Health Analysis
* Hospital Management System Integration

## Hardware Prototype
  
<img width="1111" height="1143" alt="Hardware_Implement" src="https://github.com/user-attachments/assets/1c8670e9-82ba-4ca2-91c2-adfe1fb71f2a" />

## PCB Prototype

## PCB - Schematic Diagram
<img width="803" height="583" alt="Schematic_Layout" src="https://github.com/user-attachments/assets/a4cf50ad-f15e-43b8-b075-492d146741b0" />

## PCB - Design
<img width="425" height="384" alt="PCB_design" src="https://github.com/user-attachments/assets/c948b990-6354-4b8d-b0a2-e85c24145f5d" />

## PCB - 3D Front View
<img width="1043" height="831" alt="PCB_3D_views" src="https://github.com/user-attachments/assets/63315c23-fc33-4f85-9d6a-768301a82a88" />


