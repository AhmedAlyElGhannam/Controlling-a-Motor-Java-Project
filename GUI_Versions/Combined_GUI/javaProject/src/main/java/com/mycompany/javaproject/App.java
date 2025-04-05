package com.mycompany.javaproject;

import eu.hansolo.medusa.Gauge;
import eu.hansolo.medusa.GaugeBuilder;
import eu.hansolo.medusa.TickLabelOrientation;
import eu.hansolo.medusa.skins.ModernSkin;
import javafx.animation.TranslateTransition;
import javafx.geometry.Insets;
import javafx.scene.control.ButtonType;
import javafx.scene.control.RadioButton;
import javafx.scene.control.ToggleGroup;
import javafx.scene.layout.HBox;
import javafx.scene.paint.Color;
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
import javafx.scene.layout.StackPane;
import javafx.scene.shape.Circle;
import javafx.scene.shape.Rectangle;
import javafx.scene.text.Font;
import javafx.scene.text.FontWeight;
import javafx.scene.text.Text;
import javafx.util.Duration;

public class App extends Application {
    // Color theme
    private final String PRIMARY_COLOR = "#2b2b2b";
    private final String SECONDARY_COLOR = "#3a3a3a";
    private final String ACCENT_COLOR = "#00d6d7";
    private final String TEXT_COLOR = "#ffffff";
    private final String WARNING_COLOR = "#ff3232";

    // Common fields
    private Stage primaryStage;
    private Scene modeSelectionScene;
    private Scene normalMotorControlScene;
    private Scene airConditionerScene;
    
    private SerialCommManager serialCommManager;
    
    // Normal mode fields
    private int motorSpeed = 0;
    private Label motorSpeedLabel;
    private Label motorDirectionLabel;
    private Label statusLabel;
    private String motorDirection = "MOTOR STOPPED";
    private boolean motorInitialized = false;
    private boolean clockwiseDirection = true;
    private Slider slider;
    private RadioButton clockwiseBtn;
    private RadioButton counterClockwiseBtn;
    private Button initButton;
    
    // AC mode fields 
    private Gauge gauge;
    private Slider acSlider;
    private Label acSpeedLabel;
    private Label acDirectionLabel;
    
    // Toggle switch components
    private Circle toggleKnob;
    private Rectangle toggleBackground;
    private Text statusText;
    private boolean isOn = false;

    // Speed mappings (0-5 maps to these values)
    private final int[] SPEED_MAPPING = {0, 3, 6, 9, 12, 15};

    @Override
    public void start(Stage primaryStage) {
        this.primaryStage = primaryStage;
        
        // Show a dialog for selecting the COM port as soon as the app opens
        Stage portSelectionDialog = createPortSelectionDialog(primaryStage);
        portSelectionDialog.showAndWait(); // Wait until the user selects a port

        // If no port is selected, exit the application
        if (serialCommManager == null) {
            Platform.exit();
            return;
        }
        // Create all scenes
        createModeSelectionScene();
        createNormalMotorControlScene();
        createAirConditionerScene();
        
        // Set the initial scene
        primaryStage.setScene(modeSelectionScene);
        primaryStage.setTitle("Advanced Motor Control System");
        primaryStage.setMinWidth(700);
        primaryStage.setMinHeight(700);
        
        primaryStage.setOnCloseRequest(event -> {
            if (serialCommManager != null) {
                serialCommManager.close();
            }
            Platform.exit();
        });
        primaryStage.show();
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
    
    private void showError(String message) {
        Alert alert = new Alert(Alert.AlertType.ERROR);
        alert.setTitle("Error");
        alert.setHeaderText(null);
        alert.setContentText(message);
        alert.showAndWait();
    }
    
    private void handleConnectionError(SerialCommManager serialCommManager) {
        initButton.setDisable(false);
        motorInitialized = false;
        Alert alert = new Alert(Alert.AlertType.ERROR);
        alert.setTitle("Connection Lost");
        alert.setHeaderText("Failed to connect to the motor controller!");
        alert.setContentText("Check if the device is properly connected.");

        ButtonType retryButtonType = new ButtonType("Retry");
        ButtonType exitButtonType = new ButtonType("Exit");
        alert.getButtonTypes().setAll(retryButtonType, exitButtonType);

        // Get button references
        Button retryButton = (Button) alert.getDialogPane().lookupButton(retryButtonType);
        Button exitButton = (Button) alert.getDialogPane().lookupButton(exitButtonType);

        // Get the Stage (window) of the Alert
        Stage stage = (Stage) alert.getDialogPane().getScene().getWindow();

        // Handle X button press - treat same as Retry
        stage.setOnCloseRequest(e -> {
            e.consume(); // Prevent default close behavior
            retryButton.fire(); // Trigger retry action
        });

        retryButton.setOnAction(e -> {
            retryButton.setDisable(true);
            alert.setContentText("Retrying...");
            new Thread(() -> {
                boolean success = serialCommManager.manualRetry();
                Platform.runLater(() -> {
                    if (success) {
                        alert.close();
                        // Reinitialize motor and restart transmission
                        initializeMotor();
                        statusLabel.setText("Status: Reconnected and initialized");
                    } else {
                        alert.setContentText("Retry failed. Please try again.");
                        retryButton.setDisable(false);
                    }
                });
            }).start();
        });

        exitButton.setOnAction(e -> {
            serialCommManager.close();
            Platform.exit();
        });

        alert.showAndWait().ifPresent(response -> {
            if (response == exitButtonType) {
                serialCommManager.close();
                Platform.exit();
            }
        });
    }

    private void createModeSelectionScene() {
        // Main title with gradient effect
        Label titleLabel = new Label("MOTOR CONTROL SYSTEM");
        titleLabel.setStyle("-fx-font-size: 32px; -fx-font-weight: bold; -fx-text-fill: " + ACCENT_COLOR + ";");
        
        Label subtitleLabel = new Label("Select Operation Mode");
        subtitleLabel.setStyle("-fx-font-size: 18px; -fx-text-fill: " + TEXT_COLOR + ";");

        // Modern card-style buttons
        Button normalModeBtn = createModeButton("NORMAL MODE", "#4CAF50");
        normalModeBtn.setOnAction(e -> primaryStage.setScene(normalMotorControlScene));
        
        Button acModeBtn = createModeButton("AIR CONDITIONER MODE", "#2196F3");
        acModeBtn.setOnAction(e -> primaryStage.setScene(airConditionerScene));

        // Layout with improved spacing and background
        VBox modeSelectionLayout = new VBox(30, titleLabel, subtitleLabel, normalModeBtn, acModeBtn);
        modeSelectionLayout.setAlignment(Pos.CENTER);
        modeSelectionLayout.setPadding(new Insets(40));
        modeSelectionLayout.setStyle("-fx-background-color: " + PRIMARY_COLOR + ";");

        modeSelectionScene = new Scene(modeSelectionLayout, 800, 700);
    }

    private void createNormalMotorControlScene() {
        initButton = new Button("Initialize Motor");
        initButton.setStyle("-fx-font-size: 16px; -fx-padding: 10px 20px;");
        initButton.setOnAction(e -> initializeMotor());
        
        statusLabel = new Label("Status: Motor not initialized");
        statusLabel.setStyle("-fx-text-fill: " + TEXT_COLOR + ";");

        VBox initBox = new VBox(20, initButton, statusLabel);
        initBox.setAlignment(Pos.TOP_CENTER);

        // Direction controls
        ToggleGroup directionGroup = new ToggleGroup();
        clockwiseBtn = new RadioButton("Clockwise");
        clockwiseBtn.setToggleGroup(directionGroup);
        clockwiseBtn.setSelected(true);
        clockwiseBtn.setDisable(true);
        clockwiseBtn.setStyle("-fx-text-fill: " + TEXT_COLOR + ";");
        clockwiseBtn.setOnAction(e -> {
            clockwiseDirection = true;
            updateMotorDirection();
            serialCommManager.setLastSentByte(bridgeValue());
        });
        
        counterClockwiseBtn = new RadioButton("Counter-Clockwise");
        counterClockwiseBtn.setToggleGroup(directionGroup);
        counterClockwiseBtn.setDisable(true);
        counterClockwiseBtn.setStyle("-fx-text-fill: " + TEXT_COLOR + ";");
        counterClockwiseBtn.setOnAction(e -> {
            clockwiseDirection = false;
            updateMotorDirection();
            serialCommManager.setLastSentByte(bridgeValue());
        });
        
        HBox directionBox = new HBox(20, clockwiseBtn, counterClockwiseBtn);
        directionBox.setAlignment(Pos.CENTER);

        // Normal mode slider (0-5)
        slider = new Slider(0, 5, 0);
        slider.setPrefWidth(550);
        slider.setStyle(
            "-fx-control-inner-background: #444444; " +
            "-fx-background-color: transparent; " +
            "-fx-padding: 15px; " +
            "-fx-show-tick-labels: true; " +
            "-fx-show-tick-marks: true;"
        );
        slider.setMajorTickUnit(1);
        slider.setMinorTickCount(0);
        slider.setBlockIncrement(1);
        slider.setDisable(true);
        slider.setSnapToTicks(true);
        
        motorSpeedLabel = new Label("0 (0 RPM)");
        motorSpeedLabel.setStyle(
            "-fx-font-size: 36px; " +
            "-fx-font-weight: bold; " +
            "-fx-text-fill: " + TEXT_COLOR + ";"
        );

        motorDirectionLabel = new Label(motorDirection);
        motorDirectionLabel.setStyle(
            "-fx-font-size: 24px; " +
            "-fx-font-weight: bold; " +
            "-fx-text-fill: #aaaaaa;"
        );

        slider.valueProperty().addListener((obs, oldVal, newVal) -> {
            int discreteValue = newVal.intValue();
            if (!slider.isValueChanging()) {
                updateMotorSpeed(discreteValue);
                serialCommManager.setLastSentByte(bridgeValue());
            }
        });

        VBox motorControlBox = new VBox(20, directionBox, slider, motorSpeedLabel, motorDirectionLabel);
        motorControlBox.setAlignment(Pos.CENTER);
        
        
        motorControlBox.setStyle(
            "-fx-background-color: " + SECONDARY_COLOR + "; " +
            "-fx-background-radius: 20px; " +
            "-fx-border-radius: 20px; " +
            "-fx-effect: dropshadow(three-pass-box, rgba(0,0,0,0.4), 20, 0, 0, 0);" +
            "-fx-padding: 30px;"
        );

        Button backButton = createBackButton(modeSelectionScene);

        VBox root = new VBox(40, initBox, motorControlBox, backButton);
        root.setAlignment(Pos.CENTER);
        root.setStyle("-fx-background-color: " + PRIMARY_COLOR + ";");

        normalMotorControlScene = new Scene(root, 800, 800);
    }

private void createAirConditionerScene() {
    // Create the gauge with -5 to 5 range
    gauge = GaugeBuilder.create()
            .prefSize(550, 550)
            .title("AIR CONDITIONER MODE")
            .unit("Level")
            .minValue(-5)
            .maxValue(5)
            .decimals(0)
            .valueColor(Color.WHITE)
            .titleColor(Color.web(ACCENT_COLOR))
            .barColor(Color.web(ACCENT_COLOR))
            .needleColor(Color.WHITE)
            .thresholdColor(Color.web(WARNING_COLOR))
            .tickLabelColor(Color.web("#aaaaaa"))
            .tickMarkColor(Color.BLACK)
            .tickLabelOrientation(TickLabelOrientation.ORTHOGONAL)
            .build();
    gauge.setSkin(new ModernSkin(gauge));
    
    
    
    // AC mode slider (-5 to 5)
    acSlider = new Slider(-5, 5, 0);
    acSlider.setPrefWidth(550);
    acSlider.setStyle(
        "-fx-control-inner-background: #444444; " +
        "-fx-padding: 20px; " +
        "-fx-font-size: 16px;"
    );
    acSlider.setMajorTickUnit(1);
    acSlider.setMinorTickCount(0);
    acSlider.setBlockIncrement(1);
    acSlider.setSnapToTicks(true);
    acSlider.setDisable(true);
    acSpeedLabel = new Label("SPEED: 0 (0 RPM)");
    acSpeedLabel.setStyle("-fx-font-size: 28px; -fx-font-weight: bold; -fx-text-fill: " + TEXT_COLOR + ";");
    
    acDirectionLabel = new Label("DIRECTION: STOPPED");
    acDirectionLabel.setStyle("-fx-font-size: 24px; -fx-font-weight: bold; -fx-text-fill: #aaaaaa;");

    acSlider.valueProperty().addListener((obs, oldVal, newVal) -> {
        int discreteValue = newVal.intValue();
        if (!acSlider.isValueChanging()) {
            updateACStatus(discreteValue);
            serialCommManager.setLastSentByte(bridgeValue());
        }
    });

    gauge.valueProperty().bindBidirectional(acSlider.valueProperty());
// Toggle switch
    toggleBackground = new Rectangle(120, 50, Color.RED.darker());
    toggleBackground.setArcHeight(50);
    toggleBackground.setArcWidth(50);
    toggleBackground.setStroke(Color.BLACK);

    toggleKnob = new Circle(20, Color.WHITE);
    toggleKnob.setStroke(Color.DARKGRAY);
    toggleKnob.setTranslateX(-30); // Start at OFF position (left side)

    // status text for toggle switch
    statusText = new Text("OFF");
    statusText.setFont(Font.font("Arial", FontWeight.BOLD, 16));
    statusText.setFill(Color.WHITE);
    statusText.setTranslateX(15); // Position text on the right side

    StackPane toggleContainer = new StackPane(toggleBackground, toggleKnob, statusText);
    toggleContainer.setAlignment(Pos.CENTER);
    toggleContainer.setPrefSize(120, 50);
    toggleContainer.setOnMouseClicked(e -> toggleState());
    // Create a container for the toggle to position it above the gauge
    VBox gaugeToggleContainer = new VBox(10, toggleContainer, gauge);
    gaugeToggleContainer.setAlignment(Pos.CENTER);

    VBox controlPanel = new VBox(30, gaugeToggleContainer, acSlider, acSpeedLabel, acDirectionLabel);
    controlPanel.setAlignment(Pos.CENTER);
    controlPanel.setPadding(new Insets(30));
    controlPanel.setStyle(
        "-fx-background-color: " + SECONDARY_COLOR + "; " +
        "-fx-background-radius: 20px; " +
        "-fx-effect: dropshadow(three-pass-box, rgba(0,0,0,0.3), 15, 0, 0, 0);"
    );

    Button backButton = createBackButton(modeSelectionScene);

    VBox root = new VBox(40, controlPanel, backButton);
    root.setAlignment(Pos.CENTER);
    root.setPadding(new Insets(40));
    root.setStyle("-fx-background-color: " + PRIMARY_COLOR + ";");

    airConditionerScene = new Scene(root, 800, 900);
}

    private Button createModeButton(String text, String baseColor) {
        Button button = new Button(text);
        button.setStyle(
            "-fx-font-size: 20px; " +
            "-fx-font-weight: bold; " +
            "-fx-text-fill: white; " +
            "-fx-background-color: " + baseColor + "; " +
            "-fx-background-radius: 15px; " +
            "-fx-padding: 20px 40px; " +
            "-fx-effect: dropshadow(three-pass-box, rgba(0,0,0,0.3), 10, 0, 0, 5);"
        );
        
        button.setOnMouseEntered(e -> button.setStyle(
            button.getStyle() + 
            "-fx-background-color: linear-gradient(to bottom, derive(" + baseColor + ", 20%), " + baseColor + ");" +
            "-fx-cursor: hand;"
        ));
        button.setOnMouseExited(e -> button.setStyle(
            button.getStyle() + 
            "-fx-background-color: " + baseColor + ";"
        ));
        return button;
    }

    private Button createBackButton(Scene targetScene) {
        Button backButton = new Button("← BACK TO MODE SELECTION");
        backButton.setStyle(
            "-fx-font-size: 16px; " +
            "-fx-text-fill: " + TEXT_COLOR + "; " +
            "-fx-background-color: transparent; " +
            "-fx-border-color: " + ACCENT_COLOR + "; " +
            "-fx-border-radius: 10px; " +
            "-fx-border-width: 2px; " +
            "-fx-padding: 10px 25px;"
        );
        
        backButton.setOnMouseEntered(e -> backButton.setStyle(
            backButton.getStyle() + 
            "-fx-background-color: rgba(0,214,215,0.1); " +
            "-fx-cursor: hand;"
        ));
        backButton.setOnMouseExited(e -> backButton.setStyle(
            backButton.getStyle() + 
            "-fx-background-color: transparent;"
        ));
        backButton.setOnAction(e -> ((Stage) backButton.getScene().getWindow()).setScene(targetScene));
        
        return backButton;
    }

    private void initializeMotor() {
        if (motorInitialized) {
            initButton.setDisable(true);
            statusLabel.setText("Status: Motor already initialized");
        }else{
        
            statusLabel.setText("Status: Initializing motor...");
            initButton.setDisable(true);
            motorInitialized = true;

            slider.setDisable(false);
            clockwiseBtn.setDisable(false);
            counterClockwiseBtn.setDisable(false);

            slider.setValue(0); 
            clockwiseBtn.setSelected(true); 

            serialCommManager.startPeriodicTransmission(() -> {
                Platform.runLater(() -> handleConnectionError(serialCommManager));
            });
        }
    }
        
    private void toggleState() {
        isOn = !isOn;
        animateToggle();
        updateToggleUI();
        updateControlState();
    }
    
    private void animateToggle() {
        TranslateTransition transition = new TranslateTransition(Duration.millis(200), toggleKnob);
        transition.setToX(isOn ? 30 : -30); 
        transition.play();
    }

    private void updateToggleUI() {
        if (isOn) {
            toggleBackground.setFill(Color.GREEN.darker());
            
            
            statusText.setText("ON");
            statusText.setTranslateX(-15);
        } else {
            toggleBackground.setFill(Color.RED.darker());
            statusText.setText("OFF");
            statusText.setTranslateX(15);
        }
    }
    
    private void updateControlState() {
        
        acSlider.setDisable(!isOn);
        if (isOn) {
            acSlider.setValue(0);
            updateACStatus(0);
            initializeMotor();
        }
    }
    
    private void updateMotorSpeed(int sliderValue) {
        if (!motorInitialized) {
            statusLabel.setText("Status: Please initialize motor first");
            return;
        }
        
        this.motorSpeed = SPEED_MAPPING[sliderValue];
        motorSpeedLabel.setText(sliderValue + " (" + this.motorSpeed + " RPM)");
        updateMotorDirection();
    }

    private void updateACStatus(int sliderValue) {
        int absoluteValue = Math.abs(sliderValue);
        this.motorSpeed = SPEED_MAPPING[absoluteValue];
        
        acSpeedLabel.setText("SPEED: " + absoluteValue + " (" + this.motorSpeed + " RPM)");

        if (sliderValue > 0) {
            clockwiseDirection = true;
            acDirectionLabel.setText("DIRECTION: CLOCKWISE");
            acDirectionLabel.setTextFill(Color.GREEN);
        } else if (sliderValue < 0) {
            clockwiseDirection = false;
            acDirectionLabel.setText("DIRECTION: COUNTER-CLOCKWISE");
            acDirectionLabel.setTextFill(Color.RED);
        } else {
            acDirectionLabel.setText("DIRECTION: STOPPED");
            acDirectionLabel.setTextFill(Color.web("#aaaaaa"));
        }

        if (absoluteValue >= 4) {  // High speed warning at level 4 or 5
            gauge.setBarColor(Color.web(WARNING_COLOR));
        } else {
            gauge.setBarColor(Color.web(ACCENT_COLOR));
        }
    }

    private void updateMotorDirection() {
        if (motorSpeed == 0) {
            motorDirection = "MOTOR STOPPED";
            motorDirectionLabel.setTextFill(Color.web("#aaaaaa"));
        } else {
            motorDirection = clockwiseDirection ? "CLOCKWISE" : "COUNTER-CLOCKWISE";
            motorDirectionLabel.setTextFill(clockwiseDirection ? Color.GREEN : Color.RED);
        }
        motorDirectionLabel.setText(motorDirection);
    }

    private byte bridgeValue() {
        if (!motorInitialized || !isOn) return 0;
        
        int mappedSpeed;
        boolean isNormalMode = (primaryStage.getScene() == normalMotorControlScene);
        
        if (isNormalMode) {
            // Normal mode uses direct mapping from slider
            mappedSpeed = SPEED_MAPPING[(int)slider.getValue()];
        } else {
            // AC mode uses absolute value from slider
            mappedSpeed = SPEED_MAPPING[Math.abs((int)acSlider.getValue())];
        }

        int directionBit = clockwiseDirection ? 0 : 1;
        byte result = (byte)((directionBit << 4) | mappedSpeed);

        System.out.printf("Mode: %s, Slider: %d, Speed: %d, Dir: %s, Binary: %08d%n",
            isNormalMode ? "NORMAL" : "AC",
            isNormalMode ? (int)slider.getValue() : (int)acSlider.getValue(),
            mappedSpeed,
            directionBit == 0 ? "CW" : "CCW",
            Integer.parseInt(Integer.toBinaryString(result & 0xFF)));
        
        return result;
    }

    public static void main(String[] args) {
        launch(args);
    }
}
