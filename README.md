# I2C Bus Scanner with BMP280, MPU6500, and SSD1306 Display

This project implements an I2C Bus Device Scanner that automatically detects and displays the addresses of all connected I2C devices. In this example, was used a STM32F407 as a master and an I2C barometer sensor, an MPU6500 inertial sensor and an SSD1306 display were used as I2C slave devices. In addition to counting as another address, the display also had the function of printing all captured addresses.

## Assembly
![](images/Assembly.jfif)


## Features

- **I2C Devices**: The BMP280, MPU6500, and SSD1306 are connected to the I2C bus. Their addresses are automatically detected and displayed on the SSD1306 display.
  
- **I2C Communication**: 
  - **SDA** is connected to **PB9**.
  - **SCK** is connected to **PB6**.
  
- **Frequency Switching**: A DIP switch connected to **PA7** allows switching between two I2C bus speeds:
  - **Standard Mode (100kHz)** when the switch is on (1).
  - **Fast Mode (400kHz)** when the switch is off (0).
  
- **Automatic Scanning**: The system scans the I2C bus every 5 seconds and displays the addresses of all detected devices on the SSD1306 display.

## Wiring Diagram

- **SDA (PB9)**: Connects to the SDA pin of all I2C devices (BMP280, MPU6500, SSD1306).
- **SCK (PB6)**: Connects to the SCK pin of all I2C devices.
- **DIP Switch (PA7)**: Used to select the I2C bus speed (Standard Mode or Fast Mode).
  
## Setup

1. **Hardware Setup**:
   - Connect the **BMP280**, **MPU6500**, and **SSD1306** to the I2C bus as per the wiring diagram.
   - Connect the **DIP switch** to **PA7** for frequency switching.
   - Ensure all devices are properly powered and grounded.

## Code Explanation

1. **SSD1306 Lib**

   - First of all, you need to install the ssd1306 display library, which is present in this repository: https://github.com/afiskon/stm32-ssd1306?tab=readme-ov-file
```c
ssd1306_Fill(0); // Clear the display
ssd1306_SetCursor(2, 30); // Set the starting position
ssd1306_WriteString("Hello World", Font_7x10, 1); // Write the message
ssd1306_UpdateScreen(); // Update the display
```

- **I2C Bus Scanning**: The program continuously scans for connected devices on the I2C bus and displays their addresses on the SSD1306 display.
  
- **DIP Switch Input**: The status of the DIP switch is read from **PA7**, which determines the I2C speed. If the switch is on (1), the bus operates in **Standard Mode (100kHz)**. If the switch is off (0), it switches to **Fast Mode (400kHz)**.

- **Display Output**: The SSD1306 display shows the detected I2C addresses every time the scan completes, with a 5-second interval between scans.

