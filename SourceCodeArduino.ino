//* Deflection Derivation 
//  @author Jacob Hardy 
//  @version V2.0
//  Goal: This code seeks to develop a method of determining paramaters regarding force, deflection, stiffness, acceleration, etc on a system and exports a comma dillineated (.txt file) which is further processed using a python script for data analysis
//* 07/25/2022


/*
Set up the IR sensor
*/
#include <SharpIR.h>
#define IRPin A0
#define model 430
unsigned long myTime; //defines the variable that will count time
int distance_cm;
#define sensor A0 // Sharp IR GP2Y0A41SK0F (4-30cm, analog)
int reference = 0; //used to set reference value of distance
/*
Set up the Accelerometer
*/
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h> //i2c library
Adafruit_MPU6050 mpu;
float mass = 945; //This is the mass of the object in question in [g]
/*
 * Setup Time Module
 */
// #include <TimeLib.h>    //For update/display of time
// #include <Ethernet.h>   //For communication with NTP Server via UDP
/* 
 *  Setup SD card mdule
 */
#include <SD.h>
#include <SPI.h>
File myFile;
String fileName = "Deflect";
String fileExt =  ".txt";
String fileAddress = fileName + fileExt;
#define PIN_SPI_CS 53
/*
 * Setup Buttonpush 
 */
 



void setup() {
  //
  Serial.begin(9600);
  if (SD.begin(PIN_SPI_CS))
  {
    Serial.println(F("SD card is ready to use."));
  } else
  {
    Serial.println(F("SD card initialization failed"));
    return;
  }
  int i=1;
  while (SD.exists(fileAddress)){  //If a file already exists, creates a new file.
    myFile.close();
    fileAddress = fileName + String(i) + fileExt;
    i++;
  }
  
  Serial.println(fileAddress);
  Serial.println(FreeRam());
  myFile = SD.open(fileAddress,FILE_WRITE);
 
  if (myFile = SD.open( fileAddress , FILE_WRITE)){
    Serial.println(1);
    } else{
      Serial.println(2);
  }
  Serial.println(myFile);
  // -- Setup the IR sensor---
  SharpIR mySensor(model, IRPin);
    for (int i=1; i<=6; i++){ //This for loop gets the current reference value of the distance sensor to use as the reference value. 
      float volts = analogRead(sensor)*0.0048828125;  // value from sensor * (5/1024) [V]
      float distance = 13*pow(volts, -1)*10 ; // worked out from datasheet graph and converted to mm [mm]
      reference += distance;
      if(i==6){
        reference = reference / i;
      }
      }
      
  // -- Setup the time module---
  //setTime(0, 0, 0, 7, 7, 2022);
  
  // -- Setup the Acceleromoter -- //
  while (!Serial){
    delay(10); // will pause Zero, Leonardo, etc until serial console opens
    Serial.println(F("Test for break"));
  }
  Serial.println(F("Adafruit MPU6050 test!"));
  
  // Try to initialize!
  if (!mpu.begin()) {
    Serial.println(F("Failed to find MPU6050 chip"));
    while (1) {
      delay(10);
    }
  }
  Serial.println(F("MPU6050 Found!"));

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  Serial.print(F("Accelerometer range set to: "));
  switch (mpu.getAccelerometerRange()) {
    case MPU6050_RANGE_2_G:
      Serial.println(F("+-2G"));
      break;
    case MPU6050_RANGE_4_G:
      Serial.println(F("+-4G"));
      break;
    case MPU6050_RANGE_8_G:
      Serial.println(F("+-8G"));
      break;
    case MPU6050_RANGE_16_G:
      Serial.println(F("+-16G"));
      break;
  }
  
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  Serial.print(F("Gyro range set to: "));
  switch (mpu.getGyroRange()) {
    case MPU6050_RANGE_250_DEG:
      Serial.println(F("+- 250 deg/s"));
      break;
    case MPU6050_RANGE_500_DEG:
      Serial.println(F("+- 500 deg/s"));
      break;
    case MPU6050_RANGE_1000_DEG:
      Serial.println(F("+- 1000 deg/s"));
      break;
    case MPU6050_RANGE_2000_DEG:
      Serial.println(F("+- 2000 deg/s"));
      break;
  }

  mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);
  Serial.print(F("Filter bandwidth set to: "));
  switch (mpu.getFilterBandwidth()) {
    case MPU6050_BAND_260_HZ:
      Serial.println(F("260 Hz"));
      break;
    case MPU6050_BAND_184_HZ:
      Serial.println(F("184 Hz"));
      break;
    case MPU6050_BAND_94_HZ:
      Serial.println(F("94 Hz"));
      break;
    case MPU6050_BAND_44_HZ:
      Serial.println(F("44 Hz"));
      break;
    case MPU6050_BAND_21_HZ:
      Serial.println(F("21 Hz"));
      break;
    case MPU6050_BAND_10_HZ:
      Serial.println(F("10 Hz"));
      break;
    case MPU6050_BAND_5_HZ:
      Serial.println(F("5 Hz"));
      break;
  }
}

void loop() {
  /* Get new sensor events with the readings */
 myTime = millis();
 sensors_event_t a, g, temp;
 mpu.getEvent(&a, &g, &temp);
 //myFile = SD.open(fileAddress, FILE_WRITE); 
  // ---------------------------------------------------------------------------------Get the Distance------------------------------------------------------------------
 float volts = analogRead(sensor)*0.0048828125;  // value from sensor * (5/1024)
 float distance = 13*pow(volts, -1); // worked out from datasheet graph
 // ------------------------------------------------------------------------------------Now we will begin processing real data to get what we need-----------------------
 float ddx =  (sqrt( (sq( a.acceleration.x)) + (sq( a.acceleration.y)) + (sq( a.acceleration.z))))- 9.84; //gets the net acceleration on the arduino
 float kg = mass /1000;                                                                      //gets the net mass of the object
 //         int time3[] = {hour(),minute(),second()};
 float dist_mm = distance *10; // [mm]
 //----------------------------------------------output real data------------------------------
 //------- Format:-----------
 // time [minute] | time [second] | net Acceleration [m/s^2] | net mass [g] |IR Sensor data
 //Serial.println(Minutes| Seconds | Iterator  | net Acceleration [m/s^2] | net mass [kg] | displacement [mm]");
 //time  
 Serial.println();
 Serial.print(0);
 Serial.print(",");
 Serial.print(myTime);
 Serial.print(",");
 //Iterator count
 
 //net Acceleration
 Serial.print(ddx,8);
 Serial.print(",");
 //net mass
 Serial.print(kg,4);
 Serial.print(F(","));
 //displacement 
 Serial.print(abs(dist_mm - reference),6);



 
 //write to SD card file
  myFile = SD.open(fileAddress, FILE_WRITE); 
  myFile.println();
  myFile.print(0);
  myFile.print(F(","));
  myFile.print(myTime);
  myFile.print(F(","));
  //net Acceleration
  myFile.print(ddx,8);
  myFile.print(F(","));
  //net mass
  myFile.print(kg,4);
  myFile.print(F(","));
  //displacement 
  myFile.print(abs(dist_mm - reference),6);
  myFile.close();
  
}
