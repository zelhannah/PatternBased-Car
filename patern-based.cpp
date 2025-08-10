#include <Servo.h>

// Motor driver pins
#define ENA 3
#define IN1 2
#define IN2 4
#define IN3 5
#define IN4 7
#define ENB 6

// Bluetooth module
// #define BT_RX 8
// #define BT_TX 9

// Servo
#define SERVO_PIN 11
Servo myServo;

// Ultrasonic sensor 
#define TRIG 9
#define ECHO 8

// Light sensor
#define LIGHT_SENSOR A0

// Fan
#define FAN_PIN 10

// Variables
char command;
int distance;
int lightValue;
bool fanActive = false;
unsigned long fanStartTime = 0;

// Function to measure distance
int getDistance() {
    digitalWrite(TRIG, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG, LOW);
    int duration = pulseIn(ECHO, HIGH);
    return duration * 0.034 / 2; // Convert to cm
}

// Motor control functions
void moveForward() {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    analogWrite(ENA, 150);
    analogWrite(ENB, 150);
}

void moveBackward() {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    analogWrite(ENA, 150);
    analogWrite(ENB, 150);
}

void turnLeft() {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    analogWrite(ENA, 150);
    analogWrite(ENB, 150);
}

void turnRight() {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    analogWrite(ENA, 150);
    analogWrite(ENB, 150);
}

void stopCar() {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
}

void setup() {
    Serial.begin(9600);
    pinMode(ENA, OUTPUT);
    pinMode(ENB, OUTPUT);
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
    pinMode(TRIG, OUTPUT);
    pinMode(ECHO, INPUT);
    pinMode(LIGHT_SENSOR, INPUT);
    pinMode(FAN_PIN, OUTPUT);
    digitalWrite(FAN_PIN, LOW);
    myServo.attach(SERVO_PIN);
    stopCar();
    moveForward(); // Ensure the car always moves forward
}

void loop() {
    // // Read Bluetooth command
    // if (Serial.available() > 0) {
    //     command = Serial.read();
    //     Serial.println(command);
        
    //     if (command == 'F') moveForward();
    //     else if (command == 'B') moveBackward();
    //     else if (command == 'L') turnLeft();
    //     else if (command == 'R') turnRight();
    //     else if (command == 'S') stopCar();
    // }
    
    // Check ultrasonic sensor
    distance = getDistance();
    if (distance > 0 && distance < 30) {
        stopCar();
        delay(500);
        
        myServo.write(0);
        delay(500);
        int rightDistance = getDistance();
        
        myServo.write(180);
        delay(500);
        int leftDistance = getDistance();
        
        myServo.write(90);
        delay(500);
        
        if (rightDistance > leftDistance) {
            turnRight();
        } else {
            turnLeft();
        }
        delay(500);
        moveForward(); // Ensure it continues moving forward
    }
    
    // Check light sensor
    lightValue = analogRead(LIGHT_SENSOR);
    Serial.print("Light Sensor Value: ");
    Serial.println(lightValue);
    
    if (lightValue > 800 && !fanActive) { // Fire detected and fan not already running
        stopCar();
        digitalWrite(FAN_PIN, LOW); // Turn fan on
        fanActive = true;
        fanStartTime = millis(); // Record start time
    }
    
    // Turn off fan after 4 seconds
    if (fanActive && millis() - fanStartTime >= 4000) {
        digitalWrite(FAN_PIN, HIGH);
        fanActive = false;
    }

    Serial.print("Distance : ");
    Serial.println(distance);
}