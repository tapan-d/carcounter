/*
    HC-SR04 Ping distance sensor:
    VCC to arduino 5v
    GND to arduino GND
    Echo to Arduino pin 7
    Trig to Arduino pin 8

    This sketch originates from Virtualmix: http://goo.gl/kJ8Gl

    Has been modified by Winkle ink here: http://winkleink.blogspot.com.au/2012/05/arduino-hc-sr04-ultrasonic-distance.html
    And modified further by ScottC here: http://arduinobasics.blogspot.com.au/2012/11/arduinobasics-hc-sr04-ultrasonic-sensor.html
    on 10 Nov 2012.
    */


// Breadbord
#define echoPin 5 // Echo Pin
#define trigPin 6 // Trigger Pin
#define LEDPin 13 // Onboard LED

int maximumRange = 400; // Maximum range needed
int minimumRange = 0; // Minimum range needed
unsigned long duration, distance; // Duration used to calculate distance
unsigned long timeout = maximumRange*60;

int minRangeOK=50; // If a car passes and gives these values then it is ok to count.
int maxRangeOK=350;
int carCount = 0; // number of cars
int consider = 3; // if distance detected. Count from 3 to Infinity.
int consecutive = 0; // if consecutive>=consider then carCount++;
int delayMs=50; // Check every 50 milli seconds.
unsigned long ts;

void setup() {
  // Serial connection
  //Serial.begin (57600);
  Serial.begin (9600);

  // Set pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(LEDPin, OUTPUT); // Use LED indicator (if required)

  Serial.println("SUCCESS : Done with setup.");

}

void loop() {

  /* The following trigPin/echoPin cycle is used to determine the
     distance of the nearest object by bouncing soundwaves off of it. */
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);

  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH, timeout);

  if(duration > 0 ) {
  // Calculate the distance (in cm) based on the speed of sound.
    distance = duration/58.2;
  } else {
    // pulsein timed out so assign maximum distancea
    distance = 400;
  }
  if (distance >= maxRangeOK || distance <= minRangeOK) {
// outside the 50-350 range
    Serial.print("IGNORING :: ");
    Serial.println(distance);
    if ( consecutive >= 3 ) {
      carCount++;
      ts=millis();
      Serial.print("COUNT :: ");
      Serial.print(ts);
      Serial.print(" ms  ");

      Serial.print(carCount);
      Serial.println(" ");
      digitalWrite(LEDPin, HIGH);
      delay(10);
      digitalWrite(LEDPin, LOW);
    }
      consecutive = 0;
  }
  else if (distance >= minRangeOK and distance <= maxRangeOK) {
    consecutive++;
    Serial.print("CONSIDERING :: ");
    Serial.print("[");
    Serial.print(consecutive);
    Serial.print("] ");
    Serial.print(distance);
    Serial.println(" cm");

  }

 // Delay  before next reading.
 delay(delayMs);

}
