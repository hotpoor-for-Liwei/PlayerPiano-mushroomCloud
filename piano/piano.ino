#include<Servo.h>

Servo servos[33];
const int servo_pin[] = {21,22,23,24,25,26,27,28,29,30,31,
                         32,33,34,35,36,37,38,39,40,41,42,
                         43,44,45,46,47,48,49,50,51,52,53};

String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete
int notesInfo[2] = {0, 0};    //an array that stores a pair of notes and velocity
String note, velocity;

void setup() {
  Serial.begin(115200);

  for(int i=0; i<33; i++){
    servos[i].attach(servo_pin[i]);
    servos[i].write(60);
    delay(100);  
    servos[i].write(0);
    delay(100);  
  }

  Serial.println("Ready..");
  // reserve 200 bytes for the inputString:
  inputString.reserve(20);

}
int c[33];
bool light[33];
int t_pre[33];
int t_cur[33];

void loop() {
    
  for(int a=0; a<33;a++){
    t_cur[a] = millis();
  }
  /*------------------------- Parsing the signals ------------------------------*/    
  if (stringComplete) {
    notesInfo[0] = note.toInt();   //convert note numbers in midi signal to key numbers on piano keys
    notesInfo[1] = velocity.toInt();

    if(notesInfo[1] == 0){
      notesInfo[0] = 0;
    }else{
      if(notesInfo[0] <= 66 && notesInfo[0] >= 33){
        light[notesInfo[0]-33] = true;
        c[notesInfo[0]-33] = 100;
      }
    }

    stringComplete = false;
    // clear the string:
    Serial.println(note + "\t" + velocity);  
  }
  /*------------------------- Parsing the signals ------------------------------*/  

  /*------------------------- Turning the servos -------------------------------*/
  for(int j = 0; j<33; j++){
    if (light[j] == true){
        servos[j].write(60);
        if( t_cur[j] - t_pre[j] > 200 ){
            light[j] = false;
        }
    }else{
       servos[j].write(0);
       t_pre[j] = t_cur[j];  
    }

  }

  /*------------------------- Turning the servos -------------------------------*/

}
void serialEvent() {
  while (Serial.available()) {            // get the new byte:
    char inChar = (char)Serial.read();    // if the incoming character is a newline, set a flag
    if (inChar == 'a') {
      note = inputString;
      inputString = "";
    } else if (inChar == 'b') {
      velocity = inputString;
      inputString = "";
      stringComplete = true;
    }else{
      inputString += inChar;
    }
  }
}



