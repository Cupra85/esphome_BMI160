import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor, binary_sensor
from esphome.const import CONF_ID

# Namespace mit deiner C++ Klasse
bmi160_pro_ns = cg.esphome_ns.namespace("bmi160_pro")
BMI160Pro = bmi160_pro_ns.class_("BMI160Pro", cg.PollingComponent)

# YAML-Schema, definiert erlaubte Optionen
CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(BMI160Pro),

        cv.Optional("tilt_threshold_deg", default=10.0): cv.float_,
        cv.Optional("motion_threshold_ms2", default=1.0): cv.float_,

        cv.Optional("accel_x"): sensor.sensor_schema(),
        cv.Optional("accel_y"): sensor.sensor_schema(),
        cv.Optional("accel_z"): sensor.sensor_schema(),
        cv.Optional("gyro_x"): sensor.sensor_schema(),
        cv.Optional("gyro_y"): sensor.sensor_schema(),
        cv.Optional("gyro_z"): sensor.sensor_schema(),
        cv.Optional("pitch"): sensor.sensor_schema(),
        cv.Optional("roll"): sensor.sensor_schema(),
        cv.Optional("inclination"): sensor.sensor_schema(),
        cv.Optional("temperature"): sensor.sensor_schema(),
        cv.Optional("vibration"): sensor.sensor_schema(),

        cv.Optional("tilt_alert"): binary_sensor.binary_sensor_schema(),
        cv.Optional("motion_alert"): binary_sensor.binary_sensor_schema(),
    }
).extend(cv.COMPONENT_SCHEMA)


# Verbindet die Python-Seite mit deinem C++ Code
async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    # Thresholds als Raw Expressions setzen (Zeiger "->" ist Pflicht!)
    cg.add(cg.RawExpression(f"{var}->tilt_threshold_deg = {config['tilt_threshold_deg']};"))
    cg.add(cg.RawExpression(f"{var}->motion_threshold_ms2 = {config['motion_threshold_ms2']};"))

    # Sensors als Ausgabe verknüpfen
    for key in [
        "accel_x", "accel_y", "accel_z",
        "gyro_x", "gyro_y", "gyro_z",
        "pitch", "roll", "inclination",
        "temperature", "vibration"
    ]:
        if key in config:
            sens = await sensor.new_sensor(config[key])
            cg.add(getattr(var, key), sens)

    # Binary-Sensor-Ausgänge verknüpfen
    for key in ["tilt_alert", "motion_alert"]:
        if key in config:
            bs = await binary_sensor.new_binary_sensor(config[key])
            cg.add(getattr(var, key), bs)
