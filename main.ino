
/* www.learningbuz.com */
/*Impport following Libraries*/
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         9           // Configurable, see typical pin layout above
#define SS_PIN          10          // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance

Servo myservo;

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); 

int ir1=2;
int ir2=4;
int servo=3;
int ledr=5;
int ledg=6;
String light_status="red";
int timer=5;
unsigned long times = 0;


void setup() 
{
Serial.begin(9600);
lcd.begin(16,2);
lcd.backlight();
myservo.attach(servo);
pinMode(ledr,OUTPUT);
pinMode(ledg,OUTPUT);
pinMode(ir1,INPUT);
pinMode(ir2,INPUT);
SPI.begin();               // Init SPI bus
mfrc522.PCD_Init();        // Init MFRC522 card



}

void loop() 
{
  
  bool data=RFID();
  if (data==true){
    light_status="green";
    myservo.write(0);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Emergency"); 
    lcd.setCursor(0,1);
    lcd.print("Vehicle"); 
    digitalWrite(ledg,1);
    digitalWrite(ledr,0);
    delay(10000);
    light_status="red";
    digitalWrite(ledg,0);
    digitalWrite(ledr,1);
    lcd.clear();
    
  }
  int IR1=digitalRead(ir1);
  int IR2=digitalRead(ir2);
  if(light_status=="red"){
    digitalWrite(ledr,1);
    digitalWrite(ledg,0);
    myservo.write(90);
    
    if(IR1==0 && IR2==0){
      timer=15;
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Traffic"); 
      lcd.setCursor(0,1);
      lcd.print("Density=Medium"); 
      
    }
    else if(IR1==0 && IR2==1){
      timer=10;
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Traffic"); 
      lcd.setCursor(0,1);
      lcd.print("Density=Low"); 
    }
    else if(IR1==1 && IR2==1){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("No Traffic"); 
    }
  }
  unsigned long currentMillis = millis();
  if (currentMillis - times >= 10000) {
    digitalWrite(ledr,0);
    digitalWrite(ledg,1);
    myservo.write(0);
    
    lcd.clear();
    for(int i=timer;i>=0;i--){
      lcd.setCursor(0,0);
      lcd.print("Timer: "+String(i));
      delay(1000);
      lcd.clear();
    
    }
    unsigned long currentMillis = millis();
    times = currentMillis; 
  }

  
  

  

}

bool RFID(){
  MFRC522::MIFARE_Key key;
  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;

  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return false;
  }

  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return false;
  }

  return true;
  
}

