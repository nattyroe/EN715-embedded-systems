
#include <SoftwareSerial.h>

const int thermistorPin = A0;
unsigned long avgTempVal = 0;
unsigned int tempCount = 0;

unsigned long logInterval = 10000;
unsigned long nextLogTime = millis() + logInterval;

//const int rxPin = 0; // Currently unused
const int txPin = 1;
const int logResetPin = 2;
//const int interruptPin = 3;

// Temperature Data
const int KtoC = -273.15;
float SH_A = 0;
float SH_B = 0;
float SH_C = 0;

// Voltage Data
const float inputVoltage = 5.0;
const float resistanceUsed_ohm = 10000.0;

void setup() {
    // put your setup code here, to run once:
    Serial.begin(9600);
    pinMode(logResetPin, OUTPUT);
    // Default state high, low to reset logging
    digitalWrite(logResetPin, HIGH);

    Serial.print("Time (s), Temp (F)\n");

    solveSteinhartHart();

}

void loop() {
    double prevAvg = avgTempVal * tempCount;
    ++tempCount;
    avgTempVal = (prevAvg + analogRead(thermistorPin)) / (float) tempCount;

    if (millis() > nextLogTime){
        logAvgTemp();
    }
}

float tempValToFarenheit(unsigned int tempVal)
{
    float measVoltage = ((1023.0 - tempVal)/ 1023.0) * inputVoltage;
    float measReistance = (measVoltage / inputVoltage) * resistanceUsed_ohm / (1 - (measVoltage / inputVoltage));
    
    // Steinhart-Hart Equation
    float lnTemp = log(measReistance);
    float inverseTempK = SH_A + (SH_B * lnTemp) + (SH_C * lnTemp * lnTemp * lnTemp);
    return kelvinToFarenheit(1 / inverseTempK);
}

void logAvgTemp()
{
    Serial.print(millis()/1000);
    Serial.print(", ");
    Serial.print(tempValToFarenheit(avgTempVal));
    Serial.print("\n");
    avgTempVal = 0;
    tempCount = 0;
    nextLogTime = millis() + logInterval;
}

// Pull logger interrupt low
void resetLogger() {
    digitalWrite(logResetPin, LOW);
    delay(10);
    digitalWrite(logResetPin, HIGH);
}

float kelvinToFarenheit(float tKelvin) {
    return ((tKelvin + KtoC) * 9 / 5.0) + 32;
}

float farenheitToKelvin(float tFarenheit) {
    return ((tFarenheit - 32.0) * 5 / 9.0) - KtoC;
}

void solveSteinhartHart()
{
  // Calibration Temperatures
//  float R1 = 7300;
//  float T1 = farenheitToKelvin(170);
  float R1 = 6900;
  float T1 = farenheitToKelvin(98);
//  float R2 = 13000;
//  float T2 = farenheitToKelvin(67);
  float R2 = 13000;
  float T2 = farenheitToKelvin(67);
//  float R3 = 15200;
//  float T3 = farenheitToKelvin(32);
  float R3 = 22200;
  float T3 = farenheitToKelvin(40);

  // Using Cal Temps, solve for SH Coefficients
  float L1 = log(R1);
  float L2 = log(R2);
  float L3 = log(R3);
  float Y1 = 1 / T1;
  float Y2 = 1 / T2;
  float Y3 = 1 / T3;
  float gamma2 = (Y2 - Y1) / (L2 - L1);
  float gamma3 = (Y3 - Y1) / (L3 - L1);
  SH_C = ((gamma3 - gamma2) / (L3 - L2)) / (L1 + L2 + L3);
  SH_B = gamma2 - (SH_C * ((L1*L1) + (L1*L2) + (L2*L2)));
  SH_A = Y1 - ((SH_B + (L1*L1*SH_C))*L1);
}
