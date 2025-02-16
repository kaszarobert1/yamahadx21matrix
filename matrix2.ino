/*
  DX21 INPUT
  keyboard matrix input (6db):
  br re or ye gr bl
  37 39 41 43 45 47

  keyboard matrix output(12db):
  br re or ye gr bl pr gr wh gg sb pn
  28 30 32 34 36 38 40 42 44 46 48 50

  button matrix 1 input
  wh gg sb pk
  3  4  5  6
  button matrix 1 output
  br re or ye gr bl gr vi!!!
  13 12 11 10 9  8  7  2!

  button matrix 2 input
  br re or ye gr bl
  25 27 29 31 33 35

  button matrix 2 output
  vi gy
  49 51
*/

#include <MIDI.h>
//#include <MIDIUSB.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
#include <DueFlashStorage.h>
DueFlashStorage dueFlashStorage;
MIDI_CREATE_INSTANCE(HardwareSerial, Serial3, MIDI3);
bool note[128];
bool button1[64];
bool button2[64];
int menu = 0;
int lastmenu = 0;
int menupages = 0;
int lastmenupages = 0;

byte oldvalues[128][4];
byte values[128][4];
int kiirvalue = 0;
String lrowstring[64][4];
String lastlrowstring;
int tune = 13;
bool ROWS[12];
int COLS[12] = {28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50};
int BUTTON1COLS[4] = {3, 4, 5, 6};
int BUTTON2COLS[7] = {25, 27, 29, 31, 33, 35};
String NOTENAME[14] = { "CISZ", "D", "DISZ", "E", "F", "FISZ", "G", "GISZ", "A", "B", "H", "C", "CISZ","D"};
uint64_t  PioC;
long elozoido = 0;
long mostido = 0;
byte upperlower = 0;
String upperlowerstring[4] = {"L1: ", "L2: ", "U1: ", "U2: "};

//menunumber: 3 7 11 15 19 23 27 31 2 6 10 14 18 22 26 30
//            1 5 9  13 17 21 25 29 0 4 8  12 16 20 24 28
String menustring[64] = {"NO", "Save Yes/No", "ENV-MODE", "LOWER1", "NO", "NO", "LEVEL", "LOWER2", "NO", "NO", "LOOP-SAMPLE", "UPPER1", "LFO 4-5", "NO", "NO", "UPPER2", "EQ BIAS", "NO", "NO", "WAVEFORM", "EQ-Q CH-FREQ", "NO", "NO", "COARSE", "STEP-CHASE", "NO", "NO", "FINE", "EFFECT 2", "NO", "NO", "KEY FOLLOW", "NO", "NO", "NO", "LFO MODE", "NO", "NO", "NO", "ENV MODE", "NO", "NO", "NO", "BEND MODE", ""};
String Waveform[128] = {"marimba", "vibraphone", "xilophone1", "xilophone2", "logbass", "hammer", "japanesedrum", "kalimba", "pluck1", "chink", "agogo", "triangle", "bells", "pick", "lowpiano", "pianosample", "highpiano", "hapsichord", "harp", "organpercus", "steelstrings", "nylonstrings", "electgitar1", "electgitar2", "dirtygitar", "pickbass", "popbass", "thump", "klarinet", "breath", "popbass", "thump", "steamer", "steamer", "steamer", "steamer", "steamer", "steamer", "steamer", "steamer", "steamer", "steamer", "steamer", "steamer", "violins", "pidzicart", "drawbarsloop", "highorganloop", "loworganloop", "electpiano1loop", "electpiano2loop", "claviloop", "hapsichordloop", "electbassloop1", "acusticbassloop", "electbassloop2", "electbassloop3", "electgitarloop", "chelloloop", "violinloop", "reedloop", "saxloop1", "saxloop2", "aahloop", "oohloop", "maleloop", "spectrum1loop", "pectrum2loop"};
String Reverbstrings[32] = {"Small Hall", "Medium Hall", "Large Hall", "Chapel", "Box", "SmallMetalRoom", "Small Room", "Medium Room", "Md Large Room", "Large Room", "SingleDelay102ms", "CrossDelay180ms", "CrossDelay224ms", "CrossDelay148-256ms", "ShortGate200ms", "LongGate480ms", "Bright Hall", "Large Cave", "Steel Pan" "Delay248ms", "Delay338ms", "CrossDelay157ms" "CrossDelay252ms","CrossDelay274-137ms","Gate Recerb","Reverse Gate360ms","Reverse Gate480ms","Slap Back", "Slap Back", "Slap Back","Twisted Space","Space"};
String Keyfollow[17] = {"-1", "-1/2", "-1/4", "Fixed", "1/8", "1/4", "3/8", "1/2", "5/8", "3/4", "7/8", "NORMAL", "5/4", "3/2", "2", "s1", "s2"};

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
  lcd.print(" Firmvare: 0.43 ");

  delay(800);
  lcd.setCursor(0, 1);
  lcd.print("D50 Sampler Fir ");
  delay(400);
  lcd.setCursor(0, 0);
  lcd.print(" Yamaha DX-21.1 ");
  lcd.setCursor(0, 1);
  lcd.print("                ");
  delay(800);
  lcd.setCursor(0, 0);
  programload(0);
}

void loop() {
  keyboardscanner();
  button1scanner();
  button2scanner();
  long mostido = millis();
  if (mostido - elozoido > 100) {
    elozoido = mostido;
    if (button2[9] == true) {
      if (values[menupages][upperlower] < 100)
      {

        values[menupages][upperlower]++;
      } else {

        values[menupages][upperlower] = 0;
      }
       
    }
    if (button2[8] == true) {
      if (values[menupages][upperlower] > 0)
      {

        values[menupages][upperlower]--;
      } else {

        values[menupages][upperlower] = 100;
      }
    }
  }
  if (values[menupages][upperlower] != oldvalues[menupages][upperlower]) {
    midisendsysex();
    oldvalues[menupages][upperlower] = values[menupages][upperlower];
  }


  if (menu == 3) {
    upperlower = 0;
  }
  if (menu == 7) {
    upperlower = 1;
  }
  if (menu == 11) {
    upperlower = 2;
  }
  if (menu == 15) {
    upperlower = 3;
  }



  if (menu != lastmenu) {
    if (menu != 3 && menu != 7 && menu != 11 && menu != 15)
    {
      menupages = menu;
      kiir0("        ", "        ");
      kiir0(upperlowerstring[upperlower], menustring[menupages]);
      lastmenu = menu;
    } else
    {
      kiir0("        ", "        ");
      kiir0(upperlowerstring[upperlower], menustring[menupages]);
      lastmenu = menu;
    }
  }

  if (lrowstring[menupages][upperlower] != lastlrowstring) {
    kiir("        ", "        ");
    kiir("", lrowstring[menupages][upperlower]);
    lastlrowstring = lrowstring[menupages][upperlower];

  }

}


void programsave(byte saveprog) {
  //saveprog
  int kezdocim = saveprog * 256;
  for (int i = 0; i < 64; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      menupages = i;
      upperlower = j;
      dueFlashStorage.write(kezdocim + j * 64 + i, values[i][j]);
    }
  }
  kiir("        ", "        ");
  kiir("SAVE: ", String(saveprog));
  delay(500);
}


void programload(byte loadprog) {
  int kezdocim = loadprog * 256;
  for (int i = 0; i < 64; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      menupages = i;
      upperlower = j;
      if (menupages != 1) {
        values[i][j] = dueFlashStorage.read(kezdocim + j * 64 + i);
        midisendsysex();
      }
    }
  }
  kiir("        ", "        ");
  kiir("LOAD: ", String(loadprog));
  delay(200);
}
void  midisendsysex() {
  //--------------MENU--------------------------
  if (menupages == 1)
  {
    programsave(0);
  }
  byte sendvalue = values[menupages][upperlower];
  byte paritbit = 0;
  //eq
  if (menupages == 16)
  {
    if (upperlower == 0) {
      //eq1 bias
      midisysexsend(2, 88, sendvalue);
      lrowstring[menupages][upperlower] = "EQ1 LEVEL=" + String(values[menupages][upperlower]);
    }
    if (upperlower == 1) {
      //eq1 f0
      midisysexsend(2, 86, sendvalue);
      lrowstring[menupages][upperlower] = "EQ1 F0=" + String(values[menupages][upperlower]);
    }
    if (upperlower == 2) {
      //eq2 bias
      midisysexsend(1, 24, sendvalue);
      lrowstring[menupages][upperlower] = "EQ2 LEVEL=" + String(values[menupages][upperlower]);
    }
    if (upperlower == 3) {
      //eq2 f0
      midisysexsend(1, 22, sendvalue);
      lrowstring[menupages][upperlower] = "EQ2 F0=" + String(values[menupages][upperlower]);
    }
  }

  //waveform
  if (menupages == 19)
  {
    if (upperlower == 0) {
      midisysexsend(1, 71, sendvalue);
    }
    if (upperlower == 1) {
      midisysexsend(2, 7, sendvalue);
    }
    if (upperlower == 2) {
      midisysexsend(0, 7, sendvalue);
    }
    if (upperlower == 3) {
      midisysexsend(0, 71, sendvalue);
    }

    lrowstring[menupages][upperlower] = String(Waveform[values[menupages][upperlower]]);
  }
  //course
  if (menupages == 23)
  {
    if (upperlower == 0) {
      midisysexsend(1, 64, sendvalue);
    
    }
    if (upperlower == 1) {
        midisysexsend(2, 0, sendvalue);
      
    }
    if (upperlower == 2) {
        midisysexsend(0, 0, sendvalue);
     
    }
    if (upperlower == 3) {
        midisysexsend(0, 64, sendvalue);
      
    }
    String note = NOTENAME[values[menupages][upperlower] % 12];
    String octave = String(values[menupages][upperlower] / 12);
    lrowstring[menupages][upperlower] = note + octave;
  }

  //eq-q ch freq
  if (menupages == 20)
  {
    if (upperlower == 0) {
        midisysexsend(2, 87, sendvalue);
      
      lrowstring[menupages][upperlower] = "EQ1 Q=" + String(values[menupages][upperlower]);
    }
    if (upperlower == 1) {
        midisysexsend(1, 23, sendvalue);
      
      lrowstring[menupages][upperlower] = "EQ2 Q=" + String(values[menupages][upperlower]);
    }
    if (upperlower == 2) {
        midisysexsend(2, 107, sendvalue);
     
      lrowstring[menupages][upperlower] = "CH1 FREQ=" + String(values[menupages][upperlower]);
    }
    if (upperlower == 3) {
        midisysexsend(1, 43, sendvalue);
     
      lrowstring[menupages][upperlower] = "CH2 FREQ=" + String(values[menupages][upperlower]);
    }
  }

//LFO 4-5
if (menupages == 12)
  {
    if (upperlower == 0) {
        midisysexsend(2, 94, sendvalue);
      
      lrowstring[menupages][upperlower] = "LFO4 Freq=" + String(values[menupages][upperlower]);
    }
    if (upperlower == 1) {
        midisysexsend(2, 95, sendvalue);
   
      lrowstring[menupages][upperlower] = "LFO4 Level=" + String(values[menupages][upperlower]);
    }
    if (upperlower == 2) {
        midisysexsend(2, 98, sendvalue);
     
      lrowstring[menupages][upperlower] = "LFO5 Freq==" + String(values[menupages][upperlower]);
    }
    if (upperlower == 3) {
        midisysexsend(2, 99, sendvalue);
    
      lrowstring[menupages][upperlower] = "LFO5 Level=" + String(values[menupages][upperlower]);
    }
  }



//step
if (menupages == 24)
  {
    if (upperlower == 0) {
      if (sendvalue>15 && sendvalue<25)
        {midisysexsend(3, 27, sendvalue);
        
        }
     
      lrowstring[menupages][upperlower] = "Step=" + String(values[menupages][upperlower]);
    }
    if (upperlower == 1) {
        midisysexsend(3, 34, sendvalue);
      
      lrowstring[menupages][upperlower] = "Chase Mode=" + String(values[menupages][upperlower]);
    }
    if (upperlower == 2) {
        midisysexsend(3, 35, sendvalue);
      
      lrowstring[menupages][upperlower] = "Case Level==" + String(values[menupages][upperlower]);
    }
    if (upperlower == 3) {
        midisysexsend(3, 36, sendvalue);
     
      lrowstring[menupages][upperlower] = "Case Number=" + String(values[menupages][upperlower]);
    }
  }
  //fine
  if (menupages == 27)
  {
    if (upperlower == 0) {
        midisysexsend(1, 65, sendvalue);
      
    }
    if (upperlower == 1) {
        midisysexsend(2, 1, sendvalue);
     
    }
    if (upperlower == 2) {
        midisysexsend(0, 11, sendvalue);
     
    }
    if (upperlower == 3) {
        midisysexsend(0, 65, sendvalue);
    
    }
    lrowstring[menupages][upperlower] = String(values[menupages][upperlower]);
  }

  //keyfollow
  if (menupages == 31)
  {
    if (values[menupages][upperlower] > 16) {
      values[menupages][upperlower] = 0;
    }
    if (upperlower == 0) {
        midisysexsend(1, 66, sendvalue);
     
    }
    if (upperlower == 1) {
        midisysexsend(2, 2, sendvalue);
     
    }
    if (upperlower == 2) {
        midisysexsend(0, 2, sendvalue);
     
    }
    if (upperlower == 3) {
      
        midisysexsend(0, 66, sendvalue);
     
    }
    lrowstring[menupages][upperlower] = Keyfollow[values[menupages][upperlower]];
  }

//envmode
  if (menupages == 2)
  {
    if (upperlower == 0) {
        midisysexsend(1, 67, sendvalue);  
    }
    if (upperlower == 1) {
        midisysexsend(2, 4, sendvalue);
      
    }
    if (upperlower == 2) {
        midisysexsend(0, 4, sendvalue);
      
    }
    if (upperlower == 3) {
        midisysexsend(0, 68, sendvalue);
      
    }
    String note = NOTENAME[values[menupages][upperlower] % 12];
    
   lrowstring[menupages][upperlower] =  String(values[menupages][upperlower]);
  }

//level
  if (menupages == 6)
  {
    if (upperlower == 0) {
        midisysexsend(1, 99, sendvalue);
    
    }
    if (upperlower == 1) {
        midisysexsend(2, 35, sendvalue);
      
     
    }
    if (upperlower == 2) {
        midisysexsend(0, 35, sendvalue);
    
    }
    if (upperlower == 3) {
        midisysexsend(0, 99, sendvalue);
      
    }
    String note = NOTENAME[values[menupages][upperlower] % 12];
    
   lrowstring[menupages][upperlower] =  String(values[menupages][upperlower]);
  }

//loopsample
  if (menupages == 10)
  {
    if (upperlower == 0) {
        midisysexsend(1, 113, sendvalue);
      
    }
    if (upperlower == 1) {
        midisysexsend(2, 49, sendvalue);
     
    }
    if (upperlower == 2) {
  midisysexsend(0, 49, sendvalue);
      
    }
    if (upperlower == 3) {
        midisysexsend(0, 113, sendvalue);
      
    }
    String note = NOTENAME[values[menupages][upperlower] % 12];
    
   lrowstring[menupages][upperlower] =  String(values[menupages][upperlower]);
  }

  if (menupages == 28)
  {
    if (upperlower == 0) {
      if (values[menupages][upperlower] > 32) {
        values[menupages][upperlower] = 0;
      }
      //reverb alg
        midisysexsend(3, 30, sendvalue);
    
      lrowstring[menupages][upperlower] = Reverbstrings[values[menupages][upperlower]];
    }
    if (upperlower == 1) {
      //reverb level
        midisysexsend(3, 31, sendvalue);
      
      lrowstring[menupages][upperlower] = "REV LEVEL=" + String(values[menupages][upperlower]);
    }
    if (upperlower == 2) {
      //chorusLevelLeft
        midisysexsend(2, 108, sendvalue);
      
      lrowstring[menupages][upperlower] = "CH1 LEVEL=" + String(values[menupages][upperlower]);
    }
    if (upperlower == 3) {
      //chorusLevelRight
        midisysexsend(1, 44, sendvalue);
     
      lrowstring[menupages][upperlower] = "CH2 LEVEL=" + String(values[menupages][upperlower]);
    }
  }
}

//----------------------------------------------

void midisysexsend(byte local, byte controller, byte sysvalue) {
  byte paritbit = 0 ;
  byte  sysexArray[11] = {240, 65, 0, 20, 18, 0, local, controller, sysvalue, paritbit, 247};
  MIDI3.sendSysEx(11, sysexArray, true);
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
        // kiir("BUTTO2__ON :", String(i) );

        button2[i] = true;
      }
    } else {
      if (button2[i]) {
        // kiir("BUTTO2__OFF:", String(i));
        button2[i] = false;
      }
    }

    if (!digitalRead(51))
    {
      if (!button2[i + 6]) {
        //  kiir("BUTTO2__ON :", String(i + 6) );
        button2[i + 6] = true;
      }
    } else {
      if (button2[i + 6]) {
         // kiir("BUTTO2__OFF:", String(i + 6));
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
        //  kiir("BUTTON__ON :", String(i) );
        // MIDI3.sendNoteOn(tune, 127, 1);
        menu = i;
        button1[i] = true;
      }
    } else {
      if (button1[i]) {
        //kiir("BUTTON__OFF:", String(i));
        button1[i] = false;
      }
    }


    if (!digitalRead(12))
    {
      if (!button1[i + 4]) {
        // kiir("BUTTON__ON :" , String(i + 4));
        menu = i + 4;
        // MIDI3.sendNoteOn(tune, 127, 1);
        button1[i + 4] = true;
      }
    } else {
      if (button1[i + 4]) {

        // kiir("BUTTON__OFF:", String(i + 4) );

        button1[i + 4] = false;
      }
    }
    if (!digitalRead(11))
    {
      if (!button1[i + 8]) {
        //   kiir("BUTTON__ON :" , String(i + 8));
        menu = i + 8;
        // MIDI3.sendNoteOn(tune, 127, 1);
        button1[i + 8] = true;
      }
    } else {
      if (button1[i + 8]) {

        // kiir("BUTTON__OFF:" , String(i + 8));

        button1[i + 8] = false;
      }
    }


    if (!digitalRead(10))
    {
      if (!button1[i + 12]) {
        // kiir("BUTTON__ON :", String(i + 12)  );
        menu = i + 12;
        // MIDI3.sendNoteOn(tune, 127, 1);
        button1[i + 12] = true;
      }
    } else {
      if (button1[i + 12]) {

        // kiir("BUTTON__OFF:" , String(i + 12)  );

        button1[i + 12] = false;
      }
    }


    if (!digitalRead(9))
    {
      if (!button1[i + 16]) {
        // kiir("BUTTON__ON: " , String(i + 16));
        menu = i + 16;
        // MIDI3.sendNoteOn(tune, 127, 1);
        button1[i + 16] = true;
      }
    } else {
      if (button1[i + 16]) {

        // kiir("BUTTON__OFF:" , String(i + 16));

        button1[i + 16] = false;
      }
    }


    if (!digitalRead(8))
    {
      if (!button1[i + 20]) {
        //  kiir("BUTTON__ON: " , String(i + 20));
        menu = i + 20;
        // MIDI3.sendNoteOn(tune, 127, 1);
        button1[i + 20] = true;
      }
    } else {
      if (button1[i + 20]) {

        //   kiir("BUTTON__OFF:" , String(i + 20));

        button1[i + 20] = false;
      }
    }

    if (!digitalRead(2))
    {
      if (!button1[i + 24]) {
        // kiir("BUTTON__ON: " , String(i + 24)  );
        menu = i + 24;
        // MIDI3.sendNoteOn(tune, 127, 1);
        button1[i + 24] = true;
      }
    } else {
      if (button1[i + 24]) {

        // kiir("BUTTON__OFF:" , String(i + 24) );

        button1[i + 24] = false;
      }
    }

    if (!digitalRead(7))
    {
      if (!button1[i + 28]) {
        //  kiir("BUTTON__ON: " , String(i + 28)  );
        menu = i + 28;
        // MIDI3.sendNoteOn(tune, 127, 1);
        button1[i + 28] = true;
      }
    } else {
      if (button1[i + 28]) {

        // kiir("BUTTON__OFF:" , String(i + 28) );

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
        //   kiir(NOTENAME[i], "0 ON    ");
        //Serial.print(NOTENAME[i] + "0 ON    ");
        MIDI3.sendNoteOn (tune + i, 127, 1);
        note[20 + i] = true;
      }
    } else {
      if (note[20 + i]) {
        // kiir(NOTENAME[i], "0 OFF    ");
        //Serial.print(NOTENAME[i] + "0 OFF    ");
        MIDI3.sendNoteOff(tune + i, 0, 1);
        note[20 + i] = false;
      }
    }

    if (!digitalRead(39)) //39. láb
    {
      if (!note[37 + i]) {
        // kiir(NOTENAME[i], "1 ON    ");
        MIDI3.sendNoteOn(tune + 12 + i, 127, 1);
        note[37 + i] = true;
      }
    } else {
      if (note[37 + i]) {
        // kiir(NOTENAME[i], "1 OFF   ");
        MIDI3.sendNoteOff(tune + 12 + i, 0, 1);
        note[37 + i] = false;
      }
    }

    if (!digitalRead(41)) //41. láb
    {
      if (!note[49 + i]) {
        //kiir(NOTENAME[i], "2 ON    ");
        MIDI3.sendNoteOn(tune + 24 + i, 127, 1);
        note[49 + i] = true;
      }
    } else {
      if (note[49 + i]) {
        //kiir(NOTENAME[i], "2 OFF   ");
        MIDI3.sendNoteOff(tune + 24 + i, 0, 1);
        note[49 + i] = false;
      }
    }


    if (!digitalRead(43)) //43. láb
    {
      if (!note[61 + i]) {
        // kiir(NOTENAME[i], "3 ON    ");
        MIDI3.sendNoteOn(tune + 36 + i, 127, 1);
        note[61 + i] = true;
      }
    } else {
      if (note[61 + i]) {
        //  kiir(NOTENAME[i], "3 OFF   ");
        MIDI3.sendNoteOff(tune + 36 + i, 0, 1);
        note[61 + i] = false;
      }
    }

    if (!digitalRead(45)) //45. láb
    {
      if (!note[73 + i]) {
        //  kiir(NOTENAME[i], "4 ON    ");

        MIDI3.sendNoteOn(tune + 48 + i, 127, 1);
        note[73 + i] = true;
      }
    } else {
      if (note[73 + i]) {
        //  kiir(NOTENAME[i], "4 OFF   ");
        MIDI3.sendNoteOff(tune + 48 + i, 0, 1);
        note[73 + i] = false;
      }
    }

    if (!digitalRead(47)) //47. láb
    {
      if (!note[85 + i]) {

        // kiir(NOTENAME[i], "5 ON    ");
        MIDI3.sendNoteOn(tune + 60 + i, 127, 1);
        note[85 + i] = true;
      }
    } else {
      if (note[85 + i]) {
        //    kiir(NOTENAME[i], "5 OFF    ");
        MIDI3.sendNoteOff(tune + 60 + i, 0, 1);
        note[85 + i] = false;
      }
    }
  }
}
void kiir0(String szoveg, String szam) {
  String kiirszoveg = szoveg + szam;
  Serial.print(kiirszoveg + "\n");
  lcd.setCursor(0, 0);
  lcd.print(kiirszoveg);
}

void kiir(String szoveg, String szam) {
  String kiirszoveg = szoveg + szam;
  Serial.print(kiirszoveg + "\n");
  lcd.setCursor(0, 1);
  lcd.print(kiirszoveg);
}
