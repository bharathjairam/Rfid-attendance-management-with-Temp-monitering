#include <Wire.h>
#include <Adafruit_MLX90614.h> //temp sensor

#include <SPI.h>
#include <MFRC522.h> //rfid reader

int RST_PIN = D3;          // Configurable, see typical pin layout above D3
int SS_PIN  = D4;         // Configurable, see typical pin layout above D4
const int ProxSensor=A0;
const int buzzer = D0;

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance 
Adafruit_MLX90614 mlx = Adafruit_MLX90614();

int irsensor();
String rfid();
int state;

void setup()
 
{               
  pinMode(ProxSensor,INPUT);
  Serial.begin(9600);
  mlx.begin();

  
  Serial.begin(9600);    // Initialize serial communications with the PC
  while (!Serial);    // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
  SPI.begin();      // Init SPI bus
  mfrc522.PCD_Init();   // Init MFRC522
  delay(4);       // Optional delay. Some board do need more time after init to be ready, see Readme
  mfrc522.PCD_DumpVersionToSerial();  // Show details of PCD - MFRC522 Card Reader details
  Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));

  pinMode(D0,OUTPUT);
  
}

float temp;



void loop() 
{

 if (irsensor() == 1)
 
 {
  Serial.print("temp = ");
  Serial.print(temp);
  Serial.print("F");
  Serial.println();
  String rfidkey = rfid();
  
  if (rfidkey=="")
  {
    Serial.print("data not available");
  }
   else
   {
    Serial.print(rfidkey);
     tone(buzzer, 1000); // Send 1KHz sound signal...
    delay(1000);        // ...for 1 sec
    noTone(buzzer);     // Stop sound...
    delay(5000);
    tone(buzzer, 100); // Send 1KHz sound signal...
    delay(1000);        // ...for 1 sec
    noTone(buzzer);     // Stop sound...
    delay(1000);
    }
  
 
  }

}





int irsensor() 

{
  int state = analogRead(ProxSensor);
  if(state <= 500)
   
  { 
    temp = mlx.readAmbientTempF();
    Serial.println();
    delay(1000); 
    state;
    return(1);  
  }
  else
  {
    Serial.println("Nobody is present");
    delay(1000);
    return(0);
  }
   
  
  
}



String rfid() {
  
  if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial())
    return("");
  // Serial.print(F("PICC type: "));
  MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
  // Serial.println(mfrc522.PICC_GetTypeName(piccType));

  // Check is the PICC of Classic MIFARE type
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&
    piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
    piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    Serial.println(F("Your tag is not of type MIFARE Classic."));
    return("");
  }
  
  String strID = "";
  for (byte rfi = 0; rfi < 4; rfi++) {
    strID +=
    (mfrc522.uid.uidByte[rfi] < 0x10 ? "0" : "") +
    String(mfrc522.uid.uidByte[rfi], HEX) +
    (rfi!=3 ? "" : "");
  }
  strID.toUpperCase();
Serial.println("place the card");

 Serial.print("Tap card rfkey: ");
  return(strID);

}
