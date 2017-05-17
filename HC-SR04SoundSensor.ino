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
unsigned long timeout = 450*60;
unsigned long minPulseOK = 10*59;
unsigned long lastDist = 400;
float k = 0.1;

int minRangeOK=50; // If a car passes and gives these values then it is ok to count.
int maxRangeOK=350;
int carCount = 0; // number of cars
int consider = 3; // if distance detected. Count from 3 to Infinity.
int consecutive = 0; // if consecutive>=consider then carCount++;
int delayMs=50; // Check every 50 milli seconds.
unsigned long ts;

/* Local prototype functions */
unsigned long getDistance();

unsigned long getDistance( ) {

   unsigned long pulseDuration, dist, temp;
   
    /* The following trigPin/echoPin cycle is used to determine the
     distance of the nearest object by bouncing soundwaves off of it. */ 
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2); 

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); 
 
  digitalWrite(trigPin, LOW);
  pulseDuration = pulseIn(echoPin, HIGH, timeout);
  if(pulseDuration > minPulseOK ) {
  // Calculate the distance (in cm) based on the speed of sound.
    dist = pulseDuration/58.2;
  } else {
    // pulsein timed out so assign maximum distancea
    dist = 400;
  }

/* Exponential averaging. 0 < k < 1. 
 *  temp = dist;
 *  dist = k*(lastDist - dist);
 *  lastDist = temp
 */
  return dist;
}

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
  
  char aCommand;
  
  distance = getDistance();
  
  if (distance >= maxRangeOK || distance <= minRangeOK) {
// outside the 50-350 range
    Serial.print("IGNORING :: ");
    Serial.println(distance);
    if ( consecutive >= 3 && distance >= maxRangeOK) {
      carCount++;
      ts=millis();
      Serial.print("COUNT :: ");
      Serial.print(ts);
      Serial.print(" ms  ");

      Serial.print(carCount);
      Serial.println(" ");
      /*
      digitalWrite(LEDPin, HIGH); 
      delay(10);
      digitalWrite(LEDPin, LOW); 
      */

    }
    consecutive = 0; 
  }  // End of outside consideration range
  
  else if (distance >= minRangeOK and distance <= maxRangeOK) {
    consecutive++;
    Serial.print("CONSIDERING :: ");
    Serial.print("[");
    Serial.print(consecutive);
    Serial.print("] ");
    Serial.print(distance);
    Serial.println(" cm");
    
  }
  // Check for command from raspberry pi
  if( Serial.available()>0 ) {
    aCommand = Serial.read();
    if( aCommand == 'R' ) {
      carCount = 0;
      Serial.println("RESET ARDUINO");
    }
  }
  
 // Delay  before next reading.
 delay(delayMs);
 
}
