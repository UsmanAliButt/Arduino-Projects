#include <Wire.h>
#include "Adafruit_TCS34725.h"

/* Example code for the Adafruit TCS34725 breakout library */

/* Connect SCL    to analog 5
   Connect SDA    to analog 4
   Connect VDD    to 3.3V DC
   Connect GROUND to common ground 
*/

#define in1 10   //Motor left front
#define in2 11   //Motor right back
#define in3 12  //Motor right front
#define in4 13  //Motor left back

/* Initialise with default values (int time = 2.4ms, gain = 1x) */
// Adafruit_TCS34725 tcs = Adafruit_TCS34725();

/* Initialise with specific int time and gain values */
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_700MS, TCS34725_GAIN_1X);

int command; //Int to store app command state.
int Speed = 100; // 0 - 255.
int Speedsec;

void setup() {
  pinMode(in1, OUTPUT); //Motors as output
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  Serial.begin(9600);   //Set the baud rate 
    if (tcs.begin()) {  //Start the sensor
    Serial.println("Found sensor");
  } else {
    Serial.println("No TCS34725 found ... check your connections");
    while (1);
  }//Now we are ready to extract readings from sensor
  
}

void loop() {
  uint16_t r, g, b, c, colorTemp, lux, newcolorTemp,initialColor;
  tcs.getRawData(&r, &g, &b, &c);
  // colorTemp = tcs.calculateColorTemperature(r, g, b);
  colorTemp = tcs.calculateColorTemperature_dn40(r, g, b, c);
  lux = tcs.calculateLux(r, g, b);

  Serial.print("Color Temp: "); Serial.print(colorTemp, DEC); Serial.print(" K - ");
  Serial.print("Lux: "); Serial.print(lux, DEC); Serial.print(" - ");
  Serial.print("R: "); Serial.print(r, DEC); Serial.print(" ");
  Serial.print("G: "); Serial.print(g, DEC); Serial.print(" ");
  Serial.print("B: "); Serial.print(b, DEC); Serial.print(" ");
  Serial.print("C: "); Serial.print(c, DEC); Serial.print(" ");
  Serial.println(" ");

  //Mark the color temperature 
  newcolorTemp=colorTemp;
  
  Stop(); //Initialize with motors stoped.
  
  while(1){
  
      if(colorTemp=newcolorTemp){//Temperature same move forward
        command='F';
        Speed=100;
        }
      else if(colorTemp<newcolorTemp){//Temperature difference move left and right to adjust temperature
        command='G';
        Speed=50;
        }
      else if(colorTemp>newcolorTemp){
        command='I';
        Speed=50;
        }

      switch (command) {
        case 'F':
          forward();
          break;
        //case 'B':
          //back();
          //break;
        //case 'L':
          //left();
          //break;
        //case 'R':
          //right();
          //break;
        case 'G':
          forwardleft();
          break;
        case 'I':
          forwardright();
          break;
        //case 'H':
          //backleft();
          //break;
        //case 'J':
          //backright();
          //break;
      }
      //Get color temperature
      colorTemp = tcs.calculateColorTemperature_dn40(r, g, b, c);  
    }
  
}

void forward() {
  analogWrite(in1, Speed);
  analogWrite(in3, Speed);
}

/*void back() {
  analogWrite(in2, Speed);
  analogWrite(in4, Speed);
}

void left() {
  analogWrite(in3, Speed);
  analogWrite(in2, Speed);
}

void right() {
  analogWrite(in4, Speed);
  analogWrite(in1, Speed);
}*/

void forwardleft() {
  analogWrite(in1, Speedsec);
  analogWrite(in3, Speed);
}
void forwardright() {
  analogWrite(in1, Speed);
  analogWrite(in3, Speedsec);
}
/*void backright() {
  analogWrite(in2, Speed);
  analogWrite(in4, Speedsec);
}
void backleft() {
  analogWrite(in2, Speedsec);
  analogWrite(in4, Speed);
}*/

void Stop() {
  analogWrite(in1, 0);
  analogWrite(in2, 0);
  analogWrite(in3, 0);
  analogWrite(in4, 0);
}
