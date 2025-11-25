#include "bmi160_advanced.h"
#include <cmath>

namespace esphome {
namespace bmi160_advanced {

void BMI160Advanced::setup() {
  Wire.begin();
  delay(100);

  if (!BMI160.begin(BMI160GenClass::I2C_MODE, 0x69)) {
    ESP_LOGW("bmi160", "0x69 not found, trying 0x68");
    if (!BMI160.begin(BMI160GenClass::I2C_MODE, 0x68)) {
      ESP_LOGE("bmi160", "BMI160 not detected at 0x68 or 0x69");
      return;
    }
  }
  ESP_LOGI("bmi160", "BMI160 initialized");
}

void BMI160Advanced::update() {
  float ax, ay, az, gx, gy, gz;
  BMI160.readAccelerometer(ax, ay, az);
  BMI160.readGyroscope(gx, gy, gz);
  float temp = BMI160.readTemperature() / 512.0f + 23.0f;

  // Convert accel to m/s²
  float ax_ms = ax * 9.80665f;
  float ay_ms = ay * 9.80665f;
  float az_ms = az * 9.80665f;

  accel_x->publish_state(ax_ms);
  accel_y->publish_state(ay_ms);
  accel_z->publish_state(az_ms);
  gyro_x->publish_state(gx);
  gyro_y->publish_state(gy);
  gyro_z->publish_state(gz);

  // Angles
  float pitch_val = atan2(-ax, sqrtf(ay*ay + az*az)) * 180.0f / M_PI;
  float roll_val  = atan2(ay, az) * 180.0f / M_PI;
  float incl_val  = acosf(ax / sqrtf(ax*ax + ay*ay + az*az)) * 180.0f / M_PI;

  pitch->publish_state(pitch_val);
  roll->publish_state(roll_val);
  incl->publish_state(incl_val);
  temperature->publish_state(temp);

  // Vibration
  float vib = fabsf(sqrtf(ax_ms*ax_ms + ay_ms*ay_ms + az_ms*az_ms) - 9.80665f);
  vibration->publish_state(vib);

  // Alerts
  tilt_alert->publish_state(
    fabsf(pitch_val) > tilt_threshold_deg ||
    fabsf(roll_val)  > tilt_threshold_deg
  );

  motion_alert->publish_state(
    vib > motion_threshold_ms2
  );
}

}  // namespace bmi160_advanced
}  // namespace esphome
