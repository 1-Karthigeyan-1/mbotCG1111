//Define pins


//Define robot states
#define LEFT 1
#define RIGHT 2
#define ABOUTTURN 3
#define REORIENTATE -1
#define STOP 0
#define STRAIGHT 4

//Color code states, may not be needed
#define BLACK -1
#define RED 1
#define GREEN 2
#define LIGHTBLUE 3
#define PURPLE 4
#define YELLOW 5
#define NOCOLOR 0



void setup()
{
  //Initialize pins
  executeRobot();
}

void executeRobot()
{
  //Boolean flag 
  int victory = 0;
  movement(STRAIGHT):      
  while (victory == 0)
  {
    if ((checkFront() != STRAIGHT) || (checkSide() != STRAIGHT))
    {
      movement(REORIENTATE);
    }

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
      movement(checkSound);
      return 0;
    }
    else if(checkColor() == NOCOLOR)
    {
      return 1;
    }
    else
    {
      movement(checkColor);
    }
  }
}

int checkStrip()
{
  //Line sensor checks for black strip.
}

void movement(int state)
{
  switch(state)
  {
    case FRONT:
    break;
    case LEFT: 
    break;
    case RIGHT:
    break;
    case ABOUTTURN:
    break;
    case STOP:
    break; 
    case STRAIGHT:
    break;
  }
}

int checkStraight()
{
  if(checkIR == STRAIGHT)
  {
    return 1;
  }
  else
  {
    while(checkIR != STRAIGHT) 
  }
}

int checkColor()
{
  //Detects the color of the board above
  //If it is black, return soundChallenge flag. Robot remains stationary.
  //If it is of any other color, robot moves according to the color.
  //If color detected is neither of the colors, proceed to play victory tune.
}
