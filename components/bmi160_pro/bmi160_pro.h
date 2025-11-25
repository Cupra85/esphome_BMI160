#pragma once
#include "esphome/core/component.h"
#include "esphome/core/sensor.h"
#include "esphome/core/binary_sensor.h"
#include "BMI160Gen.h"

namespace esphome {
namespace bmi160_advanced {

class BMI160Advanced : public PollingComponent {
 public:
  // Numeric sensors
  Sensor *accel_x = new Sensor();
  Sensor *accel_y = new Sensor();
  Sensor *accel_z = new Sensor();
  Sensor *gyro_x  = new Sensor();
  Sensor *gyro_y  = new Sensor();
  Sensor *gyro_z  = new Sensor();
  Sensor *pitch   = new Sensor();
  Sensor *roll    = new Sensor();
  Sensor *incl    = new Sensor();
  Sensor *temperature = new Sensor();
  Sensor *vibration   = new Sensor();

  // Alerts
  BinarySensor *tilt_alert = new BinarySensor();
  BinarySensor *motion_alert = new BinarySensor();

  // User config
  float tilt_threshold_deg = 10.0f;
  float motion_threshold_ms2 = 1.0f;

  BMI160Advanced() : PollingComponent(50) {}

  void setup() override;
  void update() override;
};

}  // namespace bmi160_advanced
}  // namespace esphome
