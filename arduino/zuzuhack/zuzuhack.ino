      /*
  Zuzuhack
  Receive the id of a song to play and word it to the audio module.
  hc-05 (bluetooth) <-> Trinket <-> WT020 (audio module)
  Turns on an LED on for one second, then off for one second, repeatedly.
 
  This example code is in the public domain.
 
  To upload to your Gemma or Trinket:
  1) Select the proper board from the Tools->Board Menu
  2) Select USBtinyISP from the Tools->Programmer
  3) Plug in the Gemma/Trinket, make sure you see the green LED lit
  4) For windows, install the USBtiny drivers
  5) Press the button on the Gemma/Trinket - verify you see
     the red LED pulse. This means it is ready to receive data
  6) Click the upload button above within 10 seconds
*/
 
 
 #include <SoftwareSerial.h>// import the serial library
 #include <Adafruit_NeoPixel.h> // Use to control the noose
 
//arduino pinout for audio module
int RST = A3;
int CLK = A4;
int DAT = A5; 
const unsigned int VOLUME_0 = 0xFFF0;
const unsigned int VOLUME_1 = 0xFFF1;
const unsigned int VOLUME_2 = 0xFFF2;
const unsigned int VOLUME_3 = 0xFFF3;
const unsigned int VOLUME_4 = 0xFFF4;
const unsigned int VOLUME_5 = 0xFFF5;
const unsigned int VOLUME_6 = 0xFFF6;
const unsigned int VOLUME_7 = 0xFFF7;
const unsigned int PLAY_PAUSE = 0xFFFE;
const unsigned int STOP = 0xFFFF;





//internal led for debug purpose
int led = 13; 
 
//motor pinouts
int motorA = 8; 
int motorB = 7; 
 


//Bluetooth parameters
SoftwareSerial bluetooth(10, 11); // RX, TX
int BluetoothData; // the data given from Computer

boolean serial=false;




// Parameter 1 = number of pixels in led
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
#define LED_PIN            5      /* Physical Pin 6: LED connection                 */
Adafruit_NeoPixel   led_noose           = Adafruit_NeoPixel(1, LED_PIN, NEO_GRB + NEO_KHZ800);
volatile unsigned char count      = 0; 
volatile unsigned char play_noose      = 1; 





void setup() {  


    
    pinMode(led, OUTPUT);
    pinMode(motorA, OUTPUT);
    pinMode(motorB, OUTPUT);
    
    //Stop the zuzupet by default
    digitalWrite(motorA, LOW);
    digitalWrite(motorB, LOW);
    
    
    //Neopixel controller for the noose
    led_noose.begin();
	led_noose.show(); // Initialize all pixels to 'off'

	
	//Audio module init
	pinMode(RST, OUTPUT);
	pinMode(CLK, OUTPUT); 
	pinMode(DAT, OUTPUT);	
	digitalWrite(RST, HIGH);
	digitalWrite(CLK, HIGH);
	digitalWrite(DAT, HIGH);


  // reset the audio module
  digitalWrite(RST, HIGH);
  delay(100);
  digitalWrite(RST, LOW);
  delay(10);
  digitalWrite(RST, HIGH);
  delay(100);
  
  sendCommand(VOLUME_7);
  delay(1000);
  
  //Bluetooth init
  bluetooth.begin(9600);
  bluetooth.println("Bluetooth On .."); 

	//open serial line for debug purpose
	if (serial==true)
	{
		Serial.begin(9600);   
		  while (!Serial) {
			; // wait for serial port to connect. Needed for Leonardo only
		  }	
		Serial.println("Welcome to ZuzuWorld"); 
	}	
      

}



void loop_() {
    digitalWrite(led, HIGH); 
    delay(500);
    digitalWrite(led, LOW);
    delay(500); 
	Serial.println("bip.."); 	
	
	
}

void loop_test_motors() {
  
 //Stoppe pendant 1 seconde
 digitalWrite(motorA, LOW);
 digitalWrite(motorB, LOW);
 delay(2000);
   
  //Avance pendant 1 seconde
 digitalWrite(motorA, HIGH); 
 digitalWrite(motorB, LOW); 
 delay(1000);
 
 //Stoppe pendant 1 seconde
 digitalWrite(motorA, LOW);
 digitalWrite(motorB, LOW);
 delay(2000);
 
 
 //Recule pendant 2 seconde
 digitalWrite(motorA, LOW);
 digitalWrite(motorB,HIGH);
 delay(2000); 
 
 //Stoppe pendant 1 seconde
 digitalWrite(motorA, LOW);
 digitalWrite(motorB, LOW);
 delay(2000);
 
  //Freine pendant 1 seconde
 digitalWrite(motorA, HIGH);
 digitalWrite(motorB, HIGH);
 delay(2000);
  

  
}

void loop_test_noose() {
	 delay(30);
	if (play_noose==1)
	{
    led_noose.setPixelColor(0, Wheel(count++));
    led_noose.show();
	}
	 delay(30);
}	


// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 84) {
    return led_noose.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else if(WheelPos >=84 && WheelPos < 88) { // fix blue color
    return led_noose.Color(0, 0, 255);  
  } else if(WheelPos < 170) {
    WheelPos -= 85;
    return led_noose.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  } else {
    WheelPos -= 170;
    return led_noose.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
}


	
	
void loop() {

	digitalWrite(led, HIGH); 
    delay(20);
	if (play_noose==1)
	{
    led_noose.setPixelColor(0, Wheel(count++));
    led_noose.show();
	}	
    delay(20);
	digitalWrite(led, HIGH); 
	if (serial==true)
	{		
		Serial.println("bip.."); 
	}			
	
	
	  int id ;
	  //Check for incomming tc
	  // s ledNumber, brightness \n
	  // for example: 
          // "p1\n":play song #1
          // "m0\n":go ahead (2) , reverse (1) , stop (0)
	  if (bluetooth.find("m")) {
		id = bluetooth.parseInt(); // parses numeric characters before the comma
		// print the results back to the sender:
		//open serial line for debug purpose
		if (serial==true)
		{		
			Serial.print("Update Motor: " );
			Serial.print(id);
		}
		if (id==0)
		{

                 //Stop
                 digitalWrite(motorA, LOW);
                 digitalWrite(motorB, LOW);

		}
		if (id==1)
		{
                  //Stop
                 digitalWrite(motorA, LOW);
                 digitalWrite(motorB, LOW);
                 delay(500);
                 
                  //Go backward
                 digitalWrite(motorA, LOW);
                 digitalWrite(motorB,HIGH);

		}
		if (id==2)
		{
  
                    //Stop
                   digitalWrite(motorA, LOW);
                   digitalWrite(motorB, LOW);
                   delay(500);  

                   //Go forward
                   digitalWrite(motorA, HIGH); 
                   digitalWrite(motorB, LOW);   
		}
          }
          
	  if (bluetooth.find("p")) {
		id = bluetooth.parseInt(); // parses numeric characters before the comma
		// print the results back to the sender:
		//open serial line for debug purpose
		if (serial==true)
		{		
			Serial.print("Playing song: " );
			Serial.print(id);
		}
                sendCommand(id);		
	  }
	  
	  if (bluetooth.find("n")) {
		id = bluetooth.parseInt(); // parses numeric characters before the comma
		// print the results back to the sender:
		//open serial line for debug purpose
		if (id==0)
		{
			//switch off noose
			play_noose=0;
			led_noose.setPixelColor(0, led_noose.Color(0, 0, 0));
			led_noose.show();			
		
		}
		if (id==1)
		{
			//enable wheel color	
			play_noose=1;	
		}
		if (id==2)
		{
			//disable wheel color
			play_noose=0;			
		}		
		
	  }
	  
	  

}

//Function for audio module
void sendCommand(unsigned int command) {
  // start bit
  digitalWrite(CLK, LOW);
  delay(2);

  // bit15, bit14, ... bit0
  for (unsigned int mask = 0x8000; mask > 0; mask >>= 1) {
    if (command & mask) {
      digitalWrite(DAT, HIGH);
    }
    else {
      digitalWrite(DAT, LOW);
    }
    // clock low
    digitalWrite(CLK, LOW);
    delayMicroseconds(200);

    // clock high
    digitalWrite(CLK, HIGH);
    delayMicroseconds(200);
  }

  // stop bit
  delay(2);
}

//Function for audio module
void send(int addr)
{
  digitalWrite(CLK, LOW);
  delay(2);
  for (int i=15; i>=0; i--)
  { 
    delayMicroseconds(50);
    if((addr>>i)&0x0001 >0)
      {
        digitalWrite(DAT, HIGH);
        //Serial.print(1);
      }
    else
       {
         digitalWrite(DAT, LOW);
        // Serial.print(0);
       }
    delayMicroseconds(50);
    digitalWrite(CLK, HIGH);
    delayMicroseconds(50);
    
    if(i>0)
    digitalWrite(DAT, LOW);
    else
    digitalWrite(DAT, HIGH);
    delayMicroseconds(50);
    
    if(i>0)
    digitalWrite(CLK, LOW);
    else
    digitalWrite(CLK, HIGH);
  }
  
  delay(20); 
}


