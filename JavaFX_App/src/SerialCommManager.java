// serial comm library imported from .jar
import com.fazecast.jSerialComm.*; 
// platform class allows interactions with JavaFx GUI thread
import javafx.application.Platform;
// data input stream class
import java.io.DataInputStream;
// data output stream class
import java.io.DataOutputStream;
// for catching IO exceptions
import java.io.IOException;
// for ScheduledExecutorService class to manage transmission thread SAFELY
import java.util.concurrent.*;
// for critical section Lock && ackReceived Condition for safe synchronization
import java.util.concurrent.locks.*;

public class SerialCommManager {
    
    // defining a reference to a SerialPort object (from jSerialComm lib)
    private SerialPort serialPort;

    // defining a reference to a DataOutputStream object (for data transmission)
    private DataOutputStream outputStream;

    // defining a flag for indicating a wait for ack signal [MAYBE CHANGE IT TO AtomicBoolean?!!]
    private boolean waitingForAck = false;

    // defining a flag to indicate failed communication with MCU
    private volatile boolean communicationFailed = false;

    // defining a reference to ScheduledExecutorService for handling transmission thread + make it run with specific periodicity 
    private ScheduledExecutorService executor;

    // defining a reference to a runnable/thread that will be called upon failure
    private Runnable onFailureCallback;

    // defining a variable to hold the last sent byte
    private volatile byte lastSentByte;
    
    // defining a reference to a Lock to make critical sections more efficient/readable
    private final Lock lock = new ReentrantLock();

    // defining a reference to a Condition to handle ack event with more precision and notify transmission thread on time
    private final Condition ackReceivedCondition = lock.newCondition();

    // defining a byte constant for ack byte
    private final byte ACK_BYTE = (byte)0xFF;

    // defining a Tx scheduling periodicity constant (in ms)
    private final int TX_PERIODICITY = 500;
    
    // defining a variable to keep track of sent byte ID
    private volatile int sentByteID;

    // SerialCommManager constructor takes a string signifying the name of the port (ex. /dev/ttyUSB0)
    public SerialCommManager(String portName, byte dataByte) {
	    // reserving port with passed name for use in app
        serialPort = SerialPort.getCommPort(portName);

	    // setting baud rate to 9600 bps
        serialPort.setBaudRate(9600);

	    // setting data length to 8 bits
        serialPort.setNumDataBits(8);

	    // setting number of stop bits to 1
        serialPort.setNumStopBits(SerialPort.ONE_STOP_BIT);

	    // setting parity mode to no parity (no need to use it)
        serialPort.setParity(SerialPort.NO_PARITY);

	    // setting read/write timeout parameters
	    // SerialPort.TIMEOUT_WRITE_BLOCKING --> a write/transmission will block until data is written on output stream
	    // SerialPort.TIMEOUT_READ_SEMI_BLOCKING --> blocks until at least one byte is read from input steam (which is more than enough cuz ack is a single byte)
	    // set timeout of read && write to 0s
        serialPort.setComPortTimeouts(
            SerialPort.TIMEOUT_READ_SEMI_BLOCKING | SerialPort.TIMEOUT_WRITE_BLOCKING, 
            0, 0
        );
        
        // setting initial ID to 0
        sentByteID = 0;
        
        // setting data byte to 0 initially
        setLastSentByte(dataByte);
    }

    // instance method to prepare Tx (outputStream) + its thread && Rx with its listener 
    public boolean openPort() {
	    // flag acts as return status for method
	    boolean isPortOpenedSuccessfully = true;

	    // check if serial port has been opened successfully after reserving it in constructor
        if (!serialPort.openPort()) {
		    // wrong port name or sth
		    isPortOpenedSuccessfully = false;
	    } else { // port was opened normally
	    
	        // surround the following statements with try...catch to handle possible IO exceptions
	        try {
		
		        // open a new data output stream and pass the serialPort's output stream to it
                outputStream = new DataOutputStream(serialPort.getOutputStream());
    
		        // call Rx listener init
                setupListener();

		        // run Tx thread periodically with a constant delay SAFELY
                executor = Executors.newSingleThreadScheduledExecutor();
            } catch (Exception e) { // in case an Exception was caught
                
		        // close serial port connection
		        close();

		        // lower flag
                isPortOpenedSuccessfully = false;
            }
	    }

        return isPortOpenedSuccessfully;
    }

    public void setLastSentByte(byte data) {
    	lastSentByte = data;
    }

    // instance method to initialize Rx listener
    private void setupListener() {

	    // creating an anonymous inner class that IMPLEMENTS SerialPortDataListener (interface found in jSerialComm)
        serialPort.addDataListener(new SerialPortDataListener() {
	        // method to tell listener when data is available on serial port
	        @Override
            public int getListeningEvents() { 
                return SerialPort.LISTENING_EVENT_DATA_AVAILABLE; 
            }
	        // method that is called whenever a serial event is called
            @Override
            public void serialEvent(SerialPortEvent event) {
		        // if the serial event that triggered this method is data available, do the following
                if (event.getEventType() == SerialPort.LISTENING_EVENT_DATA_AVAILABLE) {
		            // surround the following statements with try...catch to handle exceptions
		            try {
		    	        // creating a data input stream for reception and passing serialPort's input stream to it
                        DataInputStream input = new DataInputStream(serialPort.getInputStream());
		    	        // as long as there is data on the input stream
                        while (input.available() > 0) {
		    	            // read byte from input stream
                            byte data = input.readByte();
		    	            // if received data is an ACK byte --> handle it 
                            if (data == ACK_BYTE) {
                                handleAckReceived();
                            } else {} // keep reading 
                        }
                    } catch (IOException ex) {
                        // handle exception
                    }	
		        } else {} // do nothing
            }
        });
    }

    // method to handle a received ack byte
    private void handleAckReceived() {
	    // critical section entered (lock acquired)
        lock.lock();
	    // surround statements with a try to make sure lock is released even if an exception happens
	    try {
	        // transmitter thread can transmit normally when its time comes
            waitingForAck = false;
	        // signal to all threads (aka only transmitter thread) that ack has been received
            ackReceivedCondition.signalAll();
        } finally {
	        // critical section exited (lock will always be released regardless of above statements exit state)
            lock.unlock();
        }
    }
    
    // method called from app to start transmission
    public void startPeriodicTransmission(Runnable failureCallback) {
	    // set failure callback function to the passed runnable
        this.onFailureCallback = failureCallback;
        
        // creating a new anonymous inner runnable class (logic of the task that will be executed by the created thread) and passing it to executor (scheduler)
        executor.scheduleAtFixedRate(new Runnable() {
            @Override
            public void run() {
	        	// as long as communication is alive && thread is not interrupted
                while (!Thread.currentThread().isInterrupted() && !communicationFailed) {
                    try {
                    	// beginning of critical section (lock is acquired)
                        lock.lock();
                        // critical section must be surrounded by a try...finally to make sure lock is released 
                        try {
                            // set data byte ID to prepare for data transmission
                            byte dataByteWithID = (byte)(lastSentByte | ((sentByteID & 0x07) << 5));
                            // increment byte ID for next transmittable byte
                            sentByteID++;
                            // send the last registered value of data byte to MCU
                            sendByteInternal(dataByteWithID);
                            // raise waitingForAck flag to indicate waiting for ack after sending data byte
                            waitingForAck = true;
                            // communication deadline is set to TX_PERIODICITY ms
                            long deadline = System.currentTimeMillis() + TX_PERIODICITY;
                            // as long as waitingForAck flag is raised,
                            while (waitingForAck) {
                            	// keep track of periodicity deadline
                                long remaining = deadline - System.currentTimeMillis();
                                // if TX_PERIODICITY ms have passed --> handle communication timeout
                                if (remaining <= 0) {
                                    handleTimeout();
                                    break;
                                }
                                // thread must wait for ackReceivedCondition condition to happen for remaining ms time
                                ackReceivedCondition.await(remaining, TimeUnit.MILLISECONDS);
                            }
                        } finally {
                            // end of critical section (lock is released)
                            lock.unlock();
                        }
                    } catch (InterruptedException e) {
                    	// interrupt thread to handle graceful thread termination
                        Thread.currentThread().interrupt();
                    } catch (Exception e) {
                    	// in case an exception has happened, call this method
                        handleFailure();
                    }
                }
            }
        }, 0, TX_PERIODICITY, TimeUnit.MILLISECONDS);
    }

    // method to wrap send data byte functionality of jSerialComm class
    private void sendByteInternal(byte data) throws IOException {
        if (outputStream != null) {
            // write data on output stream (send it)
	        outputStream.writeByte(data);
	        // clear stream buffer
            outputStream.flush();
        } else {}
    }

    // method to handle timeout events during send/receive cycles [may fuse it with handleFailure]
    private void handleTimeout() {
	    // beginning of critical section (acquire lock)
        lock.lock();
	    // critical sections must be surrounded by a try...finally to make sure lock is released
        try {
	        // raise failed communication flag
            communicationFailed = true;
	        // thread is no longer waiting for an ack
            waitingForAck = false;
	        // execute passed callback function in GUI thread
            Platform.runLater(() -> onFailureCallback.run());
        } finally {
	        // end of critical section (release lock)
            lock.unlock();
        }
    }

    // method to handle exceptions raised during transmission [may fuse it with handleTimeout]
    private void handleFailure() {
	    // beginning of critical section (acquire lock)
        lock.lock();
	    // critical sections must be surrounded by a try...finally to make sure lock is released
        try {
	        // raise failed communication flag
            communicationFailed = true;
	        // execute passed callback function in GUI thread
            Platform.runLater(() -> onFailureCallback.run());
        } finally {
	        // end of critical section (release lock)
            lock.unlock();
        }
    }

    // method to stop data transmission
    public void stopTransmission() {
        if (executor != null) {
	        // shutdown thread managed by scheduled executor
            executor.shutdownNow();
            try {
		        // wait for it to terminate gracefully
                executor.awaitTermination(TX_PERIODICITY, TimeUnit.MILLISECONDS);
            } catch (InterruptedException e) {
		        // in case an exception was raised, interrupt the thread to handle termination in a graceful way
                Thread.currentThread().interrupt();
            }
        } else {}
	    // I think this statement is kinda useless but I will leave it here for now
        communicationFailed = false;
    }

    // method to stop transmission, remove Rx data listener, and close the serial port
    public void close() {
    	// stop transmission
        stopTransmission();
        // if port is still open,
        if (serialPort != null && serialPort.isOpen()) {
            // close Rx data listener
            serialPort.removeDataListener();
            // close serial port
            serialPort.closePort();
        } else {}
    }
}
