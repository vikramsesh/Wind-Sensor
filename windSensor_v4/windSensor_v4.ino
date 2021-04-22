/*
    Wind Sensor fixture v4
    Author: Vikram Seshadri
    03/28/2019

    Description:
    This sketch is used when multiple wind sensors are connected to a fixture.
    By modifying the number_of_sensors variable it is possible to add 1-4 sensors.
    This sketch takes in all wind sensor values stores it in an array to display values in different columns.
    This makes it easier to declare a vector representation of air flow.

    v4:
    Added SD card storage

    Calibrating the sensor:
    Put a glass over it, but the sensor should not be touching the desktop surface however.
    Adjust the zeroWindAdjustment until your sensor reads about zero with the glass over it.
    Ideally a vacuum chamber should do the job.

    Hardware Setup:
    Wind Sensor Signals    Arduino
    GND                    GND
    +V                     5V
    RV                     A1,A3,A5,A7    // modify the definitions below to use other pins
    TMP                    A0,A2,A4,A6    // modify the definitions below to use other pins
    SDSS                   53             // for Mega
*/

#include <SPI.h>
#include <SD.h>

#define number_of_sensors 4 // define number of wind sensors used

const int chipSelect = 53; //SDSS pin select
const float zeroWindAdjustment =  .2; // negative numbers yield smaller wind speeds and vice versa.
float RV_Wind_Volts;
unsigned long lastMillis;
int TempCtimes100;
float zeroWind_ADunits;
float zeroWind_volts;
float WindSpeed_MPH;
float WindSpeed_MS;
int sensor_number;
int cnt = 0;
String dataString = "";

float windSpeedMS[5] = {0, 0, 0, 0};
float windSpeedMPH[5] = {0, 0, 0, 0};

float wind_calculation(float TMP_Therm_ADunits, float RV_Wind_ADunits, int cnt)
{

  RV_Wind_Volts = (RV_Wind_ADunits *  0.0048828125);
  TempCtimes100 = (0.005 * ((float)TMP_Therm_ADunits * (float)TMP_Therm_ADunits)) - (16.862 * (float)TMP_Therm_ADunits) + 9075.4;
  zeroWind_ADunits = -0.0006 * ((float)TMP_Therm_ADunits * (float)TMP_Therm_ADunits) + 1.0727 * (float)TMP_Therm_ADunits + 47.172; //  13.0C  553  482.39
  zeroWind_volts = (zeroWind_ADunits * 0.0048828125) - zeroWindAdjustment;
  WindSpeed_MPH =  pow(((RV_Wind_Volts - zeroWind_volts) / .2300) , 2.7265);
  WindSpeed_MS = WindSpeed_MPH / 2.237;

  return (float)WindSpeed_MPH;
  //delay (200);

}

void setup()
{

  Serial.begin(57600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }
  Serial.println("card initialized.");
//  File dataFile = SD.open("Test.csv", FILE_WRITE);
//  if (dataFile) {
//    dataFile.println("Sample#, Timer, S1 (m/s), S1 (mph),S2 (m/s), S2 (mph),S3 (m/s), S3 (mph),S4 (m/s), S4 (mph)");
//    dataFile.close();
//
//    Serial.println(dataString);
//  }
//  else {
//    Serial.println("Error opening file");
//  }

}

void loop()
{
  float TMP_Therm_ADunits;
  float RV_Wind_ADunits;
  int i = 0, j = 0;


  if (millis() - lastMillis > 500)      // read every 200 ms - printing slows this down further
  {
    dataString += millis() / 1000;
    dataString += ",";
    dataString += cnt + 1;
    dataString += ",";

    for (i = 8, j = 0; i < 4 * number_of_sensors, j < number_of_sensors; i += 2, j += 1)
    {
      TMP_Therm_ADunits = analogRead(i + 1);
      RV_Wind_ADunits = analogRead(i);
      sensor_number = sensor_number + 1;
      windSpeedMPH[j] = wind_calculation(TMP_Therm_ADunits, RV_Wind_ADunits, sensor_number);
      if(isnan(windSpeedMPH[j])){
        windSpeedMPH[j] = 0;
      }
      windSpeedMS[j] = windSpeedMPH[j] / 2.237;

      dataString += windSpeedMS[j];
      if (sensor_number <= number_of_sensors) {
        dataString += ",";
      }
      dataString += windSpeedMPH[j];
      if (sensor_number < number_of_sensors) {
        dataString += ",";
      }

      if (sensor_number == number_of_sensors) {

        File dataFile = SD.open("Test.csv", FILE_WRITE);
        if (dataFile) {
          dataFile.println(dataString);
          dataFile.close();

          Serial.println(dataString);
        }
        else {
          Serial.println("Error opening file");
        }

      }
    }
    lastMillis = millis();
    i = 0;
    sensor_number = 0;
    cnt += 1;
    dataString = "";
  }

}
