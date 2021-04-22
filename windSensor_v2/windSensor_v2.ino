/* 
 *  Wind Sensor fixture v2
 *  Author: Vikram Seshadri
 *  03/18/2019
 *  
 *  Description:
 *  This sketch is used when multiple wind sensors are connected to a fixture. 
 *  By modifying the number_of_sensors variable it is possible to add 1-4 sensors.
 *  This sketch however prints sensor values in the same column.
 *  
 *  Calibrating the sensor:
 *  Put a glass over it, but the sensor should not be touching the desktop surface however.
 *  Adjust the zeroWindAdjustment until your sensor reads about zero with the glass over it. 
 *  Ideally a vacuum chamber should do the job.
 *  
 *  Hardware Setup: 
 *  Wind Sensor Signals    Arduino
 *  GND                    GND
 *  +V                     5V
 *  RV                     A1    // modify the definitions below to use other pins
 *  TMP                    A0    // modify the definitions below to use other pins
 */

#define number_of_sensors 4 // define number of wind sensors used

const float zeroWindAdjustment =  .2; // negative numbers yield smaller wind speeds and vice versa.
float RV_Wind_Volts;
unsigned long lastMillis;
int TempCtimes100;
float zeroWind_ADunits;
float zeroWind_volts;
float WindSpeed_MPH;
float WindSpeed_MS;
int sensor_number;

void wind_calculation(float TMP_Therm_ADunits, float RV_Wind_ADunits, int cnt)
{
    
    RV_Wind_Volts = (RV_Wind_ADunits *  0.0048828125);
    TempCtimes100 = (0.005 *((float)TMP_Therm_ADunits * (float)TMP_Therm_ADunits)) - (16.862 * (float)TMP_Therm_ADunits) + 9075.4;  
    zeroWind_ADunits = -0.0006*((float)TMP_Therm_ADunits * (float)TMP_Therm_ADunits) + 1.0727 * (float)TMP_Therm_ADunits + 47.172;  //  13.0C  553  482.39
    zeroWind_volts = (zeroWind_ADunits * 0.0048828125) - zeroWindAdjustment;  
    WindSpeed_MPH =  pow(((RV_Wind_Volts - zeroWind_volts) /.2300) , 2.7265);   
   
    Serial.print("  TMP volts ");
    Serial.print(TMP_Therm_ADunits * 0.0048828125);
    
    Serial.print(" RV volts ");
    Serial.print((float)RV_Wind_Volts);

    Serial.print("\t  TempC*100 ");
    Serial.print(TempCtimes100 );

    Serial.print("   ZeroWind volts ");
    Serial.print(zeroWind_volts);

    Serial.print("  WindSpeed MPH ");
    Serial.print((float)WindSpeed_MPH);
    
    WindSpeed_MS = WindSpeed_MPH/2.237;
    Serial.print("\t WindSpeed m/s ");
    Serial.print((float)WindSpeed_MS);

    Serial.print("\t Sensor number ");
    Serial.println(cnt);

    Serial.println((String)"DATA,DATE,TIME," + millis()/1000 + "," + WindSpeed_MPH + "," + WindSpeed_MS + "," + cnt);
    Serial.println(cnt);
    //delay (200);
    
}

void setup() 
{

  Serial.begin(57600);  
  Serial.println("CLEARSHEET"); // clears sheet starting at row 1
  
  Serial.println("LABEL,Date,Time,Timer,WindSpeed MPH, Windspeed m/s, Sensor number");
}

void loop() 
{
  float TMP_Therm_ADunits;
  float RV_Wind_ADunits;
  int i=0;

  if (millis() - lastMillis > 500)      // read every 200 ms - printing slows this down further
  {
    for (i=8; i<4*number_of_sensors; i=i+2) 
    {
      TMP_Therm_ADunits = analogRead(i+1);
      RV_Wind_ADunits = analogRead(i);
      sensor_number = sensor_number + 1;
      wind_calculation(TMP_Therm_ADunits, RV_Wind_ADunits, sensor_number);
    }
  lastMillis = millis();
  i=0;
  sensor_number = 0;
  } 

}
