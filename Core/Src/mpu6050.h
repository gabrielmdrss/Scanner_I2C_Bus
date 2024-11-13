#ifndef MPU6050_H_
#define MPU6050_H_

#define MPU6050_ADDR 0x68
#define SMPLRT_DIV_REG 0x19
#define ACCEL_CONFIG_REG 0x1C
#define ACCEL_XOUT_H_REG 0x3B
#define TEMP_OUT_H_REG 0x41
#define PWR_MGMT_1_REG 0x6B
#define WHO_AM_I_REG 0x75
int16_t Accel_X_RAW = 0;
int16_t Accel_Y_RAW = 0;
int16_t Accel_Z_RAW = 0;
float Ax, Ay, Az;

void MPU6050_Init();
void MPU6050_Read_Accel();

void MPU6050_Init (void){
uint8_t check = 0;
uint8_t Data;
// check device ID WHO_AM_I
HAL_I2C_Mem_Read (&hi2c1, MPU6050_ADDR, 0x75, 1, &check, 1, 1000);
if (check == 0x70) // 0x68 will be returned by the sensor if OK
{

// power management register 0X6B we should write all 0's to wake the sensor up
Data = 0;
HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR, 0x6B, 1,&Data, 1, 1000);
// Set DATA RATE of 1KHz by writing SMPLRT_DIV register
Data = 0x07;
HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR, 0x19, 1, &Data, 1, 1000);
// Set Gyroscopic configuration in GYRO_CONFIG Register
Data = 0x00; // XG_ST=0,YG_ST=0,ZG_ST=0, FS_SEL=0 -> ± 250 ̐/s
HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR, 0x1B, 1, &Data, 1, 1000);
// Set accelerometer configuration in ACCEL_CONFIG Register
Data = 0x00; // XA_ST=0,YA_ST=0,ZA_ST=0, FS_SEL=0 -> ± 2g
HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR, 0x1C, 1, &Data, 1, 1000);
}}

void MPU6050_Read_Accel (void)
{
uint8_t Rec_Data[6];
// Read 6 BYTES of data starting from ACCEL_XOUT_H (0x3B) register
HAL_I2C_Mem_Read (&hi2c1, MPU6050_ADDR, 0x3B, 1, Rec_Data, 6, 1000);
Accel_X_RAW = (int16_t)(Rec_Data[0] << 8 | Rec_Data [1]);
Accel_Y_RAW = (int16_t)(Rec_Data[2] << 8 | Rec_Data [3]);
Accel_Z_RAW = (int16_t)(Rec_Data[4] << 8 | Rec_Data [5]);
/*** convert the RAW values into acceleration in 'g'
we have to divide according to the Full scale value set in FS_SEL
I have configured FS_SEL = 0. So I am dividing by 16384.0
for more details check ACCEL_CONFIG Register ****/
Ax = (float)Accel_X_RAW/16384.0;
Ay = (float)Accel_Y_RAW/16384.0;
Az = (float)Accel_Z_RAW/16384.0;
}

#endif
