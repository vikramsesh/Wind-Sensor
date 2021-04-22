/*
    Wind Sensor motor fixture
    Author: Vikram Seshadri
    05/06/2019

    Description:
    This sketch uses a stepper motor and a wind sensor to map different vectors of air flow.
     - Added CSV file modifications
     - Added stepper motor
     - Added time intervals for each angle

    v3:
    Adding stepper shield
    
    Calibrating the sensor:
    Put a glass over it, but the sensor should not be touching the desktop surface however.
    Adjust the zeroWindAdjustment until your sensor reads about zero with the glass over it.
    Ideally a vacuum chamber should do the job.

    Hardware Setup:
    Wind Sensor Signals    Arduino
    GND                    GND
    +V                     5V
    RV                     A1    // modify the definitions below to use other pins
    TMP                    A0    // modify the definitions below to use other pins
    SDSS                   4// UNO   53// for Mega
*/

#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include <SD.h>

#define analogPinForRV    A1   // change to pins you the analog pins are using
#define analogPinForTMP   A0
#define SDSS 4

#define increment_angle 45

Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
Adafruit_StepperMotor *myMotor = AFMS.getStepper(200, 2);

const float zeroWindAdjustment =  .2; // negative numbers yield smaller wind speeds and vice versa.
int cnt = 0;
int angle_cnt = 0;
int angle = 0;

int TMP_Therm_ADunits;  //temp termistor value from wind sensor
float RV_Wind_ADunits;    //RV output from wind sensor
float RV_Wind_Volts;

char user_input;

int TempCtimes100;
float zeroWind_ADunits;
float zeroWind_volts;
float WindSpeed_MPH;
float WindSpeed_MS;
String dataString = "";

unsigned long counter = 0;
unsigned long previousCounter = 0;
unsigned int interval = 5000; //20 sec.  between angle switch
unsigned long lastMillis;

int j = 0;
char x[8]; //timer

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
      file.println(String(F("Sample#,Time (sec.),WindSpeed (m/s),Windspeed (mph),Angle"))); //Headers
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

void setup() {

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

  }

  else {
    Serial.println("No SD card found");
  }

  AFMS.begin();
  myMotor->setSpeed(5000);  // 10 rpm
  
  while (1) {
    user_input = Serial.read();
    if (user_input == '1') {
      lastMillis = millis();
      previousCounter = millis();
      break;
    }
  }
}

void loop() {

  unsigned long currentCounter = millis();
  double timer = 0;

  if (((currentCounter - previousCounter) >= interval) || angle_cnt == 0) {
    if (angle_cnt < (360 / increment_angle)) {
      Serial.println(angle_cnt);
      Serial.println(360 / increment_angle);
      angle = angle_cnt * increment_angle;

      myMotor->step(100, FORWARD, INTERLEAVE);
      angle_cnt += 1;
    }

    else {
      angle_cnt = 0;
      myMotor->step(800, BACKWARD, INTERLEAVE);
    }

    //delay(1000);
    previousCounter = currentCounter;
    lastMillis = millis();
  }

  if (millis() - lastMillis > 200) {

    csv.openFile();
    cnt = cnt + 1;

    TMP_Therm_ADunits = analogRead(analogPinForTMP);
    RV_Wind_ADunits = analogRead(analogPinForRV);

    RV_Wind_Volts = (RV_Wind_ADunits *  0.0048828125);

    TempCtimes100 = (0.005 * ((float)TMP_Therm_ADunits * (float)TMP_Therm_ADunits)) - (16.862 * (float)TMP_Therm_ADunits) + 9075.4;

    zeroWind_ADunits = -0.0006 * ((float)TMP_Therm_ADunits * (float)TMP_Therm_ADunits) + 1.0727 * (float)TMP_Therm_ADunits + 47.172; //  13.0C  553  482.39

    zeroWind_volts = (zeroWind_ADunits * 0.0048828125) - zeroWindAdjustment;

    WindSpeed_MPH =  pow(((RV_Wind_Volts - zeroWind_volts) / .2300) , 2.7265);
    if (isnan(WindSpeed_MPH)) {
      WindSpeed_MPH = 0;
    }

    dataString += cnt;
    dataString += ",";
    Serial.print("Sample: ");
    Serial.print(cnt);

    timer = millis();
    timer = timer / (1000);
    dtostrf(timer, 4, 3, x);
    dataString += x;
    dataString += ",";
    Serial.print(" Time: ");
    Serial.print(x);

    Serial.print(" WindSpeed MPH: ");
    Serial.print((float)WindSpeed_MPH);
    dataString += WindSpeed_MPH;
    dataString += ",";

    WindSpeed_MS = WindSpeed_MPH / 2.237;
    Serial.print(" WindSpeed m/s: ");
    Serial.print((float)WindSpeed_MS);
    dataString += WindSpeed_MS;
    dataString += ",";

    Serial.print (" Angle: ");
    Serial.println(angle);
    dataString += angle;

    csv.writeToCSV(dataString);
    csv.closeFile();
    dataString = "";

    //    previousCounter = currentCounter;
    lastMillis = millis();
  }
}
