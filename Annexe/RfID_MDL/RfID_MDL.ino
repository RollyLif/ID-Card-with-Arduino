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

byte etape;
String donnee = " ";

int val;

String Lecturedata;
int blockAddr;
char  data;
int donnesvalide;
int LongeurMot;
int bloc;
int sector;
byte dataBlock[16];

int tempoActive = 0;

// Temps à l'activation de la tempo
unsigned long tempoDepart = 0;


#include <SPI.h>


#include <MFRC522.h>

#define RST_PIN         9           // Configurable, see typical pin layout above
#define SS_PIN          10          // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

MFRC522::MIFARE_Key key;






void setup() {
  Serial.begin(9600);
  Serial.println("***** Moniteur pret*****");
  Serial.println();

  while (!Serial);    // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
  SPI.begin();        // Init SPI bus
  mfrc522.PCD_Init(); // Init MFRC522 card


  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }



}

void loop() {

  //***** Acquisition des données.
  while (Serial.available() != 0 && etape != 7) {    //Contrôle présence de données dans le buffer.

    char acquis_donnee = Serial.read(); //Lecture du buffer.
    donnee = donnee + acquis_donnee;    //Concatenisation du buffer.

    /*Configurer le moniteur en NOUVELLE LIGNE*/
    if (acquis_donnee == 10) {  //Contrôle de la fin des donnée = Nouvelle ligne.

      val = donnee.toInt(); //Conversion de la donnee CHAR en INT.
      etape++;              //Incrémentation => validation de saisi.
    }
  }



  while (Serial.available() != 0 && etape == 7 )
  {

    // Serial.print("boule de crystal");
    data = Serial.read();
    Lecturedata = Lecturedata + data;
    delay(80);
    Serial.flush();
    //etape++;

  }













  //***** TRAITEMENT DES DONNEES *****/
  int bloc_X; // Variable d'affichage donnée. => Etape 2.

  switch (etape) {

    //***** ÉTAPE 0: Choisir le secteur.
    case 0:
      etape = 1;                              //Verrouillage de l'étape.
      Serial.println("Choisir le secteur ");  //Affichage d'un message.
      for (int i = 1 ; i < 16 ; i++) {        //Affichage des choix possibles.
        Serial.print(i); Serial.print(" / "); //Affichage des choix possibles.
      }
      Serial.println();                       //Saut de ligne.
      break;

    //***** ÉTAPE 1: Choisir le bloc.
    case 2:
      //=> 16 Secteurs au choix (sans contraintes).
      if (val >= 1 && val <= 15) {
        etape = 3 ;                                                               //Verrouillage de l'étape.
        Serial.print(val); Serial.println(" est une donnees valide SECTEUR!! ");  //Affichage d'un message.
        Serial.println("*****");                                                  //Affichage d'un message.
        donnee = " ";                                                             //Initialisation = Fin de traitement.
        sector = val;                                                            //Chargement de la saisi.
        bloc = sector * 4; //Calcul de la 1er donnée du bloc choisi.
      }
      else {                                                                            //Execution par défaut.
        etape = 0 ;                                                                     //Verrouillage de l'étape.
        donnee = " ";                                                                   //Initialisation = Fin de traitement.
        Serial.print(val); Serial.println(" N'est pas une donnees valide SECTEUR!! ");  //Affichage d'un message.
        Serial.println("*****");                                                        //Affichage d'un message.

        //Serial.print("etape : "); Serial.println(etape);//Verrouillage de l'étape.
      }
      break;

    //***** ÉTAPE 2: Choisir le bloc.
    case 3:
      Serial.println("Choisir le bloc");  //Affichage d'un message.
      /*Nous avons 16 secteurs de 4 blocs.
        Donc pour chaques selection de secteur c'est le multiple de 4 qui's'applique.
        exemple :
                              3 => Bloc constructeur non exploitable.
                              2
                              1
                  Secteur 0 = 0
        Soit => 0*4 = 0

                              23 => Bloc constructeur non exploitable.
                              22
                              21
                  Secteur 5 = 20
        Soit => 5*4 = 20

                              59 => Bloc constructeur non exploitable.
                              58
                              57
                 Secteur 14 = 56
        Soit => 14*4 = 56

        Il ne reste plus qu'a incrémenter sur les trois valeurs exploitable
        en partant de cette valeur calculé.*/

      // bloc = secteur * 4; //Calcul de la 1er donnée du bloc choisi.
      bloc_X = bloc;

      for (int i = 0; i < 3; i++) {                   //Affichage des choix possibles.
        //Chargement des données
        Serial.print(bloc_X++); Serial.print(" / ");  //Affichage d'un message.
      }
      etape = 4;                                      //Verrouillage de l'étape.
      Serial.println();                               //Saut de ligne.
      break;

    case 5:
      if (val >= bloc && val <= bloc + 2) {                                       //Conditionnement d'execution
        Serial.print(val); Serial.println(" est une donnees de BLOC valide !! "); //Affichage d'un message.
        Serial.println("*****");
        blockAddr = val;
        //Affichage d'un message.
        etape = 6;                                                                //Verrouillage de l'étape.
        donnee = " ";
        break;
      }

      else {                                                                            //Execution par défaut.
        Serial.print(val); Serial.println(" N'est pas une donnees de BLOC valide !! "); //Affichage d'un message.
        Serial.println("*****");                                                        //Affichage d'un message.
        etape = 3;                                                                      //Verrouillage de l'étape.
        donnee = " ";
      }
      Serial.println();                                                                 //Saut de ligne.
      break;




    case 6:
      etape = 7;                              //Verrouillage de l'étape.
      Serial.println("Inscrit une adresse de 16 caratére maximum: ");  //Affichage d'un message.
      //Saut de ligne.
      break;

    //***** ÉTAPE 1: Choisir le bloc.
    case 7:

      Serial.print(Lecturedata);


      LongeurMot = Lecturedata.length();


      if (Lecturedata != 0)
      {

        if (LongeurMot < 16 )
        {


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
          Serial.println() ;
          Lecturedata = "";
          etape = 8;
        }
        else
        {
          Serial.println ( "****la valeur est trops grande****");
          Lecturedata = "";
          etape = 6;
        }






      }


      break;


    /*while (datavalide != 1)
          {
            Serial.print("kaya bonga");
            Serial.print(Lecturedata);

            Serial.print(val);
            datavalide = 1;
            break;

          }*/





    /*case 6:

      Serial.println("Saisir une donnée de 16 caractére:");  //Affichage d'un message.
      etape = 7;

      break;

      case 7:
      LongeurMot = Lecturedata.length();

      if (LongeurMot < 16 )
      {
        Serial.print(" Valeur :"); Serial.println(Lecturedata);
        Serial.print(" Données Hexa :");
        for (int i = 0; i < 16; i++)
        {
          char test;
          test = Lecturedata.charAt(i);
          dataBlock[i] =  char (test, HEX);

          Serial.print (String("0x") + String(dataBlock[i], HEX)  ) ;

          if (i < 15 )
          {
            Serial.print(",") ;
          }
        }
        Serial.println ( "");
        Serial.println ( "");
        etape = 8;
        Lecturedata = "";
        //Saut de ligne.
        break;
    */


    case 8:

      tempoActive = 1;

      while (donnesvalide != 1)
      {
        Serial.println("******** Presentez carte ********** "); //Affichage d'un message.
        Serial.println();                   //Saut de ligne.
        //Serial.println(" ********** FIN **********   ");
        Serial.println();                   //Saut de ligne.
        donnesvalide = 1 ;                         //Verrouillage de l'étape.

        tempoDepart = millis();

      }
     
        // Et si il s'est écoulé 15secondes,
        if ( ( millis() - tempoDepart ) >= 15000 ) {

          // Alors on affiche un petit message
          Serial.println("Tu n'a pas présenter de carte depuis 15 seconde l'opération a été anuller");
          etape = 0;
          // Et on désactive la temporisation pour ne pas afficher ce message une seconde fois
          tempoActive = 0;
          donnesvalide = 0 ;

        }

      

    case 21:

      break;
  }


  while (donnesvalide == 1)
  {
    SPI.begin();        // Init SPI bus
  mfrc522.PCD_Init();
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





    Serial.println("Authenticating again using key A...");
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


    Serial.println(F("********  FIN   **********"));

    Serial.println("");
    // Halt PICC
    mfrc522.PICC_HaltA();
    // Stop encryption on PCD
    mfrc522.PCD_StopCrypto1();
    donnesvalide = 0;
    etape = 0;
  }



}



void dump_byte_array(byte * buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);

  }
  Serial.println();













}










