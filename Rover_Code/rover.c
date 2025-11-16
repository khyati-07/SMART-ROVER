#include <AFMotor.h>
#include <NewPing.h>
#include <Servo.h>

#define TRIG_PIN A1
#define ECHO_PIN A0
#define MAX_DISTANCE 200
#define SAFE_DISTANCE 10   // Stop if object is closer than 10 cm
#define TURN_DELAY 400     // Increased for better turning
#define SCAN_DELAY 300     // Increased for better sensor reading
#define MOTOR_SPEED 50     // Lower speed for testing (can increase to 80-100 for final presentation)

// Objects
AF_DCMotor motor1(1);
AF_DCMotor motor2(2);
AF_DCMotor motor3(3);
AF_DCMotor motor4(4);
NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);
Servo myservo;

int distance = 100;

// ------------------ SETUP ------------------
void setup() {
  Serial.begin(9600);
  
  // Initialize motors with low testing speed
  setMotorSpeed(MOTOR_SPEED);
  
  // Stop all motors initially
  stopCar();

  // Attach and center servo
  myservo.attach(9);
  myservo.write(90);  // Center position
  delay(1000);
  
  Serial.println("=================================");
  Serial.println("SMART ROVER - Obstacle Avoidance");
  Serial.println("Testing Mode: Speed = 50");
  Serial.println("Safe Distance: 10 cm");
  Serial.println("=================================");
  
  // Test sensor
  delay(500);
  int testDist = getDistance();
  Serial.print("Initial sensor test: ");
  Serial.print(testDist);
  Serial.println(" cm");
  
  if (testDist == 0) {
    Serial.println("WARNING: Sensor may not be working properly!");
  }
  
  delay(1000);
  Serial.println("Starting rover...");
}

// ------------------ MAIN LOOP ------------------
void loop() {
  distance = getDistance();
  
  Serial.print("Distance Ahead: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Check if obstacle detected
  if (distance > 0 && distance <= SAFE_DISTANCE) {
    Serial.println(">>> OBSTACLE DETECTED! <<<");
    
    // Stop immediately
    stopCar();
    delay(200);
    
    // Move backward a bit
    Serial.println("Moving backward...");
    moveBackward();
    delay(500);
    
    // Stop before scanning
    stopCar();
    delay(300);

    // Scan both directions
    Serial.println("Scanning directions...");
    int rightDist = scanRight();
    int leftDist = scanLeft();

    // Decide which way to turn
    Serial.print("Comparing: Right=");
    Serial.print(rightDist);
    Serial.print(" cm, Left=");
    Serial.print(leftDist);
    Serial.println(" cm");

    if (rightDist > leftDist) {
      Serial.println("Turning RIGHT");
      turnRight();
    } else {
      Serial.println("Turning LEFT");
      turnLeft();
    }
    
    stopCar();
    delay(200);
  } 
  else if (distance == 0) {
    // Sensor returned 0 - could be error or very far
    Serial.println("No reading - moving slowly forward");
    moveForward();
  }
  else {
    // Path is clear - move forward
    Serial.println("Path clear - moving forward");
    moveForward();
  }

  delay(100);  // Loop delay for sensor stability
}

// ------------------ MOVEMENT FUNCTIONS ------------------
void moveForward() {
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
}

void moveBackward() {
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
  motor3.run(BACKWARD);
  motor4.run(BACKWARD);
}

void stopCar() {
  motor1.run(RELEASE);
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);
}

void turnLeft() {
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
  delay(TURN_DELAY);
}

void turnRight() {
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(BACKWARD);
  motor4.run(BACKWARD);
  delay(TURN_DELAY);
}

void setMotorSpeed(int spd) {
  motor1.setSpeed(spd);
  motor2.setSpeed(spd);
  motor3.setSpeed(spd);
  motor4.setSpeed(spd);
}

// ------------------ SENSOR FUNCTIONS ------------------
int getDistance() {
  delay(50);  // Wait for sensor to be ready
  int cm = sonar.ping_cm();
  
  // If sensor returns 0, it means no echo received (too far or error)
  if (cm == 0) {
    cm = MAX_DISTANCE;  // Treat as clear path
  }
  
  return cm;
}

int scanRight() {
  myservo.write(30);  // Look right (adjusted angle)
  delay(SCAN_DELAY);
  int dist = getDistance();
  myservo.write(90);  // Return to center
  delay(200);
  
  Serial.print("  Right Distance: "); 
  Serial.print(dist);
  Serial.println(" cm");
  
  return dist;
}

int scanLeft() {
  myservo.write(150);  // Look left (adjusted angle)
  delay(SCAN_DELAY);
  int dist = getDistance();
  myservo.write(90);  // Return to center
  delay(200);
  
  Serial.print("  Left Distance: "); 
  Serial.print(dist);
  Serial.println(" cm");
  
  return dist;
}
