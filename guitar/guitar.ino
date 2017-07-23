String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete
int notesInfo[2] = {0, 0};    //an array that stores a pair of notes and velocity
String note, velocity;
int t_pre[6] = {0,0,0,0,0,0};
int t_cur[6] = {0,0,0,0,0,0};

const int strip[6] = {3,5,6,9,10,11}; // pwm pins on uno

void setup() {
  Serial.begin(115200);
  for(int i=0;i<6;i++){
    pinMode(strip[i],OUTPUT);
  }

  for (int i = 0; i < 3; i++) {
    LightOn();
    delay(100);
    LightOff();
    delay(100);
  }
  
  Serial.println("Ready..");
  // reserve 200 bytes for the inputString:
  inputString.reserve(20);

}

int c[6] = {100,100,100,100,100,100};
bool light[6] = {0,0,0,0,0,0};

void loop() {

/*------------------------- Record time --------------------------*/
  for(int a=0; a<6;a++){
    t_cur[a] = millis();
  }

  /*------------------------- Parsing the signals ------------------------------*/  
    if (stringComplete) {

      notesInfo[0] = note.toInt();   //convert note numbers in midi signal to key numbers on piano keys
      notesInfo[1] = velocity.toInt();

      if(notesInfo[1] == 0){
        notesInfo[0] = 0;
      }else{
        notesInfo[0] = notesInfo[0]%6;
        light[notesInfo[0]] = true;
        c[notesInfo[0]] = 100;
      }

/*
      if (notesInfo[0] == 54 ){
        light[0] = true;
        c[0] = notesInfo[1];
      }
      if (notesInfo[0] == 56 ){
        light[1] = true;
        c[1] = notesInfo[1];
      }
      if (notesInfo[0] == 58 ){
        light[2] = true;
        c[2] = notesInfo[1];
      }
      if (notesInfo[0] == 60 ){
        light[3] = true;
        c[3] = notesInfo[1];
      }
      if (notesInfo[0] == 62 ){
        light[4] = true;
        c[4] = notesInfo[1];
      }
      if (notesInfo[0] == 64 ){
        light[5] = true;
        c[5] = notesInfo[1];
      }
*/
      Serial.println("note=" + note + "velocity=" + velocity);

      stringComplete = false;      // clear the string:

    }
 /*------------------------- Parsing the signals ------------------------------*/  

/*------------------------- Turning on/off led strips --------------------------*/
  for(int j = 0; j<6; j++){
    if (light[j] == true && t_cur[j] - t_pre[j] > 5){
      analogWrite(strip[j], c[j]);
        if(c[j] == 0 ){
          light[j] = false;
        }
        c[j]--;
        t_pre[j] = t_cur[j];  
      }
  }
/*------------------------- Turning on/off led strips --------------------------*/

}

void serialEvent(){
  while (Serial.available()) {            // get the new byte:
    char inChar = (char)Serial.read();    // if the incoming character is a newline, set a flag
    if (inChar == 'a') {
      note = inputString;
      inputString = "";
    } else if (inChar == 'b') {
      velocity = inputString;
      inputString = "";
      stringComplete = true;
    } else {
      inputString += inChar;
    }
  }
}

void LightOn(){
  for(int j=0; j<6; j++){
    digitalWrite(strip[j],HIGH);
  }
}

void LightOff(){
  for(int j=0; j<6; j++){
    digitalWrite(strip[j],LOW);
  }
}
