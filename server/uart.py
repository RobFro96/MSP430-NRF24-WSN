import logging
import threading

import coloredlogs
import serial


class Uart(threading.Thread):
    """Opens a thread for serial communication with the MSP430, using the specified protocol.
    """
    START_BYTE = 0x55
    MESSAGE_EXTRA_LEN = 2
    LEN_INDEX = 1
    DATA_INDEX = 2

    def __init__(self, port: str, baud: int = 115200):
        """Constructor.
        Initializing the thread.

        Args:
            port (str): portname
            args: CLI arguments (baud rate)
        """
        threading.Thread.__init__(self)
        self.port = port
        self.baud = baud
        self.connection: serial.Serial = None

        self.stop_event = threading.Event()
        self.read_event = lambda msg: logging.debug(list(msg))

    def set_read_event(self, event):
        self.read_event = event

    def close(self):
        """Closes the thread by setting the stop event.
        """
        self.stop_event.set()

    def is_connected(self):
        """Returns true if serial thread is running.
        Serial threads stopps if Arduino is disconnected.

        Returns:
            bool: True if running
        """
        return not self.stop_event.is_set() and self.connection is not None

    def run(self):
        """Thread run method.
        """

        msg = bytearray()
        payload_len = 0

        while not self.stop_event.wait(1):
            try:
                self.connection = serial.Serial(self.port, self.baud, timeout=1)
                while not self.stop_event.wait(1e-3):
                    n = max(0, min(self.connection.inWaiting(), 1024))  # Number of bytes to read
                    buffer = self.connection.read(n)  # Read bytes

                    for char in buffer:
                        # Analyze charwise
                        if (char == self.START_BYTE and len(msg) == 0) or len(msg) > 0:
                            msg.append(char)
                        if len(msg) == self.LEN_INDEX + 1:
                            payload_len = char
                        if len(msg) == payload_len + self.MESSAGE_EXTRA_LEN:
                            self.read_event(msg[self.DATA_INDEX:])
                            msg = bytearray()
                            payload_len = 0
            except serial.SerialException:
                logging.info("Serial port %s disconnected." % self.port)

        # Close
        if self.connection and self.connection.is_open:
            self.connection.close()


if __name__ == "__main__":
    # Test functions
    coloredlogs.install(fmt='%(asctime)s,%(msecs)03d %(levelname)-5s '
                        '[%(filename)s:%(lineno)d] %(message)s',
                        datefmt='%Y-%m-%d:%H:%M:%S',
                        level=logging.DEBUG)

    uart = Uart("COM5")
    uart.run()
