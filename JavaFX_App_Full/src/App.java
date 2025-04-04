import com.fazecast.jSerialComm.SerialPort;
import javafx.application.Application;
import javafx.application.Platform;
import javafx.geometry.Pos;
import javafx.scene.Scene;
import javafx.scene.control.Alert;
import javafx.scene.control.ComboBox;
import javafx.scene.control.Label;
import javafx.scene.control.Slider;
import javafx.scene.layout.VBox;
import javafx.stage.Modality;
import javafx.stage.Stage;
import javafx.scene.control.Button;


import static java.lang.Math.*;

public class App extends Application {
    private int motorSpeed = 0;
    private Label motorSpeedLabel;
    private Label motorDirectionLabel;
    private String motorDirection = "Motor stopped";
    private SerialCommManager serialCommManager;

    // defining a stage for error dialog
    private Stage currentErrorDialog;
    
    @Override
    public void start(Stage primaryStage) {
    
    	// Close existing serial connection if any
    if (serialCommManager != null) {
        serialCommManager.close();
        serialCommManager = null;
    }

        // Show a dialog for selecting the COM port as soon as the app opens
        Stage portSelectionDialog = createPortSelectionDialog(primaryStage);
        portSelectionDialog.showAndWait(); // Wait until the user selects a port

        // If no port is selected, exit the application
        if (serialCommManager == null) {
            Platform.exit();
            return;
        }

        // Setup motor speed control UI
        Slider slider = new Slider(-50, 50, 0);
        slider.setShowTickLabels(true);
        slider.setMajorTickUnit(10);

        motorSpeedLabel = new Label("Motor Speed: " + motorSpeed);
        motorDirectionLabel = new Label("Motor Direction: " + motorDirection);

        // Layout for main UI
        VBox root = new VBox(20, slider, motorSpeedLabel, motorDirectionLabel);
        root.setAlignment(Pos.CENTER);
        primaryStage.setMinWidth(600);
        primaryStage.setMinHeight(600);
        root.setStyle("-fx-padding: 20px;");

        Scene scene = new Scene(root, 600, 600);
        primaryStage.setTitle("Final Project GUI");
        primaryStage.setScene(scene);
        
        // Show the main stage after port selection
        primaryStage.show();

        // Slider listener for updating motor speed
        slider.valueProperty().addListener((obs, oldVal, newVal) -> {
            int snappedValue = (int) round(newVal.doubleValue() / 10) * 10;
            if (newVal.intValue() != snappedValue) {
                slider.setValue(snappedValue);
            } else if (!slider.isValueChanging()) {
                updateMotorSpeed(snappedValue);
                byte dataByte = computeDataByte();
                serialCommManager.setLastSentByte(dataByte);
                int directionBit = (snappedValue < 0) ? 1 : 0;
                System.out.printf("Speed: %d, Dir: %s -> Data Byte: 0x%02X%n",
                        Math.abs(snappedValue),
                        directionBit == 0 ? "CW" : "CCW",
                        dataByte & 0xFF);
            }
        });

        // Close the connection when the window is closed
        primaryStage.setOnCloseRequest(event -> {
            if (serialCommManager != null) {
                serialCommManager.close();
            }
        });
    }

    private Stage createPortSelectionDialog(Stage primaryStage) {
    Stage dialog = new Stage();
    dialog.initModality(Modality.APPLICATION_MODAL);
    dialog.initOwner(primaryStage);
    
    // ComboBox for selecting COM port
    ComboBox<String> portSelector = new ComboBox<>();
    portSelector.setPromptText("Select COM port");
    Button proceedButton = new Button("Proceed");
    proceedButton.setDisable(true);

    // Label with the instruction text
    Label instructionLabel = new Label("Select a Serial Port and Click Proceed");

    // Get all available serial ports using jSerialComm
    SerialPort[] availablePorts = SerialPort.getCommPorts();
    for (SerialPort port : availablePorts) {
        portSelector.getItems().add(port.getSystemPortName());
    }

    // If no ports are available, show an error and exit
    if (availablePorts.length == 0) {
        showError("No serial ports found!");
        Platform.exit();
        return dialog;
    }

    // Enable proceed button only when a port is selected
    portSelector.getSelectionModel().selectedItemProperty().addListener((obs, oldVal, newVal) -> {
        proceedButton.setDisable(newVal == null);
    });

    // Default to the first available port
    portSelector.getSelectionModel().select(0);

    // Proceed button action handler
    proceedButton.setOnAction(e -> {
        String portName = portSelector.getSelectionModel().getSelectedItem();
        if (portName == null) return;

        serialCommManager = new SerialCommManager(portName, (byte) 0x00);
        if (!serialCommManager.openPort()) {
            showError("Failed to open port!");
        } else {
            serialCommManager.startPeriodicTransmission(() -> {
                Platform.runLater(() -> showCommunicationErrorDialog(serialCommManager));
            });
            dialog.close();
        }
    });

    // Initialize dialog UI
    VBox dialogVBox = new VBox(10, instructionLabel, portSelector, proceedButton);
    dialogVBox.setAlignment(Pos.CENTER);
    dialogVBox.setStyle("-fx-padding: 20px;");
    Scene dialogScene = new Scene(dialogVBox, 300, 150);
    dialog.setScene(dialogScene);
    dialog.setTitle("Select COM Port");

    return dialog;
}

    private void updateMotorSpeed(int motorSpeed) {
        int snappedSpeed = (int) round(motorSpeed / 10.0) * 10;
        this.motorSpeed = snappedSpeed;
        motorSpeedLabel.setText("Motor Speed: " + Math.abs(snappedSpeed));

        if (snappedSpeed > 0) {
            motorDirection = "Clockwise";
        } else if (snappedSpeed < 0) {
            motorDirection = "Counter clockwise";
        } else {
            motorDirection = "Motor stopped";
        }
        motorDirectionLabel.setText("Motor Direction: " + motorDirection);
    }

    private byte computeDataByte() {
        int mappedSpeed;
        switch (Math.abs(motorSpeed)) {
            case 0:
                mappedSpeed = 0;
                break;
            case 10:
                mappedSpeed = 3;
                break;
            case 20:
                mappedSpeed = 6;
                break;
            case 30:
                mappedSpeed = 9;
                break;
            case 40:
                mappedSpeed = 12;
                break;
            case 50:
                mappedSpeed = 15;
                break;
            default:
                mappedSpeed = 0;
        }
        int directionBit = (motorSpeed < 0) ? 1 : 0;
        return (byte) ((directionBit << 4) | mappedSpeed);
    }

    private void showError(String message) {
        Alert alert = new Alert(Alert.AlertType.ERROR);
        alert.setTitle("Error");
        alert.setHeaderText(null);
        alert.setContentText(message);
        alert.showAndWait();
    }
    
    private void showCommunicationErrorDialog(SerialCommManager serialCommManager) {
    if (currentErrorDialog != null && currentErrorDialog.isShowing()) {
        return;
    }

    currentErrorDialog = new Stage();
    currentErrorDialog.initModality(Modality.APPLICATION_MODAL);
    currentErrorDialog.setTitle("Communication Error");

    Label messageLabel = new Label("Communication failed or timed out. Please retry or close the application.");
    Button retryButton = new Button("Retry");
    Button reconnectButton = new Button("Reconnect"); // New button
    Button closeButton = new Button("Close App");

    // Reconnect Button Logic
    reconnectButton.setOnAction(e -> {
    // Close all resources
    resetAppState();
    
    Platform.runLater(() -> {
        // Reinitialize with existing primary stage
        primaryStage.close(); // Close old UI
        Stage newPrimaryStage = new Stage();
        start(newPrimaryStage); // Restart cleanly
    });
    
    currentErrorDialog.close();
});

    // Retry Button Logic (existing code)
    retryButton.setOnAction(e -> {
        retryButton.setDisable(true);
        messageLabel.setText("Retrying...");
        new Thread(() -> {
            boolean success = serialCommManager.manualRetry();
            Platform.runLater(() -> {
                if (success) {
                    currentErrorDialog.close();
                } else {
                    messageLabel.setText("Retry failed. Please try again.");
                    retryButton.setDisable(false);
                }
            });
        }).start();
    });

    // Close Button Logic (existing code)
    closeButton.setOnAction(e -> {
        serialCommManager.close();
        Platform.exit();
    });

    // Add the new button to the layout
    VBox layout = new VBox(10, messageLabel, retryButton, reconnectButton, closeButton);
    layout.setAlignment(Pos.CENTER);
    layout.setStyle("-fx-padding: 20px;");

    currentErrorDialog.setScene(new Scene(layout, 400, 200));
    currentErrorDialog.setOnHidden(event -> currentErrorDialog = null);
    currentErrorDialog.show();
}

    public static void main(String[] args) {
        launch(args);
    }
}

