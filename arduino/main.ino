#include <Servo.h>

// Servo variables
Servo servo;
int servoPin = 9;
int pos = 0;
// Sensor variables
int sensorPin = A0;
// Set values
int dishAngle[] = {32,60,100,125};
// LEDS
int leds[] = {13,12,11,10};

float rawRange = 1024; // 3.3v
float logRange = 5.0; // 3.3v = 10^5 lux

float processedLux() {
	return rawToLux(analogRead(sensorPin));
}

float rawToLux(int raw)
{
	float logLux = raw * logRange / rawRange;
	return pow(10, logLux);
}

void setup()
{
	analogReference(EXTERNAL);
	Serial.begin(9600);

	servo.attach(servoPin);

	for (int i=0; i<4; i++) {
		pinMode(leds[i], OUTPUT); 
	}
}

float averagedData(int count) {
  float total = 0;

  for (int i = 0; i < count; i ++) {
    float sensorValue = processedLux();
    total += sensorValue;

    delay(2);
  }

  return total/count;
}

float observeDish(int dishNum) {
	digitalWrite(leds[dishNum], HIGH);
	servo.write(dishAngle[dishNum]);
	delay(500);

	float opacity = averagedData(20);
	delay(500);
	Serial.println(opacity);
	digitalWrite(leds[dishNum], LOW);
}

void loop() {
	if (Serial.available() > 0) {
		char incomingByte = Serial.read();
		char command = incomingByte - '0';

		if (command > -1 && command < 4)
			observeDish(command);
		else if (command == 4) {
			for (int i=0; i<4; i++) {
				observeDish(i);
			}
		}
	}
	
}