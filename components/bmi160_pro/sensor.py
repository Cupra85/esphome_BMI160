import esphome.codegen as cg
import esphome.config_validation as cv
import esphome.components.sensor as es_sensor
import esphome.components.binary_sensor as es_bsensor

# Force load of core modules without component.yaml
import esphome.components.binary_sensor
import esphome.components.sensor
import esphome.components.i2c

from esphome.const import CONF_ID

bmi160_pro_ns = cg.esphome_ns.namespace("bmi160_pro")
BMI160Pro = bmi160_pro_ns.class_("BMI160Pro", cg.PollingComponent)

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(BMI160Pro),

        cv.Optional("tilt_threshold_deg", default=10.0): cv.float_,
        cv.Optional("motion_threshold_ms2", default=1.0): cv.float_,

        cv.Optional("accel_x"): es_sensor.sensor_schema(),
        cv.Optional("accel_y"): es_sensor.sensor_schema(),
        cv.Optional("accel_z"): es_sensor.sensor_schema(),
        cv.Optional("gyro_x"): es_sensor.sensor_schema(),
        cv.Optional("gyro_y"): es_sensor.sensor_schema(),
        cv.Optional("gyro_z"): es_sensor.sensor_schema(),
        cv.Optional("pitch"): es_sensor.sensor_schema(),
        cv.Optional("roll"): es_sensor.sensor_schema(),
        cv.Optional("inclination"): es_sensor.sensor_schema(),
        cv.Optional("temperature"): es_sensor.sensor_schema(),
        cv.Optional("vibration"): es_sensor.sensor_schema(),

        cv.Optional("tilt_alert"): es_bsensor.binary_sensor_schema(),
        cv.Optional("motion_alert"): es_bsensor.binary_sensor_schema(),
    }
).extend(cv.COMPONENT_SCHEMA)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    cg.add(cg.RawExpression(f"{var}->tilt_threshold_deg = {config['tilt_threshold_deg']};"))
    cg.add(cg.RawExpression(f"{var}->motion_threshold_ms2 = {config['motion_threshold_ms2']};"))

    for key in [
        "accel_x", "accel_y", "accel_z",
        "gyro_x", "gyro_y", "gyro_z",
        "pitch", "roll", "inclination",
        "temperature", "vibration"
    ]:
        if key in config:
            sens = await es_sensor.new_sensor(config[key])
            cg.add(getattr(var, key), sens)

    for key in ["tilt_alert", "motion_alert"]:
        if key in config:
            bs = await es_bsensor.new_binary_sensor(config[key])
            cg.add(getattr(var, key), bs)
