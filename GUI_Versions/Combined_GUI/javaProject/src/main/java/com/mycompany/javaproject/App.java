package com.mycompany.javaproject;

import eu.hansolo.medusa.Gauge;
import eu.hansolo.medusa.GaugeBuilder;
import eu.hansolo.medusa.TickLabelOrientation;
import eu.hansolo.medusa.skins.ModernSkin;
import javafx.application.Application;
import javafx.application.Platform;
import javafx.geometry.Insets;
import javafx.geometry.Pos;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.RadioButton;
import javafx.scene.control.Slider;
import javafx.scene.control.ToggleGroup;
import javafx.scene.layout.HBox;
import javafx.scene.layout.VBox;
import javafx.scene.paint.Color;
import javafx.stage.Stage;

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
    private static int frameCounter = 0;
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

    // Speed mappings (0-5 maps to these values)
    private final int[] SPEED_MAPPING = {0, 3, 6, 9, 12, 15};

    @Override
    public void start(Stage primaryStage) {
        this.primaryStage = primaryStage;
        
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
//    private void handleConnectionError() {
//    // Create an ERROR alert
//    Alert alert = new Alert(AlertType.ERROR);
//    alert.setTitle("Connection Lost");
//    alert.setHeaderText("Failed to connect to the motor controller!");
//    alert.setContentText("Check if the device is properly connected.");
//
//    // Custom buttons
//    ButtonType retryButton = new ButtonType("Retry");
//    ButtonType exitButton = new ButtonType("Exit to Main Menu");
//
//    // Remove default OK button and add custom ones
//    alert.getButtonTypes().setAll(retryButton, exitButton);
//
//    // Show the alert and wait for user response
//    alert.showAndWait().ifPresent(response -> {
//        if (response == retryButton) {
//            // Attempt to reconnect
////            boolean retrySuccess = ;
//            if () {
//                // If retry fails, show the SAME alert again
//                handleConnectionError(); // Recursive call
//            } else {
//                // Success: close alert and continue
//                alert.close();
//            }
//        } else if (response == exitButton) {
//            // Return to the main menu scene
//            primaryStage.setScene(modeSelectionScene);
//            alert.close();
//        }
//    });
//}

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
            bridgeValue();
        });
        
        counterClockwiseBtn = new RadioButton("Counter-Clockwise");
        counterClockwiseBtn.setToggleGroup(directionGroup);
        counterClockwiseBtn.setDisable(true);
        counterClockwiseBtn.setStyle("-fx-text-fill: " + TEXT_COLOR + ";");
        counterClockwiseBtn.setOnAction(e -> {
            clockwiseDirection = false;
            updateMotorDirection();
            bridgeValue();
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
                bridgeValue();
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

        acSpeedLabel = new Label("SPEED: 0 (0 RPM)");
        acSpeedLabel.setStyle("-fx-font-size: 28px; -fx-font-weight: bold; -fx-text-fill: " + TEXT_COLOR + ";");
        
        acDirectionLabel = new Label("DIRECTION: STOPPED");
        acDirectionLabel.setStyle("-fx-font-size: 24px; -fx-font-weight: bold; -fx-text-fill: #aaaaaa;");

        acSlider.valueProperty().addListener((obs, oldVal, newVal) -> {
            int discreteValue = newVal.intValue();
            if (!acSlider.isValueChanging()) {
                updateACStatus(discreteValue);
                bridgeValue(); 
            }
        });

        gauge.valueProperty().bindBidirectional(acSlider.valueProperty());

        VBox controlPanel = new VBox(30, gauge, acSlider, acSpeedLabel, acDirectionLabel);
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
            statusLabel.setText("Status: Motor already initialized");
            return;
        }
        
        statusLabel.setText("Status: Initializing motor...");
        initButton.setDisable(true);
        
        new Thread(() -> {
            try {
                Thread.sleep(500);
                
                Platform.runLater(() -> {
                    motorInitialized = true;
                    statusLabel.setText("Status: Motor initialized and ready");
                    
                    slider.setDisable(false);
                    clockwiseBtn.setDisable(false);
                    counterClockwiseBtn.setDisable(false);
                    
                    slider.setValue(0); 
                    clockwiseBtn.setSelected(true); 
                    updateMotorSpeed(0);
                    updateMotorDirection();
                    bridgeValue();
                });
            } catch (InterruptedException e) {
                Platform.runLater(() -> {
                    statusLabel.setText("Status: Initialization failed");
                    initButton.setDisable(false);
                });
            }
        }).start();
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

    private char bridgeValue() {
        if (!motorInitialized) return 0;
        
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
        char result = (char) ((frameCounter << 5) | (directionBit << 4) | mappedSpeed);

        System.out.printf("Mode: %s, Slider: %d, Speed: %d, Dir: %s, Binary: %08d%n",
            isNormalMode ? "NORMAL" : "AC",
            isNormalMode ? (int)slider.getValue() : (int)acSlider.getValue(),
            mappedSpeed,
            directionBit == 0 ? "CW" : "CCW",
            Integer.parseInt(Integer.toBinaryString(result & 0xFF)));
        
        frameCounter = (frameCounter + 1) & 0x07;
        return result;
    }

    public static void main(String[] args) {
        launch(args);
    }
}