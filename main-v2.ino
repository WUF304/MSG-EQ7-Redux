#include <Time.h>
#include <Wire.h>
#include <TimeLib.h>
#include <DS1307RTC.h>
#include <Adafruit_NeoPixel.h>
#include <avr/power.h>

#define ONE_HZ_SW 6 // one Hz square wave from Ds1307
#define blinkPin 13
#define Sw0 10
#define Sw1 8
#define Sw2 9

#define N_PIXELS  51  // Number of pixels you are using
#define LED_PIN    5  // NeoPixel LED strand is connected to GPIO #0 / D0
#define POT_PIN    A1  // if defined, a potentiometer is on GPIO #3 (A3, Trinket only) 

const int analogPinL = A0; // MSGEQ7 OUT
const int strobePinL = 2; // MSGEQ7 STROBE
const int ClockPin = A2;
const int resetPinL = 4; // MSGEQ7 RESET
int spectrumValueL[6];
int spectrumAvgL;
int SwitchMode;
int SpecOne, SpecFour, SpecFive;

int Minutes;
int Seconds;
int i;

int  heightL;

int filterValue = 80;

Adafruit_NeoPixel  strip = Adafruit_NeoPixel(N_PIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {

   tmElements_t tm;



  Wire.begin(); // join i2c bus (address optional for master)
  digitalWrite(blinkPin, 0);



  pinMode(ONE_HZ_SW, INPUT);
  pinMode(Sw0, INPUT);  // for this use a slide switch
  pinMode(Sw1, INPUT);  // N.O. push button switch
  pinMode(Sw2, INPUT);  // N.O. push button switch

  digitalWrite(ONE_HZ_SW, HIGH);
  digitalWrite(Sw0, HIGH); // pull-ups on
  digitalWrite(Sw1, HIGH);
  digitalWrite(Sw2, HIGH);

  strip.begin();

//__________MSGEQ7 Section_________
Serial.begin(9600);
  // Read from MSGEQ7 OUT
  pinMode(POT_PIN, INPUT);
  pinMode(analogPinL, INPUT);
  pinMode(ClockPin, INPUT);

  // Write to MSGEQ7 STROBE and RESET
  pinMode(strobePinL, OUTPUT);
  pinMode(resetPinL, OUTPUT);


  // Set analogPin's reference voltage
  analogReference(DEFAULT); // 5V

  // Set startup values for pins
  digitalWrite(resetPinL, LOW);
  digitalWrite(strobePinL, HIGH);

//__________________________________
}
void loop()
{

uint8_t bright = 255;   
#ifdef POT_PIN            
   bright = analogRead(POT_PIN);  // Read pin (0-255) (adjust potentiometer 
                                  //   to give 0 to Vcc volts
#endif
  strip.setBrightness(bright);    // Set LED brightness (if POT_PIN at top
                                  //  define commented out, will be full)

   SwitchMode = analogRead(ClockPin);
 //  Serial.print("Clock Switch Reads  ");
  // Serial.print(SwitchMode);
 //  Serial.print("   ");


  //_________Spectrum Values Section______________
// Set reset pin low to enable strobe
  digitalWrite(resetPinL, HIGH);
  digitalWrite(resetPinL, LOW);


  // Get all 7 spectrum values from the MSGEQ7
  for (int i = 0; i < 7; i++)
  {
    digitalWrite(strobePinL, LOW);
    delayMicroseconds(30); // Allow output to settle

    spectrumValueL[i] = analogRead(analogPinL);


    // Constrain any value above 1023 or below filterValue
    spectrumValueL[i] = constrain(spectrumValueL[i], filterValue, 1023);


    // Remap the value to a number between 0 and 255
    spectrumValueL[i] = map(spectrumValueL[i], filterValue, 1023, 0, 255);

  // Serial.print(spectrumValueL[i]);
 //  Serial.print(" ");

    digitalWrite(strobePinL, HIGH);
   }


   spectrumAvgL = ((spectrumValueL[1]+spectrumValueL[2]+spectrumValueL[3]+spectrumValueL[4]+spectrumValueL[5]+spectrumValueL[6]+spectrumValueL[7])/7);

// Serial.println(spectrumAvgL);
 // Serial.print("   ");

//_____________________________________________________________


  uint8_t  i;


heightL = map(heightL, 0, 255, 0, 10); */
SpecOne = map(spectrumValueL[1], 0, 255, 0, 25);
SpecFour = map(spectrumValueL[4], 0, 255, 0, 25);
SpecFive = map(spectrumValueL[5], 0, 255, 0, 25);

//Serial.print(SpecOne);
//Serial.print("  ");
//Serial.print(SpecFour);
//Serial.print("  ");
//Serial.println(SpecFive);

//Serial.print(heightL);
    for(i=0; i<= 23; i++)
    {
      // above all
     if((i >= SpecOne) && (i >= SpecFour) && (i >= SpecFive))  
     {   
       strip.setPixelColor(i, 0, 0, 0);
       strip.setPixelColor(i+27, 0, 0, 0);
       strip.setPixelColor(24, 0, 0, 0);
       strip.setPixelColor(25, 0, 0, 0);
       strip.setPixelColor(26, 0, 0, 0);
     }
     // one by itself (1)
    else if((i <= SpecOne) && (i > SpecFour) && (i > SpecFive))
    {
       strip.setPixelColor(i, spectrumValueL[1], spectrumValueL[4]/50, spectrumValueL[5]/50);
       strip.setPixelColor(i+27, spectrumValueL[1], spectrumValueL[4]/50, spectrumValueL[5]/50);
    }
    // one overlapped by four but not five (1+4)
    else if((i <= SpecOne) && (i <= SpecFour) && (i > SpecFive))
    {
      strip.setPixelColor(i, spectrumValueL[1]/2, spectrumValueL[4]/2, spectrumValueL[5]/45);
      strip.setPixelColor(i+27, spectrumValueL[1]/2, spectrumValueL[4]/2, spectrumValueL[5]/45);
    }
    // one overlapped by five but not four (1+5)
    else if((i <= SpecOne) && (i <= SpecFive) && (i > SpecFour))
    {
      strip.setPixelColor(i, spectrumValueL[1]/2, spectrumValueL[4]/45, spectrumValueL[5]/2);
      strip.setPixelColor(i+27, spectrumValueL[1]/2, spectrumValueL[4]/45, spectrumValueL[5]/2);
    }
    // one overlapped by four and five (ALL OVERLAP) (1+4+5)
    else if((i <= SpecOne) && (i <= SpecFour) && (i <= SpecFive))
    {
        if((spectrumValueL[1] > spectrumValueL[4]) && (spectrumValueL[1] > spectrumValueL[5]))
        {
           strip.setPixelColor(i, spectrumValueL[1], spectrumValueL[4]/30, spectrumValueL[5]/30);
           strip.setPixelColor(i+27, spectrumValueL[1], spectrumValueL[4]/30, spectrumValueL[5]/30);
        }
        else if((spectrumValueL[4] > spectrumValueL[1]) && (spectrumValueL[4] > spectrumValueL[5]))
        {
           strip.setPixelColor(i, spectrumValueL[1]/30, spectrumValueL[4], spectrumValueL[5]/30);
           strip.setPixelColor(i+27, spectrumValueL[1]/30, spectrumValueL[4], spectrumValueL[5]/30);
        } 
        else if((spectrumValueL[5] > spectrumValueL[4]) && (spectrumValueL[7] > spectrumValueL[1]))
        {
           strip.setPixelColor(i, spectrumValueL[1]/30, spectrumValueL[4]/30, spectrumValueL[5]);
           strip.setPixelColor(i+27, spectrumValueL[1]/30, spectrumValueL[4]/30, spectrumValueL[5]);
        } 
    }
    // four by itself (4)
    else if((i <= SpecFour) && (i > SpecOne) && (i > SpecFive))
    {
      strip.setPixelColor(i, 0, spectrumValueL[4], 0);
      strip.setPixelColor(i+27, 0, spectrumValueL[4], 0);
    }
    // four overlapped by five but not one (4+5)
    else if((i <= SpecFour) && (i <= SpecOne) && (i > SpecOne))
    {
      strip.setPixelColor(i, 0, spectrumValueL[4]/2, spectrumValueL[5]/2);
      strip.setPixelColor(i+27, 0, spectrumValueL[4]/2, spectrumValueL[5]/2);
    }
    // five by itself (5)
    else if((i <= SpecFive) && (i > SpecOne) && (i > SpecFour))
    {
      strip.setPixelColor(i, 0, 0, spectrumValueL[5]);
      strip.setPixelColor(i+27, 0, 0, spectrumValueL[5]);
    }
    }

uint32_t color = strip.getPixelColor(0);

       strip.setPixelColor(24, color);
       strip.setPixelColor(25, color);
       strip.setPixelColor(26, color);

   strip.show(); // Update strip
//delay(30);

while (spectrumAvgL <= 10)
   {
    for(i=0; i<= 52; i++)
    {
       strip.setPixelColor(i,   0,   0, 0);
       strip.setPixelColor(24, 0, 0, 0);
       strip.setPixelColor(25, 0, 0, 0);
       strip.setPixelColor(26, 0, 0, 0);
    }
    strip.show();

      digitalWrite(resetPinL, HIGH);
  digitalWrite(resetPinL, LOW);


  // Get all 7 spectrum values from the MSGEQ7
  for (int i = 0; i < 7; i++)
  {
    digitalWrite(strobePinL, LOW);
    delayMicroseconds(30); // Allow output to settle

    spectrumValueL[i] = analogRead(analogPinL);


    // Constrain any value above 1023 or below filterValue
    spectrumValueL[i] = constrain(spectrumValueL[i], filterValue, 1023);


    // Remap the value to a number between 0 and 255
    spectrumValueL[i] = map(spectrumValueL[i], filterValue, 1023, 0, 255);


  // Remove serial stuff after debugging
  Serial.println(spectrumValueL[i]);
  Serial.print(" ");

    digitalWrite(strobePinL, HIGH);
   }


   spectrumAvgL = ((spectrumValueL[1]+spectrumValueL[2]+spectrumValueL[3]+spectrumValueL[4]+spectrumValueL[5]+spectrumValueL[6]+spectrumValueL[7])/7);
   Serial.print("Average is -->  ");
   Serial.println(spectrumAvgL);
   }

  while (SwitchMode > 500)
  {
  ClockMode();
    }         
}                        

void print2digits(int number) {
  if (number >= 0 && number < 10) {
    Serial.write('0');
  }
  Serial.print(number);
}

// toggle the state on a pin
void toggle(int pinNum) 
{  
  int pinState = digitalRead(pinNum);
  pinState = !pinState;
  digitalWrite(pinNum, pinState); 
}
//End of Loop


//Functions
void ClockMode()
{

    uint8_t bright = 255;   
#ifdef POT_PIN            
   bright = analogRead(POT_PIN);  // Read pin (0-255) (adjust potentiometer 
                                  //   to give 0 to Vcc volts
#endif
  strip.setBrightness(bright);    // Set LED brightness (if POT_PIN at top
                                  //  define commented out, will be full)

   tmElements_t tm;

  if (RTC.read(tm)) {
    Serial.print("Ok, Time = ");
    print2digits(tm.Hour);
    Serial.write(':');
    print2digits(tm.Minute);
    Serial.write(':');
    print2digits(tm.Second);
    Serial.print(", Date (D/M/Y) = ");
    Serial.print(tm.Day);
    Serial.write('/');
    Serial.print(tm.Month);
    Serial.write('/');
    Serial.print(tmYearToCalendar(tm.Year));
    Serial.println();
  } else {
    if (RTC.chipPresent()) {
      Serial.println("The DS1307 is stopped.  Please run the SetTime");
      Serial.println("example to initialize the time and begin running.");
      Serial.println();
    } else {
      Serial.println("DS1307 read error!  Please check the circuitry.");
      Serial.println();
    }
    delay(9000);
  }
  delay(1000);

   if (!(digitalRead(Sw0))) set_time(); // hold the switch to set time
    while(digitalRead(ONE_HZ_SW)); // wait for low

    toggle(blinkPin);

for(i = 0; i <= 23; i++)
{
       if(i >= tm.Hour)  
     {   
       strip.setPixelColor(i+27,   0,   0, 0);
     }
       else
     {
      strip.setPixelColor(i+27, tm.Minute/3, tm.Minute/3, tm.Minute/3);
     }
}

Minutes = map(tm.Minute, 0, 60, 0, 24);

for(i = 0; i <= 23; i++)  
{
      if(i >= Seconds)  
     {   
       strip.setPixelColor(i,   0,   0, 0);
     }
      else
     {
      strip.setPixelColor(i, tm.Second/3, tm.Second/3, tm.Second/3);
     }
}

Seconds = map(tm.Second, 0, 59, 0, 23);

for(i = 0; i <= 3; i++)  
{
      if(i >= Seconds)  
     {   
       strip.setPixelColor(i+24,   0,   0, 0);
     }
       else
     {
      strip.setPixelColor(i+24, tm.Second/3, tm.Second/3, tm.Second/3);
     }
}
     strip.show();
     SwitchMode = analogRead(ClockPin);
}

void set_time()   {
  byte minutes = 0;
  byte hours = 0;

  while (!digitalRead(Sw0))  // set time switch must be released to exit
  {
    while (!digitalRead(Sw1)) // set minutes
    { 
      minutes++;          
      if ((minutes & 0x0f) > 9) minutes = minutes + 6;
      if (minutes > 0x59) minutes = 0;
      Serial.print("Minutes = ");
      if (minutes >= 9) Serial.print("0");
      Serial.println(minutes, HEX);

      delay(250);
    }

    while (!digitalRead(Sw2)) // set hours
    { 
      hours++;          
      if ((hours & 0x0f) > 9) hours =  hours + 6;
      if (hours > 0x23) hours = 0;
      Serial.print("Hours = ");
      if (hours <= 9) Serial.print("0");
      Serial.println(hours, HEX);
      delay(250);
    }

    Wire.beginTransmission(0x68); // activate DS1307
    Wire.write(0); // where to begin
    Wire.write(0x00);          //seconds
    Wire.write(minutes);          //minutes
    Wire.write(0x80 | hours);    //hours (24hr time)
    Wire.write(0x06);  // Day 01-07
    Wire.write(0x16);  // Date 0-31
    Wire.write(0x11);  // month 0-12
    Wire.write(0x13);  // Year 00-99
    Wire.write(0x10); // Control 0x10 produces a 1 HZ square wave on pin 7. 
    Wire.endTransmission();
  }    
    SwitchMode = analogRead(ClockPin);
   }
