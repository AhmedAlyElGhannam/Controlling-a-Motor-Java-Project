package com.mycompany.finalproject;

import eu.hansolo.medusa.Gauge;
import eu.hansolo.medusa.GaugeBuilder;
import eu.hansolo.medusa.TickLabelOrientation;
import eu.hansolo.medusa.skins.ModernSkin;
import javafx.application.Application;
import javafx.geometry.Insets;
import javafx.geometry.Pos;
import javafx.scene.Scene;
import javafx.scene.control.Label;
import javafx.scene.control.Slider;
import javafx.scene.layout.HBox;
import javafx.scene.layout.VBox;
import javafx.scene.paint.Color;
import javafx.stage.Stage;
import static java.lang.Math.*;

public class App extends Application {
    private Gauge gauge;
    private Label motorSpeedLabel;
    private Label motorDirectionLabel;
    private String motorDirection = "Motor stopped";
    private static int frameCounter = 0;
    private int motorSpeed = 0;

    @Override
    public void start(Stage primaryStage) {
        gauge = GaugeBuilder.create()
                .prefSize(700, 700) 
                .title("MOTOR SPEED")
                .unit("RPM")
                .minValue(-50)
                .maxValue(50)
                .decimals(0)
                .valueColor(Color.WHITE)
                .titleColor(Color.WHITE)
                .barColor(Color.rgb(0, 214, 215))
                .needleColor(Color.WHITE)
                .thresholdColor(Color.rgb(204, 0, 0))
                .tickLabelColor(Color.rgb(151, 151, 151))
                .tickMarkColor(Color.BLACK)
                .tickLabelOrientation(TickLabelOrientation.ORTHOGONAL)
                .build();
        gauge.setSkin(new ModernSkin(gauge));

        // Create extra large slider for control
        Slider slider = new Slider(-50, 50, 0);
        slider.setPrefWidth(700);  // Increased to match gauge width
        slider.setMinWidth(700);
        slider.setShowTickLabels(true);
        slider.setMajorTickUnit(10);
        slider.setMinorTickCount(1);
        slider.setBlockIncrement(10);
        slider.setStyle("-fx-padding: 30px; -fx-font-size: 18px;");  // Larger padding and font

        // Create labels with even larger font
        motorSpeedLabel = new Label("Motor Speed: 0");
        motorSpeedLabel.setStyle("-fx-font-size: 28px; -fx-font-weight: bold;");
        motorSpeedLabel.setTextFill(Color.WHITE);
        
        motorDirectionLabel = new Label("Motor Direction: Stopped");
        motorDirectionLabel.setStyle("-fx-font-size: 28px; -fx-font-weight: bold;");
        motorDirectionLabel.setTextFill(Color.WHITE);

        // Value change listener for the slider
        slider.valueProperty().addListener((obs, oldVal, newVal) -> {
            int snappedValue = (int) round(newVal.doubleValue() / 10) * 10;
            if (newVal.intValue() != snappedValue) {
                slider.setValue(snappedValue);
            } else if (!slider.isValueChanging()) {
                updateMotorStatus(snappedValue);
                bridgeValue();
            }
        });

        // Gauge value binding
        gauge.valueProperty().addListener((obs, oldVal, newVal) -> {
            int snappedValue = (int) round(newVal.doubleValue() / 10) * 10;
            if (Math.abs(newVal.intValue() - snappedValue) > 0) {
                gauge.setValue(snappedValue);
            } else {
                updateMotorStatus(snappedValue);
            }
        });

        // Synchronize gauge and slider
        gauge.valueProperty().bindBidirectional(slider.valueProperty());

        // Layout setup with adjusted spacing
        HBox controlPanel = new HBox(20, slider);
        controlPanel.setAlignment(Pos.CENTER);
        controlPanel.setPadding(new Insets(30, 0, 30, 0));  // Increased padding
        
        VBox labelPanel = new VBox(25, motorSpeedLabel, motorDirectionLabel);  // Increased spacing
        labelPanel.setAlignment(Pos.CENTER);
        
        VBox root = new VBox(50, gauge, controlPanel, labelPanel);  // Increased spacing
        root.setAlignment(Pos.CENTER);
        root.setPadding(new Insets(50));  // Increased padding
        root.setStyle("-fx-background-color: #2b2b2b;");

        // Increased window size to accommodate larger components
        Scene scene = new Scene(root, 600, 750);
        primaryStage.setTitle("Motor Control System");
        primaryStage.setScene(scene);
        primaryStage.show();
    }

    private void updateMotorStatus(int motorSpeed) {
        this.motorSpeed = motorSpeed;
        
        // Update labels
        motorSpeedLabel.setText("Motor Speed: " + motorSpeed);
        
        if (motorSpeed > 0) {
            motorDirection = "Clockwise";
            motorDirectionLabel.setTextFill(Color.GREEN);
        } else if (motorSpeed < 0) {
            motorDirection = "Counter-clockwise";
            motorDirectionLabel.setTextFill(Color.RED);
        } else {
            motorDirection = "Motor stopped";
            motorDirectionLabel.setTextFill(Color.WHITE);
        }
        motorDirectionLabel.setText("Motor Direction: " + motorDirection);
        
        // Update gauge appearance based on speed
        if (Math.abs(motorSpeed) >= 40) {
            gauge.setBarColor(Color.rgb(255, 50, 50));  // Red for high speed
        } else {
            gauge.setBarColor(Color.rgb(0, 214, 215)); // Normal color
        }
    }

    private char bridgeValue() {
        int mappedSpeed;
        switch (Math.abs(motorSpeed)) {
            case 0:  mappedSpeed = 0; break;
            case 10: mappedSpeed = 3; break;
            case 20: mappedSpeed = 6; break;
            case 30: mappedSpeed = 9; break;
            case 40: mappedSpeed = 12; break;
            case 50: mappedSpeed = 15; break;
            default: mappedSpeed = 0;
        }

        int directionBit = (motorSpeed < 0) ? 1 : 0;
        char result = (char) (
            (frameCounter << 5) |
            (directionBit << 4) |
            mappedSpeed
        );
        
        System.out.printf("Speed: %d, Dir: %s, ID: %d -> Binary: %8s%n",
            motorSpeed,
            directionBit == 0 ? "CW" : "CCW",
            frameCounter,
            String.format("%8s", Integer.toBinaryString(result & 0xFF)).replace(' ', '0'));
        
        frameCounter = (frameCounter + 1) & 0x07;
        return result;
    }

    public static void main(String[] args) {
        launch(args);
    }
}