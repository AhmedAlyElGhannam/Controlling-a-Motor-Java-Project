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
import javafx.scene.control.Slider;
import javafx.scene.layout.VBox;
import javafx.scene.paint.Color;
import javafx.stage.Stage;

import javafx.scene.control.Alert;
import javafx.scene.control.ButtonType;
import javafx.scene.control.Alert.AlertType;

import static java.lang.Math.*;

public class App extends Application {
    // Common fields
    private Stage primaryStage;
    private Scene modeSelectionScene;
    private Scene normalMotorControlScene;
    private Scene airConditionerScene;
    
    // Normal mode fields
    private int motorSpeed = 0;
    private Label motorSpeedLabel;
    private Label motorDirectionLabel;
    private String motorDirection = "Motor stopped";
    private SerialCommManager serialCommManager;
    
    // AC mode fields
    private Gauge gauge;
    private Slider acSlider;
    private Label acSpeedLabel;
    private Label acDirectionLabel;

    // Color theme
    private final String PRIMARY_COLOR = "#2b2b2b";
    private final String SECONDARY_COLOR = "#3a3a3a";
    private final String ACCENT_COLOR = "#00d6d7";
    private final String TEXT_COLOR = "#ffffff";
    private final String WARNING_COLOR = "#ff3232";
    private boolean demo = true;

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
    
    private void demoLogicXOR(){
        if(this.demo)
            this.demo = false;
        else
            this.demo = true;
       
        
    }
    private void handleConnectionError() {
    // Create an ERROR alert
    Alert alert = new Alert(AlertType.ERROR);
    alert.setTitle("Connection Lost");
    alert.setHeaderText("Failed to connect to the motor controller!");
    alert.setContentText("Check if the device is properly connected.");

    // Custom buttons
    ButtonType retryButton = new ButtonType("Retry");
    ButtonType exitButton = new ButtonType("Exit to Main Menu");

    // Remove default OK button and add custom ones
    alert.getButtonTypes().setAll(retryButton, exitButton);

    // Show the alert and wait for user response
    alert.showAndWait().ifPresent(response -> {
        if (response == retryButton) {
            // Attempt to reconnect
//            boolean retrySuccess = ;
            demoLogicXOR();
            if (!demo) {
                // If retry fails, show the SAME alert again
                handleConnectionError(); // Recursive call
            } else {
                // Success: close alert and continue
                alert.close();
            }
        } else if (response == exitButton) {
            // Return to the main menu scene
            primaryStage.setScene(modeSelectionScene);
            alert.close();
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
        
        // Interactive effects
        button.setOnMouseEntered(e -> button.setStyle(
            button.getStyle() + 
            "-fx-background-color: linear-gradient(to bottom, derive(" + baseColor + ", 20%), " + baseColor + ");" +
            "-fx-cursor: hand;"
        ));
        button.setOnMouseExited(e -> button.setStyle(
            button.getStyle() + 
            "-fx-background-color: " + baseColor + ";"
        ));
        button.setOnMousePressed(e -> button.setStyle(
            button.getStyle() + 
            "-fx-background-color: derive(" + baseColor + ", -20%);"
        ));
        
        return button;
    }

    private void createNormalMotorControlScene() {
        // Main container with modern card styling
        VBox controlCard = new VBox(25);
        controlCard.setAlignment(Pos.CENTER);
        controlCard.setPadding(new Insets(40));
        controlCard.setStyle(
            "-fx-background-color: " + SECONDARY_COLOR + "; " +
            "-fx-background-radius: 20px; " +
            "-fx-border-radius: 20px; " +
            "-fx-effect: dropshadow(three-pass-box, rgba(0,0,0,0.4), 20, 0, 0, 0);"
        );

        // Title with accent color
        Label titleLabel = new Label("NORMAL MOTOR CONTROL");
        titleLabel.setStyle("-fx-font-size: 26px; -fx-font-weight: bold; -fx-text-fill: " + ACCENT_COLOR + ";");

        // Enhanced slider with better styling
        Slider slider = new Slider(-50, 50, 0);
        slider.setPrefWidth(550);
        slider.setStyle(
            "-fx-control-inner-background: #444444; " +
            "-fx-background-color: transparent; " +
            "-fx-padding: 15px; " +
            "-fx-show-tick-labels: true; " +
            "-fx-show-tick-marks: true;"
        );
        slider.setMajorTickUnit(10);
        slider.setMinorTickCount(1);
        slider.setShowTickLabels(true);
        slider.setBlockIncrement(10);

        // Improved value display
        motorSpeedLabel = new Label("0 RPM");
        motorSpeedLabel.setStyle(
            "-fx-font-size: 36px; " +
            "-fx-font-family: 'Consolas'; " +
            "-fx-font-weight: bold; " +
            "-fx-text-fill: " + TEXT_COLOR + ";"
        );

        motorDirectionLabel = new Label("MOTOR STOPPED");
        motorDirectionLabel.setStyle(
            "-fx-font-size: 24px; " +
            "-fx-font-weight: bold; " +
            "-fx-text-fill: #aaaaaa;"
        );

        // Serial communication setup
        serialCommManager = new SerialCommManager("/dev/ttyUSB0", (byte) 0x00);
        if (!serialCommManager.openPort()) {
            motorDirectionLabel.setText("PORT ERROR!");
            motorDirectionLabel.setTextFill(Color.web(WARNING_COLOR));
            handleConnectionError();

        } else {
            serialCommManager.startPeriodicTransmission(() -> {
                Platform.runLater(() -> {
                    motorDirectionLabel.setText("COM FAILURE!");
                    motorDirectionLabel.setTextFill(Color.web(WARNING_COLOR));
                });
            });
        }

        // Slider value listener
        slider.valueProperty().addListener((obs, oldVal, newVal) -> {
            int snappedValue = (int) round(newVal.doubleValue() / 10) * 10;
            if (newVal.intValue() != snappedValue) {
                slider.setValue(snappedValue);
            } else if (!slider.isValueChanging()) {
                updateMotorSpeed(snappedValue);
                byte dataByte = computeDataByte();
                if (serialCommManager != null) {
                    serialCommManager.setLastSentByte(dataByte);
                }
                logMotorData(snappedValue, dataByte);
            }
        });

        // Back button with consistent styling
        Button backButton = createBackButton();

        // Add elements to card
        controlCard.getChildren().addAll(titleLabel, slider, motorSpeedLabel, motorDirectionLabel);

        // Main layout
        VBox root = new VBox(40, controlCard, backButton);
        root.setAlignment(Pos.CENTER);
        root.setPadding(new Insets(40));
        root.setStyle("-fx-background-color: " + PRIMARY_COLOR + ";");

        normalMotorControlScene = new Scene(root, 800, 800);
    }

    private Button createBackButton() {
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
        backButton.setOnAction(e -> primaryStage.setScene(modeSelectionScene));
        
        return backButton;
    }

    private void createAirConditionerScene() {
        // Create the gauge with enhanced styling
        gauge = GaugeBuilder.create()
                .prefSize(550, 550)
                .title("AIR CONDITIONER MODE")
                .unit("RPM")
                .minValue(-50)
                .maxValue(50)
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

        // Enhanced slider for AC mode
        acSlider = new Slider(-50, 50, 0);
        acSlider.setPrefWidth(550);
        acSlider.setStyle(
            "-fx-control-inner-background: #444444; " +
            "-fx-padding: 20px; " +
            "-fx-font-size: 16px;"
        );
        acSlider.setMajorTickUnit(10);
        acSlider.setMinorTickCount(1);
        acSlider.setBlockIncrement(10);

        // Improved labels
        acSpeedLabel = new Label("SPEED: 0 RPM");
        acSpeedLabel.setStyle("-fx-font-size: 28px; -fx-font-weight: bold; -fx-text-fill: " + TEXT_COLOR + ";");
        
        acDirectionLabel = new Label("DIRECTION: STOPPED");
        acDirectionLabel.setStyle("-fx-font-size: 24px; -fx-font-weight: bold; -fx-text-fill: #aaaaaa;");

        // Slider listener
        acSlider.valueProperty().addListener((obs, oldVal, newVal) -> {
            int snappedValue = (int) round(newVal.doubleValue() / 10) * 10;
            if (newVal.intValue() != snappedValue) {
                acSlider.setValue(snappedValue);
            } else if (!acSlider.isValueChanging()) {
                updateACStatus(snappedValue);
                byte dataByte = computeDataByte();
                if (serialCommManager != null) {
                    serialCommManager.setLastSentByte(dataByte);
                }
                logMotorData(snappedValue, dataByte);

            }
            
                
            
        });

        // Gauge binding
        gauge.valueProperty().bindBidirectional(acSlider.valueProperty());

        // Layout setup
        VBox controlPanel = new VBox(30, gauge, acSlider, acSpeedLabel, acDirectionLabel);
        controlPanel.setAlignment(Pos.CENTER);
        controlPanel.setPadding(new Insets(30));
        controlPanel.setStyle(
            "-fx-background-color: " + SECONDARY_COLOR + "; " +
            "-fx-background-radius: 20px; " +
            "-fx-effect: dropshadow(three-pass-box, rgba(0,0,0,0.3), 15, 0, 0, 0);"
        );

        // Back button
        Button backButton = createBackButton();

        // Main layout
        VBox root = new VBox(40, controlPanel, backButton);
        root.setAlignment(Pos.CENTER);
        root.setPadding(new Insets(40));
        root.setStyle("-fx-background-color: " + PRIMARY_COLOR + ";");

        airConditionerScene = new Scene(root, 800, 900);
    }

    private void updateMotorSpeed(int motorSpeed) {
        int snappedSpeed = (int) round(motorSpeed / 10.0) * 10;
        this.motorSpeed = snappedSpeed;
        motorSpeedLabel.setText(Math.abs(snappedSpeed) + " RPM");

        if (snappedSpeed > 0) {
            motorDirection = "CLOCKWISE";
            motorDirectionLabel.setTextFill(Color.GREEN);
        } else if (snappedSpeed < 0) {
            motorDirection = "COUNTER-CLOCKWISE";
            motorDirectionLabel.setTextFill(Color.RED);
        } else {
            motorDirection = "MOTOR STOPPED";
            motorDirectionLabel.setTextFill(Color.web("#aaaaaa"));
        }
        motorDirectionLabel.setText(motorDirection);
    }

    private void updateACStatus(int motorSpeed) {
        int snappedSpeed = (int) round(motorSpeed / 10.0) * 10;

        acSpeedLabel.setText("SPEED: "+ snappedSpeed + " RPM");
        this.motorSpeed= snappedSpeed;
        
        if (motorSpeed > 0) {
            acDirectionLabel.setText("DIRECTION: CLOCKWISE");
            acDirectionLabel.setTextFill(Color.GREEN);
        } else if (motorSpeed < 0) {
            acDirectionLabel.setText("DIRECTION: COUNTER-CLOCKWISE");
            acDirectionLabel.setTextFill(Color.RED);
        } else {
            acDirectionLabel.setText("DIRECTION: STOPPED");
            acDirectionLabel.setTextFill(Color.web("#aaaaaa"));
        }
        
        // Visual feedback for high speed
        if (Math.abs(motorSpeed) >= 40) {
            gauge.setBarColor(Color.web(WARNING_COLOR));
        } else {
            gauge.setBarColor(Color.web(ACCENT_COLOR));
        }
    }

    private void logMotorData(int speed, byte dataByte) {
        int directionBit = (speed < 0) ? 1 : 0;
        System.out.printf("Speed: %d, Dir: %s → Data: 0x%02X%n",
                Math.abs(speed),
                directionBit == 0 ? "CW" : "CCW",
                dataByte & 0xFF);
    }

    private byte computeDataByte() {
        int mappedSpeed;
        switch (Math.abs(motorSpeed)) {
            case 0: mappedSpeed = 0; break;
            case 10: mappedSpeed = 3; break;
            case 20: mappedSpeed = 6; break;
            case 30: mappedSpeed = 9; break;
            case 40: mappedSpeed = 12; break;
            case 50: mappedSpeed = 15; break;
            default: mappedSpeed = 0;
        }
        int directionBit = (motorSpeed < 0) ? 1 : 0;
        return (byte) ((directionBit << 4) | mappedSpeed);
    }

    public static void main(String[] args) {
        launch(args);
    }
}