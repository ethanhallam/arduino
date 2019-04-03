#define ultrasonicSend1 7 // Front 
#define ultrasonicRecieve1 12 // Front

// Second (right) ultrasonic sensor
#define ultrasonicSend2 4 // Right side
#define ultrasonicRecieve2 13 // Right side

// Third (left) ultrasonic sensor
#define ultrasonicSend3 A0// Left side
#define ultrasonicRecieve3 A1//Left side

// IR sensors
#define irPinLeft A2
#define irPinCentre A3
#define irPinRight A4

void setup() {
  // Motors
  pinMode(5, OUTPUT); //set ENA as an output
  pinMode(6, OUTPUT); //set ENB as an output
  pinMode(8, OUTPUT); //set IN1 as an output
  pinMode(9, OUTPUT); //set IN2 as an output
  pinMode(10, OUTPUT); //set IN3 as an output
  pinMode(11, OUTPUT); //set IN4 as an output

  // Ultrasonic Sensor 1
  pinMode(ultrasonicSend1, OUTPUT);  // Pin 7
  pinMode(ultrasonicRecieve1, INPUT); // Pin 12

  // Ultrasonic Sensor 2
  pinMode(ultrasonicSend2, OUTPUT);  // Pin 4
  pinMode(ultrasonicRecieve2, INPUT); // Pin 13

  // Ultrasonic Sensor 3
  pinMode(ultrasonicSend3, OUTPUT);   // Pin A0
  pinMode(ultrasonicRecieve3, INPUT); // Pin A1

  // IR sensors
  pinMode(irPinLeft, INPUT);
  pinMode(irPinCentre, INPUT);
  pinMode(irPinRight, INPUT);

  //Serial.begin(9600);
}


void loop() {
  // IR sensors
  checkSensor();

  // Measure distance;
  unsigned int mm = 0;
  unsigned int mm2 = 0;//This variable will hold the distance
  unsigned int mm3 = 0;
  mm = sonar_mm();//call the function sonar_mm and store the result in
  mm2 = sonar_mm2();//...distance_mm
  mm3 = sonar_mm3();
  //Serial.println(mm);

  //Movement
  while (mm3 <= 100) {
    checkSensor();
    mm3 = sonar_mm3();
    move(110, 0);
    delay(100);
    move(0, 0);
  }
  while (mm >= 600 && mm2 >= 600 ) { // When it isn't perpendicular to a wall - aka search for wall
    checkSensor();
    mm = sonar_mm();
    mm2 = sonar_mm2();
    move(-90, 90);
    delay(100);
    move(0, 0);
    delay(100);
  }

  while (mm <= 130 || mm <= 130 && mm2 <= 200) { // If front is near a wall (or corner) turn left
    mm = sonar_mm();
    mm2 = sonar_mm2();
    checkSensor();
    move(-90, 90);
    delay(100);
    move(0, 0);
    delay(100);
  }

  while (mm2 <= 100 && mm > 130) { // If side is too close to wall move slightly away from it
    checkSensor();
    mm = sonar_mm();
    mm2 = sonar_mm2();
    while (mm2 < 40 && mm > 130) { //When too close to the wall
      mm = sonar_mm();
      mm2 = sonar_mm2();
      checkSensor();
      move(0, 0);
      delay(100);
      move(0, 100);
      delay(100);
      move(0, 0);
      delay(100);
      move(0, 100);
      delay(100);
      move(0, 0);
      delay(100);
      move(100, 100);
      delay(100);
      move(0, 0);
      delay(100);
      move(100, 0);
      delay(100);
      move(0, 0);
      delay(100);
    }
    move(0, 0);
    delay(100);
    move(0, 100);
    delay(100);
    move(0, 0);
  }

  while (mm2 <= 200 && mm2 > 100 && mm > 130) { // If too far away from wall - Head towards the wall and move forward
    checkSensor();
    mm = sonar_mm();
    mm2 = sonar_mm2();
    move(0, 0);
    delay(100);
    move(100, 0);
    delay(100);
    move(0, 0);
  }

  while (mm < 1200 && mm2 < 1200 && mm > 130 && mm2 > 200 && mm3 > 200) { // Move Forward if walls are dectected on both sensors
    checkSensor();
    mm = sonar_mm();
    mm2 = sonar_mm2();
    mm3 = sonar_mm3();
    move(0, 0);
    delay(100);
    move(110, 120);
    delay(100);
    //    while(mm<1200 && mm2<1200 && mm>200 && mm2>200) {
    //      checkSensor();
    //      mm=sonar_mm();
    //      mm2=sonar_mm2();
    //      move(60,60); }
  }

}


// Measure Ultrasonic Sensor 1 (front)
unsigned int sonar_mm(void) {
  long duration = 0;
  const float speed_sound = 340.29;
  digitalWrite(ultrasonicSend1, HIGH);
  delayMicroseconds(10);
  digitalWrite(ultrasonicSend1, LOW);
  duration = pulseIn(ultrasonicRecieve1, HIGH);
  return (unsigned int)(0.5 * duration * 1e-6 * speed_sound * 1e3);
}

// Measure Ultrasonic Sensor 2 (right side)
unsigned int sonar_mm2(void) {
  long duration = 0;
  const float speed_sound = 340.29;
  digitalWrite(ultrasonicSend2, HIGH);
  delayMicroseconds(10);
  digitalWrite(ultrasonicSend2, LOW);
  duration = pulseIn(ultrasonicRecieve2, HIGH);
  return (unsigned int)(0.5 * duration * 1e-6 * speed_sound * 1e3);
}

//Measure Ultrasonic Sensor 3 (left side)
unsigned int sonar_mm3(void) {
  long duration = 0;
  const float speed_sound = 340.29;
  digitalWrite(ultrasonicSend3, HIGH);
  delayMicroseconds(10);
  digitalWrite(ultrasonicSend3, LOW);
  duration = pulseIn(ultrasonicRecieve3, HIGH);
  return (unsigned int) (0.5 * duration * 1e-6 * speed_sound * 1e3);
}


void move (int lSpeed, int rSpeed) {
  lSpeed = (lSpeed * (float)(0.75)); //Left motor more powerful than right
  if (lSpeed < 0) { //Left motor backwards
    digitalWrite(8, HIGH);
    digitalWrite(9, LOW);
    analogWrite(5, -lSpeed);
  }

  if (lSpeed >= 0) { //Left motor forwards
    digitalWrite(9, HIGH);
    digitalWrite(8, LOW);
    analogWrite(5, lSpeed);
  }

  if (rSpeed < 0) { //Right motor backwards
    digitalWrite(10, HIGH);
    digitalWrite(11, LOW);
    analogWrite(6, -rSpeed);
  }

  if (rSpeed >= 0) { //Right motor forwards
    digitalWrite(11, HIGH);
    digitalWrite(10, LOW);
    analogWrite(6, rSpeed);
  }

}

int checkSensor() {

  // Gets IR Sensor Data
  int irReadingLeft = digitalRead(irPinLeft);
  int irReadingCentre = digitalRead(irPinCentre);
  int irReadingRight = digitalRead(irPinRight);

  // Variable basically stores whether or not the black box has been detected
  int  blackBox;

  if (irReadingLeft == 1 || irReadingCentre == 1 || irReadingRight == 1) {
    blackBox = 1;
  }

  
  // Stops robot
  while (blackBox == 1) {
    move(0, 0);
    irReadingLeft = digitalRead(irPinLeft);
    irReadingCentre = digitalRead(irPinCentre);
    irReadingRight = digitalRead(irPinRight);
    if (irReadingLeft == 1 || irReadingCentre == 1 || irReadingRight == 1) {
      blackBox = 1;
    } else {
      blackBox = 0;
    }
  }
}
