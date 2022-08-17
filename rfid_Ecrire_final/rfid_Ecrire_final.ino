/*
 PlaisirArduino.fr EDL

 Ce programme est réaliser avec la librairie  MFRC522 disponible sur  https://github.com/miguelbalboa/rfid
 Code est tiré des exemple de la librairie, tester   avec la platine arduino UNO et addapter pour la platine arduino Mega

 Branchement de la platine Arduino:
   -----------------------------------------------------------------------------------------
               MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino
               Reader/PCD   Uno/101       Mega      Nano v3    Leonardo/Micro   Pro Micro
   Signal      Pin          Pin           Pin       Pin        Pin              Pin
   -----------------------------------------------------------------------------------------
   RST/Reset   RST          9             5         D9         RESET/ICSP-5     RST
   SPI SS      SDA(SS)      10            53        D10        10               10
   SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16
   SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14
   SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15

   section 1. 4: le nom;
              5: le prénom;
              6: Age;
              
          2.  8: Adresse;

              
   

*/


String Lecturedata;
uint8_t TableauData[16];
int LongeurMot;
int R1 = 0;
int R2 = 0;
int R3 = 0;
int R4 = 0;
int Question1 = 0;
int Question2 = 0;
int Question3 = 0;
int Question4 = 0;
char  data;
int sector = 2;
int blockAddr = 8;
byte dataBlock[16];
int var = 0;
int bloc_X;
char Confirmation = "";
int StockAdresse[3];
byte tableau[16] [4] = { //Déclaration
  {0, 1, 2},
  {4, 5, 6},
  {8, 9, 10},
  {12,  13,  14},
  {16,  17,  18},
  {20,  21,  22},
  {24,  25,  26},
  {28,  29,  30},
  {32,  33,  34},
  {36,  37,  38},
  {40,  41,  42},
  {44,  45,  46},
  {48,  49,  50},
  {52,  53,  54},
  {56,  57,  58},
  {60,  61,  62}

};





#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         9          // Configurable, see typical pin layout above
#define SS_PIN          10         // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

MFRC522::MIFARE_Key key;



void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while (!Serial);    // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
  SPI.begin();        // Init SPI bus
  mfrc522.PCD_Init(); // Init MFRC522 card


  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }
}




void loop() {
SPI.begin();        // Init SPI bus
  mfrc522.PCD_Init();
  while (Serial.available())
  {

    Serial.read();
    delay(80);
  }


  while (Question1 != 1 && R1 == 0)
  {

    Serial.print( " Entrez le secteur:  ") ;

    Question1 = 1;
    R4 = 0;
    Confirmation = "";
  }




  while (R1 == 0 )
  {

    if (Serial.available() == 0 )
    {


    }
    else

    {
      if (Serial.available() == 0 )
      {
        return;

      }


      else
      {
        Serial.print("");
        while (Serial.available() > 0 )
        {

          data = Serial.read();
          Lecturedata = Lecturedata + data;
          delay(80);


        }


        while (Serial.available () > 0) {
          Serial.read();
        }
        Serial.println(Lecturedata);
        sector = Lecturedata.toInt();
        Lecturedata = "";

        R1 = 1;
        Serial.print(R1);
      }
    }


  }





  while (Question2 != 1 && R1 == 1 )
  {
    bloc_X = sector * 4 ;

    for (int i = 0; i < 3; i++) {                   //Affichage des choix possibles.
      //Chargement des données
      StockAdresse[i] = bloc_X;
      //Serial.print(bloc_X++); Serial.println("  ");
      bloc_X++;

      //Affichage d'un message.
    }


    Serial.print( " Entrez une adresse comprise entre ") ; Serial.print( StockAdresse[0]); Serial.print( " et "); Serial.print( StockAdresse[2]); Serial.println( " : ");

    Question2 = 1;

  }
  while (R2 == 0 )
  {

    if (Serial.available() == 0 )
    {


    }
    else

    {
      while (Serial.available() != 0 )
      {
        data = Serial.read();
        Lecturedata = Lecturedata + data;
        delay(80);

      }
      while (Serial.available () > 0) {
        Serial.read();
      }
      Serial.print(" Adresse :"); Serial.println(Lecturedata);
      blockAddr  = Lecturedata.toInt();
      Lecturedata = "";
      R2 = 1;
      var = 0;
    }


  }

  for (int i = 0; i < 3; i++)
  {

    if (tableau[sector][i] == blockAddr && var != 1 && sector != 0)
    {
      Serial.print( " Vous avez validé  le secteur ") ; Serial.print(sector) ; Serial.print(" et l'adresse ") ; Serial.println(blockAddr) ;Serial.println();

      var = 1;
    }
  }
  if (var == 0  )
  {

    Serial.println("<-------ATTENTION L'ADRESSE OU LE SECTEUR NE CONVIENT PAS ------->");

    R1 = 0;
    R2 = 0;
    R3 = 0;
    R4 = 0;
    Question1 = 0;
    Question2 = 0;
    Question3 = 0;
    Question4 = 0;
    return;

  }

  while (Question3 != 1 && var == 1 )
  {

    Serial.println( " Entrez une valeur a 16 caractérés maximum: ");

    Question3 = 1;

  }
  while (R3 == 0 )
  {

    if (Serial.available() == 0 )
    {


    }
    else

    {
      while (Serial.available() != 0 )
      {
        data = Serial.read();
        Lecturedata = Lecturedata + data;
        delay(80);
        Serial.flush();

      }

      LongeurMot = Lecturedata.length();

      if (LongeurMot < 16 )
      {
        Serial.print(" Valeur :"); Serial.println(Lecturedata);
        Serial.print(" Données Hexa :");
        for (int i = 0; i < 16; i++)
        {
          char test;
          test = Lecturedata.charAt(i);
          dataBlock[i] =  convertisseurhexa(test);

          Serial.print (String("0x") + String(dataBlock[i], HEX)  ) ;

          if (i < 15 )
          {
            Serial.print(",") ;
          }
        }
        Serial.println ( "");
        Serial.println ( "");

        Lecturedata = "";
        R3 = 1;


      }
      else
      {
        Serial.println ( "la valeur est trops grande");
        Lecturedata = "";

      }

    }


  }


  while (Question4 != 1)
  {

    Serial.println( " Voulez continuer avec ces valeurs ? O/N ");

    Question4 = 1;

  }


  while (R4 == 0  )
  {

    if (Serial.available() != 0 )
    {
      Confirmation = Serial.read();
      Serial.flush();
      delay(80);
      R4 = 1;

    }

    /*if (Serial.available() > 0 )
      {
      Confirmation = Serial.read();
      Serial.flush();

      R4 = 12;
      Serial.println(String(R4) + String(Confirmation));
      }
      while (Serial.available() > 0 )
      {
      Serial.read();
      }*/

    switch (Confirmation) {
      case 'N':
        R1 = 0;
        R2 = 0;
        R3 = 0;
        R4 = 1;
        var = 0;
        Question1 = 0;
        Question2 = 0;
        Question3 = 0;
        Question4 = 0;

        while (Serial.available() != 0 ) {
          Serial.read();
        }
        Serial.println("<---- Le programme est relancer ---->");



        return;
      case 'O':
        Serial.println(" Prensentez la carte pret du module RFID");
        R4 = 1;
        return;
      default:
        R4 = 0;

    }




  }


























  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent())
    return;

  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial())
    return;


  // In this sample we use the second sector,
  // that is: sector #1, covering block #4 up to and including block #7

  byte trailerBlock   = 7;



  MFRC522::StatusCode status;
  byte buffer[18];
  byte size = sizeof(buffer);

  // Authenticate using key A





  Serial.println("Authenticating again using key B...");
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("PCD_Authenticate() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }

  Serial.println(F("Avant écriture"));
  mfrc522.PICC_DumpMifareClassicSectorToSerial(&(mfrc522.uid), &key, sector);
  Serial.println();



  status = (MFRC522::StatusCode) mfrc522.MIFARE_Write(blockAddr, dataBlock, 16);
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Erreur d'écriture "));
  }
  else {

    Serial.print(F("écriture dans le secteur ")); Serial.print(sector); Serial.print(" block "); Serial.println(blockAddr);


  }
  Serial.println();

  Serial.println(F("Aprés modification"));


  mfrc522.PICC_DumpMifareClassicSectorToSerial(&(mfrc522.uid), &key, sector);
  Serial.println();


  // Halt PICC
  mfrc522.PICC_HaltA();
  // Stop encryption on PCD
  mfrc522.PCD_StopCrypto1();


  R1 = 0;
  R2 = 0;
  R3 = 0;
  R4 = 0;
  var = 0;
  Question1 = 0;
  Question2 = 0;
  Question3 = 0;
  Question4 = 0;
}



void dump_byte_array(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);

  }
  Serial.println();













}
