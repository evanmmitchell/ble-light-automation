/**
 ******************************************************************************
 * @file    LSM6DSOX_FIFO_Interrupt.ino
 * @author  alvaro-oliver
 * @version V1.0.0
 * @date    May 2021
 * @brief   Example for LSM6DSOX library with FIFO status interrupts.
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT(c) 2019 STMicroelectronics</center></h2>
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 *   3. Neither the name of STMicroelectronics nor the names of its contributors
 *      may be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************
*/

/* WIRING
  In order to use the Adafruit lsm6dsox sensor with a ST nucleo board,
  plug Nucleo "+3.3V" to AdafruitLSM6DOX "VIN",
  plug Nucleo "GND" to AdafruitLSM6DOX "GND",
  plug Nucleo "SCL"(D15) to AdafruitLSM6DOX "SCL",
  plug Nucleo "SDA"(D14) to AdafruitLSM6DOX "SDA",
  plug Nucleo "A0" to AdafruitLSM6DOX "I1".*/

#include "LSM6DSOXSensor.h"

#define SR 417.0f // Sample rate. Options are: 12.5, 26, 52, 104, 208, 417, 833, 1667, 3333 and 6667 Hz
#define WTM_LV 102 // Watermark threshold level. Max samples in this FIFO configuration is 512 (accel and gyro only).

// Define interrupt pins according to MCU board and sensor wiring.
#define INT1_pin A0 // MCU input pin connected to sensor INT1 output pin

/** LSM6DSOX i2c address:
 * LSM6DSOX_I2C_ADD_L: 0x6A (default)
 * LSM6DSOX_I2C_ADD_H: 0x6B 
 **/
LSM6DSOXSensor lsm6dsoxSensor = LSM6DSOXSensor(&Wire, LSM6DSOX_I2C_ADD_L);

volatile uint8_t fullFlag = 0; // FIFO full flag

// ISR callback for INT1
void INT1_fullEvent_cb() {
  fullFlag = 1;
}

void setup() {

  Serial.begin(921600);
  // Comment this line to skip waiting for serial:
  while(!Serial) delay(10);
  
  // i2c, fast mode
  Wire.begin();
  Wire.setClock(400000);

  // Interrupt pin settings
  pinMode(INT1_pin, INPUT);
  attachInterrupt(digitalPinToInterrupt(INT1_pin), INT1_fullEvent_cb, RISING); // attach watermark event to INT1 input pin

  // Initialize sensors
  lsm6dsoxSensor.begin();
  if (lsm6dsoxSensor.Enable_G() == LSM6DSOX_OK && lsm6dsoxSensor.Enable_X() == LSM6DSOX_OK) {
    Serial.println("Success enabling accelero and gyro");
  } else {
    Serial.println("Error enabling accelero and gyro");
    abort();
  }

  // Check device id
  uint8_t id;
  lsm6dsoxSensor.ReadID(&id);
  if (id != LSM6DSOX_ID) {
    Serial.println("Wrong id for LSM6DSOX sensor. Check that device is plugged");
    abort();
  } else {
    Serial.println("Success checking id for LSM6DSOX sensor");
  }

  // Set accelerometer scale. Available values are: 2, 4, 8, 16 G
  lsm6dsoxSensor.Set_X_FS(2);
  // Set gyroscope scale. Available values are: 125, 250, 500, 1000, 2000 dps
  lsm6dsoxSensor.Set_G_FS(250);
  
  // Set accelerometer Output Data Rate. Available values are: 1.6, 12.5, 26, 52, 104, 208, 417, 833, 1667, 3333, 6667 Hz
  lsm6dsoxSensor.Set_X_ODR(SR);
  // Set gyroscope Output Data Rate. Available values are 12.5, 26, 52, 104, 208, 417, 833, 1667, 3333, 6667 Hz
  lsm6dsoxSensor.Set_G_ODR(SR);
  
  // Set FIFO Batch Data Rate for accelerometer and gyroscope. Available values are: 0, 12.5, 26, 52, 104, 208, 417, 833, 1667, 3333, 6667 Hz
  lsm6dsoxSensor.Set_FIFO_X_BDR(SR);
  lsm6dsoxSensor.Set_FIFO_G_BDR(SR);

  /** Set FIFO operation mode. Available values are:
   * LSM6DSOX_BYPASS_MODE: FIFO is not used, the buffer content is cleared
   * LSM6DSOX_FIFO_MODE: bufer continues filling until it becomes full. Then it stops collecting data.
   * LSM6DSOX_STREAM_MODE: continuous mode. Older data are replaced by the new data.
   * LSM6DSOX_STREAM_TO_FIFO_MODE: FIFO buffer starts operating in Continuous mode and switches to FIFO mode when an event condition occurs.
   * LSM6DSOX_BYPASS_TO_STREAM_MODE: FIFO buffer starts operating in Bypass mode and switches to Continuous mode when an event condition occurs.
   * */
  lsm6dsoxSensor.Set_FIFO_Mode(LSM6DSOX_BYPASS_MODE); // flush any previous value in FIFO before start

  lsm6dsoxSensor.Set_FIFO_INT1_FIFO_Full(1); // enable FIFO full interrupt on sensor INT1 pin

  // Set FIFO watermark level. Can be used to check when the number of samples in buffer reaches this defined threshold level.
  lsm6dsoxSensor.Set_FIFO_Watermark_Level(WTM_LV);
  
  // FIFO size can be limited to the watermark level by setting the STOP_ON_WTM flag to 1
  lsm6dsoxSensor.Set_FIFO_Stop_On_Fth(1);

  // start batching in continous mode
  lsm6dsoxSensor.Set_FIFO_Mode(LSM6DSOX_STREAM_MODE);

  Serial.println("Starting...");
}

void loop() {

  uint16_t numSamples = 0; // number of samples in FIFO
  uint8_t Tag; // FIFO data sensor identifier
  int32_t acceleration[3]; // X, Y, Z accelerometer values in mg
  int32_t rotation[3]; // X, Y, Z giroscope values in mdps
  int16_t dummy[3];
  uint8_t fullStatus = 0; // full status
  int32_t count_acc_samples = 0;
  int32_t count_gyro_samples = 0;
  int32_t count_dummy_samples = 0;

  // Get number of samples in buffer
  lsm6dsoxSensor.Get_FIFO_Num_Samples(&numSamples);
  Serial.print("Samples in FIFO: ");
  Serial.println(numSamples);

  // Check if FIFO threshold level was reached.
  if (fullFlag != 0) {
    fullFlag = 0;

    lsm6dsoxSensor.Get_FIFO_Full_Status(&fullStatus);

    if(fullStatus) {
      Serial.println("-- FIFO Watermark level reached!, fetching data.");

      lsm6dsoxSensor.Get_FIFO_Num_Samples(&numSamples);

      Serial.print("numSamples=");
      Serial.println(numSamples);

      // fetch data from FIFO
      for (uint16_t i = 0; i < numSamples; i++) {
  
        lsm6dsoxSensor.Get_FIFO_Tag(&Tag); // get data identifier
      
        // Get gyroscope data
        if (Tag == 1) { 
          lsm6dsoxSensor.Get_FIFO_G_Axes(rotation);
          count_gyro_samples++;
          #if 1 // set to 1 for printing values
          Serial.print("mdps: "); Serial.print(rotation[0]); 
          Serial.print(", "); Serial.print(rotation[1]); 
          Serial.print(", "); Serial.print(rotation[2]); 
          Serial.println();
          #endif
        } 
      
        // Get accelerometer data
        else if (Tag == 2) {
          lsm6dsoxSensor.Get_FIFO_X_Axes(acceleration);
          count_acc_samples++; 
          #if 1 // set to 1 for printing values
          Serial.print("mG: "); Serial.print(acceleration[0]); 
          Serial.print(", "); Serial.print(acceleration[1]); 
          Serial.print(", "); Serial.print(acceleration[2]); 
          Serial.println();
          #endif
        }

        // Flush unused tag
        else {
          lsm6dsoxSensor.Get_FIFO_Data((uint8_t *)dummy);
          count_dummy_samples++;
        }
      }
      Serial.print("Acc Samples: ");
      Serial.println(count_acc_samples);
      Serial.print("Gyro Samples: ");
      Serial.println(count_gyro_samples);
      Serial.print("Dummy Samples: ");
      Serial.println(count_dummy_samples);
    }
  }
}