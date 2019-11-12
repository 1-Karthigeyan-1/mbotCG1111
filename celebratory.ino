#include <MeMCore.h>

#define NOTE_C4  262
#define NOTE_D4  294
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_G4  392


MeBuzzer buzzer;
void setup() {
  // put your setup code here, to run once:

}

void loop() {
  play();//Play the music.
  delay(300);//Pause for a while.
}

int melody[] = {
  NOTE_G4,NOTE_E4,NOTE_E4,
  NOTE_F4,NOTE_D4,NOTE_D4,
  NOTE_C4,NOTE_D4,NOTE_E4,NOTE_F4,
  NOTE_G4,NOTE_G4,NOTE_G4, //
  NOTE_G4,NOTE_E4,NOTE_E4,
  NOTE_F4,NOTE_D4,NOTE_D4,
  NOTE_C4,NOTE_E4,NOTE_G4,NOTE_G4,
  NOTE_E4, //
  NOTE_D4,NOTE_D4,NOTE_D4,NOTE_D4,
  NOTE_D4,NOTE_E4,NOTE_F4,
  NOTE_E4,NOTE_E4,NOTE_E4,NOTE_E4,
  NOTE_E4,NOTE_F4,NOTE_G4,//
  NOTE_G4,NOTE_E4,NOTE_E4,
  NOTE_F4,NOTE_D4,NOTE_D4,
  NOTE_C4,NOTE_E4,NOTE_G4,NOTE_G4,
  NOTE_C4
  
};

int noteDurations[] = {
  4,4,2,
  4,4,2,
  4,4,4,4,
  4,4,2, //
  4,4,2,
  4,4,2,
  4,4,4,4,
  1, //
  4,4,4,4,
  4,4,2,
  4,4,4,4,
  4,4,2, //
  4,4,2,
  4,4,2,
  4,4,4,4,
  1
  
};

void play()
{
    for (int thisNote = 0; thisNote < 49; thisNote++) {

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
