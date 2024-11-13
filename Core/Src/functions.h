#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

void int_to_hex(uint8_t value, char *buffer) {
	buffer[0] = '0';
	buffer[1] = 'x';
	buffer[2] = "0123456789ABCDEF"[(value >> 4) & 0x0F]; // Extrai o nibble mais significativo
	buffer[3] = "0123456789ABCDEF"[value & 0x0F]; // Extrai o nibble menos significativo
	buffer[4] = '\0'; // Finaliza a string
}

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

#endif
