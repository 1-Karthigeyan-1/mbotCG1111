//Libraries Used.
#include <MeMCore.h> 

//Define Movement States
#define LEFT_90 1                 //90 degree left turn
#define RIGHT_90 2                //90 degree right turn 
#define ABOUTTURN 3               //180 degree turn on the spot
#define UTURN_LEFT 4              //Uturns by using 2 left 90-degree turns over 2 grids
#define UTURN_RIGHT 5             //Uturns by using 2 right 90-degree turns over 2 grids
#define STOP 0                    // Stops moving
#define STRAIGHT 6                //Moves straight
#define LEFT_ADJUST 7             //Adjust to the left side in a straight pathway
#define RIGHT_ADJUST 8            //Adjust to the right side in a straight pathway
#define BACK 10                   //Move backwards

//Miscellaneous definitions
#define USDIST 6                  //UltraSonic sensor distance                 
#define TIME 630                  //time delay for turning 90 degrees
#define LDRWait 100               //
#define IRLEFT 15.7               //Left IR threshold
#define IRRIGHT 13.2              //Right IR threshold
#define BP_threshold 0.3          //Bandpass threshold
#define HP_threshold 0.0          //High pass threshold

/*_______________Victory music note definations___________________________________*/
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

//Song Tunes
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

//Duration of each note
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

/*________________PORTS____________________*/

//Motor ports:
MeDCMotor motor1(M1);                     //Left Motor
MeDCMotor motor2(M2);                     //Right Motor

//Sensor Ports
MeUltrasonicSensor ultraSensor(PORT_1);   //Ultrasonic Sensor
MeLineFollower lineFinder(PORT_2);        //Linefinder sensor
MePort Sound(PORT_3);                     //soundSensor
MePort IR(PORT_4);                        //IR sensor
MeLightSensor lightSensor(6);             //LightSensor Port
MeRGBLed led(7,2);                        //Led Port

//Miscellaneous Port
MeBuzzer buzzer;                          //Speaker Port


/*________Function Prototypes_________*/
void movement(int state);                 //Takes in movmentState and moves accordingly
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


void setup(){
  //Initialzation
  float colorArr[3] = {0};                 //Colour values are stoered in an array
  int Victory = 0;                         //Victory is state of maze                                             
  
  executeRobot(colorArr,Victory);
}

/*________________Main_Function______________*/
void executeRobot(float colorArr[],int Victory){
  
  if (Victory == 0){
    
    movement_straight_nc();
    movement(STOP);
    delay(TIME);
    
    if (checkChallenge(colorArr)){
      Victory = 1;
    }
    
    return executeRobot(colorArr,Victory);
  }
  
  else{
    movement(STOP);
    playVictory();
  }
  
}



/*____________Movement_Functions__________________*/
void movement(int state){
  switch (state){
    
    //Turn left 90 degress
    case LEFT_90:                          
      motor1.run(100);
      motor2.run(100);
      delay(TIME);
      movement_straight_nc();
      break;
    
    //Turn Right 90 degrees
    case RIGHT_90: 
      motor1.run(-100);
      motor2.run(-100);
      delay(TIME);
      movement_straight_nc();
      break;
      
    //Turns 10 degrees on the spot  
    case ABOUTTURN:
      motor1.run(200);
      motor2.run(200);
      delay(TIME - 30);
      movement_straight_nc();
      break;
      
    //Stop
    case STOP: 
      motor1.stop(
      motor2.stop();
      break;
    
    //2 successive left turns over 2 grids    
    case UTURN_LEFT: 
      Serial.println("Uturn Left");
      movement(BACK);
      movement_uturn_left();
      break;
        
    //2 successive right turns over 2 grids 
    case UTURN_RIGHT: 
      Serial.println("Uturn Right");
      movement(BACK);
      movement_uturn_right();
      break;
        
    //Moves straight  
    case STRAIGHT:
      motor1.run(-100);
      motor2.run(100);
      break;
        
    //Adjust to the right    
    case RIGHT_ADJUST:
      motor1.run(-100);
      motor2.run(40);
      break;
    
     //Adjust to the left    
    case LEFT_ADJUST:
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
        
//Does 2 successive 90 degree right turns over 2 grids        
void movement_uturn_right() {
  movement(RIGHT_90);
  delay(TIME);
  movement(STOP);
  delay(TIME);
  
  while (ultraSensor.distanceCm() > USDIST) {         //Detects for wall 
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

//Does 2 successive 90 degree leftturns over 2 grids        
void movement_uturn_left() {
  movement(LEFT_90);
  delay(TIME);
  movement(STOP);
  delay(TIME);
  
  while (ultraSensor.distanceCm() > USDIST) {       //Detects for wall
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

//Moves straight when theres no challenge
void movement_straight_nc() {  
  while(checkStrip() != 1){                   //checks for black strips along the way.
     movement(check_side());                  //Chwcks the sides and adjust accordingly
     delay(25);
  }

}

//Use IR sensor to check for sides 
int check_side(){
  float Leftval;                               //Left IR
  float Rightval;                              //Right IR
  
  Leftval = IR.aRead1()/1023.0*15 + 2; 
  Rightval = IR.aRead2()/1023.0*15; 
 
  if(Leftval > IRLEFT){                         //If left IR exceed threshold value
    return RIGHT_ADJUST;                        //Adjust to the right
  }

  else if(Rightval > IRRIGHT){                  //If right IR exceed threshold value
    return LEFT_ADJUST;                         //Adjust to the left
  }
  
  else{                                         
    return STRAIGHT;
  }
  //IR sensors to check if the robot is equidistant to two parallel sides of the walls.
  //A left negotiation is represented by a positive value, a right negotiation is represented by a negative value.
  //If the robot is moving straight, the function should return 0.
}



/*_________________Challenge__Checking_____________*/
int checkChallenge(float colorArr[]) {
  int color, sound;
  
  readColor(colorArr);                    //Reads colour , and return its values                        
  color = checkColor(colorArr);           //Checks what colour category the values are in 
  
  //If the colour is black , check for sound
  if (color == 0){                         
    sound = checkSound();
    
    if (sound == 0){
      return 1;
    }
    
    else{
      //Moves accordingly with type of sound
      movement(sound);                        
    }
  }
  
  else{
    //Moves accordinglty with non-black color
    movement(color);
  }
  return 0;
}



/*_________Sound_checking_____________*/
int checkSound()
{
  Serial.println("Checking Sound");
  float valBP;                        //Bandpass valuw
  float valHP;                        //Highpass value
  float maxBP = 0, maxHP = 0;
  int count = 0;
  float diff = 0;

  //run for around 3 seconds to sample at least the whole duration of one song cycle of each song
  while (count < 300) { 
    
    valBP = Sound.aRead1()/ 1023.0 * 5;       //Get low pass values  
    valHP = Sound.aRead2()/ 1023.0 * 5;       //Get high pass values
    
    if (valBP > maxBP) {                      //Finds the maximum bandpass value
      maxBP = valBP;
    }

    if (valHP > maxHP) {                      //Finds the maximum highpass value
      maxHP = valHP;                          
    }
    
    count++;
    delay(10);
  }
  
  if ((maxBP - maxHP) > BP_threshold) {       //Indicates that the frequency is below 100-300Hz
    return LEFT_90;

  }
  else if ((maxHP - maxBP) > HP_threshold) {  //Indicates that the frquencyt is above 300Hz
    return RIGHT_90;
  }
           
  else {
    return 0;
  }

}

/*__________Check_Black_Strip__________*/

int checkStrip(){
  int sensorState = lineFinder.readSensors();
  if (sensorState){
    switch (sensorState){
        
      case S1_IN_S2_IN:
        return 1;
        break;
        
      case S1_IN_S2_OUT: 
        return 1;
        break;
        
      case S1_OUT_S2_IN: 
        return 1;
        break;
        
      default: 
        return 0;
        break;
    }
  }
}


/*_____________Reads_Colour____________*/
void readColor(float colorArr[]) {   //Reads Colour
  float whiteArr[3];
  float blackArr[3];
  float greyDiff[3];

  whiteArr[0] = 563; //569, 566, 587
  whiteArr[1] = 381; //447, 451, 480
  whiteArr[2] = 437; //452, 463, 494
  blackArr[0] = 418;  
  blackArr[1] = 279;
  blackArr[2] = 322;
  
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

//ChecksColour
int checkColor(float colorArr[]) {

  if (colorArr[0] < 20 && colorArr[1] < 20 && colorArr[2] < 20) { //if all values small, black
    return 0;
  } 
  else {
    if (colorArr[1] > colorArr[2] && colorArr[1] > colorArr[0]) {//if green highest, green
      return RIGHT_90;
    }

  if (colorArr[0] > colorArr[1] && colorArr[0] > colorArr[2]) { //if red is highest value,
    if (abs(colorArr[1] - colorArr[2]) < 11) {                  //if green and blue similiar, red
        return LEFT_90;
      } 
    else {                                //if not, yellow
        return ABOUTTURN;
      }
    }

   if (colorArr[2] > colorArr[1] && colorArr[2] > colorArr[0]) { //if blue is highest
     if (colorArr[0] > colorArr[1]) {                              //if red higher than green, purple
       return UTURN_LEFT;
      }
      if (colorArr[1] > colorArr[0]) {                           //if green higher than red, light blue
        return UTURN_RIGHT;
      }
    }
  }
  delay(200);
}

/*play music once the maxe has ended*/
void playVictory(){
  int noteDuration,pauseBetweenNotes;
  for (int thisNote = 0; thisNote < 112; thisNote++){
    noteDuration = 1000/noteDurations[thisNote];
    buzzer.tone(melody[thisNote],noteDuration);
    pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    buzzer.noTone();
  }
}       
        
/*____________Miscellaneous__Functions_____________*/
        
//Gets average reading for the light sensor
        
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
        
//Colour calibration     
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

  

loop(){}
