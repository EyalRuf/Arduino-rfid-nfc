/*
 * Created by ArduinoGetStarted.com
 *
 * This example code is in the public domain
 *
 * Tutorial page: https://arduinogetstarted.com/tutorials/arduino-rfid-nfc
 */

#include <SPI.h>
#include <MFRC522.h>
#include <LinkedList.h>

#define SS_PIN 10
#define RST_PIN 5

MFRC522 rfid(SS_PIN, RST_PIN);

class UidEntry {
  public:
    byte *uid;
};

void setup() {
  Serial.begin(9600);
  SPI.begin(); // init SPI bus
  rfid.PCD_Init(); // init MFRC522

  Serial.println("Tap RFID/NFC Tag on reader");

  byte savedUID[4] = {0x29,0x18,0x66,0xB3};
  UidEntry *preSaved = new UidEntry();
  preSaved->uid = savedUID;

  LinkedList<UidEntry*> myUIDS = LinkedList<UidEntry*>();
  myUIDS.add(preSaved);
}

void loop() {
  if (rfid.PICC_IsNewCardPresent()) { // new tag is available
    if (rfid.PICC_ReadCardSerial()) { // NUID has been readed
      MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
      //Serial.print("RFID/NFC Tag Type: ");
      //Serial.println(rfid.PICC_GetTypeName(piccType));

      // print NUID in Serial Monitor in the hex format
      byte currUID[4] = {0,0,0,0};
      readRfidIntoArr(currUID, rfid.uid.uidByte, 4);

      Serial.print("Current UID:");
      printArrAsHex(currUID, 4);




      
//      if (compareCharArr(savedUID, currUID, 4)) {
//        Serial.print("Authorized.");
//        Serial.println();
//      }
//      else {
//        Serial.print("Unauthorized.");
//        Serial.println();
//      }

      rfid.PICC_HaltA(); // halt PICC
      rfid.PCD_StopCrypto1(); // stop encryption on PCD
    }
  }
}

void printArrAsHex(byte arr[], int arrSize) {
    for (int i = 0; i < arrSize; i++) {
      Serial.print(arr[i] < 0x10 ? " 0" : " ");
      Serial.print(arr[i], HEX);
    }
    Serial.println();
}

void readRfidIntoArr (byte arr[], byte rfid[], int rfidSize) {
  for (int i = 0; i < rfidSize; i++) {
    arr[i] = rfid[i];
  }
}

bool compareCharArr (byte arr1[], byte arr2[], int arrSize) {
  for (int i = 0; i < arrSize; i++) {
    if (arr1[i] != arr2[i]) {
      return false;
    }
  }

  return true;
}
