/*************************************** Hardware Pin Mapping ***************************************/
#define FRONT_ULTRASONIC_TRIG_PIN                                  2
#define FRONT_ULTRASONIC_ECHO_PIN                                  3

#define BACK_ULTRASONIC_TRIG_PIN                                   8
#define BACK_ULTRASONIC_ECHO_PIN                                   9

#define RIGHT_ULTRASONIC_TRIG_PIN                                  6
#define RIGHT_ULTRASONIC_ECHO_PIN                                  7

#define LEFT_ULTRASONIC_TRIG_PIN                                   10
#define LEFT_ULTRASONIC_ECHO_PIN                                   11

#define MOTOR_LEFT_FORWARD_PIN                                     A0
#define MOTOR_LEFT_BACKWARD_PIN                                    A1

#define MOTOR_RIGHT_FORWARD_PIN                                    A2
#define MOTOR_RIGHT_BACKWARD_PIN                                   A3

#define IR_FRONT_RIGHT_PIN                                         4
#define IR_FRONT_LEFT_PIN                                          5

#define IR_BACK_RIGHT_PIN                                          A5
#define IR_BACK_LEFT_PIN                                           A4
/****************************************************************************************************/

/********************************** Global Configuration Settings ***********************************/
#define ULTRASONIC_MAX_DISTANCE                                    25
#define BAUD_RATE                                                  9600
#define STARTUP_DELAY_MS                                           5000
#define TURN_DELAY_MS                                              150
#define MOVE_DELAY_MS                                              1000
#define STOP_DELAY_MS                                              1000
/****************************************************************************************************/

/************************************** Sensor Reading Macros **************************************/
#define GetDistance_FRONT_ULTRASONIC()            getDistance(FRONT_ULTRASONIC_TRIG_PIN, FRONT_ULTRASONIC_ECHO_PIN)
#define GetDistance_BACK_ULTRASONIC()             getDistance(BACK_ULTRASONIC_TRIG_PIN, BACK_ULTRASONIC_ECHO_PIN)

#define GetDistance_RIGHT_ULTRASONIC()            getDistance(RIGHT_ULTRASONIC_TRIG_PIN, RIGHT_ULTRASONIC_ECHO_PIN)
#define GetDistance_LEFT_ULTRASONIC()             getDistance(LEFT_ULTRASONIC_TRIG_PIN, LEFT_ULTRASONIC_ECHO_PIN)

#define GetReading_FRONT_RIGHT_IR()               digitalRead(IR_FRONT_RIGHT_PIN)
#define GetReading_FRONT_LEFT_IR()                digitalRead(IR_FRONT_LEFT_PIN)

#define GetReading_BACK_RIGHT_IR()                digitalRead(IR_BACK_RIGHT_PIN)
#define GetReading_BACK_LEFT_IR()                 digitalRead(IR_BACK_LEFT_PIN)
/****************************************************************************************************/

/********************* Functions Prototypes **********************/
inline int getDistance(byte trig_pin, byte echo_pin);
/*****************************************************************/

/********************* Global Variables **************************/
int frontDist, backDist, leftDist, rightDist;
/*****************************************************************/

void setup() {

  /*********************** Front UltraSonic ***********************/
  pinMode(FRONT_ULTRASONIC_TRIG_PIN, OUTPUT);
  pinMode(FRONT_ULTRASONIC_ECHO_PIN, INPUT);
  /****************************************************************/

  /************************ Back UltraSonic ************************/
  pinMode(BACK_ULTRASONIC_TRIG_PIN, OUTPUT);
  pinMode(BACK_ULTRASONIC_ECHO_PIN, INPUT);
  /*****************************************************************/

  /************************ Left UltraSonic ************************/
  pinMode(LEFT_ULTRASONIC_TRIG_PIN, OUTPUT);
  pinMode(LEFT_ULTRASONIC_ECHO_PIN, INPUT);
  /*****************************************************************/

  /************************ Right UltraSonic ***********************/
  pinMode(RIGHT_ULTRASONIC_TRIG_PIN, OUTPUT);
  pinMode(RIGHT_ULTRASONIC_ECHO_PIN, INPUT);
  /*****************************************************************/

  /*************************** DC Motors ***************************/
  pinMode(MOTOR_LEFT_FORWARD_PIN  , OUTPUT);
  pinMode(MOTOR_LEFT_BACKWARD_PIN , OUTPUT);
  pinMode(MOTOR_RIGHT_FORWARD_PIN , OUTPUT);
  pinMode(MOTOR_RIGHT_BACKWARD_PIN, OUTPUT);
  /*****************************************************************/

  delay(STARTUP_DELAY_MS);
  Serial.begin(BAUD_RATE);
}

void loop() {

  // Capture ultrasonic distances at the start of the loop for efficient decision-making
  frontDist = GetDistance_FRONT_ULTRASONIC();
  backDist = GetDistance_BACK_ULTRASONIC();
  leftDist = GetDistance_LEFT_ULTRASONIC();
  rightDist = GetDistance_RIGHT_ULTRASONIC();

  // React fast to IR sensor readings (real-time boundary detection)
  if (GetReading_FRONT_LEFT_IR() == 1 || GetReading_FRONT_RIGHT_IR() == 0) {
    // Front IR triggered: Move aggressively forward
    digitalWrite(MOTOR_LEFT_FORWARD_PIN, 1);
    digitalWrite(MOTOR_LEFT_BACKWARD_PIN, 0);
    digitalWrite(MOTOR_RIGHT_FORWARD_PIN, 1);
    digitalWrite(MOTOR_RIGHT_BACKWARD_PIN, 0);
    delay(MOVE_DELAY_MS);

  } else if (GetReading_BACK_LEFT_IR() == 0 && GetReading_BACK_RIGHT_IR() == 0) {
    // Both back IR triggered: Retreat (possibly near arena boundary)
    digitalWrite(MOTOR_LEFT_FORWARD_PIN, 0);
    digitalWrite(MOTOR_LEFT_BACKWARD_PIN, 1);
    digitalWrite(MOTOR_RIGHT_FORWARD_PIN, 0);
    digitalWrite(MOTOR_RIGHT_BACKWARD_PIN, 1);
    delay(MOVE_DELAY_MS);

  } else if (GetReading_BACK_LEFT_IR() == 0 || GetReading_BACK_RIGHT_IR() == 0) {
    // One back IR triggered: Stop immediately to avoid falling out
    digitalWrite(MOTOR_LEFT_FORWARD_PIN, 0);
    digitalWrite(MOTOR_LEFT_BACKWARD_PIN, 0);
    digitalWrite(MOTOR_RIGHT_FORWARD_PIN, 0);
    digitalWrite(MOTOR_RIGHT_BACKWARD_PIN, 0);
    delay(STOP_DELAY_MS);

  } else if (GetReading_BACK_RIGHT_IR() == 0 && GetReading_FRONT_RIGHT_IR() == 0) {
    // Right side detected: Quick turn to reposition
    digitalWrite(MOTOR_LEFT_FORWARD_PIN, 0);
    digitalWrite(MOTOR_LEFT_BACKWARD_PIN, 1);
    digitalWrite(MOTOR_RIGHT_FORWARD_PIN, 1);
    digitalWrite(MOTOR_RIGHT_BACKWARD_PIN, 0);
    delay(TURN_DELAY_MS);
    // Additional turn for full repositioning
    digitalWrite(MOTOR_LEFT_FORWARD_PIN, 0);
    digitalWrite(MOTOR_LEFT_BACKWARD_PIN, 1);
    digitalWrite(MOTOR_RIGHT_FORWARD_PIN, 0);
    digitalWrite(MOTOR_RIGHT_BACKWARD_PIN, 1);
    delay(MOVE_DELAY_MS);

  } else if (GetReading_FRONT_LEFT_IR() == 1 && GetReading_BACK_LEFT_IR() == 0) {
    // Left side detected: Turn left to face opponent
    digitalWrite(MOTOR_LEFT_FORWARD_PIN, 1);
    digitalWrite(MOTOR_LEFT_BACKWARD_PIN, 0);
    digitalWrite(MOTOR_RIGHT_FORWARD_PIN, 0);
    digitalWrite(MOTOR_RIGHT_BACKWARD_PIN, 1);
    delay(TURN_DELAY_MS);
    // Final adjustment
    digitalWrite(MOTOR_LEFT_FORWARD_PIN, 0);
    digitalWrite(MOTOR_LEFT_BACKWARD_PIN, 1);
    digitalWrite(MOTOR_RIGHT_FORWARD_PIN, 0);
    digitalWrite(MOTOR_RIGHT_BACKWARD_PIN, 1);
    delay(MOVE_DELAY_MS);

  } else {
    // Ultrasonic sensor-based opponent avoidance
    if (frontDist <= ULTRASONIC_MAX_DISTANCE || 
        backDist <= ULTRASONIC_MAX_DISTANCE || 
        leftDist <= ULTRASONIC_MAX_DISTANCE || 
        rightDist <= ULTRASONIC_MAX_DISTANCE) {
      
      if (frontDist <= backDist && frontDist <= leftDist && frontDist <= rightDist) {
        // Opponent detected in front: Charge forward
        digitalWrite(MOTOR_LEFT_FORWARD_PIN, 1);
        digitalWrite(MOTOR_LEFT_BACKWARD_PIN, 0);
        digitalWrite(MOTOR_RIGHT_FORWARD_PIN, 1);
        digitalWrite(MOTOR_RIGHT_BACKWARD_PIN, 0);

      } else if (backDist <= leftDist && backDist <= rightDist) {
        // Opponent behind: Retreat and reposition
        digitalWrite(MOTOR_LEFT_FORWARD_PIN, 0);
        digitalWrite(MOTOR_LEFT_BACKWARD_PIN, 1);
        digitalWrite(MOTOR_RIGHT_FORWARD_PIN, 0);
        digitalWrite(MOTOR_RIGHT_BACKWARD_PIN, 1);

      } else if (leftDist <= rightDist) {
        // Opponent on left: Turn left
        digitalWrite(MOTOR_LEFT_FORWARD_PIN, 0);
        digitalWrite(MOTOR_LEFT_BACKWARD_PIN, 1);
        digitalWrite(MOTOR_RIGHT_FORWARD_PIN, 1);
        digitalWrite(MOTOR_RIGHT_BACKWARD_PIN, 0);
        delay(TURN_DELAY_MS);

      } else if (rightDist <= leftDist) {
        // Opponent on right: Turn right
        digitalWrite(MOTOR_LEFT_FORWARD_PIN, 1);
        digitalWrite(MOTOR_LEFT_BACKWARD_PIN, 0);
        digitalWrite(MOTOR_RIGHT_FORWARD_PIN, 0);
        digitalWrite(MOTOR_RIGHT_BACKWARD_PIN, 1);
        delay(TURN_DELAY_MS);
      }
    } else {
      // No opponent or boundary detected: Stop
      digitalWrite(MOTOR_LEFT_FORWARD_PIN, 0);
      digitalWrite(MOTOR_LEFT_BACKWARD_PIN, 0);
      digitalWrite(MOTOR_RIGHT_FORWARD_PIN, 0);
      digitalWrite(MOTOR_RIGHT_BACKWARD_PIN, 0);
    }
  }
}

inline int getDistance(byte trig_pin, byte echo_pin) {
  digitalWrite(trig_pin, LOW);
  delayMicroseconds(2);
  digitalWrite(trig_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig_pin, LOW);

  long duration = pulseIn(echo_pin, HIGH);
  return duration * 0.034 / 2;
}
