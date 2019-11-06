/*************************************
Progress Tracker:
1. Skeleton code
2. Movement direction 3/5

Find your functions are code on a seperate file your function 
with the appropriate return values
**************************************/


#include <MeMCore.h>

//Define robot states
#define LEFT 1
#define RIGHT 2
#define ABOUTTURN 3
#define REORIENTATE -1
#define STOP 0
#define STRAIGHT 4

//Color code states, may not be needed
#define USDIST 300

//Define ports
MeLineFollower lineFinder(PORT_3); /* Line Finder module can only be connected to PORT_3, PORT_4, PORT_5, PORT_6 of base shield. */
MeDCMotor motor1(M1);
MeDCMotor motor2(M2);
MeUltrasonicSensor ultraSensor(PORT_1);

//Define pins

//Function prototypes



void setup()
{
  //Initialize pins
  executeRobot();
}


void executeRobot()
{
  //Boolean flag 
  int victory = 0;     
  while (victory == 0)
  {
    movement(STRAIGHT):
    
    if ((checkStrip == 1)
    {
      movement(STOP);
        if (checkChallenge() == 1)
        {
          victory = 1;
        }
        else
        {
          movement(REORIENTATE);
        }
      }
      else
      {
        movement(REORIENTATE);
      }
    }
    
    if (victory == 1)
    {
      exit(0);
    }
  }
  playVictory();
}

//Use ultrasonic sensor to check for front collision
int checkFront()
{
  //If ultrasound sensor sensors a sufficiently short distance, it means the robot is either meeting a challenge or is approaching a wall in a collision.
  //If distance is greater than d, Ultrasound returns 1.
  //Else, Ultrasound returns 0.
}

//Use IR sensor to check for side collision
int checkSide()
{
  //IR sensors to check if the robot is equidistant to two parallel sides of the walls.
  //A left negotiation is represented by a positive value, a right negotiation is represented by a negative value.
  //If the robot is moving straight, the function should return 0.
}


int checkChallenge()
{
  //Line sensor checks for black strip
  if(checkStrip() == 1)
  {
    //Checks for the type of challenge in place.
    if(checkColor() == BLACK)
    {
      if (checkSound() == 0);
      {
        return 1;
      }  
      else 
      {
        movement(checkSound());
      }
    }
    else
    {
      movement(checkColor);
    }
  }
  return 0;
}


int checkSound()
{
  if ()// Frequency is between 100 to 300 Hz
  {
    return LEFT;
  }

  else if()// Frequency is above 3000 Hz
  {
    return RIGHT
  }

  else
  {
    return 0;
  }
}


int checkStrip()
{
  //Line sensor checks for black strip. 
  int sensorState = lineFinder.readSensors();
  if (sensorState)
  {
    switch(sensorState)
    {
      case S1_IN_S2_IN: 
      case S1_IN_S2_OUT: 
      case S1_OUT_S2_IN: return 1;               
      break;
      default: return 0;
      break;
    }
}


void movement(int state)
{
  switch(state)
  {
    case LEFT: motor1.run(100);
               motor2.run(100);
    break;
    case RIGHT: motor1.run(-100);
                motor2.run(-100);
    break;
    case ABOUTTURN: motor1.run(200);
                    motor2.run(200);
    break;
    case STOP: motor1.stop();
               motor2.stop();
    break; 
    case STRAIGHT: motor1.run(-100);  //Incorporate IR and US here!
                   motor2.run(100);
    break;
  }
}


int checkStraight()
{
  if(checkIR == STRAIGHT)
  {
    return STRAIGHT;
  }
  else
  {
    while(checkIR != STRAIGHT) 
    {
      
    }
  }
}


int checkColor()
{
  //Detects the color of the board above
  //If it is black, return soundChallenge flag. Robot remains stationary.
  //If it is of any other color, robot moves according to the color.
  //If color detected is neither of the colors, proceed to return false flag
}





