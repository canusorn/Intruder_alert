/*
  Credit
  HC-SR04 https://create.arduino.cc/projecthub/abdularbi17/ultrasonic-sensor-hc-sr04-with-arduino-tutorial-327ff6
*/

#define echoPin PB10 // ต่อกับ Echo ของ HC-SR04
#define trigPin PB11 // ต่อกับ Trig ของ HC-SR04

#define LDR PA5  // ต่อกับ A0 ของ LDR Sensor

void setup() {
  Serial.begin(115200);
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT
}

void loop() {
  // อ่านค่าระยะห่างจาก HC-SR04
  int distance = distanceRead();  
  Serial.print("distance: "); Serial.print(distance); Serial.println(" cm");

  // อ่านค่าแสงจาก LDR Sensor
  int light = analogRead(LDR);
  Serial.print("light: "); Serial.println(light);

  delay(1000);
}

int distanceRead() {

  long duration; // variable for the duration of sound wave travel
  int distance; // variable for the distance measurement

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)

  return distance;
}
