# I2C Bus Device Scanner

This project implements an I2C Bus Device Scanner that automatically detects and displays the addresses of all connected I2C devices. In this example, was used a STM32F407 as a master and an I2C barometer sensor, an MPU6500 inertial sensor and an SSD1306 display were used as I2C slave devices. In addition to counting as another address, the display also had the function of printing all captured addresses.

## Assembly
[![Assista ao vídeo no YouTube](images/Assembly.jfif)](https://youtu.be/R-K91Ljkl5E)

<div align="center">
    <b>Click on the image to be redirected to the video on YouTube.</b>
</div>

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
![](images/Hello-world.jfif)

<br>
<br>

2. **I2C Bus Scanning**: The program continuously scans for connected devices on the I2C bus and displays their addresses on the SSD1306 display.

```c
for (uint8_t i = 0; i < 128; i++) {
  ret = HAL_I2C_IsDeviceReady(&hi2c1, (i << 1), 1, 100);

  if (ret == HAL_OK) {
    ssd1306_SetCursor(2, y); // Positions the "cursor" at the corresponding pixel
    ssd1306_WriteString("Address: ", Font_6x8, 1); // Writes the text to the buffer
    int_to_hex(i, buffer_address);
    ssd1306_WriteString(buffer_address, Font_6x8, 1); // Writes the text to the buffer
    ssd1306_UpdateScreen();
    y = y + 10;
  }
}
```

<br>
<br>

3. **DIP Switch Input**: The status of the DIP switch is read from **PA7**, which determines the I2C speed. If the switch is on (1), the bus operates in **Standard Mode (100kHz)**. If the switch is off (0), it switches to **Fast Mode (400kHz)**.

```c
verifica = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7);
```

- By assigning the variable **verifica**, which will have the value of the pin connected to the Dip switch, it will now be possible to create two branches, thus being able to define the bus Clock speed between Standard Mode and Fast Mode.
<br>

```c
if(verifica)
{
  i2c_clock_speed(100000);
  ssd1306_SetCursor(30, y); // Positions the "cursor" at the corresponding pixel
  ssd1306_WriteString("FREQ 100KHZ", Font_6x8, 1); // Writes the text to the buffer
  y = y + 20;

  for (uint8_t i = 0; i < 128; i++) {
    ret = HAL_I2C_IsDeviceReady(&hi2c1, (i << 1), 1, 100);

    if (ret == HAL_OK) {
      ssd1306_SetCursor(2, y); // Positions the "cursor" at the corresponding pixel
      ssd1306_WriteString("Address: ", Font_6x8, 1); // Writes the text to the buffer
      int_to_hex(i, buffer_address);
      ssd1306_WriteString(buffer_address, Font_6x8, 1); // Writes the text to the buffer
      ssd1306_UpdateScreen();
      y = y + 10;
    }
  }
} else {

  i2c_clock_speed(400000);
  ssd1306_SetCursor(30, y); // Positions the "cursor" at the corresponding pixel
  ssd1306_WriteString("FREQ 400KHZ", Font_6x8, 1); // Writes the text to the buffer
  y = y + 20;

  for (uint8_t i = 0; i < 128; i++) {
    ret = HAL_I2C_IsDeviceReady(&hi2c1, (i << 1), 1, 100);

    if (ret == HAL_OK) {
      ssd1306_SetCursor(2, y); // Positions the "cursor" at the corresponding pixel
      ssd1306_WriteString("Address: ", Font_6x8, 1); // Writes the text to the buffer
      int_to_hex(i, buffer_address);
      ssd1306_WriteString(buffer_address, Font_6x8, 1); // Writes the text to the buffer
      ssd1306_UpdateScreen();
      y = y + 10;
    }
  }
}
```

4. **Observations**:
- Because the SSD1306 display library only works with Strings, a function would be needed that was capable of receiving an integer value and converting it to a char array capable of being printed on the display.
```c
void int_to_hex(uint8_t value, char *buffer) {
	buffer[0] = '0';
	buffer[1] = 'x';
	buffer[2] = "0123456789ABCDEF"[(value >> 4) & 0x0F]; // Extracts the most significant nibble
	buffer[3] = "0123456789ABCDEF"[value & 0x0F]; // Extracts the least significant nibble
	buffer[4] = '\0'; // Ends the string
}
```
- Through testing, it was observed that to update the bus clock frequency value it would be necessary to restart the entire bus, for this reason, a function was created that would receive the frequency value and update the communication protocol.
```c
void i2c_clock_speed(uint32_t clock) {


	hi2c1.Instance = I2C1;
	hi2c1.Init.ClockSpeed = clock;
	hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
	hi2c1.Init.OwnAddress1 = 0;
	hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c1.Init.OwnAddress2 = 0;
	hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	if (HAL_I2C_Init(&hi2c1) != HAL_OK) {
		Error_Handler();
	}
}
```
