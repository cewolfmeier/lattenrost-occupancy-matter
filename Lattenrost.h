#ifndef LATTENROST_H
#define LATTENROST_H

#include <Adafruit_MPU6050.h>

class Lattenrost {
private:
  Adafruit_MPU6050& _mpu;

public:
  Lattenrost(Adafruit_MPU6050& mpu) : _mpu(mpu) {}

  float get_pitch() {
    sensors_event_t a, g, t;
    _mpu.getEvent(&a, &g, &t);
    return atan2(-a.acceleration.x,
                 sqrt(a.acceleration.y * a.acceleration.y +
                      a.acceleration.z * a.acceleration.z)) * 180.0 / M_PI;
  }

  float get_roll() {
    sensors_event_t a, g, t;
    _mpu.getEvent(&a, &g, &t);
    return atan2(a.acceleration.y, a.acceleration.z) * 180.0 / M_PI;
  }

  float get_yaw() {
    sensors_event_t a, g, t;
    _mpu.getEvent(&a, &g, &t);
    return g.gyro.z * 180.0 / M_PI;  // rad/s -> deg/s
  }

  bool is_occupied(float threshold, bool use_roll=false) {
    if (use_roll) {
      return threshold > 0 ? get_roll() > threshold : get_roll() < threshold;
    }
    return threshold > 0 ? get_pitch() > threshold : get_pitch() < threshold;
  }
};

#endif
