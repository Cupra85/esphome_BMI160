#include "bmi160.h"
#include <cmath>

namespace esphome {
namespace bmi160 {

void BMI160Component::setup() {
  Wire.begin();
  delay(100);

  if (!BMI160.begin(BMI160GenClass::I2C_MODE, 0x69)) {
    ESP_LOGE("bmi160", "BMI160 init failed at 0x69, trying 0x68...");
    if (!BMI160.begin(BMI160GenClass::I2C_MODE, 0x68)) {
      ESP_LOGE("bmi160", "BMI160 init failed at 0x68 - Sensor not found!");
      return;
    }
  }
  ESP_LOGI("bmi160", "BMI160 initialized successfully!");
}

void BMI160Component::update() {
  float ax, ay, az, gx, gy, gz;
  BMI160.readAccelerometer(ax, ay, az);
  BMI160.readGyroscope(gx, gy, gz);

  float temp = BMI160.readTemperature() / 512.0f + 23.0f;

  float ax_ms = ax * 9.80665f;
  float ay_ms = ay * 9.80665f;
  float az_ms = az * 9.80665f;

  accel_x->publish_state(ax_ms);
  accel_y->publish_state(ay_ms);
  accel_z->publish_state(az_ms);
  gyro_x->publish_state(gx);
  gyro_y->publish_state(gy);
  gyro_z->publish_state(gz);

  float pitch_val = atan2(-ax, sqrt(ay * ay + az * az)) * 180.0f / M_PI;
  float roll_val  = atan2(ay, az) * 180.0f / M_PI;
  float incl_val = acos(ax / sqrt(ax * ax + ay * ay + az * az)) * 180.0f / M_PI;

  pitch->publish_state(pitch_val);
  roll->publish_state(roll_val);
  incl->publish_state(incl_val);
  temperature->publish_state(temp);

  float vib = fabs(sqrt(ax_ms*ax_ms + ay_ms*ay_ms + az_ms*az_ms) - 9.80665f);
  vibration->publish_state(vib);
}

}  // namespace bmi160
}  // namespace esphome
