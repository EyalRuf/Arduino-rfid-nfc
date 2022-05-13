/*
 * Created by Eyal Rufeisen
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

void setup() {
  // More initialization stuff
  Serial.begin(9600);
  SPI.begin(); // init SPI bus
  rfid.PCD_Init(); // init MFRC522
  Serial.println("Tap RFID/NFC Tag on reader");
 
}

void loop() {
  if (rfid.PICC_IsNewCardPresent()) { // new tag is available
    if (rfid.PICC_ReadCardSerial()) { // NUID has been readed
      MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);

      // Printing it in hex format
      Serial.print("Current UID:");
      printArrAsHex(rfid.uid.uidByte, 4);

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
    Serial.println();
}
