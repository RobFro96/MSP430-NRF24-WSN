import logging
import sqlite3
import time

import coloredlogs

from config import OutRegisters, config
from uart import Uart


class Main:
    def __init__(self):
        self.database = sqlite3.connect("database.db")
        self.database.execute("""create table if not exists Data (%s)""" % config.sql_table_types)
        self.database.commit()

        self.uart = Uart(config.uart_port, config.uart_baud)
        self.uart.set_read_event(self.on_data_received)

    def run(self):
        self.uart.run()

    def on_data_received(self, message: bytearray):
        sql_list = OutRegisters.convert_to_sql_list(message)
        sql_list = [str(int(time.time()))] + sql_list

        self.database.execute("""insert into Data(%s) values(%s)""" %
                              (config.sql_table_fields, ", ".join(sql_list)))
        self.database.commit()


if __name__ == "__main__":
    coloredlogs.install(fmt='%(asctime)s,%(msecs)03d %(levelname)-5s '
                        '[%(filename)s:%(lineno)d] %(message)s',
                        datefmt='%Y-%m-%d:%H:%M:%S',
                        level=logging.DEBUG)
    Main().run()
