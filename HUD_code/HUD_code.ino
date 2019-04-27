/***************************
Code written by Bill Wong
Code edited by Aidas Lukosius
Intended for use with a 7-segment Heads-up Display
reflected off the car windshield

abnormalbill@gmail.com

bwongtech.blogspot.com
***************************/

#include <OBD2UART.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal.h>

//change this is you hooked up the 7-segment display to another pin:
const int ssegpin = 6;

SoftwareSerial sseg(8,ssegpin);//(input,output)
//we don't use the input pin, so the first number doesn't mattera

COBD obd;

int kph = 0;
float num;
char dig1;
char dig2;
char dig3;
char dig4;

void setup()
{
  pinMode(ssegpin,OUTPUT);
  sseg.begin(9600);
  
  //clear the display
  sseg.print("v");
  sseg.print("----");
  
  //set up decimal point for 000.0
  sseg.print("w");
  sseg.write(1);
  
  //set brightness
  sseg.print("z");
  sseg.write((byte)0);
  
  //wait for obd cable to respond
  while (!obd.init());
}

void loop()
{
if (obd.readPID(PID_SPEED,kph))//if the speed is available, store it in 'kph'
  {
    //convert kph to mph
    float newmph = kph * 0.6214 * 10;
    
    //round it to nearest tenth
    int temp = newmph;
    float temp2 = newmph - temp;
    if (temp2 >= 0.5) newmph += (1 - temp2);
    else newmph -= temp2;
    
    //send the number to the mirrorprint function
    mirrorprint(newmph);
  }
}

//the numbers must be mirrored because they are reflected on the windsheid
void mirrorprint(int num)
{
  String newnum = String(num);
  if (newnum.length() == 3)
  {
    dig1 = '}';
    dig2 = '|';
    dig3 = '{';
    dig4 = '~';
  }
  else if (newnum.length() == 2)
  {
    dig1 = '|';
    dig2 = '{';
    dig3 = '}';
    dig4 = '~';
  }
  else if (newnum.length() == 1)
  {
    dig1 = '{';
    dig2 = '|';
    dig3 = '}';
    dig4 = '~';
  }
  for (int i = 0; i < 4; i++)
  {
    if (i == 0) sseg.print(dig1);
    else if (i == 1) sseg.print(dig2);
    else if (i == 2) sseg.print(dig3);
    else if (i == 3) sseg.print(dig4);

    if (newnum[i] == '0') sseg.write(63);
    else if (newnum[i] == '1') sseg.write(48);
    else if (newnum[i] == '2') sseg.write(109);
    else if (newnum[i] == '3') sseg.write(121);
    else if (newnum[i] == '4') sseg.write(114);
    else if (newnum[i] == '5') sseg.write(91);
    else if (newnum[i] == '6') sseg.write(95);
    else if (newnum[i] == '7') sseg.write(49);
    else if (newnum[i] == '8') sseg.write(127);
    else if (newnum[i] == '9') sseg.write(123);
    else sseg.write(byte(0));
  }
}
