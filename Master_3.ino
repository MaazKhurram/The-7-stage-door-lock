#include <Keypad.h>
#include <Password.h>
#include <Servo.h>
#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
#include <SPI.h>
#include <RFID.h>








Servo myservo; //declares servo



Password password = Password( "711999" ); //password to unlock, can be changed
const byte ROWS = 4; // Four rows
const byte COLS = 3; // columns
char keys[ROWS][COLS] = {     // Define the Keymap
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};
// Connect keypad ROW0, ROW1, ROW2 and ROW3 to these Arduino pins.
byte rowPins[ROWS] = { 37, 39, 41, 43 }; // Connect keypad COL0, COL1 and COL2 to these Arduino pins.
byte colPins[COLS] = { 45, 47, 49 };
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );// Create the Keypad

int x = 0; //variable for keypad
int keypadCheck()  //function for keypad
{
  x = x + 1;
  return x;
}


LiquidCrystal lcd(8, 9, 4, 5, 6 , 7);


int getFingerprintIDez();
SoftwareSerial mySerial(A15, A14); //( yellow,  green)
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

RFID rfid(22, 24);







void setup() {

  Serial.begin(9600);
  myservo.attach(23);
  lcd.begin(16, 2);

  SPI.begin();
  rfid.init();


  keypad.addEventListener (keypadEvent); //add an event listener for this keypad

  // set the data rate for the sensor serial port
  finger.begin(57600);

  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");



  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1);
  }



}




//====================================================================================================================

void loop() {

  Serial.println("Starting RFID");


  bool flag = false; // for rfid

  do
  {
    if (rfid.isCard()) {

      static int counterRFID = 0;

      if (rfid.readCardSerial()) { 
        //this will show the 16 digits of the card
        counterRFID++;

        if (counterRFID >= 3)
        {

          {
            Serial.println("PROGRAM HANGED.TRY AFTER 30 SECONDS...");
            for (int i = 0; i < 10; i++)
            {
              delay(3000);
            }
          }
          counterRFID=0;

        }
        Serial.println(" ");
        Serial.println("the serial number of the card is: ");
        Serial.print(rfid.serNum[0], DEC);
        Serial.print(" , ");
        Serial.print(rfid.serNum[1], DEC);
        Serial.print(" , ");
        Serial.print(rfid.serNum[2], DEC);
        Serial.print(" , ");
        Serial.print(rfid.serNum[3], DEC);
        Serial.print(" , ");
        Serial.print(rfid.serNum[4], DEC);
        Serial.println(" ");

        if (rfid.serNum[0] == 228 && rfid.serNum[1] == 185 && rfid.serNum[2] == 59 && rfid.serNum[3] == 82 && rfid.serNum[4] == 52) { //"29" is the first part of the 5 sets of digits from the card
          Serial.println("Card Recognized...");
          delay(10);
          flag = true;


        }

        else {
          Serial.println("Invalid RFID ...");
          delay(1000);
          flag = false;

        }
      }
      rfid.halt();

    }
  }
  while (flag == false);




  Serial.println("Starting keypad..");
  do {

    keypad.getKey();


  }
  while (x != 1);








  Serial.println("Starting fingerprint");

  bool flag3 = false;


  int ID;

  do {
    //Serial.println("Initialing fingerprint test");
    ID = getFingerprintIDez();

    if (ID == (1)) // correct
    {
      Serial.println("Giving control to the servo...");

    }

    else if (ID == (2)) //wrong
    {
      Serial.println("Unidentified finger");
      delay(100);
    }

    else if (ID == (-1)) //wrong
    {
      delay(100);
    }


  } while (ID != 1);




  Serial.println("Opening the Door");
  myservo.write(10);
  delay(1000);
  myservo.write(10);
  delay(1000);//open door lock
  //once open return to state 0 and relock door


}









//======================================================================================================================

uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("No finger detected");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK success!

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK converted!
  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println("Found a print match!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("Did not find a match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }

  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID);
  Serial.print(" with confidence of "); Serial.println(finger.confidence);
}

// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return 2;

  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID);
  Serial.print(" with confidence of "); Serial.println(finger.confidence);
  return finger.fingerID;
}


void keypadEvent(KeypadEvent eKey) {

  bool flag = false;
  switch (keypad.getState()) {
    case PRESSED:

      if (eKey != '#') {


        Serial.print("Enter:");
        Serial.println(eKey);
      }

      delay(10);

      Serial.write(254);

      switch (eKey) {
        case '#':
          {
            checkPassword();
            delay(1);

            break;
          }

        case '*':
          {
            password.reset();
            delay(1);
            Serial.println("Password is cleared. Enter again.");
            break;
          }

        default: password.append(eKey); delay(1);
      }
  }
}


void checkPassword() {

  static int counterKeypad=0;
  counterKeypad++;
  
  if (counterKeypad>3)
  {
     {
            Serial.println("PROGRAM HANGED.TRY AFTER 30 SECONDS...");
            for (int i = 0; i < 10; i++)
            {
              delay(3000);
            }
          }
          counterKeypad=0;
  }

  if (password.evaluate()) { //if password is right open

    Serial.println("Accepted");

    Serial.println ("Access granted !");
    Serial.write(254); delay(10);
    keypadCheck();

    //Add code to run if it works
    //deg




  }


  else {
    Serial.println("Denied"); //if password is wrong, keep locked
    Serial.write(254); delay(10);



  }





}

