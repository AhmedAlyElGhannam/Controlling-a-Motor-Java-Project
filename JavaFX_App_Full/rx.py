import serial
import datetime
import time

def main():
    port = '/dev/ttyUSB1'
    baudrate = 9600
    ack_byte = b'\xFF'

    try:
        ser = serial.Serial(
            port=port,
            baudrate=baudrate,
            bytesize=serial.EIGHTBITS,
            stopbits=serial.STOPBITS_ONE,
            parity=serial.PARITY_NONE,
            timeout=None  # Block until data is read
        )

        print(f"Listening on {port}... Press Ctrl+C to exit.")

        while True:
            # Read one byte from the serial port
            received_data = ser.read(1)
            if received_data:
                # Get current timestamp for reception
                recv_time = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S.%f")
                print(f"[{recv_time}] Received data: {received_data.hex().upper()}")

                # Send ACK byte
                ser.write(ack_byte)
                ack_time = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S.%f")
                print(f"[{ack_time}] Sent ACK: {ack_byte.hex().upper()}")

    except KeyboardInterrupt:
        print("\nExiting...")
        if 'ser' in locals() and ser.is_open:
            ser.close()
    except Exception as e:
        print(f"Error: {e}")
        if 'ser' in locals() and ser.is_open:
            ser.close()

if __name__ == "__main__":
    main()
