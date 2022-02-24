import logging
import sqlite3
import time

import coloredlogs
from apscheduler.schedulers.background import BackgroundScheduler

from config import InRegisters, OutRegisters, config
from uart import Uart


class Main:
    def __init__(self):
        self.database = sqlite3.connect("database.db", check_same_thread=False)
        self.database.execute("create table if not exists Data (%s)" % config.sql_table_types)
        self.database.execute(
            "create table if not exists Node (id INTEGER, name STRING, led_dis INTEGER)")
        self.database.commit()

        self.uart = Uart(config.uart_port, config.uart_baud)
        self.uart.set_read_event(self.on_data_received)

        self.scheduler = BackgroundScheduler()
        self.testing_sensor = 0

    def run(self):
        self.on_in_regs_update()
        self.scheduler.add_job(self.on_in_regs_update, trigger="cron", **config.in_regs_update)
        self.scheduler.start()
        self.uart.run()

    def on_data_received(self, message: bytearray):
        sql_list = OutRegisters.convert_to_sql_list(message)
        sql_list = [str(int(time.time()))] + sql_list

        self.database.execute("""insert into Data(%s) values(%s)""" %
                              (config.sql_table_fields, ", ".join(sql_list)))
        self.database.commit()

    def on_in_regs_update(self):
        logging.debug("on_in_regs_update()")
        node_table = self.database.execute("select * from node").fetchall()
        struct = bytearray([0]*InRegisters.SIZE)

        InRegisters.set_led_dis(struct, node_table)
        InRegisters.testing.to_struct(struct, self.testing_sensor)
        InRegisters.status_led.to_struct(struct, 0)

        self.uart.write(struct)


if __name__ == "__main__":
    coloredlogs.install(fmt='%(asctime)s,%(msecs)03d %(levelname)-5s '
                        '[%(filename)s:%(lineno)d] %(message)s',
                        datefmt='%Y-%m-%d:%H:%M:%S',
                        level=logging.DEBUG)
    logging.getLogger('apscheduler').setLevel(logging.WARNING)
    Main().run()
