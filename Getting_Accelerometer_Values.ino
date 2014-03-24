/* Connections
Board in use - Arduino UNO/ Mega 2560

3.3 V    -->    3.3V
SDA --1K(ohm)--> A4 / 20
SCL --1k(ohm)--> A5 / 21
GND      -->     GND
*/

#include<Wire.h>


#define OUT_X_MSB 0x01
#define WHO_AM_I  0x0D
#define ADDRESS 0x1D
#define CTRL_REG1 0x2A
#define GVAL 2
#define XYZ_DATA_CFG 0x0E
#define GSCALE 00


void setup()
{ 
  Serial.begin(9600);

  Wire.begin();   // Joins bus in master mode

//-----Initialise Accelerometer and set G value----

Wire.beginTransmission(ADDRESS);
Wire.write(WHO_AM_I);
Wire.endTransmission(false);

While(!Wire.available())
	{Wire.requestFrom(ADDRESS,1);}

if (wire.read()== 0x2A)
Serial.println("Connection established with Accelerometer);
else
Serial.println("Connecion failed");

Standby();

if(GVAL==2)
GSCALE=B00;
if(GVAL==4)
GSCALE=B01;
if(GVAL==8)
GSCALE=B10;

writereg(XYZ_DATA_CFG, GSCALE);

Active();

//-------------------------------------------------

}


void loop()
{ 
  int accelvalue[3]; 	// To hold acceleration final ADC values
  byte accel[6]; 	// Variable to hold acceleration
  float accelg[3];	// To store real g values 


  Wire.beginTransmission(ADDRESS);
  Wire.write(OUT_X_MSB);
  Wire.endTransmission(false);

  while(Wire.available() < 6)
	{Wire.requestFrom(ADDRESS, 6);}	// Request 6 bytes from the device

  for (int i=0 i<6; ++i)
  accel[i] = Wire.read();  
  
  for(int i = 0; i < 3 ; i++)
     {
	int gCount = (accel[i*2] << 8) | accel[(i*2)+1]; //Convert 8 bit to 12 bit values
	gCount >>= 4;
 
	if (accel[i*2] > 0x7F)	//All values above 0x7F are negative, hence need to be converted to 2's complement
	   {
	     gCount = ~gCount + 1;
	     gCount *= -1; 
	    }

	accelvalue[i] = gCount;	//Writing ADC Values 
       }
  
  for (int i = 0 ; i < 3 ; i++)
	{
	  accelG[i] = (float) accelCount[i] / ((1<<12)/(2*GVAL)); 
	}

  
  for (int i = 0 ; i < 3 ; i++)
	{
	  Serial.print(accelG[i], 4);
	  Serial.print("\t"); 
	 }

  delay(10);
}


//----------- Standby Function ----------------

void Standby()
{
  byte c = readreg(CTRL_REG1);
  writereg(CTRL_REG1, c & ~(0x01)); 
}

//---------------------------------------------

//----------- Active Function ----------------

void Active()
{
  byte c = readreg(CTRL_REG1);
  writeregister(CTRL_REG1, c | 0x01); 
}

//---------------------------------------------


//----------- To Write to Register ----------------

void writereg( byte addr, byte data)
{ 
  Wire.beginTransmission(ADDRESS);
  Wire.write(addr);
  Wire.write(data);
  Wire.endTransmission();
}

//---------------------------------------------


//----------- To Read from Register ----------------

byte readreg(byte addr)
{ 
  Wire.beginTransmission(ADDRESS);
  Wire.write(addr);
  Wire.endTransmission(false); 
  
  While(!Wire.available())
	{Wire.requestFrom(ADDRESS,1);}
  return Wire.read();
}

//----------------------------------------------
