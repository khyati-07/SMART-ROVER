#include <AFMotor.h>
#include <NewPing.h>
#include <Servo.h>

#define TRIG_PIN A1
#define ECHO_PIN A0
#define MAX_DISTANCE 200
#define SAFE_DISTANCE 25   // Stop if object is closer than 25 cm
#define TURN_DELAY 300
#define SCAN_DELAY 250

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

  // 🔹 Motor speed: reduced drastically (~70% slower)
  motor1.setSpeed(60);
  motor2.setSpeed(60);
  motor3.setSpeed(60);
  motor4.setSpeed(60);

  myservo.attach(9);
  myservo.write(100);
  delay(1000);
  Serial.println("Ultra-Safe Obstacle Avoidance Car Started");
}

// ------------------ MAIN LOOP ------------------
void loop() {
  distance = getDistance();
  Serial.print("Distance Ahead: ");
  Serial.println(distance);

  if (distance <= SAFE_DISTANCE) {
    stopCar();
    delay(150);
    moveBackward();
    delay(400);
    stopCar();
    delay(200);

    int rightDist = scanRight();
    int leftDist = scanLeft();

    if (rightDist > leftDist) {
      turnRight();
    } else {
      turnLeft();
    }
    stopCar();
  } 
  else {
    moveForward();
  }

  delay(80);  // shorter loop delay → quicker response
}

// ------------------ MOVEMENT FUNCTIONS ------------------
void moveForward() {
  setMotorSpeed(70); // very slow
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
}

void moveBackward() {
  setMotorSpeed(60);
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
  setMotorSpeed(65);
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(BACKWARD);
  motor4.run(BACKWARD);
  delay(TURN_DELAY);
}

void turnRight() {
  setMotorSpeed(65);
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
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
  delay(40);
  int cm = sonar.ping_cm();
  if (cm == 0) cm = MAX_DISTANCE;
  return cm;
}

int scanRight() {
  myservo.write(50);
  delay(SCAN_DELAY);
  int dist = getDistance();
  myservo.write(100);
  delay(150);
  Serial.print("Right Distance: "); Serial.println(dist);
  return dist;
}

int scanLeft() {
  myservo.write(150);
  delay(SCAN_DELAY);
  int dist = getDistance();
  myservo.write(100);
  delay(150);
  Serial.print("Left Distance: "); Serial.println(dist);
  return dist;
}