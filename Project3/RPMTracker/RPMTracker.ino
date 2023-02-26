
// Pin Data
const int readPin = 3;

// RPM Data
unsigned long interruptCount = 0;
unsigned long lastLogTime;
const float NUM_BLADES = 2;
const float MILLIS_TO_MIN = 60000;

// Interrupt Data
ISR(TIMER1_COMPA_vect) {
    logRPM();
}

// Setup Serial bus transmissions and interrupts for
// timer interrupt and digital pin interrupt
void setup() {
    // put your setup code here, to run once:
    Serial.begin(9600);
    pinMode(readPin, INPUT);
    Serial.print("Time (s), Rotation Speed (RPM)\n");

    // Setup timer interrupt for logging
    TCCR1A = 0;
    TCCR1B = 0;
    // Set Timer 1 compare value for 1s (0x3D09 at 1024prescalar)
    OCR1A = 0x3D09;
    // Set Timer 1 to prescaler 1024
    TCCR1B = (1<<CS12) | (1<<CS10);
    // Set for comparison to OCR1A
    TCCR1B = TCCR1B | (1<<WGM12);
    // Set Timer 1 to use compare match
    TIMSK1 = (1<<OCIE1A);

    // Setup digital pin interrupt
    lastLogTime = millis();
    attachInterrupt(digitalPinToInterrupt(readPin),
                    updateInterruptCount,
                    RISING);
}

// Loop doing nothing unless interrupts called
void loop() {
    // Empty, wait for interrupts
}

// Update propeller pass count on digital pin interrupt
void updateInterruptCount() {
    ++interruptCount;
}

// Log current RPM rate to serial bus
void logRPM() {
    Serial.print(millis()/1000.);
    Serial.print(", ");
    Serial.print(calculateRPM());
    Serial.print("\n");
    interruptCount = 0;
    lastLogTime = millis();
}

// Calculate RPM based on propellor pass count and delta time
unsigned int calculateRPM() {
    return ((interruptCount / NUM_BLADES) / (float) (millis() - lastLogTime)) * MILLIS_TO_MIN;
}
