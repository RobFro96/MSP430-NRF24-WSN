import dataclasses
import logging
import typing


class config:
    uart_port = "COM5"
    uart_baud = 115200
    sql_table_types = "time INTEGER, addr INTEGER, retries INTEGER, vbat REAL, temperature_internal REAL, si7021_humidity REAL, si7021_temperature REAL, bmp180_temperature REAL, bmp180_pressure REAL, reed INTEGER"
    sql_table_fields = "time, addr, retries, vbat, temperature_internal, si7021_humidity, si7021_temperature, bmp180_temperature, bmp180_pressure, reed"
    barometric_height = 118


@dataclasses.dataclass
class Register:
    index: int
    size: int
    dtype: str

    def from_struct(self, struct):
        segment = struct[self.index:self.index+self.size]
        if self.dtype == "c":
            return segment
        return int.from_bytes(segment, "little", signed=self.dtype == "s")

    def to_struct(self, struct, value: int):
        if self.dtype != "c":
            value = value.to_bytes(self.size, "little", signed=self.dtype == "s")
        for i, val in enumerate(value):
            struct[self.index+i] = val


class OutRegisters:
    addr = Register(0, 1, "u")
    retries = Register(1, 1, "u")
    vbat = Register(2, 2, "u")
    temperature_internal = Register(4, 2, "u")
    si7021_humidity = Register(6, 2, "u")
    si7021_temperature = Register(8, 2, "u")
    bmp180_temperature = Register(10, 2, "s")
    bmp180_pressure = Register(12, 4, "s")
    reed = Register(16, 1, "u")

    @classmethod
    def convert_vbat(cls, struct):
        adc_val = cls.vbat.from_struct(struct)
        if adc_val == 0xffff:
            return None
        if adc_val & 0x8000:
            # 2.5 V reference
            return (adc_val & 0x7FFF) * 2 * 2.5 / 1023
        # 1.5 V reference
        return adc_val * 2 * 1.5 / 1023

    @classmethod
    def convert_temperature_internal(cls, struct):
        adc_val = cls.temperature_internal.from_struct(struct)
        if adc_val == 0xffff:
            return None
        return adc_val * 1.5 / 1023 / 0.00355 - 0.986 / 0.00355

    @classmethod
    def convert_si7021_humidity(cls, struct):
        adc_val = cls.si7021_humidity.from_struct(struct)
        if adc_val == 0xffff:
            return None
        return 125 * adc_val / 65536 - 6

    @classmethod
    def convert_si7021_temperature(cls, struct):
        adc_val = cls.si7021_temperature.from_struct(struct)
        if adc_val == 0xffff:
            return None
        return 175.72 * adc_val / 65536 - 46.85

    @classmethod
    def convert_bmp180_temperature(cls, struct):
        adc_val = cls.bmp180_temperature.from_struct(struct)
        if adc_val == 0xffff:
            return None
        return adc_val * 0.1

    @classmethod
    def convert_bmp180_pressure(cls, struct):
        adc_val = cls.bmp180_pressure.from_struct(struct)
        if adc_val == 0xffff:
            return None
        return adc_val * 0.01

    @classmethod
    def convert_reed(cls, struct):
        val = cls.reed.from_struct(struct)
        if val == 0xff:
            return None
        return val == 0xaa

    @classmethod
    def convert_to_sql_list(cls, struct) -> typing.List[str]:
        logging.debug(list(struct))
        addr = OutRegisters.addr.from_struct(struct)
        retries = OutRegisters.retries.from_struct(struct)
        vbat = OutRegisters.convert_vbat(struct)
        temperature_internal = OutRegisters.convert_temperature_internal(struct)
        si7021_humidity = OutRegisters.convert_si7021_humidity(struct)
        si7021_temperature = OutRegisters.convert_si7021_temperature(struct)
        bmp180_temperature = OutRegisters.convert_bmp180_temperature(struct)
        bmp180_pressure = OutRegisters.convert_bmp180_pressure(struct)
        reed = OutRegisters.convert_reed(struct)
        logging.debug({k: v for k, v in locals().items() if k not in ["self", "struct"]})

        sql_list = []
        sql_list.append(str(addr))
        sql_list.append(str(retries))
        sql_list.append("NULL" if vbat is None else str(vbat))
        sql_list.append("NULL" if temperature_internal is None else str(temperature_internal))
        sql_list.append("NULL" if si7021_humidity is None else str(si7021_humidity))
        sql_list.append("NULL" if si7021_temperature is None else str(si7021_temperature))
        sql_list.append("NULL" if bmp180_temperature is None else str(bmp180_temperature))
        sql_list.append("NULL" if bmp180_pressure is None else str(bmp180_pressure))
        sql_list.append("NULL" if reed is None else str(reed))
        return sql_list
