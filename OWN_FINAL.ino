 
  
/*************************************
  Progress Tracker:
  1. Skeleton code
  2. Movement direction 3/5
  Find your functions are code on a seperate file your function
  with the appropriate return values
**************************************/


#include <MeMCore.h>

//#define MICBP 2    //Define sound : bandpass 
//#define MICHP 3    //Define sound: highpass

//Define robot states
#define LEFT_90 1
#define RIGHT_90 2
#define ABOUTTURN 3
#define UTURN_LEFT 4
#define UTURN_RIGHT 5
#define REORIENTATE -1
#define STOP 0
#define STRAIGHT 6
#define LEFT_ADJUST 7
#define RIGHT_ADJUST 8
#define BACK 10

//Miscellaneous definitions
#define USDIST 6
#define TIME 630 //625rt
#define LDRWait 100
#define IRLEFT 15.7
#define IRRIGHT 13.2

#define NOTE_C4  262
#define NOTE_D4  294
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_G4  392
#define NOTE_A4  440
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_D5  587
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_G5  784
  
int melody[112] = {
  NOTE_G5,NOTE_E5,NOTE_F5,NOTE_G5,NOTE_E5,NOTE_F5,NOTE_G5,NOTE_G4,NOTE_A4,NOTE_B4,NOTE_C5,NOTE_D5,NOTE_E5,NOTE_F5,
  NOTE_E5,NOTE_C5,NOTE_D5,NOTE_E5,NOTE_E4,NOTE_F4,NOTE_G4,NOTE_A4,NOTE_G4,NOTE_F4,NOTE_G4,NOTE_E4,NOTE_F4,NOTE_G4,
  NOTE_F4,NOTE_A4,NOTE_G4,NOTE_F4,NOTE_E4,NOTE_D4,NOTE_E4,NOTE_D4,NOTE_C4,NOTE_D4,NOTE_E4,NOTE_F4,NOTE_G4,NOTE_A4,
  NOTE_F4,NOTE_A4,NOTE_G4,NOTE_A4,NOTE_B4,NOTE_C5,NOTE_G4,NOTE_A4,NOTE_B4,NOTE_C5,NOTE_D5,NOTE_E5,NOTE_F5,NOTE_G5,
  NOTE_E5,NOTE_C5,NOTE_D5,NOTE_E5,NOTE_D5,NOTE_C5,NOTE_D5,NOTE_B4,NOTE_C5,NOTE_D5,NOTE_E5,NOTE_D5,NOTE_C5,NOTE_B4,
  NOTE_C5,NOTE_A4,NOTE_B4,NOTE_C5,NOTE_C4,NOTE_D4,NOTE_E4,NOTE_F4,NOTE_E4,NOTE_D4,NOTE_E4,NOTE_C5,NOTE_B4,NOTE_C5,
  NOTE_A4,NOTE_C5,NOTE_B4,NOTE_A4,NOTE_G4,NOTE_F4,NOTE_G4,NOTE_F4,NOTE_E4,NOTE_F4,NOTE_G4,NOTE_A4,NOTE_B4,NOTE_C4,
  NOTE_A4,NOTE_C5,NOTE_B4,NOTE_C5,NOTE_B4,NOTE_A4,NOTE_B4,NOTE_C5,NOTE_D5,NOTE_C5,NOTE_B4,NOTE_C5,NOTE_A4,NOTE_B4
};

int noteDurations[112] = {
  8,16,16,8,16,16,16,16,16,16,16,16,16,16,
  8,16,16,8,16,16,16,16,16,16,16,16,16,16,
  8,16,16,8,16,16,16,16,16,16,16,16,16,16,
  8,16,16,8,16,16,16,16,16,16,16,16,16,16,
  8,16,16,8,16,16,16,16,16,16,16,16,16,16,
  8,16,16,8,16,16,16,16,16,16,16,16,16,16,
  8,16,16,8,16,16,16,16,16,16,16,16,16,16,
  8,16,16,8,16,16,16,16,16,16,16,16,16,16,
  
};


//Define ports
MeLineFollower lineFinder(PORT_2); /* Line Finder module can only be connected to PORT_3, PORT_4, PORT_5, PORT_6 of base shield. */
MeDCMotor motor1(M1);
MeDCMotor motor2(M2);
MeUltrasonicSensor ultraSensor(PORT_1);
MeLightSensor lightSensor(6);
MeRGBLed led(7,2);
MePort IR(PORT_4);
MePort Sound(PORT_3);

MeBuzzer buzzer;


//Function prototypes
int checkChallenge();                   //checks for challenhe
int checkSound();                       //checks for sound
int checkStrip();                       //checks for black strip
int check_side();                       //Uses IR snesors to check side
void movement(int state);               //Checks movementstate
void readColor();                       //reads color values
int getAvr(int times);
int checkColor(float colorArr[]);       
void colorCalibrate();                  //Calibrates colours
void movement_straight_nc();               //Moves Straight with side Checking
void movement_uturn_left();
void movement_uturn_right();
void playVictory();                     //Plays Victory Song
void executeRobot(float colorArr[], int );      //MAIN FUNCTION
void loop();


void setup()
{
  Serial.begin(9600);
  //pinMode(MICBP , INPUT);   
  //pinMode(MICHP , INPUT);
  float colorArr[3] = {0};
  int Victory = 0;
  //Initialize pins
  //Begin maze
  executeRobot(colorArr,Victory);
}


void executeRobot(float colorArr[],int Victory){
  if (Victory == 0){
    Serial.println("Stage1 - Starting Robot");
    movement_straight_nc();
    Serial.println("Stage2 - Blackline clear");
    movement(STOP);
    delay(TIME);
    Serial.println("Stage 3 - Starting colour");
    if (checkChallenge(colorArr)){
      Serial.println("Stage 4");
      Victory = 1;
    }
    Serial.println("Looping movement");
    return executeRobot(colorArr,Victory);
  }
  else{
    Serial.println("Challenge completed!");
    movement(STOP);
    playVictory();
  }
}

//Use ultrasonic sensor to check for front collision
/*int checkFront()
  {
  //If ultrasound sensor sensors a sufficiently short distance, it means the robot is either meeting a challenge or is approaching a wall in a collision.
  //If distance is greater than d, Ultrasound returns 1.
  //Else, Ultrasound returns 0.
  }*/

//Use IR sensor to check for side collision
int check_side(){
  float val1;
  float val2;
  float diff;

  // put your main code here, to run repeatedly:
  val1 = (IR.aRead1()/1023.0*15 + 2); //BLUE // left
  val2 = IR.aRead2()/1023.0*15; //RED // RIGHT
 
  if(val1 > IRLEFT){         //diff > 0.25
    return RIGHT_ADJUST;
  }

   else if(val2 > IRRIGHT){     //0=diff < 0.5
    return LEFT_ADJUST; 
  }
  else{
    return STRAIGHT;
  }
  //IR sensors to check if the robot is equidistant to two parallel sides of the walls.
  //A left negotiation is represented by a positive value, a right negotiation is represented by a negative value.
  //If the robot is moving straight, the function should return 0.
}


int checkChallenge(float colorArr[]) {
  int color, sound;
  //Checks for the type of challenge in place.
  Serial.println("Checking for challenge");
  readColor(colorArr);
  color = checkColor(colorArr);
  
  
  if (color == 0) //color == 0
  {
    sound = checkSound();
    if (sound == 0)
    {
      return 1;
    }
    else
    {
      movement(sound);
    }
  }
  else
  {
    movement(color);
  }
  return 0;
}






int checkSound()
{
  Serial.println("Checking Sound");
  float valBP;
  float valHP;
  float maxBP = 0, maxHP = 0;
  int count = 0;
  float diff = 0;

  while (count < 600) { //I set it to run for around 3 seconds to sample at least the whole duration of one song cycle of each song
    valBP = Sound.aRead1()/ 1023.0 * 5;       //Get low pass values  
    valHP = Sound.aRead2()
    / 1023.0 * 5;       //Get high pss values
    if (valBP > maxBP) {
      maxBP = valBP;
    }

    if (valHP > maxHP) {
      maxHP = valHP;
    }
    //Serial.print("Voltage of valBP: "); Serial.print(valBP); Serial.println("  "); //BP
    //Serial.print("Voltage of valHP: "); Serial.print(valHP); Serial.println("  "); //BP
    //Serial.print("Voltage of diff: "); Serial.print(maxBP - maxHP); Serial.println("  "); //BP
    count++;
    delay(10);
  }
  Serial.print("Voltage of valBP: "); Serial.print(maxBP); Serial.println("  "); //BP
  Serial.print("Voltage of valHP: "); Serial.print(maxHP); Serial.println("  "); //BP
  Serial.print("Voltage of diff: "); Serial.print(maxBP - maxHP); Serial.println("  "); //BP
  count++;
  if ((maxBP - maxHP) > 0.3) {
    Serial.println("Frequency is 100 to 300!");
    return LEFT_90;

  }
  else if ((maxHP - maxBP) > 0.0) {//was 0.15
    Serial.println("Frequency is above 3000!");
    return RIGHT_90;
  }
  else {
    Serial.println("Your mom is gay");
    return 0;
  }
  /*Serial.print("Voltage of sumBP: "); Serial.print(maxBP); Serial.print("  ");
    Serial.println("Voltage of sumHP: "); Serial.print(maxHP); Serial.print("  ");
    Serial.println("Voltage of diff: "); Serial.print(maxBP - maxHP); Serial.print("  ");
    return 1;*/

}


int checkStrip()
{
  //Line sensor checks for black strip.
  int sensorState = lineFinder.readSensors();
  if (sensorState)
  {
    switch (sensorState)
    {
      case S1_IN_S2_IN:
        //Serial.println("Sensor 1 and 2 are inside of black line"); 
        return 1;
        break;
      case S1_IN_S2_OUT:
        //Serial.println("Sensor 2 is outside of black line"); 
        return 1;
        break;
      case S1_OUT_S2_IN: 
        //Serial.println("Sensor 1 is outside of black line");
        return 1;
        break;
      default: 
        //Serial.println("Sensor 1 and 2 are outside of black line");
        return 0;
        break;
    }
  }
}


void movement(int state)    //challenge Movements
{
  switch (state)
  {
    case LEFT_90:
      Serial.println("Turning left 90"); 
      motor1.run(100);
      motor2.run(100);
      delay(TIME);
      movement_straight_nc();
      break;
    case RIGHT_90: 
      Serial.println("Turning right 90");
      motor1.run(-100);
      motor2.run(-100);
      delay(TIME);
      movement_straight_nc();
      break;
    case ABOUTTURN:
      Serial.println("About turn"); 
      motor1.run(200);
      motor2.run(200);
      delay(TIME - 30);
      movement_straight_nc();
      break;
    case STOP: 
      Serial.println("STOP");
      motor1.stop();
      motor2.stop();
      //delay(TIME);
      break;
    case UTURN_LEFT: 
      Serial.println("Uturn Left");
      movement(BACK);
      movement_uturn_left();
      break;
    case UTURN_RIGHT: 
      Serial.println("Uturn Right");
      movement(BACK);
      movement_uturn_right();
      break;
    case STRAIGHT:
      //Serial.println("Straight"); 
      motor1.run(-100);
      motor2.run(100);
      //Incorporate IR and US here!
      break;
    case RIGHT_ADJUST:
      //Serial.println("Adjusting Left");
      motor1.run(-100);
      motor2.run(40);
      break;
    case LEFT_ADJUST:
      //Serial.println("Adjusting Right");
      motor1.run(-60);
      motor2.run(100);
      break;
    case BACK:
      motor1.run(100);
      motor2.run(-100);
      delay(20);
      break;

  }
}

void readColor(float colorArr[]) {   //Reads Colour
  Serial.println("Reading color");
  //int colorArr[3];
  float whiteArr[3];
  float blackArr[3];
  float greyDiff[3];

  whiteArr[0] = 563; //569, 566, 587
  whiteArr[1] = 381; //447, 451, 480
  whiteArr[2] = 437; //452, 463, 494
  blackArr[0] = 418;  
  blackArr[1] = 279;
  blackArr[2] = 322;

//  whiteArr[0] = 556; //569, 566, 587
//  whiteArr[1] = 457; //447, 451, 480
//  whiteArr[2] = 476; //452, 463, 494
//  blackArr[0] = 212;  
//  blackArr[1] = 156;
//  blackArr[2] = 161; 

  int i;
  for (i = 0; i < 3; i++) {
    greyDiff[i] = whiteArr[i] - blackArr[i];
  }

  //get color values
  led.setColor(0, 255, 0, 0);
  led.show();
  delay(300);
  colorArr[0] = ( (getAvr(7) - blackArr[0]) / greyDiff[0] ) * 255;
  led.setColor(0, 0, 255, 0);
  led.show();
  delay(300);
  colorArr[1] = ( (getAvr(7) - blackArr[1]) / greyDiff[1] ) * 255;
  led.setColor(0, 0, 0, 255);
  led.show();
  delay(300);
  colorArr[2] = ( (getAvr(7) - blackArr[2]) / greyDiff[2] ) * 255;
  led.setColor(0, 0, 0, 0);
  led.show();
}


int getAvr(int times) {
  int i;
  int sum = 0;
  int reading;
  int avr;
  for (i = 0; i < times; i++) {
    reading = lightSensor.read();
    sum = sum + reading;
  }
  avr = sum / i;
  return avr;
}

int checkColor(float colorArr[]) {//Checks colour
  Serial.println("Checking color");
  Serial.println(colorArr[0]);
  Serial.println(colorArr[1]);
  Serial.println(colorArr[2]);
  if (colorArr[0] < 20 && colorArr[1] < 20 && colorArr[2] < 20) { //if all values small, black
    Serial.println("black");
    return 0;
  } else {
    if (colorArr[1] > colorArr[2] && colorArr[1] > colorArr[0]) {//if green highest, green
      Serial.println("green");
      return RIGHT_90;
    }

    if (colorArr[0] > colorArr[1] && colorArr[0] > colorArr[2]) { //if red is highest value,
      if (abs(colorArr[1] - colorArr[2]) < 11) {                  //if green and blue similiar, red
        Serial.println("red");
        return LEFT_90;
      } else {
        Serial.println("yellow");                                //if not, yellow
        return ABOUTTURN;
      }
    }

    if (colorArr[2] > colorArr[1] && colorArr[2] > colorArr[0]) { //if blue is highest
      if (colorArr[0] > colorArr[1]) {
        Serial.println("purple");                                //if red higher than green, purple
        return UTURN_LEFT;
      }
      if (colorArr[1] > colorArr[0]) {                           //if green higher than red, light blue
        Serial.println("light blue");
        return UTURN_RIGHT;
      }
    }
  }
  delay(200);
}

int getAvgReading(int times) {
  int reading;
  int total = 0;
  for (int i = 0; i < times; i++) {
    reading = lightSensor.read();
    total = reading + total;
    delay(LDRWait);
  }
  //calculate the average and return it
  return total / times;
}

/*void colorCalibrate() {
  int i;
  Serial.println("White calibration...");
  delay(5000);
  led.setColor(0, 255, 0, 0);  //white calibration start
  led.show();
  delay(200);
  whiteArr[0] = getAvr(7);
  led.setColor(0, 0, 255, 0);
  led.show();
  delay(200);
  whiteArr[1] = getAvr(7);
  led.setColor(0, 0, 0, 255);
  led.show();
  delay(200);
  whiteArr[2] = getAvr(7);     //white calibration end
  Serial.print("White -> R: ");  //print whiteArr
  Serial.print(whiteArr[0]);
  Serial.print(" G: ");
  Serial.print(whiteArr[1]);
  Serial.print(" B: ");
  Serial.println(whiteArr[2]);
  led.setColor(0, 0, 0, 0);
  led.show();
  Serial.println("Black calibration..."); //black calibration start
  delay(5000);
  led.setColor(0, 255, 0, 0);
  led.show();
  delay(200);
  blackArr[0] = getAvr(7);
  led.setColor(0, 0, 255, 0);
  led.show();
  delay(200);
  blackArr[1] = getAvr(7);
  led.setColor(0, 0, 0, 255);
  led.show();
  delay(200);
  blackArr[2] = getAvr(7);     //black calibration end
  Serial.print("black -> R: ");    //print blackArr
  Serial.print(blackArr[0]);
  Serial.print(" G: ");
  Serial.print(blackArr[1]);
  Serial.print(" B: ");
  Serial.println(blackArr[2]);
  for (i = 0; i < 3; i++) {
    greyDiff[i] = whiteArr[i] - blackArr[i];
  }
  }*/

void movement_straight_nc() {   //Movement straight
  while(checkStrip() != 1){ //if (ultraSensor.distanceCm() > USDIST) {
     movement(check_side());
     delay(25);
  }

}
  
  /*if (check_side() == RIGHT){
    motor2.run(25);
  }
  else if(check_side() == LEFT){
    motor1.run(-25);
  }
  motor1.run(-50);
  motor2.run(50); */
   

void movement_uturn_left() {
  movement(LEFT_90);
  delay(TIME);
  movement(STOP);
  delay(TIME);
  
  while (ultraSensor.distanceCm() > USDIST) {
    Serial.println(ultraSensor.distanceCm());
    motor1.run(-100);
    motor2.run(100);
    movement(check_side());
  }
    movement(STOP);
    movement(LEFT_90);
    delay(TIME);
    movement(STRAIGHT);
}

void movement_uturn_right() {
  movement(RIGHT_90);
  delay(TIME);
  movement(STOP);
  delay(TIME);
  
  while (ultraSensor.distanceCm() > USDIST) {
    Serial.println(ultraSensor.distanceCm());
    motor1.run(-100);
    motor2.run(100);
    movement(check_side());
  }
    movement(STOP);
    movement(RIGHT_90);
    delay(TIME);
    movement(STRAIGHT);
}

void playVictory(){
      for (int thisNote = 0; thisNote < 112; thisNote++) {

    // to calculate the note duration, take one second 
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000/noteDurations[thisNote];
    buzzer.tone(melody[thisNote],noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    buzzer.noTone();
  }
}
void loop(){}
