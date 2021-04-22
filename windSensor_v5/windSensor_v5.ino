/*
    Wind Sensor fixture v5
    Author: Vikram Seshadri
    03/29/2019

    Description:
    This sketch is used when multiple wind sensors are connected to a fixture.
    By modifying the number_of_sensors variable it is possible to add 1-4 sensors.
    This sketch takes in all wind sensor values stores it in an array to display values in different columns.
    This makes it easier to declare a vector representation of air flow.

    v5:
    added CSV file modifications
    formatting according to sample file

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
#define SDSS 53

const float zeroWindAdjustment =  .2; // negative numbers yield smaller wind speeds and vice versa.
float RV_Wind_Volts;
unsigned long lastMillis;
float TempCtimes100;
float zeroWind_ADunits;
float zeroWind_volts;
float WindSpeed_MPH;
float WindSpeed_MS;
int sensor_number;
int cnt = 0;
String dataString = "";

float windSpeedMS[5] = {0, 0, 0, 0};
float windSpeedMPH[5] = {0, 0, 0, 0};


/*************************************************************/
/* SD Class */
class CSVFile
{
  public:
    char filenamechar[100];
    File file;
    //SdFat SD;
    String base_filename;
    int file_number;
    String full_filename;

    CSVFile(String bfilename)
    {
      base_filename = bfilename;
      file_number = 1;
      full_filename = bfilename + String(file_number) + String(F(".csv"));
      full_filename.toCharArray(filenamechar, 100);
      //SD.begin(SDSS, SPI_HALF_SPEED);
    }

    void beginNow()
    {
      SD.begin(SDSS);
    }

    void openFile()
    {
      file = SD.open(filenamechar, FILE_WRITE);
    }

    void closeFile()
    {
      file.close();
    }

    void writeToCSV(String data)
    {
      file.print(data);
      file.print(String(F("\r\n")));
    }

    void writeHeaders()
    {
      file.println(String(F("Sample#,Time (sec.),S1 (m/s),S1 (mph),S2 (m/s),S2 (mph),S3 (m/s),S3 (mph),S4 (m/s),S4 (mph)"))); //Headers
    }

    void _update_file_number()
    {
      // See if the file already exists, if so, see if it has _###
      // If file doesn't exist, add _###
      // If file does exists, make _### 1 more

      while (SD.exists(filenamechar)) {
        file_number += 1;
        full_filename = base_filename + String(file_number) + String(F(".csv"));
        full_filename.toCharArray(filenamechar, 100);
      }
    }
};

//setup csv file object
String filename = "Airflow";
CSVFile csv(filename);

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
  if (SD.begin(SDSS)) {
    csv.beginNow();
    csv._update_file_number();
    csv.openFile();
    csv.writeHeaders();
    csv.closeFile();
    Serial.println("Card Initialized");
    Serial.println(String("Sample#,Time (sec.),S1 (m/s),S1 (mph),S2 (m/s),S2 (mph),S3 (m/s),S3 (mph),S4 (m/s),S4 (mph)")); //Headers
  }

  else {
    Serial.println("No SD card found");
    Serial.println(String("Sample#,Time (sec.),S1 (m/s),S1 (mph),S2 (m/s),S2 (mph),S3 (m/s),S3 (mph),S4 (m/s),S4 (mph)"));
  }
}

void loop()
{
  float TMP_Therm_ADunits;
  float RV_Wind_ADunits;
  int i = 0, j = 0;

  if (millis() - lastMillis > 500)      // read every 200 ms - printing slows this down further
  {
    csv.openFile();
    dataString += cnt + 1; //Sample number
    dataString += ",";
    dataString += millis() / 1000; //Timer
    dataString += ",";

    for (i = 8, j = 0; i < 4 * number_of_sensors, j < number_of_sensors; i += 2, j += 1)
    {
      TMP_Therm_ADunits = analogRead(i + 1);
      RV_Wind_ADunits = analogRead(i);
      sensor_number = sensor_number + 1;
      windSpeedMPH[j] = wind_calculation(TMP_Therm_ADunits, RV_Wind_ADunits, sensor_number);
      
      if (isnan(windSpeedMPH[j])) {
        windSpeedMPH[j] = 0;
      }
      windSpeedMS[j] = windSpeedMPH[j] / 2.237; //mph to m/s conversion

      dataString += windSpeedMS[j];
      if (sensor_number <= number_of_sensors) {
        dataString += ",";
      }
      dataString += windSpeedMPH[j];
      if (sensor_number < number_of_sensors) {
        dataString += ",";
      }

      if (sensor_number == number_of_sensors) {
        csv.writeToCSV(dataString);
        csv.closeFile();
        Serial.println(dataString);
      }
    }
    delay (20);
    lastMillis = millis();
    i = 0;
    sensor_number = 0;
    cnt += 1;
    dataString = "";
  }

}
