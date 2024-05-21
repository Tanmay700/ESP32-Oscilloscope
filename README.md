# ESP32-Oscilloscope: Analog Input to OLED Display and Serial Plotter

Overview

This project aims to read analog inputs from an ESP32 and display the corresponding waveforms on an OLED display and the Serial Plotter. The project utilizes the Adafruit_GFX and Adafruit_SSD1306 libraries for OLED display handling, and the analog inputs are sampled and visualized.

Wokwi simulation
https://wokwi.com/projects/398313390338233345

Hardware Requirements

ESP32
OLED Display (SSD1306)
Connecting wires
Software Requirements

Arduino IDE
Adafruit GFX Library
Adafruit SSD1306 Library
Circuit Diagram

Connect the OLED display to the ESP32 as follows:

OLED SDA (data line) -> GPIO 21
OLED SCL (clock line) -> GPIO 22
OLED VCC -> 3.3V
OLED GND -> GND
Connect the analog sensors or inputs to the ESP32 ADC pins:

Analog Input 0 -> GPIO 34
Analog Input 1 -> GPIO 35
