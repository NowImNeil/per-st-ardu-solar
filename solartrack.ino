
#include <Servo.h>
#include <LiquidCrystal_I2C.h> // includes the LiquidCrystal_I2C Library
#include <Wire.h>
#include <BH1750.h>

#define photosF A3
#define photosB A2

LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display
BH1750 lightMeter;
  float minLux = 100;

Servo serY;
  int pinSerY = 10;
  int initAngleY = 60;
  int limitAngleY = 0;
  int limitAngleYd = 60;
  int newAngle = 0;
Servo serX;
  int pinSerX = 9;
  int initAngleX = 0;
  int limitAngleX = 180;
  int newAngleX;



int movQY = 5;
int movQX = 10;

boolean isRotated = false;

volatile int fValue = 0;
volatile int bValue = 0;

String lightDir;

// put your setup code here, to run once:
void setup() {
  
  lcd.init();
     lcd.backlight(); //set backlight
  lcd.setCursor(0,0);
  Serial.begin(9600);
  Wire.begin();
  //Wire.begin(D2, D1);
  lightMeter.begin();

  serY.attach(pinSerY);
  serX.attach(pinSerX);

  serX.write(initAngleX);
  serY.write(initAngleY);
   


  delay(500);
}

void loop() {
  
  float lux = lightMeter.readLightLevel();
  fValue = analogRead(photosF);
  bValue = analogRead(photosB);
 
// Serial.println(getLightDif(fValue, bValue));
 
  if (lux < minLux ){
        Serial.println("move!");

    if (serY.read() != limitAngleY && !isRotated) {
        

        writeAngleY(limitAngleY, limitAngleYd, movQY, isRotated);

    } else if (serY.read() == limitAngleY && isRotated) {
 
      
        writeAngleY(limitAngleY, limitAngleYd, movQY, isRotated);

    } else if (serY.read() == limitAngleY && !isRotated && serX.read() != limitAngleX)  {
       
       
        
        writeAngleX(limitAngleX, movQX);

  }
  }
  
  Serial.println(serX.read());
  Serial.println(serY.read());

  
  Serial.print("Light: ");
  Serial.print(lux);
  Serial.println(" lx");
 
}

String getLightDir(int front, int back){
   if (bValue > fValue) {
    //Serial.println("Back Light");
    return "back";
  } else if (fValue > bValue){
    //Serial.println("Front Light");
    return "front";
  }
}

int getLightDif(int front, int back){
  int lightDif = front - back;
  return lightDif;
}




void writeAngleY(int limitAngleUp, int limitAngleDown, int movQ, boolean isRotated){
 
  
  int currentAngle = serY.read();
  int newAngle;

 if (!isRotated) {
  if (currentAngle > limitAngleUp){
    for (int i = currentAngle; i > limitAngleUp; i--){

     newAngle = currentAngle - movQ;
     serY.write(newAngle);
     delay(15);
    }
  } 
 } else if (isRotated == true) {
   if (currentAngle < limitAngleDown){
   for (int j = currentAngle; j < limitAngleDown; j++){
      newAngle = currentAngle + movQ;
      serY.write(newAngle);
      Serial.println("READING");
      delay(15);
   }
  }
 }
  
}




void writeAngleX(int limitAngle, int movQ){
 
 int currentAngle = serX.read();
 int newAngle;

 if (currentAngle < limitAngle){
   for (int i = currentAngle; i < limitAngle; i++){

    newAngle = currentAngle + movQ;
    serX.write(newAngle);
    
    if (currentAngle == (limitAngle - 1)){
      isRotated = true;
      Serial.println("true");
    }

    delay(7);
   }
 } 
}



