//Libraries Used.
#include <MeMCore.h> 

//Define Movement States
#define LEFT_90 1                 //90 degree left turn
#define RIGHT_90 2                //90 degree right turn 
#define ABOUTTURN 3               //180 degree turn on the spot
#define UTURN_LEFT 4              //Uturns by using 2 left 90-degree turns over 2 grids
#define UTURN_RIGHT 5             //Uturns by using 2 right 90-degree turns over 2 grids
#define STOP 0                    //Stops moving
#define STRAIGHT 6                //Moves straight
#define LEFT_ADJUST 7             //Adjust to the left side in a straight pathway
#define RIGHT_ADJUST 8            //Adjust to the right side in a straight pathway

//Miscellaneous definitions
#define USDIST 6                  //UltraSonic sensor distance                 
#define TIME 630                  //time delay for turning 90 degrees
#define LDRWait 300               //Response time of the LDR
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
void executeRobot(float [], int);        
void movement(int);                       
void movement_uturn_right();
void movement_uturn_left();
void movement_straight_nc(); 
int check_side(); 
int checkChallenge();                  
int checkSound();                      
int checkStrip();                                        
void readColor();                       
int checkColor(float colorArr[]);       
void colorCalibrate();                 
void playVictory();                    
int getAvr(int);
void colorCalibrate();    
void loop();


void setup(){
  //Initialzation
  float colorArr[3] = {0};                 //Colour values are stored in an array
  int Victory = 0;                         //Victory is state of maze                                             
  
  executeRobot(Victory);                   //The function is put here instead of loop() so that the executed run can end.
}


/*________________Main_Function______________*/
// Recursive function that takes in the int Victory boolean flag value as the parameter
// And calls upon itself repeatedly until the termination condition is fulfilled
// whereby int Victory = 1.

void executeRobot(int Victory){
  float colorArr[3] = {0};                 
  
  if (Victory == 0){                       //If boolean flag for end of run reads false
    
    movement_straight_nc();                //mBOT will move straight in a while loop, with upon termination executes the next statement.
    movement(STOP);                        
    
    if (checkChallenge(colorArr)){         //If the end-maze condition is fulfilled after executing the challenges in checkChallenge()
      Victory = 1;                         //Set boolean flag for end of run to positive
    }
    
    return executeRobot(Victory);         //Calls upon itself as long as boolean flag reads negative to such that next iteration of movement can be executed
  }
  
  else{                                   //Else if boolean flag reads negative and the maze has ended    
    movement(STOP);                       //Robot to stop moving
    playVictory();                        //Plays victory tune
  }
  
}


/*____________Movement_Functions__________________*/
//Repository function of possible challenge moves for the mBOT
//to execute at a challenge waypoint. 
//Can be called upon throughout different points along the code
//to execute movement of the robot.

void movement(int state){
  int time = millis();                  //Set int time to current run time of the code 
  switch (state){                       //Executes different movements depending on predefined constants in function parameter
    
    //Turn left 90 degress
    case LEFT_90:                       
      do{
        motor1.run(100);
        motor2.run(100);
      }while((millis() - time) <= TIME);  //To ensure that the time taken for the turn is as precise as possible. 
      break;
    
    //Turn Right 90 degrees
    case RIGHT_90:
      do{
        motor1.run(-100);
        motor2.run(-100);
      }while((millis() - time) <= TIME);
      break;
      
    //Turns 180 degrees on the spot  
    case ABOUTTURN:
      do{
        motor1.run(200);                  //The robot executes an about turn by doubling motor speed of turn for the same amount of time.
        motor2.run(200);
      }while((millis() - time) <= TIME);
      break;
      
    //Stop
    case STOP: 
      motor1.stop();
      motor2.stop();
      break;
    
    //2 successive left turns over 2 grids    
    case UTURN_LEFT:
      movement_uturn_left();             //Called upon as a secondary function due to relative complexity of the code
      break;
        
    //2 successive right turns over 2 grids 
    case UTURN_RIGHT: 
      movement_uturn_right();
      break;
        
    //Moves straight  
    case STRAIGHT:
      motor1.run(-100);
      motor2.run(100);
      break;
        
    //Adjust to the right    
    case RIGHT_ADJUST:                  //LEFT_ADJUST and RIGHT_ADJUST are executed in movement_straight_nc() to reorientate the mBOT to the center of the track.
      motor1.run(-100);
      motor2.run(40);
      break;
    
     //Adjust to the left    
    case LEFT_ADJUST:
      motor1.run(-60);
      motor2.run(100);
      break;

  }
}
  

//Does 2 successive 90 degree right turns over 2 grids        
void movement_uturn_right() {
  do{
    movement(RIGHT_90);                               //Executes the first turn
  }while((millis() - time) <= 20);                    
    movement(STOP);                             
  while (ultraSensor.distanceCm() > USDIST) {         //While distance between mBOT and the wall is greater than threshold value
    motor1.run(-100);                                 //mBOT will move straight towards the front wall
    motor2.run(100);
    movement(check_side());                           //mBOT movement will be corrected by side IR proximity sensors
  }
    movement(STOP);                                   //When distance is equal to threshold distance, the mBOT will stop before collision
  
  do{                                                 //mBOT executes the second right turn. Thereafter, the next iteration of the executeRobot() will be called upon and it will move straight.
    movement(RIGHT_90);         
  }while((millis() - time) <= 20);
  
}


//Does 2 successive 90 degree leftturns over 2 grids        
void movement_uturn_left() {
  do{
    movement(LEFT_90);
  }while((millis() - time) <= 20);
    movement(STOP);
  
  while (ultraSensor.distanceCm() > USDIST) {       //Detects for wall
    motor1.run(-100);
    motor2.run(100);
    movement(check_side());
  }
    movement(STOP);
  
  do{
    movement(LEFT_90);
  }while((millis() - time) <= 20);
}


//mBOT moves straight while checking for strip to execute checkChallenge()
//If black strip is detected, while loop is terminated 
void movement_straight_nc() {  
  while(checkStrip() != 1){                   //checks for black strips along the way.
     movement(check_side());                  //Chwcks the sides and adjust accordingly
     delay(25);
  }
}


/*__________________IR sensor________________*/
//Function to reorientate the mBOT throughout its movement
//Such that it remains at the center of the track with minimal deviation from the front 
//in its direction of travel.

int check_side(){
  float Leftval;                               //Left IR
  float Rightval;                              //Right IR
  
  Leftval = IR.aRead1()/1023.0*15;         
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
//Executes the appropriate actions of the robot depending
//on the stimulus detected at the challenge waypoint.

int checkChallenge(float colorArr[]) {
  int color, sound;
  
  readColor(colorArr);                    //Reads colour , and return its values  
  color = checkColor(colorArr);           //Checks what colour category the values are in 
  
  if (color == 0){                        //If the colour is black , check for sound                    
    sound = checkSound();
    
    if (sound == 0){                      //If no music is detected
      return 1;                           //In executeRobot(), the boolean flag int Victory will be set to positive
    }
    
    else{
      movement(sound);                    //Moves accordingly with type of sound                 
    }
  }
  
  else{                                   //If color is not black
    movement(color);                      //Moves according to the color detected
  }
  return 0;                               //In executeRobot(), the boolean flag int Victory will be set to negative
}



/*_________Sound_checking_____________*/
//Compares output voltages from the active band pass filter and active high pass filter
//Against their respective threshold values
//And returns the appropriate integer value that corresponds with the correct direction of turn
//depending on frequency of audio played.

int checkSound()
{
  Serial.println("Checking Sound");
  float valBP;                        //Bandpass valuw
  float valHP;                        //Highpass value
  float maxBP = 0, maxHP = 0;         
  float diff = 0;
  int time = millis();

  while (millis() - time < 3000) {            //Run for around 3 seconds to sample at least the whole duration of one song cycle of each song
    
    valBP = Sound.aRead1()/ 1023.0 * 5;       //Get band pass values  
    valHP = Sound.aRead2()/ 1023.0 * 5;       //Get high pass values
    
    if (valBP > maxBP) {                      //Finds the maximum bandpass value
      maxBP = valBP;
    }

    if (valHP > maxHP) {                      //Finds the maximum highpass value
      maxHP = valHP;                          
    }
    
  }
  
  if ((maxBP - maxHP) > BP_threshold) {       //Indicates that the frequency is below 100-300Hz
    return LEFT_90;

  }
  else if ((maxHP - maxBP) > HP_threshold) {  //Indicates that the frquencyt is above 300Hz
    return RIGHT_90;
  }
           
  else {
    return 0;                                 //If there is no music, victory tune will proceed to play
  }

}

/*__________Check_Black_Strip__________*/
//Checks for presence of black strip
//Such that challenge will be detected
//As long as one of the 2 IR detects the black strip
//Challenge will be considered as detected.
//This circumvents the case whereby the robot approaches the strip at an angle
//and consequently only one of the IR sensors detects the black strip

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
//Detects the intensity of reflected light for each color of LED, namely red, green and blue
//Thereafter, the RGB values are calibrated with respect to calibration values 

void readColor(float colorArr[]) {   //Reads Colour
  float whiteArr[3];
  float blackArr[3];
  float greyDiff[3];

  whiteArr[0] = 563;                //Calibration values based on null conditions whereby no color is shown
  whiteArr[1] = 381; 
  whiteArr[2] = 437; 
  blackArr[0] = 418;  
  blackArr[1] = 279;
  blackArr[2] = 322;
  
  int i;
  for (i = 0; i < 3; i++) {         //Acquires the calibrated range of RGB values that accounts for surrounding lighting.
    greyDiff[i] = whiteArr[i] - blackArr[i];
  }

  led.setColor(0, 255, 0, 0);       //For each color out of red, green and blue, the two embedded LEDs display the colour to determine intensity of reflected light. Last three parameters indicate the RGB value of light to be shone.
  led.show();                       //LED will to emit light of the corresponding color 
  delay(LDRWait);                       //To account for the response time of the LDR
  colorArr[0] = ( (getAvr(7) - blackArr[0]) / greyDiff[0] ) * 255;  //The array will the contain the correctly calibrated RGB values.
  led.setColor(0, 0, 255, 0);
  led.show();
  delay(LDRWait);
  colorArr[1] = ( (getAvr(7) - blackArr[1]) / greyDiff[1] ) * 255;
  led.setColor(0, 0, 0, 255);
  led.show();
  delay(LDRWait);
  colorArr[2] = ( (getAvr(7) - blackArr[2]) / greyDiff[2] ) * 255;
  led.setColor(0, 0, 0, 0);          //Turn off the LED.
  led.show();
}

//The acquired RGB values are compared against one another to determine the color of the board.
int checkColor(float colorArr[]) {

  if (colorArr[0] < 20 && colorArr[1] < 20 && colorArr[2] < 20) {   //if all values small, black
    return 0;
  } 
  else {
    if (colorArr[1] > colorArr[2] && colorArr[1] > colorArr[0]) {   //if green highest, green
      return RIGHT_90;
    }

  if (colorArr[0] > colorArr[1] && colorArr[0] > colorArr[2]) {     //if red is highest value,
    if (abs(colorArr[1] - colorArr[2]) < 11) {                      //if green and blue similiar, red
        return LEFT_90;
      } 
    else {                                                          //if not, yellow
        return ABOUTTURN;
      }
    }

   if (colorArr[2] > colorArr[1] && colorArr[2] > colorArr[0]) {   //if blue is highest
     if (colorArr[0] > colorArr[1]) {                              //if red higher than green, purple
       return UTURN_LEFT;
      }
      if (colorArr[1] > colorArr[0]) {                             //if green higher than red, light blue
        return UTURN_RIGHT;
      }
    }
  }
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
//Such that RGB values used are more reliable
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
        
//Colour calibration to acquire RGB values of white and black
//Under room lighting condition
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
