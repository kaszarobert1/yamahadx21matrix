#include <MIDI.h>
//#include <MIDIUSB.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
MIDI_CREATE_INSTANCE(HardwareSerial, Serial3, MIDI3);
bool note[128];

bool ROWS[12];
int COLS[12] = {28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50};
String NOTENAME[14] = { "CISZ", "D", "DISZ", "E", "F", "FISZ", "G", "GISZ", "A", "B", "H", "C", "CISZ"};
uint64_t  PioC;

void setup() {
 
  for (int i = 0; i < 128; i++)
  {
    note[i] = false;
  }

  Serial.begin(115200); 
  //For printing out the output
  Serial.print("tuti nem lesz jó\n");
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
  pinMode(21, INPUT_PULLUP);
  pinMode(23, INPUT_PULLUP);
  pinMode(25, INPUT_PULLUP);
  pinMode(27, INPUT_PULLUP);
  pinMode(29, INPUT_PULLUP);
  pinMode(31, INPUT_PULLUP);
  pinMode(33, INPUT_PULLUP);
  pinMode(35, INPUT_PULLUP);
  pinMode(37, INPUT_PULLUP);
  pinMode(39, INPUT_PULLUP);
  pinMode(41, INPUT_PULLUP);
  pinMode(43, INPUT_PULLUP);
  pinMode(45, INPUT_PULLUP);
  pinMode(47, INPUT_PULLUP);
  pinMode(49, INPUT_PULLUP);
  pinMode(51, INPUT_PULLUP);
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
  for (int i = 0; i < 12; i++)
  {
    // PioC = PIOC->PIO_PDSR ;
    // PIOC->PIO_PUDR = PIO_PC12;
    //PIOC->PIO_SODR=(1u<<25); // Set Pin High
    //PIOC->PIO_CODR=(1u<<25); // Set Pin Low

    //PIOC->PIO_SODR=0x11111111;

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



    digitalWrite(COLS[i], LOW);



    //String sor=String(bitRead(var,0))+String(bitRead(var,2))+String(bitRead(var,4))+String(bitRead(var,6))+String(bitRead(var,8))+String(bitRead(var,10))+String(bitRead(var,12))+String(bitRead(var,14))+String(bitRead(var,16))+String(bitRead(var,18))+String(bitRead(var,20))+String(bitRead(var,22))+String(bitRead(var,24))+String(bitRead(var,26))+String(bitRead(var,28)+String(bitRead(var,30)));
    //Serial.println(sor);

    if (!digitalRead(21)) //21. láb
    {
      Serial.print("GOMB21");
    }

    if (!digitalRead(23)) //23. láb
    {
      Serial.print("GOMB23");
    }

    if (!digitalRead(25)) //25. láb
    {
      Serial.print("GOMB25");
    }


    if (!digitalRead(27)) //27. láb
    {
      Serial.print("GOMB27");
    }


    if (!digitalRead(29)) //29. láb
    {
      Serial.print("GOMB29");
    }

    if (!digitalRead(31)) //31. láb
    {
      Serial.print("GOMB31");
    }

    //    if (! ((PioC & 1u << 1) >> 1)) //33. láb
    if (!digitalRead(33)) //33. láb
    {
      Serial.print("GOMB33");
    }
    //if (! ((PioC & 1u << 3) >> 3)) //35. láb
    if (!digitalRead(35)) //35. láb
    {
      Serial.print("GOMB35");
    }
    //  if (! ((PioC & 1u << 5) >> 5)) //41. láb
    if (!digitalRead(37)) //37. láb
    {
      if (!note[20 + i]) {
        Serial.print(NOTENAME[i] + "0 ON    ");
        MIDI3.sendNoteOn (20 + i, 127, 1);
        note[20 + i] = true;
      }
    } else {
      if (note[20 + i]) {
        Serial.print(NOTENAME[i] + "0 OFF    ");
        MIDI3.sendNoteOff(20 + i, 0, 1);
        note[20 + i] = false;
      }

    }

    // if (! ((PioC & 1u << 7) >> 7)) //39. láb
    if (!digitalRead(39)) //39. láb
    {
      if (!note[37 + i]) {
        kiir(NOTENAME[i], "1 ON    ");
        MIDI3.sendNoteOn(37 + i, 127, 1);
        note[37 + i] = true;
      }
    } else {
      if (note[37 + i]) {
        kiir(NOTENAME[i], "1 OFF   ");
        MIDI3.sendNoteOff(37 + i, 0, 1);
        note[37 + i] = false;
      }
    }


    //   if (! ((PioC & 1u << 9) >> 9)) //41. láb
    if (!digitalRead(41)) //41. láb
    {
      if (!note[49 + i]) {
        kiir(NOTENAME[i], "2 ON    ");
        MIDI3.sendNoteOn(49 + i, 127, 1);
        note[49 + i] = true;
      }
    } else {
      if (note[49 + i]) {
        kiir(NOTENAME[i], "2_OFF   ");
        MIDI3.sendNoteOff(49 + i, 0, 1);
        note[49 + i] = false;
      }
    }

    ////!!!
    //  if (! ((PioC & 1u << 20) >> 20)) //43. láb
    if (!digitalRead(43)) //43. láb
    {
      if (!note[61 + i]) {
        kiir(NOTENAME[i], "3 ON    ");
        MIDI3.sendNoteOn(61 + i, 127, 1);
        note[61 + i] = true;
      }
    } else {
      if (note[61 + i]) {
        kiir(NOTENAME[i], "3 OFF   ");
        MIDI3.sendNoteOff(61 + i, 0, 1);
        note[61 + i] = false;
      }
    }
    ////!!!!


    //  if (! ((PioC & 1u << 18) >> 18)) //45. láb
    if (!digitalRead(45)) //45. láb
    {
      if (!note[73 + i]) {
        kiir(NOTENAME[i], "4 ON    ");

        MIDI3.sendNoteOn(73 + i, 127, 1);
        note[73 + i] = true;
      }
    } else {
      if (note[73 + i]) {
        kiir(NOTENAME[i], "4 OFF   ");
        MIDI3.sendNoteOff(73 + i, 0, 1);
        note[73 + i] = false;
      }
    }
    // if (! ((PioC & 1u << 16) >> 16)) //47. láb
    if (!digitalRead(47)) //47. láb
    {
      if (!note[85 + i]) {

        kiir(NOTENAME[i], "5 ON    ");
        MIDI3.sendNoteOn(85 + i, 127, 1);
        note[85 + i] = true;
      }
    } else {
      if (note[85 + i]) {
        kiir(NOTENAME[i], "5 OFF    ");
        MIDI3.sendNoteOff(85 + i, 0, 1);
        note[85 + i] = false;
      }
    }

    // ROWS[1] = (PioC & 1u << 16) >> 16;  //  47. gomb
    // ROWS[2] = (PioC & 1u << 14) >> 14;  //  49. gomb
    // ROWS[2] = (PioC & 1u << 13) >> 13;  //  50. gomb
    // ROWS[3] = (PioC & 1u << 12) >> 12;  //  51. gomb


    /*
      delay(50);
      ROWS[0] = (PioC & 1u << 18) >> 18;  //  45. gomb
      ROWS[1] = (PioC & 1u << 16) >> 16;  //  47. gomb
      ROWS[2] = (PioC & 1u << 14) >> 14;  //  49. gomb
      ROWS[3] = (PioC & 1u << 12) >> 12;  //  51. gomb

      if ((ROWS[0] == LOW) && (ROWS[1] == HIGH) && (ROWS[2] == HIGH) && (ROWS[3] == HIGH)) {
      Serial.println(i*4+1);
      }
      if ((ROWS[0] == HIGH) && (ROWS[1] == LOW) && (ROWS[2] == HIGH) && (ROWS[3] == HIGH)) {
      Serial.println(i*4+2);
      }
      if ((ROWS[0] == HIGH) && (ROWS[1] == HIGH) && (ROWS[2] == LOW) && (ROWS[3] == HIGH)) {

      Serial.println(i*4+3);
      }
      if ((ROWS[0] == HIGH) && (ROWS[1] == HIGH) && (ROWS[2] == HIGH) && (ROWS[3] == LOW)) {
      Serial.println(i*4+4);
    */
    //delay(1);

  }
}

// Serial.println("IIIIIIII54IIIIII");

void kiir(String szoveg, String szam) {
  String kiirszoveg = szoveg + szam;
  Serial.print(kiirszoveg + "\n");
 lcd.setCursor(0, 1);
 lcd.print(kiirszoveg);

}
