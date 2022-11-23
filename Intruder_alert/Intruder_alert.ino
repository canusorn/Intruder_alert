/*
  Credit
  HC-SR04 https://create.arduino.cc/projecthub/abdularbi17/ultrasonic-sensor-hc-sr04-with-arduino-tutorial-327ff6

  INPUT เซนเซอร์ระยะทาง HC-SR04 เซนเซอร์ดิจิตอล
  echoPin PB10    // ต่อกับ Echo ของ HC-SR04
  trigPin PB11    // ต่อกับ Trig ของ HC-SR04

  INPUT เซนเซอร์แสง LDR อนาลอก
  LDR PA5         // ต่อกับ A0 ของ LDR Sensor

  OUTPUT ไฟ LED
   LED1 PC13      // กำหนดขาต่อกับ LED1
   LED2 PC14      // กำหนดขาต่อกับ LED2
*/

#define echoPin PB10    // ต่อกับ Echo ของ HC-SR04
#define trigPin PB11    // ต่อกับ Trig ของ HC-SR04

#define LDR PA5         // ต่อกับ A0 ของ LDR Sensor

#define LED1 PC13      // กำหนดขาต่อกับ LED1
#define LED2 PC14      // กำหนดขาต่อกับ LED2

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600); // Serial2 on rx,tx = PA3,PA2
  
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT

  digitalWrite(LED1, LOW);
  pinMode(LED1, OUTPUT);
  digitalWrite(LED2, LOW);
  pinMode(LED2, OUTPUT);
}

void loop() {
  // อ่านค่าระยะห่างจาก HC-SR04
  int distance = distanceRead();
  Serial.print("distance: "); Serial.print(distance); Serial.println(" cm");

  // อ่านค่าแสงจาก LDR Sensor
  int light = analogRead(LDR);   // อ่านค่าอนาลอก มีค่า 0-4095
  Serial.print("light: "); Serial.println(light);

  // เงือนไขการทำงานหลอดไฟ1
  if (distance < 100) {         // ระยะนอ้ยกว่า 100cm ให้ LED1 ทำงาน
    digitalWrite(LED1, HIGH);
  } else {                                      // led ดับ
    digitalWrite(LED1, LOW);
  }

  // เงือนไขการทำงานหลอดไฟ2
  if (light > 3000) {         // ค่าจากแสงแสงมากกว่า 3000 ให้ LED2 ทำงาน
    digitalWrite(LED2, HIGH);
  } else {                                      // led ดับ
    digitalWrite(LED2, LOW);
  }

  // ส่งค่าไปยัง ESP
  Serial2.print("distance:"); Serial2.print(distance);
  Serial2.print("  light:"); Serial2.print(light);
  Serial2.print("  led_one:"); Serial2.print(digitalRead(LED1));
  Serial2.print("  led_two:"); Serial2.print(digitalRead(LED2));
  Serial2.print("\n");
  
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
