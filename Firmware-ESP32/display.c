#include "display.h"
#include "arduino.h"

// pinouts for common cathode 7 segment LED display:

// segment is active high
// segment | ESP32 pin
#define SEG_A1  18
#define SEG_B1  19
#define SEG_C1  27
#define SEG_D1  14
#define SEG_E1  12
#define SEG_F1  17
#define SEG_G1  5
//#define SEG_DP1 26

#define SEG_A2  22
#define SEG_B2  23
#define SEG_C2  26
#define SEG_D2  33
#define SEG_E2  25
#define SEG_F2  21
#define SEG_G2  32
//#define SEG_DP2 34

// digit is active low
// digit | ESP32 pin
#define DC_1  13
#define DC_2  16
#define DC_3  4
#define DC_4  0
#define DC_5  2
#define DC_6  23

struct Time_Digits {
   byte dig1;
   byte dig2;
   byte dig3;
   byte dig4;
   byte dig5;
   byte dig6;
   byte dig7;
   byte dig8;
   byte dig9;
   byte dig10;
}; 

struct Time_Digits display_time;

void setTime(int unix_time)
{
  // loop through the time and set each digit
  
  // get lowest digit
  display_time.dig10 = unix_time % 10;
  // shift over
  unix_time = unix_time / 10;
  // get next lowest digit
  display_time.dig9 = unix_time % 10;
  // shift over agin for the next round
  unix_time = unix_time / 10;

  // loop over each digit

  display_time.dig8 = unix_time % 10;
  unix_time = unix_time / 10;
  display_time.dig7 = unix_time % 10;
  unix_time = unix_time / 10;
  display_time.dig6 = unix_time % 10;
  unix_time = unix_time / 10;
  display_time.dig5 = unix_time % 10;
  unix_time = unix_time / 10;
  display_time.dig4 = unix_time % 10;
  unix_time = unix_time / 10;
  display_time.dig3 = unix_time % 10;
  unix_time = unix_time / 10;
  display_time.dig2 = unix_time % 10;
  unix_time = unix_time / 10;
  display_time.dig1 = unix_time % 10;
}

// set display pins to outputs
void displaySetup()
{
  pinMode(SEG_A1, OUTPUT);
  pinMode(SEG_B1, OUTPUT);
  pinMode(SEG_C1, OUTPUT);
  pinMode(SEG_D1, OUTPUT);
  pinMode(SEG_E1, OUTPUT);
  pinMode(SEG_F1, OUTPUT);
  pinMode(SEG_G1, OUTPUT);
  //pinMode(SEG_DP1, OUTPUT);

  pinMode(SEG_A2, OUTPUT);
  pinMode(SEG_B2, OUTPUT);
  pinMode(SEG_C2, OUTPUT);
  pinMode(SEG_D2, OUTPUT);
  pinMode(SEG_E2, OUTPUT);
  pinMode(SEG_F2, OUTPUT);
  pinMode(SEG_G2, OUTPUT);
  //pinMode(SEG_DP2, OUTPUT);

  pinMode(DC_1, OUTPUT);
  pinMode(DC_2, OUTPUT);
  pinMode(DC_3, OUTPUT);
  pinMode(DC_4, OUTPUT);
  pinMode(DC_5, OUTPUT);
  pinMode(DC_6, OUTPUT);
}

// cycle through and display the time across all digits
void displayTime(void)
{
  /*
  // go from digit 5 to digit 1
  for (int display = 5; display > 0; display--)
  {
    // get lowest digit
    int timeSlice2 = time % 10;
    // shift over
    time = time / 10;
    // get next lowest digit
    int timeSlice1 = time % 10;
    // shift over agin for the next round
    time = time / 10;

    // show those digits on the display
    selectDisplay(display, timeSlice1, timeSlice2);
  }
  */
  selectDisplay(1, display_time.dig1, display_time.dig2);
  selectDisplay(2, display_time.dig3, display_time.dig4);
  selectDisplay(3, display_time.dig5, display_time.dig6);
  selectDisplay(4, display_time.dig7, display_time.dig8);
  selectDisplay(5, display_time.dig9, display_time.dig10);
}

// display a number on the selected display
// display can be between 1-5
// numbers can be between 0-9
void selectDisplay(int display, int number1, int number2)
{
  // turn all digits off
  digitalWrite(DC_1, LOW);
  digitalWrite(DC_2, LOW);
  digitalWrite(DC_3, LOW);
  digitalWrite(DC_4, LOW);
  digitalWrite(DC_5, LOW);
  //digitalWrite(DC_6, LOW);

  // set up segments while digits off,
  // so that the full digit comes up simultanously
  // (although who is going to be able to detect a couple clk cycles?)
  resetSegments();
  setSegment1(number1);
  setSegment2(number2);

  // then turn on specfic digit
  switch(display)
  {
    case 1:
      digitalWrite(DC_1, HIGH);
      break;
    case 2:
      digitalWrite(DC_2, HIGH);
      break;
    case 3:
      digitalWrite(DC_3, HIGH);
      break;
    case 4:
      digitalWrite(DC_4, HIGH);
      break;
    case 5:
      digitalWrite(DC_5, HIGH);
      break;
    //default:
      // bad - should't be here
  }
}

// reset all segments
void resetSegments(void)
{
  digitalWrite(SEG_A1, LOW);
  digitalWrite(SEG_B1, LOW);
  digitalWrite(SEG_C1, LOW);
  digitalWrite(SEG_D1, LOW);
  digitalWrite(SEG_E1, LOW);
  digitalWrite(SEG_F1, LOW);
  digitalWrite(SEG_G1, LOW);

  digitalWrite(SEG_A2, LOW);
  digitalWrite(SEG_B2, LOW);
  digitalWrite(SEG_C2, LOW);
  digitalWrite(SEG_D2, LOW);
  digitalWrite(SEG_E2, LOW);
  digitalWrite(SEG_F2, LOW);
  digitalWrite(SEG_G2, LOW);
}

// display a number on the first 7 segment display
// number can be between 0-9
void setSegment1(int number)
{
  // set the coorespoding segments
  switch(number)
  {
    case 0:
      // A B C D E F
      digitalWrite(SEG_A1, HIGH);
      digitalWrite(SEG_B1, HIGH);
      digitalWrite(SEG_C1, HIGH);
      digitalWrite(SEG_D1, HIGH);
      digitalWrite(SEG_E1, HIGH);
      digitalWrite(SEG_F1, HIGH);
      break;
    case 1:
      // B C
      digitalWrite(SEG_B1, HIGH);
      digitalWrite(SEG_C1, HIGH);
      break;
    case 2:
      // A B D E G
      digitalWrite(SEG_A1, HIGH);
      digitalWrite(SEG_B1, HIGH);
      digitalWrite(SEG_D1, HIGH);
      digitalWrite(SEG_E1, HIGH);
      digitalWrite(SEG_G1, HIGH);
      break;
    case 3:
      // A B C D G
      digitalWrite(SEG_A1, HIGH);
      digitalWrite(SEG_B1, HIGH);
      digitalWrite(SEG_C1, HIGH);
      digitalWrite(SEG_D1, HIGH);
      digitalWrite(SEG_G1, HIGH);
      break;
    case 4:
      // B C F G
      digitalWrite(SEG_B1, HIGH);
      digitalWrite(SEG_C1, HIGH);
      digitalWrite(SEG_F1, HIGH);
      digitalWrite(SEG_G1, HIGH);
      break;
    case 5:
      // A C D F G
      digitalWrite(SEG_A1, HIGH);
      digitalWrite(SEG_C1, HIGH);
      digitalWrite(SEG_D1, HIGH);
      digitalWrite(SEG_F1, HIGH);
      digitalWrite(SEG_G1, HIGH);
      break;
    case 6:
      // A C D E F G
      digitalWrite(SEG_A1, HIGH);
      digitalWrite(SEG_C1, HIGH);
      digitalWrite(SEG_D1, HIGH);
      digitalWrite(SEG_E1, HIGH);
      digitalWrite(SEG_F1, HIGH);
      digitalWrite(SEG_G1, HIGH);
      break;
    case 7:
      // A B C
      digitalWrite(SEG_A1, HIGH);
      digitalWrite(SEG_B1, HIGH);
      digitalWrite(SEG_C1, HIGH);
      break;
    case 8:
      // A B C D E F G
      digitalWrite(SEG_A1, HIGH);
      digitalWrite(SEG_B1, HIGH);
      digitalWrite(SEG_C1, HIGH);
      digitalWrite(SEG_D1, HIGH);
      digitalWrite(SEG_E1, HIGH);
      digitalWrite(SEG_F1, HIGH);
      digitalWrite(SEG_G1, HIGH);
      break;
    case 9:
      // A B C D F G
      digitalWrite(SEG_A1, HIGH);
      digitalWrite(SEG_B1, HIGH);
      digitalWrite(SEG_C1, HIGH);
      digitalWrite(SEG_D1, HIGH);
      digitalWrite(SEG_F1, HIGH);
      digitalWrite(SEG_G1, HIGH);
      break;
  }
}

// display a number on the second 7 segment display
// number can be between 0-9
void setSegment2(int number)
{
  // set the coorespoding segments
  switch(number)
  {
    case 0:
      // A B C D E F
      digitalWrite(SEG_A2, HIGH);
      digitalWrite(SEG_B2, HIGH);
      digitalWrite(SEG_C2, HIGH);
      digitalWrite(SEG_D2, HIGH);
      digitalWrite(SEG_E2, HIGH);
      digitalWrite(SEG_F2, HIGH);
      break;
    case 1:
      // B C
      digitalWrite(SEG_B2, HIGH);
      digitalWrite(SEG_C2, HIGH);
      break;
    case 2:
      // A B D E G
      digitalWrite(SEG_A2, HIGH);
      digitalWrite(SEG_B2, HIGH);
      digitalWrite(SEG_D2, HIGH);
      digitalWrite(SEG_E2, HIGH);
      digitalWrite(SEG_G2, HIGH);
      break;
    case 3:
      // A B C D G
      digitalWrite(SEG_A2, HIGH);
      digitalWrite(SEG_B2, HIGH);
      digitalWrite(SEG_C2, HIGH);
      digitalWrite(SEG_D2, HIGH);
      digitalWrite(SEG_G2, HIGH);
      break;
    case 4:
      // B C F G
      digitalWrite(SEG_B2, HIGH);
      digitalWrite(SEG_C2, HIGH);
      digitalWrite(SEG_F2, HIGH);
      digitalWrite(SEG_G2, HIGH);
      break;
    case 5:
      // A C D F G
      digitalWrite(SEG_A2, HIGH);
      digitalWrite(SEG_C2, HIGH);
      digitalWrite(SEG_D2, HIGH);
      digitalWrite(SEG_F2, HIGH);
      digitalWrite(SEG_G2, HIGH);
      break;
    case 6:
      // A C D E F G
      digitalWrite(SEG_A2, HIGH);
      digitalWrite(SEG_C2, HIGH);
      digitalWrite(SEG_D2, HIGH);
      digitalWrite(SEG_E2, HIGH);
      digitalWrite(SEG_F2, HIGH);
      digitalWrite(SEG_G2, HIGH);
      break;
    case 7:
      // A B C
      digitalWrite(SEG_A2, HIGH);
      digitalWrite(SEG_B2, HIGH);
      digitalWrite(SEG_C2, HIGH);
      break;
    case 8:
      // A B C D E F G
      digitalWrite(SEG_A2, HIGH);
      digitalWrite(SEG_B2, HIGH);
      digitalWrite(SEG_C2, HIGH);
      digitalWrite(SEG_D2, HIGH);
      digitalWrite(SEG_E2, HIGH);
      digitalWrite(SEG_F2, HIGH);
      digitalWrite(SEG_G2, HIGH);
      break;
    case 9:
      // A B C D F G
      digitalWrite(SEG_A2, HIGH);
      digitalWrite(SEG_B2, HIGH);
      digitalWrite(SEG_C2, HIGH);
      digitalWrite(SEG_D2, HIGH);
      digitalWrite(SEG_F2, HIGH);
      digitalWrite(SEG_G2, HIGH);
      break;
  }
}
