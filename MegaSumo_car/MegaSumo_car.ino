#define FRONT_TRIG_PIN 2
#define FRONT_ECHO_PIN 3
#define BACK_TRIG_PIN 8
#define BACK_ECHO_PIN 9
#define LEFT_TRIG_PIN 10
#define LEFT_ECHO_PIN 11
#define RIGHT_TRIG_PIN 6
#define RIGHT_ECHO_PIN 7

#define MOTOR1_PIN1 A0
#define MOTOR1_PIN2 A1
#define MOTOR2_PIN1 A2
#define MOTOR2_PIN2 A3

#define IR1 4
#define IR2 5
#define IR3 A4
#define IR4 A5

#define MAX_DISTANCE 30

void setup() {
  pinMode(FRONT_TRIG_PIN, OUTPUT);
  pinMode(FRONT_ECHO_PIN, INPUT);
  pinMode(BACK_TRIG_PIN, OUTPUT);
  pinMode(BACK_ECHO_PIN, INPUT);
  pinMode(LEFT_TRIG_PIN, OUTPUT);
  pinMode(LEFT_ECHO_PIN, INPUT);
  pinMode(RIGHT_TRIG_PIN, OUTPUT);
  pinMode(RIGHT_ECHO_PIN, INPUT);
  pinMode(MOTOR1_PIN1, OUTPUT);
  pinMode(MOTOR1_PIN2, OUTPUT);
  pinMode(MOTOR2_PIN1, OUTPUT);
  pinMode(MOTOR2_PIN2, OUTPUT);

  delay(5000);
  Serial.begin(9600);
}

void loop() {
  int frontDist = getDistance(FRONT_TRIG_PIN, FRONT_ECHO_PIN);
  int backDist = getDistance(BACK_TRIG_PIN, BACK_ECHO_PIN);
  int leftDist = getDistance(LEFT_TRIG_PIN, LEFT_ECHO_PIN);
  int rightDist = getDistance(RIGHT_TRIG_PIN, RIGHT_ECHO_PIN);
  int irFrontLeft = digitalRead(IR4);
  int irFrontRight = digitalRead(IR2);
  int irBackLeft = digitalRead(IR3);
  int irBackRight = digitalRead(IR1);

  if (irFrontLeft == 1 || irFrontRight == 0)  {
    // If front IR sensors detect white
    digitalWrite(MOTOR1_PIN1, 1);
    digitalWrite(MOTOR1_PIN2, 0);
    digitalWrite(MOTOR2_PIN1, 1);
    digitalWrite(MOTOR2_PIN2, 0);
    delay(1000);

  } else if (irBackLeft == 0 && irBackRight == 0) {
    // If back IR sensors detect white
    digitalWrite(MOTOR1_PIN1, 0);
    digitalWrite(MOTOR1_PIN2, 1);
    digitalWrite(MOTOR2_PIN1, 0);
    digitalWrite(MOTOR2_PIN2, 1);
    delay(1000);

  } else if (irBackLeft == 0 || irBackRight == 0) {
    // If any back IR sensor detects white
    digitalWrite(MOTOR1_PIN1, 0);
    digitalWrite(MOTOR1_PIN2, 0);
    digitalWrite(MOTOR2_PIN1, 0);
    digitalWrite(MOTOR2_PIN2, 0);
    delay(1000);

  } else if (irBackRight == 0 && irFrontRight == 0) {
    // If right IR sensors detect white
    digitalWrite(MOTOR1_PIN1, 0);
    digitalWrite(MOTOR1_PIN2, 1);
    digitalWrite(MOTOR2_PIN1, 1);
    digitalWrite(MOTOR2_PIN2, 0);
    delay(100);
    digitalWrite(MOTOR1_PIN1, 0);
    digitalWrite(MOTOR1_PIN2, 1);
    digitalWrite(MOTOR2_PIN1, 0);
    digitalWrite(MOTOR2_PIN2, 1);
    delay(500);
    
  } else if (irFrontLeft == 1 && irBackLeft == 0) {
    // If left IR sensors detect white
    digitalWrite(MOTOR1_PIN1, 1);
    digitalWrite(MOTOR1_PIN2, 0);
    digitalWrite(MOTOR2_PIN1, 0);
    digitalWrite(MOTOR2_PIN2, 1);
    delay(100);
    digitalWrite(MOTOR1_PIN1, 0);
    digitalWrite(MOTOR1_PIN2, 1);
    digitalWrite(MOTOR2_PIN1, 0);
    digitalWrite(MOTOR2_PIN2, 1);
    delay(500);
    
  } else if (irBackLeft == 0 && irBackRight == 0 && irFrontLeft == 1 && irBackLeft == 0 ) {
    // Stop if all IR sensors detect white
    digitalWrite(MOTOR1_PIN1, 0);
    digitalWrite(MOTOR1_PIN2, 0);
    digitalWrite(MOTOR2_PIN1, 0);
    digitalWrite(MOTOR2_PIN2, 0);

  } else {
    // Default ultrasonic-based obstacle avoidance
    if (frontDist <= MAX_DISTANCE || backDist <= MAX_DISTANCE || leftDist <= MAX_DISTANCE || rightDist <= MAX_DISTANCE) {
      if (frontDist <= backDist && frontDist <= leftDist && frontDist <= rightDist) {
        // Move forward
        digitalWrite(MOTOR1_PIN1, 0);
        digitalWrite(MOTOR1_PIN2, 1);
        digitalWrite(MOTOR2_PIN1, 0);
        digitalWrite(MOTOR2_PIN2, 1);
        
      } else if (backDist <= leftDist && backDist <= rightDist) {
        // Move backward
        digitalWrite(MOTOR1_PIN1, 1);
        digitalWrite(MOTOR1_PIN2, 0);
        digitalWrite(MOTOR2_PIN1, 1);
        digitalWrite(MOTOR2_PIN2, 0);
        
      } else if (leftDist <= rightDist) {
        // Turn left
        digitalWrite(MOTOR1_PIN1, 0);
        digitalWrite(MOTOR1_PIN2, 1);
        digitalWrite(MOTOR2_PIN1, 1);
        digitalWrite(MOTOR2_PIN2, 0);
        delay(150);
        
      } else if (rightDist <= leftDist) {
        // Turn right
        digitalWrite(MOTOR1_PIN1, 1);
        digitalWrite(MOTOR1_PIN2, 0);
        digitalWrite(MOTOR2_PIN1, 0);
        digitalWrite(MOTOR2_PIN2, 1);
        delay(150);
      }
    } else {
      // Stop if no obstacles detected
      digitalWrite(MOTOR1_PIN1, 0);
      digitalWrite(MOTOR1_PIN2, 0);
      digitalWrite(MOTOR2_PIN1, 0);
      digitalWrite(MOTOR2_PIN2, 0);
    }
  }
}

int getDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH);
  int distance = duration * 0.034 / 2;
  Serial.print("Sensor ");
  Serial.print(trigPin);
  Serial.print(" reading: ");
  Serial.print(distance);
  Serial.println(" cm");
  return distance;
}
