#include "bmi160_pro.h"
#include "esphome/core/log.h"
#include <cmath>

namespace esphome {
namespace bmi160_pro {

static const char *const TAG = "bmi160_pro";

void BMI160Pro::setup() {
  ESP_LOGCONFIG(TAG, "Setting up BMI160...");
  this->write_byte(0x7E, 0xB6);
  delay(100);
  this->write_byte(0x40, 0x28);
  this->write_byte(0x42, 0x26);
  last_time_ = millis();
  ESP_LOGI(TAG, "BMI160 setup done");
}

void BMI160Pro::update() {
  uint8_t data[12];
  if (!this->read_bytes(0x12, data, 12)) return;

  int16_t ax = (data[1] << 8) | data[0];
  int16_t ay = (data[3] << 8) | data[2];
  int16_t az = (data[5] << 8) | data[4];
  int16_t gx = (data[7] << 8) | data[6];
  int16_t gy = (data[9] << 8) | data[8];
  int16_t gz = (data[11] << 8) | data[10];

  const float A = 9.80665f / 16384.0f;
  const float G = 2000.0f / 32768.0f;

  float axm = ax * A, aym = ay * A, azm = az * A;
  float gxm = gx * G, gym = gy * G, gzm = gz * G;

  float pitch = atan2f(aym, sqrtf(axm * axm + azm * azm)) * 180.0f / M_PI;
  float roll  = atan2f(-axm, azm) * 180.0f / M_PI;

  uint8_t tmp;
  if (this->read_byte(0x20, &tmp)) temperature_ = tmp * 0.5f + 23.0f;

  float vibe = fabsf(axm) + fabsf(aym) + fabsf(azm) - 9.80665f;

  // ---- COMPLEMENTARY FILTER ----
  uint32_t now = millis();
  float dt = (now - last_time_) / 1000.0f;
  last_time_ = now;
  if (dt < 0.0001f) dt = 0.01f;

  pitch_filt_ = filter_alpha_ * (pitch_filt_ + gym * dt) + (1.0f - filter_alpha_) * pitch;
  roll_filt_  = filter_alpha_ * (roll_filt_  + gxm * dt) + (1.0f - filter_alpha_) * roll;
  float incl_filt = sqrtf(pitch_filt_ * pitch_filt_ + roll_filt_ * roll_filt_);

  // ---- PUBLISH ----
  if (accel_x_sensor_) accel_x_sensor_->publish_state(axm);
  if (accel_y_sensor_) accel_y_sensor_->publish_state(aym);
  if (accel_z_sensor_) accel_z_sensor_->publish_state(azm);

  if (gyro_x_sensor_) gyro_x_sensor_->publish_state(gxm);
  if (gyro_y_sensor_) gyro_y_sensor_->publish_state(gym);
  if (gyro_z_sensor_) gyro_z_sensor_->publish_state(gzm);

  if (pitch_sensor_) pitch_sensor_->publish_state(pitch_filt_);
  if (roll_sensor_) roll_sensor_->publish_state(roll_filt_);
  if (inclination_sensor_) inclination_sensor_->publish_state(incl_filt);

  if (temperature_sensor_) temperature_sensor_->publish_state(temperature_);
  if (vibration_sensor_) vibration_sensor_->publish_state(vibe);

  // ---- ALERTS ----
  if (tilt_alert_) tilt_alert_->publish_state(fabsf(pitch_filt_) > tilt_threshold_deg_ || fabsf(roll_filt_) > tilt_threshold_deg_);
  if (motion_alert_) motion_alert_->publish_state(fabsf(axm) > motion_threshold_ms2_ || fabsf(aym) > motion_threshold_ms2_ || fabsf(azm) > motion_threshold_ms2_);
}

}  // namespace bmi160_pro
}  // namespace esphome
