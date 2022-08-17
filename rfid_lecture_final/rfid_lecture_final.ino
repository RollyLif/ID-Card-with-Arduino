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

*/

#include <SPI.h>
#include <MFRC522.h>


String vardata[63];

byte tableau[16] [4] = { 

  {0, 1, 2, 3},
  {4, 5, 6, 7},
  {8, 9, 10,  11},
  {12,  13,  14,  15},
  {16,  17,  18,  19},
  {20,  21,  22,  23},
  {24,  25,  26,  27},
  {28,  29,  30,  31},
  {32,  33,  34,  35},
  {36,  37,  38,  39},
  {40,  41,  42,  43},
  {44,  45,  46,  47},
  {48,  49,  50,  51},
  {52,  53,  54,  55},
  {56,  57,  58,  59},
  {60,  61,  62,  63}

};

byte incementvar = 1;
String entier = "";
String QSecteur = "";
bool premiereFois;
bool premiereFois2;
#define RST_PIN         9           // Intialisation de la pin reset
#define SS_PIN          10          // Intialisation de la pin SS

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Création de l'instance MFRC522.

MFRC522::MIFARE_Key key;



void setup() {


  Serial.begin(9600);

  pinMode(7, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(5, OUTPUT);

  while (!Serial);
  SPI.begin();        // Initialisation du bus SPI
  mfrc522.PCD_Init(); // Inititilisation de la carte MFRC522

  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }

  Serial.println( "******* Presenté une carte *******") ;

}
void loop() {

  if (incementvar < 5)
  {
    SPI.begin();        // Initialisation du bus SPI
    mfrc522.PCD_Init(); // Inititilisation de la carte MFRC522

    if ( ! mfrc522.PICC_IsNewCardPresent())
    {
      return;

    }

    // Select one of the cards
    if (  mfrc522.PICC_ReadCardSerial())
    {

      arthure(incementvar);
      incementvar++;
      mfrc522.PCD_Reset();

    }
  }

  else
  {
    if (!premiereFois)
    {
      carteid();
      authentification();
      premiereFois = true;
      Serial.println() ; Serial.println( "Voulez vous refaire une lecture ?") ;
    }
    
    while (Serial.available() != 0)
    {
      char  data = Serial.read();
      QSecteur = data;
      delay(80);
    }

    if ( QSecteur == "O")
    {
      QSecteur = Serial.read();
      premiereFois = false;
      premiereFois2 = false;
      incementvar = 1;
    }


  }


}


