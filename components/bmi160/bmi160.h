#pragma once
#include "esphome/core/component.h"
#include "esphome/core/sensor.h"
#include "BMI160Gen.h"

namespace esphome {
namespace bmi160 {

class BMI160Component : public PollingComponent {
 public:
  // Sensor objects
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

  BMI160Component() : PollingComponent(50) {} // 20Hz Update

  void setup() override;
  void update() override;
};

}  // namespace bmi160
}  // namespace esphome
