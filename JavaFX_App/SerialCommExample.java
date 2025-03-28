import com.fazecast.jSerialComm.*;
import java.io.IOException;
import java.util.Scanner;

public class SerialCommExample {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);

        // Get the specific port ttyUSB0
        SerialPort serialPort = SerialPort.getCommPort("/dev/ttyUSB0");
        
        if (serialPort == null) {
            System.out.println("Port /dev/ttyUSB0 not found.");
            return;
        }

        // Configure port settings
        serialPort.setBaudRate(9600);
        serialPort.setNumDataBits(8);
        serialPort.setNumStopBits(SerialPort.ONE_STOP_BIT);
        serialPort.setParity(SerialPort.NO_PARITY);

        if (!serialPort.openPort()) {
            System.out.println("Failed to open /dev/ttyUSB0.");
            return;
        }

        System.out.println("Port /dev/ttyUSB0 opened successfully.");

        // Start reading in a separate thread
        new Thread(() -> {
            Scanner serialInput = new Scanner(serialPort.getInputStream());
            while (serialInput.hasNextLine()) {
                System.out.println("Received: " + serialInput.nextLine());
            }
            serialInput.close();
        }).start();

        // Sending data to serial port
        System.out.println("Type messages to send (type 'exit' to quit):");
        while (true) {
            String message = scanner.nextLine();
            if (message.equalsIgnoreCase("exit")) {
                break;
            }
            try {
                serialPort.getOutputStream().write((message + "\n").getBytes());
                serialPort.getOutputStream().flush();
            } catch (IOException e) {
                System.out.println("Error writing to serial port: " + e.getMessage());
                e.printStackTrace();
            }
        }

        // Close port
        serialPort.closePort();
        System.out.println("Port closed.");
    }
}

