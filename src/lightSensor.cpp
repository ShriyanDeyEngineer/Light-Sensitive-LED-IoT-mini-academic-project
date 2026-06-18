// Include Particle Device OS APIs
#include "Particle.h"
#include "neopixel.h"
// Let Device OS manage the connection to the Particle Cloud
SYSTEM_MODE(AUTOMATIC);
// Run the application and system concurrently in separate threads
SYSTEM_THREAD(ENABLED);
// Show system, cloud connectivity, and application logs over USB
// View logs with CLI using 'particle serial monitor --follow'
SerialLogHandler logHandler(LOG_LEVEL_INFO);

//Set up the strip object with the number of pixels (LED's), pin, and type of strip from the Adafruit_NeoPixel class/library
#define PIXEL_PIN SPI
Adafruit_NeoPixel strip = Adafruit_NeoPixel(1, PIXEL_PIN, WS2812);

int lightReading;
double lightLevel;
int brightness;

void setup()
{
  //Open the serial port for communication with the computer
  Serial.begin(9600);
  //Setup D7 pin to output a heartbeat
  pinMode(D7, OUTPUT);
  //initialize the stip object that was created above
  
  strip.begin();
}

void loop()
{
  //Heartbeat, show we are alive
  digitalWrite(D7, HIGH);
  delay(250);
  //Read data from analog pins (which returns number from 0 to 4095)
  lightReading = analogRead(A1);
  
  //Convert the data to a voltage and print the data to the serial port
  lightLevel = lightReading / 4095.0 * 3.3;
  Serial.print("Current light reading (0-4095): ");
  Serial.print(lightReading);
  Serial.println("|");
  Serial.print("Current light level (Volts): ");
  Serial.print(lightLevel);
  Serial.println("|");
  //Set the brightness of the LED based on the light level (the brighter the light, the dimmer the LED)
  //The sensor reads lower numbers for brighter light, so the conversion should just be:
  brightness = lightReading / 4095.0 * 255; //This will ensure the brightness of the LED is less when the light reading is low (surrounding light is bright)
  
  Serial.print("Current brightness level (0-255): ");
  Serial.print(brightness);
  Serial.println("|");
  Serial.println("----------------------------------------");
  
  //Input the brightness variable in only one of the GRB parameters. The brightness value will change how dim that color is, controlling the overall brighteness of the LED
  strip.setPixelColor(0, strip.Color(brightness, 0, brightness));
  strip.show(); //This will update the LED to the color that was set above
  //Wait 1 second before the next reading
  delay(1000);
  
  //Heartbeat, show we are alive
  digitalWrite(D7, LOW);
  delay(250);
}

Version 2 (Thresholds):
// Include Particle Device OS APIs
#include "Particle.h"
#include "neopixel.h"
// Let Device OS manage the connection to the Particle Cloud
SYSTEM_MODE(AUTOMATIC);
// Run the application and system concurrently in separate threads
SYSTEM_THREAD(ENABLED);
// Show system, cloud connectivity, and application logs over USB
// View logs with CLI using 'particle serial monitor --follow'
SerialLogHandler logHandler(LOG_LEVEL_INFO);

//Set up the strip object with the number of pixels (LED's), pin, and type of strip from the Adafruit_NeoPixel class/library
#define PIXEL_PIN SPI
Adafruit_NeoPixel strip = Adafruit_NeoPixel(1, PIXEL_PIN, WS2812);

int lightReading;
double lightLevel;
int brightness;

void setup()
{
//Open the serial port for communication with the computer
Serial.begin(9600);
//Setup D7 pin to output a heartbeat
pinMode(D7, OUTPUT);
//initialize the stip object that was created above
strip.begin();
}

void loop()
{
  //Heartbeat, show we are alive
  digitalWrite(D7, HIGH);
  delay(250);
  //Read data from analog pins (which returns number from 0 to 4095)
  lightReading = analogRead(A1);
  
  //Convert the data to a voltage and print the data to the serial port
  lightLevel = lightReading / 4095.0 * 3.3;
  
  Serial.print("Current light reading (0-4095): ");
  Serial.print(lightReading);
  Serial.println("|");
  Serial.print("Current light level (Volts): ");
  Serial.print(lightLevel);
  Serial.println("|");
  //Set the brightness of the LED based on the light level (the brighter the light, the dimmer the LED)
  //The sensor reads lower numbers for brighter light, so the conversion should just be:
  brightness = lightReading / 4095.0 * 255; //This will ensure the brightness of the LED is less when the light reading is low (surrounding light is bright)
  //Set thresholds to make the brightness change more obvious (nearly off when environment is bright, and nearly full brightness when environment is dark)
  if(lightLevel <= 2.3)
  {
    brightness /= 10; //Set to 10% of the calculated brightness for better response to brighter enivronment
  }
  else if(lightLevel > 2.3)
  {
    brightness = brightness + (brightness / 2); //Set to 150% of the calculated brightness for better response to darker environment
    if(brightness > 255) //Cap the brightness at 255, which is the maximum for the LED
    {
      brightness = 255;
    }
  }
  Serial.print("Current brightness level (0-255): ");
  Serial.print(brightness);
  Serial.println("|");
  Serial.println("----------------------------------------");
  //Input the brightness variable in only one of the GRB parameters. The brightness value will change how dim that color is, controlling the overall brighteness of the LED
  strip.setPixelColor(0, strip.Color(brightness, 0, brightness));
  strip.show(); //This will update the LED to the color that was set above
  
  //Wait 1 second before the next reading
  delay(1000);
  
  //Heartbeat, show we are alive
  digitalWrite(D7, LOW);
  delay(250);
}
