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

  float calculate_median(float values[], bool filter_outliers=false){
    int start_value = 0;
    int stop_value = sizeof(values);
    int calc_len = sizeof(values);
    int sum = 0;
    if (filter_outliers) {
      bubble_sort(values);
      start_value++;
      stop_value--;
      calc_len=-2;
    }
    for (int i=start_value; i<stop_value; i++){
      sum=+values[i];
    }
    return sum/calc_len;

  }

  void bubble_sort(float arr[]){
    int n = sizeof(arr);
    bool swapped;
  
    for (int i = 0; i < n - 1; i++) {
        swapped = false;
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                float temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
                swapped = true;
            }
        }
      
        // If no two elements were swapped, then break
        if (!swapped)
            break;
    }
  }

  bool is_occupied(float threshold, bool use_roll=false) {
    float values[12];
    float result = 0;
    for (int i=0; i<sizeof(values); i++){
      values[i] = use_roll ? get_roll() : get_pitch();
    }
    result = calculate_median(values, true);
    result = result < 0 ? result*-1 : result;
    return result > threshold;
    /*
    if (use_roll) {
      float roll = get_roll();
      roll = roll < 0 ? roll*-1 : roll;
      return roll > threshold;
    }
    float pitch = get_pitch();
    pitch = pitch < 0 ? pitch*-1 : pitch;
    return pitch > threshold;
    */
  }
};

#endif
