
void arthure(int sector) {

  byte buffer[18] ;
  byte trailerBlock   = 7;

  MFRC522::StatusCode status;

  byte size = sizeof(buffer);

  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_B, trailerBlock, &key, &(mfrc522.uid));

  uidcarte();

  mfrc522.PICC_DumpMifareClassicSectorToSerial(&(mfrc522.uid), &key, sector);
  for (byte i = 0 ; i < 3; i++)
  {
    //LECTURE DE LA CARTE.
    mfrc522.MIFARE_Read(tableau[sector][i], buffer, &size);
    //Serial.print(tableau[sector][i]); Serial.print(F(" : ")); Serial.println(dump_byte_array(buffer, 16));
    dump_byte_array(buffer, 16);
    vardata[tableau[sector][i]] = entier;
    entier = "";

  }

  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
  return;





}



void carteid()
{
  Serial.println(); Serial.println("***************** CARTE IDENTITÉ*****************"); Serial.println();

  Serial.print("Nom :"); Serial.println(vardata[4]);
  Serial.print("Prénom :"); Serial.println(vardata[5]);
  Serial.print("Age :"); Serial.println(vardata[6]);

  Serial.print("Adresse :"); Serial.println(vardata[8]);
  Serial.print("Ville :"); Serial.println(vardata[9]);
  Serial.print("Pays :"); Serial.println(vardata[10]);
  Serial.print("Commune :"); Serial.println(vardata[12]);



}

void authentification()
{

  digitalWrite(7, LOW);
  digitalWrite(6, LOW);
  digitalWrite(5, LOW);

  if (!vardata[5].indexOf("arduino"))
  {
    digitalWrite(7, HIGH);
    digitalWrite(6, HIGH);
    digitalWrite(5, HIGH);
    Serial.println("** Bonjour, " + vardata[5]); Serial.print("** vous avez accés à toutes les zones ** "); Serial.println();


  }
  if (!vardata[5].indexOf("Eugenio"))
  {
    digitalWrite(7, HIGH);
    digitalWrite(6, LOW);
    digitalWrite(5, LOW);
    Serial.println("** Bonjour, " + vardata[5]); Serial.print("** vous avez accés a la zone rouge ** "); Serial.println();


  }
  if (!vardata[5].indexOf("Arthure"))
  {
    digitalWrite(7, LOW);
    digitalWrite(6, HIGH);
    digitalWrite(5, LOW);
    Serial.println("** Bonjour, " + vardata[5]); Serial.print("** vous avez accés a la zone verte ** "); Serial.println();
  }

}

void uidcarte()
{
  if (!premiereFois2)
  {
    Serial.print("UID de la Carte :");
    for (byte i = 0; i < 4; i++) {
      if (mfrc522.uid.uidByte[i] < 0x10)
        Serial.print(F(" 0"));
      else
        Serial.print(F(" "));
      Serial.print(mfrc522.uid.uidByte[i], HEX);
    }
    Serial.println();
    premiereFois2 = true;
  }


}
