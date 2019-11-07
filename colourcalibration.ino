#include "MeMCore.h"
#include "Wire.h"
//in milliseconds
#define RGBWait 100
#define LDRWait 10

MeRGBLed led(7, 2);
MeLightSensor lightSensor(6);

//floats to hold colour arrays
float colourArray[] = {0,0,0};
float whiteArray[] = {0,0,0};
float blackArray[] = {0,0,0};
float greyDiff[] = {0,0,0};

void setup(){
  //begin serial communication
  Serial.begin(9600);
  start_calibration();  //calibration
}

int get_avg_reading(int times) {
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

void loop(){
  Serial.println("Put another colour");
  delay(5000);
  test_colour();
} 

void get_colour(){
  for (int c = 0; c <= 2; c++) {
      display_colour(c);
      delay(RGBWait);
      colourArray[c] = get_avg_reading(5);
      colourArray[c] = (colourArray[c] - blackArray[c]) / (greyDiff[c]) * 255;
      turn_off_LED();
      delay(RGBWait);
  }
}

void turn_off_LED()  {
  led.setColor(0, 0, 0, 0);
  led.show();
}

void display_colour(int colour) {
  switch (colour) {
    case 0:
      led.setColor(0, 255, 0, 0);
      break;
    case 1:
      led.setColor(0, 0, 255, 0);
      break;
    case 2:
      led.setColor(0, 0, 0, 255);
      break;
  }
  led.show();
}

void test_colour() {
  get_colour();
  print_RGB_value();
}

//show the value for the current colour LED, which corresponds to either the R, G or B of the RGB code
void print_RGB_value()  {
  for (int c = 0; c <= 2; c++) {
    switch (c)  {
      case 0:
        Serial.print("R: ");
        break;
      case 1:
        Serial.print("G: ");
        break;
      case 2:
        Serial.print("B: ");
        break;
    }
    Serial.println(colourArray[c]);
    check_colour();    
  }
  Serial.print("\n");

}
void start_calibration()  {
  turn_off_LED();
  float whiteArray[] = {0, 0, 0};
  //set white balance
  Serial.println("Put White Sample For Calibration ...");
  delay(5000);
  for (int i = 0; i <= 2; i++) {
    display_colour(i);
    delay(RGBWait);
    whiteArray[i] = get_avg_reading(5);
    turn_off_LED();
    delay(RGBWait);
  }

  Serial.println("Put Black Sample For Calibration ...");
  delay(5000);
  for (int i = 0; i <= 2; i++) {
    display_colour(i);
    delay(RGBWait);
    blackArray[i] = get_avg_reading(5);
    turn_off_LED();
    delay(RGBWait);
    //the difference between the maximum and the minimum gives the range
    greyDiff[i] = whiteArray[i] - blackArray[i];
  }

  Serial.println("----------Calibration Output----------");
  Serial.print("float blackArray[] = {");
  Serial.print(blackArray[0]);
  Serial.print(",");
  Serial.print(blackArray[1]);
  Serial.print(",");
  Serial.print(blackArray[2]);

  Serial.print("};\nfloat whiteArray[] = {");
  Serial.print(whiteArray[0]);
  Serial.print(",");
  Serial.print(whiteArray[1]);
  Serial.print(",");
  Serial.print(whiteArray[2]);
  Serial.println("};");
  
  Serial.print("};\nfloat greyDiff[] = {");
  Serial.print(greyDiff[0]);
  Serial.print(",");
  Serial.print(greyDiff[1]);
  Serial.print(",");
  Serial.print(greyDiff[2]);
  Serial.println("};");

  Serial.println("--------------------------------------");
}
//based on Black(212,156,161) White(556,457,476) greydiff(344,301,315)
int check_colour(){
  if (colourArray[1] > colourArray[0] && colourArray[1] > colourArray[2]){
    Serial.println("Green");
    return 1;
  }
  
  if (colourArray[0] > colourArray[1] && colourArray[0] > colourArray[2] && colourArray[1] < 100 && colourArray[2] < 100){
    Serial.println("Red");
    return 2;
  }
  
  if (colourArray[0] > colourArray[1] && colourArray[0] > colourArray[2]){
    Serial.println("Yellow");
    return 3;
  }  
  
  if (colourArray[0] > colourArray[1] && colourArray[2] > colourArray[1]){
    Serial.println("Purple");
    return 4;
  }
  
  else{
    Serial.println("Light Blue");
    return 5;
  }
}
