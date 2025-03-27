package com.mycompany.finalproject;

import javafx.application.Application;
import javafx.geometry.Pos;
import javafx.scene.Scene;
import javafx.scene.control.Label;
import javafx.scene.control.Slider;
import javafx.scene.layout.VBox;
import javafx.stage.Stage;

import static java.lang.Math.*;

public class App extends Application {
    private int motorSpeed = 0;
    private Label motorSpeedLabel;
    private Label motorDirectionLabel;
    private String motorDirection = "Motor stopped";
    private static int frameCounter = 0;

    @Override
    public void start(Stage primaryStage) {
        Slider slider = new Slider(-50, 50, 0);
        slider.setShowTickLabels(true);
        slider.setMajorTickUnit(10);

        motorSpeedLabel = new Label("Motor Speed: " + motorSpeed);
        motorDirectionLabel = new Label("Motor Direction: " + motorDirection);

        slider.valueProperty().addListener((obs, oldVal, newVal) -> {
            int snappedValue = (int) round(newVal.doubleValue() / 10) * 10;
            if (newVal.intValue() != snappedValue) {
                slider.setValue(snappedValue);
            } else if (!slider.isValueChanging()) {
                updateMotorSpeed(snappedValue);
                bridgeValue();
            }
        });

        VBox root = new VBox(50, slider, motorSpeedLabel, motorDirectionLabel);
        root.setAlignment(Pos.CENTER);
        primaryStage.setMinWidth(600);
        primaryStage.setMinHeight(600);
        root.setStyle("-fx-padding: 20px;");
        Scene scene = new Scene(root, 600, 600);
        primaryStage.setTitle("Final project GUI");
        primaryStage.setScene(scene);
        primaryStage.show();
    }

    private void updateMotorSpeed(int motorSpeed) {
        int snappedSpeed = (int) round(motorSpeed / 10.0) * 10;
        this.motorSpeed = snappedSpeed;
        motorSpeedLabel.setText("Motor Speed: " + snappedSpeed);

        if (snappedSpeed > 0) {
            motorDirection = "Clockwise";
        } else if (snappedSpeed < 0) {
            motorDirection = "Counter clockwise";
        } else {
            motorDirection = "Motor stopped";
        }
        motorDirectionLabel.setText("Motor Direction: " + motorDirection);
    }

    private char bridgeValue() {
        int mappedSpeed;
        switch (abs(motorSpeed)) {
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
        char result = (char) (
            (frameCounter << 5) |
            (directionBit << 4) |
            mappedSpeed
        );
        

        System.out.printf("Speed: %d, Dir: %s, ID: %d -> Binary: %08d%n",
            motorSpeed,
            directionBit == 0 ? "CW" : "CCW",
            frameCounter,
            Integer.parseInt(Integer.toBinaryString(result & 0xFF)));
        frameCounter = (frameCounter + 1) & 0x07;
        return result;
    }
    public static void main(String[] args) {
        launch(args);
    }
}