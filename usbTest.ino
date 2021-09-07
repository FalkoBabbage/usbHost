
/*
  I2C LCD and USB HOST MOUSE
  Example Arduino sketch to communicate with HobbyTronics I2CLCD backpack and
  USB HOST board with mouse attached
  
  Simply moves a cursor block around the screen
  
  Requires the Hobbytronics HTI2CLCD library
  
  Copyright (c) 2012 www.hobbytronics.co.uk 
 
  This example code is in the public domain.
*/
 
#include <Wire.h>


const int  lcd_address=58;     // I2C Address of LCD backpack
const int  host_address=41;    // I2C Address of USB Host (Mouse)

unsigned int mouse_data[10];   // Array to store Mouse values 
unsigned int mousex, mousey;   // Mouse X and Y screen position

unsigned long currentTime;
unsigned long cloopTime;

void setup()
{
  delay(200);                              // Give LCD time to configure itself  
  Wire.begin();                            // join i2c bus
  
  Wire.beginTransmission(host_address);    // transmit to device
  Wire.write(20);                          // Scale X Register
  Wire.write(0x00);                        // Scale X
  Wire.write(0x14);                        // Scale X (20)  
  Wire.endTransmission();                  // end transmission

  Wire.beginTransmission(host_address);    // transmit to device
  Wire.write(22);                          // Scale Y Register
  Wire.write(0x00);                        // Scale Y 
  Wire.write(0x14);                        // Scale Y (4)  
  Wire.endTransmission();                  // end transmission

  Wire.beginTransmission(host_address);    // transmit to device
  Wire.write(24);                          // Start X Register
  Wire.write(0x00);                        // Start X
  Wire.write(0x00);                        // Start X (0)  
  Wire.endTransmission();                  // end transmission

  Wire.beginTransmission(host_address);    // transmit to device
  Wire.write(26);                          // Start Y Register
  Wire.write(0x00);                        // Start Y (500)
  Wire.write(0x00);                        // Start Y (0)  
  Wire.endTransmission();                  // end transmission
  
  Wire.beginTransmission(host_address);    // transmit to device
  Wire.write(28);                          // Speed Register
  Wire.write(0x01);                        // Speed  
  Wire.endTransmission();                  // end transmission


  currentTime = millis();
  cloopTime = currentTime; 
   
}

void get_mouse()
{
  // Get 8-bit Mouse data
  // We only want single byte values (0 to 255)
  unsigned char data_values_rcvd=0;     // keep track of characters received 
  unsigned char value, i;  
  Wire.beginTransmission(host_address); // transmit to device
  Wire.write(0);                        // Start receiving data from register 0
  Wire.endTransmission();               // end transmission
  
  Wire.requestFrom(host_address, 10);   // request 10 bytes from slave device
  // Data Format
  // 0 - X Pos High Byte
  // 1 - X Pos Low Byte
  // 2 - Y Pos High Byte
  // 3 - Y Pos Low Byte 
  // 4 - Left Button
  // 5 - Right Button
  // 6 - Scroll Wheel Button
  // 7 - X mouse scroll value
  // 8 - Y Mouse Scroll value
  // 9 - Scroll Wheel scroll value
  data_values_rcvd=0; 
  i=0;
  while(Wire.available())
  { 
    if(data_values_rcvd < 10)
    { 
       value = Wire.read();
       mouse_data[data_values_rcvd] =value;    
    }
    data_values_rcvd++;
  }
  
}

void loop()
{
    currentTime = millis(); 
    if(currentTime >= (cloopTime + 100)){  

      get_mouse();  // get mouse data

      for( int ii = 0; ii < 10; ii++ ) {
        Serial.print(mouse_data[ii]);
        Serial.print(" ");
      }
      Serial.println("");
      cloopTime = currentTime;  // Updates cloopTime
    }
}