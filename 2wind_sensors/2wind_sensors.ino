/* Modern Device Wind Sensor Sketch for Rev C Wind Sensor
 This sketch is only valid if the wind sensor if powered from 
 a regulated 5 volt supply.
 
 When using an Arduino to power the sensor, an external power supply is better. Most Arduinos have a 
 polyfuse which protects the USB line. This fuse has enough resistance to reduce the voltage
 available to around 4.7 to 4.85 volts, depending on the current draw. 
 
 The sketch uses the on-chip temperature sensing thermistor to compensate the sensor
 for changes in ambient temperature. Because the thermistor is just configured as a
 voltage divider, the voltage will change with supply voltage. This is why the 
 sketch depends upon a regulated five volt supply.
 
 Other calibrations could be developed for different sensor supply voltages, but would require
 gathering data for those alternate voltages, or compensating the ratio.
 
 Hardware Setup: 
 Wind Sensor Signals    Arduino
 GND                    GND
 +V                     5V
 RV                     A1    // modify the definitions below to use other pins
 TMP                    A0    // modify the definitions below to use other pins

*/

#define number_of_sensors 2 // define number of wind sensors used

//#define analogPinForRV1    1   // change to pins you the analog pins are using
//#define analogPinForTMP1   0
//
//#define analogPinForRV2    3   // change to pins you the analog pins are using
//#define analogPinForTMP2   2

// to calibrate your sensor, put a glass over it, but the sensor should not be
// touching the desktop surface however.
// adjust the zeroWindAdjustment until your sensor reads about zero with the glass over it. 

const float zeroWindAdjustment =  .2; // negative numbers yield smaller wind speeds and vice versa.

//int TMP_Therm_ADunits;  //temp termistor value from wind sensor
//float RV_Wind_ADunits;    //RV output from wind sensor 
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

    // these are all derived from regressions from raw data as such they depend on a lot of experimental factors
    // such as accuracy of temp sensors, and voltage at the actual wind sensor, (wire losses) which were unaccouted for.
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

  Serial.begin(57600);   // faster printing to get a bit better throughput on extended info
  // remember to change your serial monitor

  Serial.println("CLEARSHEET"); // clears sheet starting at row 1
  
  Serial.println("LABEL,Date,Time,Timer,WindSpeed MPH, Windspeed m/s, Sensor number");
}

void loop() 
{
  float TMP_Therm_ADunits;
  float RV_Wind_ADunits;
  int i=0;

  if (millis() - lastMillis > 200)      // read every 200 ms - printing slows this down further
  {
    for (i=0; i<2*number_of_sensors; i=i+2) 
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
