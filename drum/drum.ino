#include <Adafruit_NeoPixel.h>

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:

Adafruit_NeoPixel strip[5] = {Adafruit_NeoPixel(47, 41, NEO_GRB + NEO_KHZ800),Adafruit_NeoPixel(47, 42, NEO_GRB + NEO_KHZ800),
Adafruit_NeoPixel(56, 43, NEO_GRB + NEO_KHZ800),Adafruit_NeoPixel(47, 44, NEO_GRB + NEO_KHZ800),Adafruit_NeoPixel(47, 45, NEO_GRB + NEO_KHZ800)};

String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete
int notesInfo[2] = {0, 0};    //an array that stores a pair of notes and velocity
String note, velocity;
int t_pre[7] = {0,0,0,0,0,0,0};
int t_cur[7] = {0,0,0,0,0,0,0};

void setup() {
  Serial.begin(115200);
   for(int i=0;i<5;i++){
    strip[i].begin();
    strip[i].show(); // Initialize all pixels to 'off'
   }

  LightOff();

  for (int i = 0; i < 5; i++) {
    LightOn();
    delay(100);
    LightOff();
    delay(100);
  }

  Serial.println("Ready..");
  // reserve 200 bytes for the inputString:
  inputString.reserve(20);

}

int c[7] = {100,100,100,100,100,100,100};
bool light[7] = {0,0,0,0,0,0,0};

void loop() {

/*------------------------- Record time --------------------------*/
  for(int a=0; a<5;a++){
    t_cur[a] = millis();
  }

/*------------------------- Parsing the signals ------------------------------*/  
    if (stringComplete) {

      notesInfo[0] = note.toInt();   //convert note numbers in midi signal to key numbers on piano keys
      notesInfo[1] = velocity.toInt();

      if(notesInfo[1] == 0){
        notesInfo[0] = 0;
      }else{
        notesInfo[0] = notesInfo[0]%5;
        light[notesInfo[0]] = true;
        c[notesInfo[0]] = 100;
      }

    Serial.println(note + "\t" + velocity);  

      stringComplete = false;      // clear the string:

    }
/*------------------------- Parsing the signals ------------------------------*/  


/*------------------------- Turning on/off led strips --------------------------*/
  for(int j = 0; j<5; j++){
    if (light[j] == true && t_cur[j] - t_pre[j] > 2){
        for (int i = 0; i < strip[j].numPixels(); i++) {
          strip[j].setPixelColor(i, strip[j].Color(c[j], c[j], c[j]));
        } 
        
        strip[j].show();
        if(c[j] == 0 ){
          light[j] = false;
        }
        c[j]--;
        t_pre[j] = t_cur[j];  
      }
  }
/*------------------------- Turning on/off led strips --------------------------*/

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
    } else {
      inputString += inChar;
    }
  }
}

void LightOn() {
  for(int j=0;j<5;j++){
    for (uint16_t i = 0; i < strip[j].numPixels(); i++) {
      strip[j].setPixelColor(i, strip[j].Color(100, 100, 100));
    }
    strip[j].show();
  }
}

void LightOff() {
  for(int j=0;j<5;j++){
    for (uint16_t i = 0; i < strip[j].numPixels(); i++) {
      strip[j].setPixelColor(i, strip[j].Color(0, 0, 0));
    }
    strip[j].show();
  }
}
