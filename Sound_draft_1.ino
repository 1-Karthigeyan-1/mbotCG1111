int checkSound()
{
  Serial.println("Checking Sound");
  float valBP;                        //Bandpass valuw
  float valHP;                        //Highpass value
  float maxBP = 0, maxHP = 0;
  int count = 0;
  float diff = 0;

  //run for around 3 seconds to sample at least the whole duration of one song cycle of each song
  int time = millis();
  while ((millis() - time) < 3000) { 
    time = millis();
    valBP = Sound.aRead1()/ 1023.0 * 5;       //Get low pass values  
    valHP = Sound.aRead2()/ 1023.0 * 5;       //Get high pass values
    
    if (valBP > maxBP) {                      //Finds the maximum bandpass value
      maxBP = valBP;
    }

    if (valHP > maxHP) {                      //Finds the maximum highpass value
      maxHP = valHP;                          
    }
    delayMicroseconds(250);                   //
   
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
