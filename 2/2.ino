/*
 * Created by Eyal Rufeisen
 *
 * Compares any scanned nfc to the saved one.
 * Can do any action you want based on whether it matches (whether it's 'authorized').
 * Right now it just prints out if it's the right one or not.
 *
 * Based on example code from: https://arduinogetstarted.com/tutorials/arduino-rfid-nfc
 */

/* Required Libraries -> If you get an error about not finding these libraries ->
Go to tools -> Manage Libraries and search for 'MFRC522' -> install it. 
SPI should be installed by default 
*/ 
#include <SPI.h>
#include <MFRC522.h>

// Initialization stuff
#define SS_PIN 10
#define RST_PIN 5
MFRC522 rfid(SS_PIN, RST_PIN);

byte savedUID[4] = {0x29,0x18,0x66,0xB3};

void setup() {
  // Initialization stuff
  Serial.begin(9600);
  SPI.begin(); // init SPI bus
  rfid.PCD_Init(); // init MFRC522

  // Print saved uid
  Serial.print("Saved UID:" );
  printArrAsHex(savedUID, 4);
  Serial.println();

  Serial.println("Tap RFID/NFC Tag on reader");
}

void loop() {
  if (rfid.PICC_IsNewCardPresent()) { // new tag is available
    if (rfid.PICC_ReadCardSerial()) { // NUID has been readed
      MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);

      // Read rfid into our own arr
      byte currUID[4] = {0,0,0,0};
      copyByteArr(currUID, rfid.uid.uidByte, 4);

      // Print current UID
      Serial.println();
      Serial.print("Current UID:");
      printArrAsHex(currUID, 4);
      Serial.println();

      if (compareByteArr(savedUID, currUID, 4)) {
        Serial.print("Authorized.");
        Serial.println();
      }
      else {
        Serial.print("Unauthorized.");
        Serial.println();
      }

      // Finish reading current card/tag
      rfid.PICC_HaltA(); // halt PICC
      rfid.PCD_StopCrypto1(); // stop encryption on PCD
    }
  }
}

/*
 * Prints the hexadecimal representation of the rfid/nfc UID - code
 * arr - The byte array to sace the UID into
 * arrSize - size of UID code to print
 */
void printArrAsHex(byte arr[], int arrSize) {
    for (int i = 0; i < arrSize; i++) {
      Serial.print(arr[i] < 0x10 ? " 0" : " ");
      Serial.print(arr[i], HEX);
    }
}

/*
 * Copys/reads rfid bytes into the array byte by byte
 * copyInto - copys the bytes into this array
 * copyFrom - copys the bytes from this array
 * arrSize - array size
 */
void copyByteArr (byte copyInto[], byte copyFrom[], int arrSize) {
  for (int i = 0; i < arrSize; i++) {
    copyInto[i] = copyFrom[i];
  }
}

/*
 * Compares two byte arrays - returns true if they're indentical, returns false otherwise.
 * arr1 - first byte array to compare
 * arr2 - second byte array to compare
 * arrSize - size of the arrays
 */
bool compareByteArr (byte arr1[], byte arr2[], int arrSize) {
  for (int i = 0; i < arrSize; i++) {
    if (arr1[i] != arr2[i]) {
      return false;
    }
  }

  return true;
}
