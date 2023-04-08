/*
  DX21 INPUT
  keyboard matrix input (6db):
  br re or ye gr bl
  37 39 41 43 45 47
  keyboard matrix output(12db):
  br re or ye gr bl pr gr wh gg sb pn
  28 30 32 34 36 38 40 42 44 46 48 50

  button matrix 1 output
  wh gg sb pk
  3  4  5  6
  button matrix 1 input
  br re or ye gr bl gr vi!!!
  13 12 11 10 9  8  7  2!

  button matrix 2 input?
  br re or ye gr bl
  25 27 29 31 33 35

  button matrix 2 output?
  vi gy
  49 51
*/

#include <MIDI.h>
//#include <MIDIUSB.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
MIDI_CREATE_INSTANCE(HardwareSerial, Serial3, MIDI3);
bool note[128];
bool button1[64];
bool button2[64];
int lastmenu = 0;
int tune = 13;
bool ROWS[12];
int COLS[12] = {28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50};
int BUTTON1COLS[4] = {3, 4, 5, 6};

int BUTTON2COLS[7] = {25, 27, 29, 31, 33, 35};
String NOTENAME[14] = { "CISZ", "D", "DISZ", "E", "F", "FISZ", "G", "GISZ", "A", "B", "H", "C", "CISZ"};
uint64_t  PioC;

void setup() {

  for (int i = 0; i < 128; i++)
  {
    note[i] = false;
  }
  for (int i = 0; i < 64; i++)
  {
    button1[i] = false;
  }
  for (int i = 0; i < 64; i++)
  {
    button2[i] = false;
  }

  Serial.begin(115200);
  //For printing out the output
  Serial.print("in out set\n");
  pinMode(26, OUTPUT);
  pinMode(28, OUTPUT);
  pinMode(30, OUTPUT);
  pinMode(32, OUTPUT);
  pinMode(34, OUTPUT);
  pinMode(36, OUTPUT);
  pinMode(38, OUTPUT);
  pinMode(40, OUTPUT);
  pinMode(42, OUTPUT);
  pinMode(44, OUTPUT);
  pinMode(46, OUTPUT);
  pinMode(48, OUTPUT);
  pinMode(50, OUTPUT);
  Serial.print("end output\n");
  //switch1 part input
  pinMode(13,  INPUT_PULLUP);
  pinMode(12,  INPUT_PULLUP);
  pinMode(11,  INPUT_PULLUP);
  pinMode(10,  INPUT_PULLUP);
  pinMode(9,  INPUT_PULLUP);
  pinMode(8,  INPUT_PULLUP);
  pinMode(7,  INPUT_PULLUP);
  pinMode(2,  INPUT_PULLUP);

  pinMode(6, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(3, OUTPUT);


  //switch part input
  //  pinMode(23, OUTPUT);
  pinMode(25, OUTPUT);
  pinMode(27, OUTPUT);
  pinMode(29, OUTPUT);
  pinMode(31, OUTPUT);
  pinMode(33, OUTPUT);
  pinMode(35, OUTPUT);

  //keyboard part input
  pinMode(37, INPUT_PULLUP);
  pinMode(39, INPUT_PULLUP);
  pinMode(41, INPUT_PULLUP);
  pinMode(43, INPUT_PULLUP);
  pinMode(45, INPUT_PULLUP);
  pinMode(47, INPUT_PULLUP);

  //
  pinMode(49, INPUT_PULLUP);
  pinMode(51, INPUT_PULLUP);
  pinMode(53, INPUT_PULLUP);
  Serial.print("end input\n");

  MIDI3.begin(MIDI_CHANNEL_OMNI);
  // MIDI3.turnThruOff();
  //Serial3.begin(31250);
  //PMC->PMC_PCER0 = PMC_PCER0_PID13; // PIOC power ON
  Wire.setClock(1000000);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print(" Firmvare: 0.42 ");

  delay(800);
  lcd.setCursor(0, 1);
  lcd.print("  DigitalSynth  ");
  delay(400);
  lcd.setCursor(0, 0);
  lcd.print(" Yamaha DX-21.1 ");
  lcd.setCursor(0, 1);
  lcd.print("                ");
  delay(800);
  lcd.setCursor(0, 0);
}

void loop() {
  keyboardscanner();
  button1scanner();
  button2scanner();
}

void button2scanner() {
  //button2 scanner
  for (int i = 0; i < 6; i++)
  {
    //  digitalWrite(23, HIGH);
    digitalWrite(25, HIGH);
    digitalWrite(27, HIGH);
    digitalWrite(29, HIGH);
    digitalWrite(31, HIGH);
    digitalWrite(33, HIGH);
    digitalWrite(35, HIGH);

    //i bus low level
    digitalWrite(BUTTON2COLS[i], LOW);

    if (!digitalRead(49))
    {
      if (!button2[i]) {
        kiir("BUTTO2__ON :", String(i) );

        button2[i] = true;
      }
    } else {
      if (button2[i]) {
        kiir("BUTTO2__OFF:", String(i));
        button2[i] = false;
      }
    }

    if (!digitalRead(51))
    {
      if (!button2[i + 6]) {
        kiir("BUTTO2__ON :", String(i + 6) );
        button2[i + 6] = true;
      }
    } else {
      if (button2[i + 6]) {
        kiir("BUTTO2__OFF:", String(i + 6));
        button2[i + 6] = false;
      }

    }
  }
}

void button1scanner() {
  //button1 scanner
  for (int i = 0; i < 4; i++)
  {
    digitalWrite(3, HIGH);
    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);
    digitalWrite(6, HIGH);

    //i bus low level
    digitalWrite(BUTTON1COLS[i], LOW);

    if (!digitalRead(13))
    {
      if (!button1[i]) {
        kiir("BUTTON__ON :", String(i) );
        MIDI3.sendNoteOn(tune, 127, 1);
        lastmenu = i;
        button1[i] = true;
      }
    } else {
      if (button1[i]) {
        kiir("BUTTON__OFF:", String(i));
        button1[i] = false;
      }
    }


    if (!digitalRead(12))
    {
      if (!button1[i + 4]) {
        kiir("BUTTON__ON :" , String(i + 4));
        lastmenu = i + 4;
        // MIDI3.sendNoteOn(tune, 127, 1);
        button1[i + 4] = true;
      }
    } else {
      if (button1[i + 4]) {

        kiir("BUTTON__OFF:", String(i + 4) );

        button1[i + 4] = false;
      }
    }
    if (!digitalRead(11))
    {
      if (!button1[i + 8]) {
        kiir("BUTTON__ON :" , String(i + 8));
        lastmenu = i + 8;
        // MIDI3.sendNoteOn(tune, 127, 1);
        button1[i + 8] = true;
      }
    } else {
      if (button1[i + 8]) {

        kiir("BUTTON__OFF:" , String(i + 8));

        button1[i + 8] = false;
      }
    }


    if (!digitalRead(10))
    {
      if (!button1[i + 12]) {
        kiir("BUTTON__ON :", String(i + 12)  );
        lastmenu = i + 12;
        // MIDI3.sendNoteOn(tune, 127, 1);
        button1[i + 12] = true;
      }
    } else {
      if (button1[i + 12]) {

        kiir("BUTTON__OFF:" , String(i + 12)  );

        button1[i + 12] = false;
      }
    }


    if (!digitalRead(9))
    {
      if (!button1[i + 16]) {
        kiir("BUTTON__ON: " , String(i + 16));
        lastmenu = i + 16;
        // MIDI3.sendNoteOn(tune, 127, 1);
        button1[i + 16] = true;
      }
    } else {
      if (button1[i + 16]) {

        kiir("BUTTON__OFF:" , String(i + 16));

        button1[i + 16] = false;
      }
    }


    if (!digitalRead(8))
    {
      if (!button1[i + 20]) {
        kiir("BUTTON__ON: " , String(i + 20));
        lastmenu = i + 20;
        // MIDI3.sendNoteOn(tune, 127, 1);
        button1[i + 20] = true;
      }
    } else {
      if (button1[i + 20]) {

        kiir("BUTTON__OFF:" , String(i + 20));

        button1[i + 20] = false;
      }
    }

    if (!digitalRead(2))
    {
      if (!button1[i + 24]) {
        kiir("BUTTON__ON: " , String(i + 24)  );
        lastmenu = i + 24;
        // MIDI3.sendNoteOn(tune, 127, 1);
        button1[i + 24] = true;
      }
    } else {
      if (button1[i + 24]) {

        kiir("BUTTON__OFF:" , String(i + 24) );

        button1[i + 24] = false;
      }
    }

    if (!digitalRead(7))
    {
      if (!button1[i + 28]) {
        kiir("BUTTON__ON: " , String(i + 28)  );
        lastmenu = i + 28;
        // MIDI3.sendNoteOn(tune, 127, 1);
        button1[i + 28] = true;
      }
    } else {
      if (button1[i + 28]) {

        kiir("BUTTON__OFF:" , String(i + 28) );

        button1[i + 28] = false;
      }
    }



  }
}


void keyboardscanner() {
  //keyboard scanner:
  for (int i = 0; i < 12; i++)
  {
    //12adress bus high level
    digitalWrite(28, HIGH);//28
    digitalWrite(30, HIGH);//30
    digitalWrite(32, HIGH); //32
    PIOC->PIO_SODR = (1u << 2); //34
    PIOC->PIO_SODR = (1u << 4); //36
    PIOC->PIO_SODR = (1u << 6); //38
    PIOC->PIO_SODR = (1u << 8); //40
    digitalWrite(42, HIGH);//42
    //PIOC->PIO_SODR=(1u<<21); //42
    PIOC->PIO_SODR = (1u << 19); //44
    digitalWrite(46, HIGH);//46
    PIOC->PIO_SODR = (1u << 15); //48
    PIOC->PIO_SODR = (1u << 13); //50
    //i bus low level
    digitalWrite(COLS[i], LOW);
    //String sor=String(bitRead(var,0))+String(bitRead(var,2))+String(bitRead(var,4))+String(bitRead(var,6))+String(bitRead(var,8))+String(bitRead(var,10))+String(bitRead(var,12))+String(bitRead(var,14))+String(bitRead(var,16))+String(bitRead(var,18))+String(bitRead(var,20))+String(bitRead(var,22))+String(bitRead(var,24))+String(bitRead(var,26))+String(bitRead(var,28)+String(bitRead(var,30)));
    //Serial.println(sor);
    if (!digitalRead(37)) //37. láb
    {
      if (!note[20 + i]) {
        kiir(NOTENAME[i], "0 ON    ");
        //Serial.print(NOTENAME[i] + "0 ON    ");
        MIDI3.sendNoteOn (tune + i, 127, 1);
        note[20 + i] = true;
      }
    } else {
      if (note[20 + i]) {
        kiir(NOTENAME[i], "0 OFF    ");
        //Serial.print(NOTENAME[i] + "0 OFF    ");
        MIDI3.sendNoteOff(tune + i, 0, 1);
        note[20 + i] = false;
      }
    }

    if (!digitalRead(39)) //39. láb
    {
      if (!note[37 + i]) {
        kiir(NOTENAME[i], "1 ON    ");
        MIDI3.sendNoteOn(tune + 12 + i, 127, 1);
        note[37 + i] = true;
      }
    } else {
      if (note[37 + i]) {
        kiir(NOTENAME[i], "1 OFF   ");
        MIDI3.sendNoteOff(tune + 12 + i, 0, 1);
        note[37 + i] = false;
      }
    }

    if (!digitalRead(41)) //41. láb
    {
      if (!note[49 + i]) {
        kiir(NOTENAME[i], "2 ON    ");
        MIDI3.sendNoteOn(tune + 24 + i, 127, 1);
        note[49 + i] = true;
      }
    } else {
      if (note[49 + i]) {
        kiir(NOTENAME[i], "2 OFF   ");
        MIDI3.sendNoteOff(tune + 24 + i, 0, 1);
        note[49 + i] = false;
      }
    }


    if (!digitalRead(43)) //43. láb
    {
      if (!note[61 + i]) {
        kiir(NOTENAME[i], "3 ON    ");
        MIDI3.sendNoteOn(tune + 36 + i, 127, 1);
        note[61 + i] = true;
      }
    } else {
      if (note[61 + i]) {
        kiir(NOTENAME[i], "3 OFF   ");
        MIDI3.sendNoteOff(tune + 36 + i, 0, 1);
        note[61 + i] = false;
      }
    }

    if (!digitalRead(45)) //45. láb
    {
      if (!note[73 + i]) {
        kiir(NOTENAME[i], "4 ON    ");

        MIDI3.sendNoteOn(tune + 48 + i, 127, 1);
        note[73 + i] = true;
      }
    } else {
      if (note[73 + i]) {
        kiir(NOTENAME[i], "4 OFF   ");
        MIDI3.sendNoteOff(tune + 48 + i, 0, 1);
        note[73 + i] = false;
      }
    }

    if (!digitalRead(47)) //47. láb
    {
      if (!note[85 + i]) {

        kiir(NOTENAME[i], "5 ON    ");
        MIDI3.sendNoteOn(tune + 60 + i, 127, 1);
        note[85 + i] = true;
      }
    } else {
      if (note[85 + i]) {
        kiir(NOTENAME[i], "5 OFF    ");
        MIDI3.sendNoteOff(tune + 60 + i, 0, 1);
        note[85 + i] = false;
      }
    }
  }
}

void kiir(String szoveg, String szam) {
  String kiirszoveg = szoveg + szam;
  Serial.print(kiirszoveg + "\n");
  lcd.setCursor(0, 1);
  lcd.print(kiirszoveg);
}
