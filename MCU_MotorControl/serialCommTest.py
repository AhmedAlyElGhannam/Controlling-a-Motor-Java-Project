import serial
import time

def generate_byte(counter, value):
    """
    Generates a byte with the following format:
    - Bits 7-5: Incrementing counter (0-7)
    - Bit 4: Always 1
    - Bits 3-0: One of the allowed values (0, 3, 6, 9, 12, 15)
    """
    return ((counter & 0x07) << 5) | (1 << 4) | (value & 0x0F)

def main():
    port = '/dev/ttyUSB0'
    baudrate = 9600
    allowed_values = [0, 3, 6, 9, 12, 15]
    counter = 0
    ack_byte = 0xFF
    
    try:
        with serial.Serial(port, baudrate, timeout=1) as ser:
            print(f"Connected to {port} at {baudrate} baud")
            
            first_frame = True
            
            while True:
                for value in allowed_values:
                    byte_to_send = generate_byte(counter, value)
                    timestamp = time.strftime("%Y-%m-%d %H:%M:%S")
                    
                    # Send first frame unconditionally
                    if not first_frame:
                        # Wait for ACK before sending new byte
                        while True:
                            if ser.in_waiting:
                                received_data = ser.read(ser.in_waiting)
                                if ack_byte in received_data:
                                    break
                            time.sleep(0.1)  # Avoid busy-waiting
                    
                    print(f"{timestamp} | Sent: {byte_to_send:08b} (Hex: {byte_to_send:02X}, Dec: {byte_to_send})")
                    ser.write(bytes([byte_to_send]))
                    
                    first_frame = False  # Disable unconditional sending after first frame
                    counter = (counter + 1) % 8  # Loop counter 0-7
                    time.sleep(0.5)
    except serial.SerialException as e:
        print(f"Serial error: {e}")
    except KeyboardInterrupt:
        print("Program terminated.")

if __name__ == "__main__":
    main()

