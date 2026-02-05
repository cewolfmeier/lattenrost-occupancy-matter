#include <Matter.h>
#include <MatterOccupancy.h>
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

Adafruit_MPU6050 lattenrost_1;
Adafruit_MPU6050 lattenrost_2;

MatterOccupancy matter_occupancy_sensor_1;
MatterOccupancy matter_occupancy_sensor_2;

void setup()
{
  // ---- serial setup---- 
  Serial.begin(115200);

  // ---- onboard component setup---- 
  pinMode(BTN_BUILTIN, INPUT_PULLUP);
  pinMode(LEDR, OUTPUT);
  digitalWrite(LEDR, HIGH);
  
  // ---- first I2C interface and sensor setup---- 
  Wire.begin();
  if (!lattenrost_1.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("First MPU6050 Found!");

  // ---- second I2C interface and sensor setup---- 
  
  Wire1.begin();
  if (!lattenrost_2.begin(104, &Wire1)) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("Second MPU6050 Found!");

  // ---- Matter setup---- 
  
  Matter.begin();
  matter_occupancy_sensor_1.begin();
  matter_occupancy_sensor_2.begin();

  Serial.println("Matter occupancy sensor");

  if (!Matter.isDeviceCommissioned()) {
    Serial.println("Matter device is not commissioned");
    Serial.println("Commission it to your Matter hub with the manual pairing code or QR code");
    Serial.printf("Manual pairing code: %s\n", Matter.getManualPairingCode().c_str());
    Serial.printf("QR code URL: %s\n", Matter.getOnboardingQRCodeUrl().c_str());
  }
  while (!Matter.isDeviceCommissioned()) {
    delay(200);
    decommission_handler();
  }

  Serial.println("Waiting for Thread network...");
  while (!Matter.isDeviceThreadConnected()) {
    delay(200);
    decommission_handler();
  }
  Serial.println("Connected to Thread network");

  Serial.println("Waiting for Matter device discovery...");
  while (!matter_occupancy_sensor_1.is_online() && !matter_occupancy_sensor_2.is_online()) {
    delay(200);
  }

  Serial.println("Matter device is now online");
}

void loop()
{
  decommission_handler();
  static uint32_t last_action = 0;
  // Wait 1 second
  if ((last_action + 1000) < millis()) {
    last_action = millis();

    sensors_event_t a, g, temp;

    lattenrost_1.getEvent(&a, &g, &temp);
    bool occ_1 = is_occupied(a, 1.0);

    lattenrost_2.getEvent(&a, &g, &temp);
    bool occ_2 = is_occupied(a, 1.0);

    matter_occupancy_sensor_1.set_occupancy(occ_1);
    matter_occupancy_sensor_2.set_occupancy(occ_2);
    Serial-printf("Current ouccupancy state 1: %s\n", occ_1 ? "occupied" : "unoccupied");
    Serial-printf("Current ouccupancy state 2: %s\n", occ_2 ? "occupied" : "unoccupied");
  }
}
