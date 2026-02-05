#include <Adafruit_MPU6050.h>
/**
* Get the pitch value from a MPU6050.
*/
float get_pitch(sensors_event_t a) {
  float ax=a.acceleration.x/9.81;
  float ay=a.acceleration.y/9.81;
  float az=a.acceleration.z/9.81;
  float pitch = atan2(ay,sqrt(az*az+ax*ax))*360/(2*3.14);
  return pitch; 
}

/**
* Determine if a sensor registers an occupancy.
*/
bool is_occupied(sensors_event_t a, float threshold) {
  bool result = false;
  float pitch = get_pitch(a);
  if (pitch > threshold) {
    result = true;
  }
  return result;
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