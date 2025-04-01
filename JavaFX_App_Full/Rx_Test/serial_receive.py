import serial
import sys
import time

def main():
    try:
        ser = serial.Serial(
            port='/dev/ttyUSB1',
            baudrate=9600,
            bytesize=serial.EIGHTBITS,
            parity=serial.PARITY_NONE,
            stopbits=serial.STOPBITS_ONE,
            timeout=None  # Blocking read
        )
    except serial.SerialException as e:
        print(f"Error opening serial port: {e}")
        sys.exit(1)

    print("Terminal app started. Waiting for data...")

    try:
        while True:
            # Read one byte
            data = ser.read(1)
            if data:
                # Get current timestamp
                timestamp = time.strftime('%Y-%m-%d %H:%M:%S', time.localtime())
                
                # Print received byte with timestamp
                print(f"[{timestamp}] Received: 0x{data.hex().upper()}")
                
                # Send ACK (0xFF)
                ser.write(b'\xFF')
                # Ensure immediate send
                ser.flush()
    except KeyboardInterrupt:
        print("\nExiting...")
    except Exception as e:
        print(f"Error: {e}")
    finally:
        ser.close()
        print("Serial port closed.")

if __name__ == "__main__":
    main()

