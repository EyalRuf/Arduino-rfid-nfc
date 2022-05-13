/*
   Created by Eyal Rufeisen

   Based on example code from: https://arduinogetstarted.com/tutorials/arduino-rfid-nfc
*/

/* 
  Required Libraries -> If you get an error about not finding these libraries ->
  Go to tools -> Manage Libraries and search for 'MFRC522' (and LinkedList) -> install them.
  SPI should be installed by default
*/
#include <SPI.h>
#include <MFRC522.h>

// Initialization stuff
#define SS_PIN 10
#define RST_PIN 5
MFRC522 rfid(SS_PIN, RST_PIN);


// Our UID 'database'
int uidDBSize = 100;
byte myUIDS[100][4] = {
    {0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},
    {0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},
    {0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},
    {0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},
    {0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},
    {0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},
    {0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},
    {0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},
    {0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},
    {0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},
    {0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},
    {0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},
    {0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},
    {0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},
    {0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},
    {0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},
    {0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},
    {0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},
    {0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},
    {0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},
  };
int uidsCounter = 0;

const int buttonPin = 2;     // the number of the pushbutton pin
int buttonState = 0;         // variable for reading the pushbutton status

void setup() {
  // Initialization stuff
  Serial.begin(9600);
  SPI.begin(); // init SPI bus
  rfid.PCD_Init(); // init MFRC522

  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);

  // Printing saved UIDs or saying that there aren't any saved.
  if (uidsCounter == 0) {
    Serial.println("There are currently no Authorized chips. Click the button to add more.");
  } else {
    printUIDArr();
  }
  Serial.println("Tap RFID/NFC Tag on reader");
}

void loop() {
  if (rfid.PICC_IsNewCardPresent()) { // new tag is available
    if (rfid.PICC_ReadCardSerial()) { // NUID has been readed
      MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);

      // Read rfid into our own arr
      byte currUID[4] = {0, 0, 0, 0};
      copyByteArr(currUID, rfid.uid.uidByte, 4);

      // Print current UID
      Serial.println();
      Serial.print("Current UID:");
      printArrAsHex(currUID, 4);
      Serial.println();

      bool doesExist = UIDExistsInList(currUID);

      if (buttonState == HIGH) {
          if (doesExist) {
            Serial.println("Currently presented NFC is already saved and authorized...");
            Serial.println("Adding operation aborted.");
          } else {
            AddUIDToList(currUID);
          }
          buttonState = 0;
      } else {
          if (doesExist) {
            Serial.print("Authorized.");
            Serial.println();
          }
          else {
            Serial.print("Unauthorized.");
            Serial.println();
          }
      }

      // Finish reading current card/tag
      rfid.PICC_HaltA(); // halt PICC
      rfid.PCD_StopCrypto1(); // stop encryption on PCD
    }
  } else if (buttonState != HIGH) {
      buttonState = digitalRead(buttonPin); 
      if (buttonState == HIGH) {
        Serial.println("Adding Next Presented NFC to Authorized List");
        Serial.println("Awaiting a chip...");
      }
  }
}

/*
   Prints the hexadecimal representation of the rfid/nfc UID - code
   arr - The byte array to sace the UID into
   arrSize - size of UID code to print
*/
void printArrAsHex(byte arr[], int arrSize) {
  for (int i = 0; i < arrSize; i++) {
    Serial.print(arr[i] < 0x10 ? " 0" : " ");
    Serial.print(arr[i], HEX);
  }
}

/*
   Prints the whole UIDs array
*/
void printUIDArr() {
  Serial.println("-------UIDS:-------");
  for (int i = 0; i < uidsCounter; i++) {
    printArrAsHex(myUIDS[i], 4);
    Serial.println("");
  }
  Serial.println("-------------------");
  Serial.println("");
}

/*
   Copys/reads rfid bytes into the array byte by byte
   copyInto - copys the bytes into this array
   copyFrom - copys the bytes from this array
   arrSize - array size
*/
void copyByteArr (byte copyInto[], byte copyFrom[], int arrSize) {
  for (int i = 0; i < arrSize; i++) {
    copyInto[i] = copyFrom[i];
  }
}

/*
   Compares two byte arrays - returns true if they're indentical, returns false otherwise.
   arr1 - first byte array to compare
   arr2 - second byte array to compare
   arrSize - size of the arrays
*/
bool compareByteArr (byte arr1[], byte arr2[], int arrSize) {
  for (int i = 0; i < arrSize; i++) {
    if (arr1[i] != arr2[i]) {
      return false;
    }
  }

  return true;
}

/*
    Checks the whole Database for a specific UID, returns true if found, false otherwise
*/
bool UIDExistsInList (byte uidToSearch[]) {
  for (int i = 0; i < uidsCounter; i++) {
    if (compareByteArr(uidToSearch, myUIDS[i], 4))
    {
      return true;
    }
  }

  return false;
}

/*
    Checks the whole Database for a specific UID, if it's not there already, it's added to the database. It prints a message according to result.
    Also, if the database is full it doesn't add it to the list and prints a message accordingly.
    
    uidToAdd - the UID to add to the database (if it's not added already)
*/
void AddUIDToList (byte uidToAdd[4]) { 
  if (uidsCounter >= uidDBSize) {
    Serial.println("Sorry, max amount of UIDS have already been added.");
  } else {
    copyByteArr(myUIDS[uidsCounter],uidToAdd, 4);
    uidsCounter++;
    Serial.println("UID has been succesfully added.");
  }
}
