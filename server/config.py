import dataclasses

from led_condition import LedColor, LedCondition, LedConditionType


@dataclasses.dataclass
class Node:
    addr: int
    name: str
    led_dis: bool


class config:
    uart_port = "COM5"
    uart_baud = 115200
    sql_table_types = "time INTEGER, addr INTEGER, retries INTEGER, vbat REAL, temperature_internal REAL, si7021_humidity REAL, si7021_temperature REAL, bmp180_temperature REAL, bmp180_pressure REAL, reed INTEGER"
    sql_table_fields = "time, addr, retries, vbat, temperature_internal, si7021_humidity, si7021_temperature, bmp180_temperature, bmp180_pressure, reed"
    barometric_height = 118
    in_regs_update = dict(second="*/5")
    led_conditions = [
        LedCondition(1, LedColor.BLUE, "bmp180_temperature < 23", 5*60, LedConditionType.ALL)
    ]
    nodes = [
        Node(1, "Wetterstation", False)
    ]
