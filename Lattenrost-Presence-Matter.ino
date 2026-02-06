#include <Matter.h>
#include <MatterOccupancy.h>
#include <Wire.h>
#include <MPU6050.h>

float OFFSET_0[] = {-4289, 906, 1279, 41, -16, 17};
float OFFSET_1[] = {1700, -1334, 1016, 54, -16, 9};

MPU6050 lattenrost_0;
MPU6050 lattenrost_1;

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
  lattenrost_0.initialize();
  if (!lattenrost_0.testConnection()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("First MPU6050 Found!");
  /* Use the code below to change accel/gyro offset values. Use MPU6050_Zero to obtain the recommended offsets */ 
  Serial.println("Updating internal sensor offsets...\n");
  lattenrost_0.setXAccelOffset(OFFSET_0[0]); //Set your accelerometer offset for axis X
  lattenrost_0.setYAccelOffset(OFFSET_0[1]); //Set your accelerometer offset for axis Y
  lattenrost_0.setZAccelOffset(OFFSET_0[2]); //Set your accelerometer offset for axis Z
  lattenrost_0.setXGyroOffset(OFFSET_0[3]);  //Set your gyro offset for axis X
  lattenrost_0.setYGyroOffset(OFFSET_0[4]);  //Set your gyro offset for axis Y
  lattenrost_0.setZGyroOffset(OFFSET_0[5]);  //Set your gyro offset for axis Z

  // ---- second I2C interface and sensor setup---- 
  
  Wire1.begin();
  lattenrost_1.initialize();
  if (!lattenrost_1.testConnection()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("Second MPU6050 Found!");
  Serial.println("Updating internal sensor offsets...\n");
  lattenrost_1.setXAccelOffset(OFFSET_1[0]); //Set your accelerometer offset for axis X
  lattenrost_1.setYAccelOffset(OFFSET_1[1]); //Set your accelerometer offset for axis Y
  lattenrost_1.setZAccelOffset(OFFSET_1[2]); //Set your accelerometer offset for axis Z
  lattenrost_1.setXGyroOffset(OFFSET_1[3]);  //Set your gyro offset for axis X
  lattenrost_1.setYGyroOffset(OFFSET_1[4]);  //Set your gyro offset for axis Y
  lattenrost_1.setZGyroOffset(OFFSET_1[5]);  //Set your gyro offset for axis Z

  // ---- Matter setup---- 
  /*
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
  */
}

void loop()
{
  //decommission_handler();
  static uint32_t last_action = 0;
  // Wait 1 second
  if ((last_action + 1000) < millis()) {
    last_action = millis();

    Serial.print("Sensor 0: ");
    Serial.println(get_pitch(&lattenrost_0));
    Serial.println(get_roll(&lattenrost_0));
    Serial.println(get_yaw(&lattenrost_0));
    Serial.print("Sensor 1: ");
    Serial.println(get_pitch(&lattenrost_1));
    Serial.println(get_roll(&lattenrost_1));
    Serial.println(get_yaw(&lattenrost_1));
    Serial.println();
    //bool occ_0 = is_occupied(&lattenrost_1, 1.0);

    //bool occ_1 = is_occupied(&lattenrost_1, 1.0);
    /*
    matter_occupancy_sensor_1.set_occupancy(occ_0);
    matter_occupancy_sensor_2.set_occupancy(occ_1);
    Serial-printf("Current ouccupancy state 1: %s\n", occ_0 ? "occupied" : "unoccupied");
    Serial-printf("Current ouccupancy state 2: %s\n", occ_1 ? "occupied" : "unoccupied");
    */
  }
}
