#include <Matter.h>
#include <MatterOccupancy.h>
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

Adafruit_MPU6050 mpu_1;
MatterOccupancy matter_occupancy_sensor;

float Ax;
float Ay;
float Az;
float pitch;

void setup()
{
  Serial.begin(115200);
  Wire.begin();
  Matter.begin();
  matter_occupancy_sensor.begin();

  pinMode(BTN_BUILTIN, INPUT_PULLUP);
  pinMode(LEDR, OUTPUT);
  digitalWrite(LEDR, HIGH);

  if (!mpu_1.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  Serial.println("Matter occupancy sensor");

  if (!Matter.isDeviceCommissioned()) {
    Serial.println("Matter device is not commissioned");
    Serial.println("Commission it to your Matter hub with the manual pairing code or QR code");
    Serial.printf("Manual pairing code: %s\n", Matter.getManualPairingCode().c_str());
    Serial.printf("QR code URL: %s\n", Matter.getOnboardingQRCodeUrl().c_str());
  }
  while (!Matter.isDeviceCommissioned()) {
    delay(200);
    decommission_handler();  // if the user button is pressed for 10 seconds
  }

  Serial.println("Waiting for Thread network...");
  while (!Matter.isDeviceThreadConnected()) {
    delay(200);
    decommission_handler();
  }
  Serial.println("Connected to Thread network");

  Serial.println("Waiting for Matter device discovery...");
  while (!matter_occupancy_sensor.is_online()) {
    delay(200);
  }

  Serial.println("Matter device is now online");
}

void loop()
{
  decommission_handler();  // if the user button is pressed for 10 seconds

  
  static uint32_t last_action = 0;
  // Wait 10 seconds
  if ((last_action + 10000) < millis()) {
    last_action = millis();
    // Toggle the state of the occupancy sensor
    bool new_state = !matter_occupancy_sensor.get_occupancy();
    // Publish the occupancy value - you can also use 'matter_occupancy_sensor.set_occupancy(new_state)'
    matter_occupancy_sensor = new_state;
    Serial.printf("Current ouccupancy state: %s\n", new_state ? "occupied" : "unoccupied");
    get_pitch();
  }
}

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

void get_pitch() {
  sensors_event_t a, g, temp;
  mpu_1.getEvent(&a, &g, &temp);
  Ax=a.acceleration.x/9.81;
  Ay=a.acceleration.y/9.81;
  Az=a.acceleration.z/9.81;
  pitch = atan2(Ay,sqrt(Az*Az+Ax*Ax))*360/(2*3.14);
  Serial.print("Pitch:");
  Serial.println(pitch);
}
