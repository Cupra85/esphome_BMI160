#pragma once
#include "esphome/core/component.h"
#include "esphome/components/i2c/i2c.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/binary_sensor/binary_sensor.h"

namespace esphome {
namespace bmi160_pro {

class BMI160Pro : public PollingComponent, public i2c::I2CDevice {
 public:
  void setup() override;
  void update() override;
  float get_setup_priority() const override { return setup_priority::DATA; }

  // Sensor setter
  void set_accel_x_sensor(sensor::Sensor *s) { accel_x_sensor_ = s; }
  void set_accel_y_sensor(sensor::Sensor *s) { accel_y_sensor_ = s; }
  void set_accel_z_sensor(sensor::Sensor *s) { accel_z_sensor_ = s; }
  void set_gyro_x_sensor(sensor::Sensor *s) { gyro_x_sensor_ = s; }
  void set_gyro_y_sensor(sensor::Sensor *s) { gyro_y_sensor_ = s; }
  void set_gyro_z_sensor(sensor::Sensor *s) { gyro_z_sensor_ = s; }
  void set_pitch_sensor(sensor::Sensor *s) { pitch_sensor_ = s; }
  void set_roll_sensor(sensor::Sensor *s) { roll_sensor_ = s; }
  void set_inclination_sensor(sensor::Sensor *s) { inclination_sensor_ = s; }
  void set_temperature_sensor(sensor::Sensor *s) { temperature_sensor_ = s; }
  void set_vibration_sensor(sensor::Sensor *s) { vibration_sensor_ = s; }

  // Binary sensors
  void set_tilt_alert(binary_sensor::BinarySensor *b) { tilt_alert_ = b; }
  void set_motion_alert(binary_sensor::BinarySensor *b) { motion_alert_ = b; }

  // Threshold setter
  void set_tilt_threshold_deg(float v) { tilt_threshold_deg_ = v; }
  void set_motion_threshold_ms2(float v) { motion_threshold_ms2_ = v; }
  void set_vibration_threshold_ms2(float v) { vibration_threshold_ms2_ = v; }

 protected:
  // Outputs
  sensor::Sensor *accel_x_sensor_{nullptr};
  sensor::Sensor *accel_y_sensor_{nullptr};
  sensor::Sensor *accel_z_sensor_{nullptr};
  sensor::Sensor *gyro_x_sensor_{nullptr};
  sensor::Sensor *gyro_y_sensor_{nullptr};
  sensor::Sensor *gyro_z_sensor_{nullptr};
  sensor::Sensor *pitch_sensor_{nullptr};
  sensor::Sensor *roll_sensor_{nullptr};
  sensor::Sensor *inclination_sensor_{nullptr};
  sensor::Sensor *temperature_sensor_{nullptr};
  sensor::Sensor *vibration_sensor_{nullptr};

  // Binary outputs
  binary_sensor::BinarySensor *tilt_alert_{nullptr};
  binary_sensor::BinarySensor *motion_alert_{nullptr};

  // Thresholds
  float tilt_threshold_deg_{15.0f};
  float motion_threshold_ms2_{0.3f};
  float vibration_threshold_ms2_{0.5f};

  float temperature_{0.0f};
};

}  // namespace bmi160_pro
}  // namespace esphome
