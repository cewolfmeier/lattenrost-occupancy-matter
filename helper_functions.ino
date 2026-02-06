/**
* Get the pitch value from a MPU6050.
*/
float get_pitch(MPU6050 *mpu) {
  int16_t ax, ay, az;
  mpu->getAcceleration(&ax, &ay, &az);
  float pitch = atan2((float)-ax, sqrt((float)ay*ay + (float)az*az)) * 180.0 / M_PI;
  return pitch; 
}

/**
* Get the roll value from a MPU6050.
*/
float get_roll(MPU6050 *mpu) {
  int16_t ax, ay, az;
  mpu->getAcceleration(&ax, &ay, &az);
  float roll = atan2((float)ay, (float)az) * 180.0 / M_PI;
  return roll;
}

/**
* Get the yaw rate from a MPU6050 in degrees/sec.
* Note: True yaw angle requires a magnetometer. The MPU6050 only has a
* gyroscope, so this returns the instantaneous rotation rate around Z.
*/
float get_yaw(MPU6050 *mpu) {
  int16_t gx, gy, gz;
  mpu->getRotation(&gx, &gy, &gz);
  float yaw_rate = (float)gz / 131.0;  // 131 LSB/(deg/s) at default ±250 deg/s
  return yaw_rate;
}

/**
* Determine if a sensor registers an occupancy.
*/
bool is_occupied(MPU6050 *mpu, float threshold) {
  float pitch = get_pitch(mpu);
  return pitch > threshold ? true : false;
}

/**
* Decommission the device from the network.
* 
* The button has to be pressed for at least 10 seconds to initiate the procedure.
*/
void decommission_handler() {
  if (digitalRead(BTN_BUILTIN) == LOW) {  //Push button pressed
    // measures time pressed
    int startTime = millis();
    while (digitalRead(BTN_BUILTIN) == LOW) {
      //delay(50);

      int elapsedTime = (millis() - startTime) / 1000.0;

      if (elapsedTime > 10) {
        Serial.printf("Decommissioning!\n");
        for (int i = 0; i < 10; i++) {
          digitalWrite(LEDR, !(digitalRead(LEDR)));
          delay(100);
        };

        if (!Matter.isDeviceCommissioned()) {
          Serial.println("Decommission done!");
          digitalWrite(LEDR, LOW);
        } else {
          Serial.println("Matter device is commissioned-> Starting Decommission process");
          nvm3_eraseAll(nvm3_defaultHandle);  // Decomission command
          digitalWrite(LED_BUILTIN, LOW);
          Serial.println("Decommission done!");
        }
        break;
      }
    }
  }
}