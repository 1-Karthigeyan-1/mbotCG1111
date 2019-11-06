#define MICBP 4
#define MICHP 3
#define tuneTime 400

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(MICBP, INPUT);
  pinMode(MICHP, INPUT);
}

void loop(){
  float valBP = analogRead(MICBP)/1023.0*5;
  float valHP = analogRead(MICHP)/1023.0*5;
  /*Serial.print("Voltage of sumBP: "); Serial.print(valBP); Serial.print("  "); //BP
  Serial.print("Voltage of sumHP: "); Serial.print(valHP); Serial.print("  "); //BP
  Serial.println("uT");*/
  sound();
  delay(100);
}
void sound() {
  float valBP;
  float valHP;
  float maxBP = 0, maxHP = 0;
  int count = 0;
  float diff=0;
  
  
  while(count < 600){
      valBP = analogRead(MICBP)/1023.0*5;
      valHP = analogRead(MICHP)/1023.0*5;
    if (valBP > maxBP){
      maxBP = valBP;
    }

    if (valHP > maxHP){
      maxHP = valHP;
    }
    //Serial.print("Voltage of valBP: "); Serial.print(valBP); Serial.println("  "); //BP
    //Serial.print("Voltage of valHP: "); Serial.print(valHP); Serial.println("  "); //BP
    //Serial.print("Voltage of diff: "); Serial.print(maxBP - maxHP); Serial.println("  "); //BP
    count++;
    delay(10);
  }
  if((maxBP - maxHP) > 0.15){
    Serial.println("Frequency is 100 to 300!");
    
  }
  else if ((maxHP - maxBP) > 0.15){
    Serial.println("Frequency is above 3000!");
  }
  else{
    Serial.println("Your mom is gay");
  }
  Serial.print("Voltage of sumBP: "); Serial.print(maxBP); Serial.print("  "); //BP
  Serial.println("Voltage of sumHP: "); Serial.print(maxHP); Serial.print("  "); //BP
  Serial.println("Voltage of diff: "); Serial.print(maxBP - maxHP); Serial.print("  "); //BP
}
