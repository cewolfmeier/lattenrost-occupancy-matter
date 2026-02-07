#include <Matter.h>
#include <MatterOccupancy.h>
#include "Lattenrost.h"

// ---- CONFIG BLOCK -- CHANGE AS NEEDED ----
//Set offsets. To measure them look at the instructions provided here: https://wired.chillibasket.com/2015/01/calibrating-mpu6050/
float OFFSET_0[] = {-4289, 906, 1279, 41, -16, 17}; // ax, ay, az, gx, gy, gz
float OFFSET_1[] = {1700, -1334, 1016, 54, -16, 9}; // ax, ay, az, gx, gy, gz
//Set thresholds for occupancy detection. These are set for roll angle.
float THRESHOLD_0 = 178.1;
float THRESHOLD_1 = -177.5;
bool USE_ROLL_0 = true;
bool USE_ROLL_1 = true;
// ---- CONFIG BLOCK END ----

Adafruit_MPU6050 mpu_0;
Adafruit_MPU6050 mpu_1;

Lattenrost lattenrost_0(mpu_0);
Lattenrost lattenrost_1(mpu_1);

MatterOccupancy matter_occupancy_0;
MatterOccupancy matter_occupancy_1;

void writeOffset(TwoWire& wire, uint8_t addr, uint8_t reg, int16_t value) {
  wire.beginTransmission(addr);
  wire.write(reg);
  wire.write((uint8_t)(value >> 8));
  wire.write((uint8_t)(value & 0xFF));
  wire.endTransmission();
}

void applyOffsets(TwoWire& wire, uint8_t addr, float offsets[]) {
  // Accel offsets: registers 0x06, 0x08, 0x0A
  writeOffset(wire, addr, 0x06, (int16_t)offsets[0]);
  writeOffset(wire, addr, 0x08, (int16_t)offsets[1]);
  writeOffset(wire, addr, 0x0A, (int16_t)offsets[2]);
  // Gyro offsets: registers 0x13, 0x15, 0x17
  writeOffset(wire, addr, 0x13, (int16_t)offsets[3]);
  writeOffset(wire, addr, 0x15, (int16_t)offsets[4]);
  writeOffset(wire, addr, 0x17, (int16_t)offsets[5]);
}

void setup()
{
  // ---- serial setup----
  Serial.begin(115200);

  // ---- onboard component setup----
  pinMode(BTN_BUILTIN, INPUT_PULLUP);
  pinMode(LEDR, OUTPUT);
  digitalWrite(LEDR, HIGH);

  // ---- first I2C interface and sensor setup----
  if (!mpu_0.begin(0x68, &Wire)) {
    Serial.println("Failed to find first MPU6050 chip");
    while (1) { delay(10); }
  }
  Serial.println("First MPU6050 Found!");
  applyOffsets(Wire, 0x68, OFFSET_0);

  // ---- second I2C interface and sensor setup----
  if (!mpu_1.begin(0x68, &Wire1)) {
    Serial.println("Failed to find second MPU6050 chip");
    while (1) { delay(10); }
  }
  Serial.println("Second MPU6050 Found!");
  applyOffsets(Wire1, 0x68, OFFSET_1);

  // ---- Matter setup----

  Matter.begin();
  matter_occupancy_0.begin();
  matter_occupancy_0.set_device_name("Lattenrost Sensor 1");
  matter_occupancy_1.begin();
  matter_occupancy_1.set_device_name("Lattenrost Sensor 2");

  Serial.println("Matter endpoints initialized");

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
  while (!matter_occupancy_0.is_online() && !matter_occupancy_1.is_online()) {
    delay(200);
  }

  Serial.println("Matter device is now online");
  
}

void loop()
{
  decommission_handler();
  static uint32_t last_action = 0;
  // Wait 0.5 seconds
  if ((last_action + 500) < millis()) {
    last_action = millis();

    bool occ_0 = lattenrost_0.is_occupied(THRESHOLD_0, USE_ROLL_0);
    bool occ_1 = lattenrost_1.is_occupied(THRESHOLD_1, USE_ROLL_1);

    matter_occupancy_0.set_occupancy(occ_0);
    matter_occupancy_1.set_occupancy(occ_1);

    Serial.printf("Sensor 0 - pitch: %.1f, roll: %.1f, threshold: %.1f, mode: %s, %s\n",
                  lattenrost_0.get_pitch(), lattenrost_0.get_roll(), THRESHOLD_0, USE_ROLL_0 ? "roll" : "pitch",
                  occ_0 ? "occupied" : "unoccupied");
    Serial.printf("Sensor 1 - pitch: %.1f, roll: %.1f, threshold: %.1f, mode: %s, %s\n",
                  lattenrost_1.get_pitch(), lattenrost_1.get_roll(), THRESHOLD_1, USE_ROLL_1 ? "roll" : "pitch",
                  occ_1 ? "occupied" : "unoccupied");
  }
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
