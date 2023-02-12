
#include <SoftwareSerial.h>

const int thermistorPin = A0;
unsigned long avgTempVal = 0;
unsigned int tempCount = 0;

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

// Interrupt Data
bool logNow = false;
ISR(TIMER1_COMPA_vect) {
    logAvgTemp();
}

void setup() {
    // put your setup code here, to run once:
    Serial.begin(9600);
    pinMode(logResetPin, OUTPUT);
    // Default state high, low to reset logging
    digitalWrite(logResetPin, HIGH);

    Serial.print("Time (s), Temp (F)\n");

    solveSteinhartHart();

    /*
     * Uses Compare-Match interrupt with max compare value FFFF
     * This is equivalent to doing overflow - overflow would have
     * TIMER1_COMPA_vect replaced with TIMER1_OVF_vect
     * and OCIE1A replaced with TOIE1
     */
    // Reset Timer 1 Registers
    TCCR1A = 0;
    TCCR1B = 0;
    // Set Timer 1 compare value to max
    OCR1A = 0xFFFF;
    // Set Timer 1 to prescaler 1024
    TCCR1B = (1<<CS12) | (1<<CS10);
    // Set for comparison to OCR1A
    TCCR1B = TCCR1B | (1<<WGM12);
    // Set Timer 1 to use compare match
    TIMSK1 = (1<<OCIE1A);
}

void loop() {
    // Keep rolling average of input pin read value
    double prevAvg = avgTempVal * tempCount;
    ++tempCount;
    avgTempVal = (prevAvg + analogRead(thermistorPin)) / (float) tempCount;
}

float tempValToFarenheit(unsigned int tempVal)
{
    float measVoltage = ((1023.0 - tempVal)/ 1023.0) * inputVoltage;
    float measReistance = (measVoltage / inputVoltage) * resistanceUsed_ohm / (1 - (measVoltage / inputVoltage));
//    Serial.println(measReistance);
    // Steinhart-Hart Equation
    float lnTemp = log(measReistance);
    float inverseTempK = SH_A + (SH_B * lnTemp) + (SH_C * lnTemp * lnTemp * lnTemp);
    return kelvinToFarenheit(1 / inverseTempK);
}

void logAvgTemp()
{
    if (logNow){
        Serial.print(millis()/1000);
        Serial.print(", ");
        Serial.print(tempValToFarenheit(avgTempVal));
        Serial.print("\n");
        avgTempVal = 0;
        tempCount = 0;
    }
    logNow = !logNow;
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
//    float R1 = 6900;
    float R1 = 5500;
    float T1 = farenheitToKelvin(98);
    float R2 = 13000;
    float T2 = farenheitToKelvin(67);
    float R3 = 27000;
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
