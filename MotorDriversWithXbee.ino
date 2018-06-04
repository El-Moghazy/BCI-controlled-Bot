#include <Wire.h>
#define    LIDARLite_ADDRESS   0x62          // Default I2C Address of LIDAR-Lite.
#define    RegisterMeasure     0x00          // Register to write to initiate ranging.
#define    MeasureValue        0x04          // Value to initiate ranging.
#define    RegisterHighLowB    0x8f          // Register to get both High and Low bytes in 1 call.
int reading = 0;

int speed = 250;

int motorpin1 = 2 ;
int motorpin2 = 4 ;
int speedpin1 = 3 ;
int speedpin2 = 5 ;
int beeb = 8 ;
void setup() {
  pinMode(motorpin1, OUTPUT);
  pinMode(motorpin2, OUTPUT);
  pinMode(speedpin1, OUTPUT);
  pinMode(speedpin2, OUTPUT);
  pinMode(beeb, OUTPUT);
  Wire.begin(); // join i2c bus

  Serial.begin(9600);
}
void loop()
{

  while (Serial.available() > 0) {
    char incomingByte = Serial.read();
    Serial.println(incomingByte);
    if (incomingByte == 'w') {
      forward();
    }
    else if (incomingByte == 's') {
      backward();
    }
    else if (incomingByte == 'd') {
      right();
    }
    else if (incomingByte == 'a') {

      left();
    }

    else if (incomingByte == 'i') {
      if ( speed <=  240 ) {
        speed += 10;
      }
    }

    else if (incomingByte == 'p') {
      if ( speed >=  10 ) {
        speed -= 10;
      }
    }

    else if (incomingByte == 'z')
    {

      analogWrite(speedpin1, 0);
      analogWrite(speedpin2, 0);

    }

    else if (incomingByte == 'b')
    {

      digitalWrite(beeb, 1);
      delay(120);
      digitalWrite(beeb, 0);
    }

  }
  Serial.println(distance());
  if (distance() < 30 ) {
    analogWrite(speedpin1, 0);
    analogWrite(speedpin2, 0);
  }

}

void backward() {
  digitalWrite (motorpin1, LOW);
  digitalWrite (motorpin2, HIGH);
  analogWrite(speedpin1, speed);
  analogWrite(speedpin2, speed);
}

void forward() {
  digitalWrite (motorpin1, HIGH);
  digitalWrite (motorpin2, LOW);
  analogWrite(speedpin1, speed);
  analogWrite(speedpin2, speed);
}

void right() {
  digitalWrite (motorpin1, HIGH);
  digitalWrite (motorpin2, HIGH);
  analogWrite(speedpin1, speed);
  analogWrite(speedpin2, speed);
}

void left() {
  digitalWrite (motorpin1, LOW);
  digitalWrite (motorpin2, LOW);
  analogWrite(speedpin1, speed);
  analogWrite(speedpin2, speed);
}

int distance() {

  Wire.beginTransmission((int)LIDARLite_ADDRESS); // transmit to LIDAR-Lite
  Wire.write((int)RegisterMeasure); // sets register pointer to  (0x00)
  Wire.write((int)MeasureValue); // sets register pointer to  (0x00)
  Wire.endTransmission(); // stop transmitting

  delay(20); // Wait 20ms for transmit

  Wire.beginTransmission((int)LIDARLite_ADDRESS); // transmit to LIDAR-Lite
  Wire.write((int)RegisterHighLowB); // sets register pointer to (0x8f)
  Wire.endTransmission(); // stop transmitting

  delay(20); // Wait 20ms for transmit

  Wire.requestFrom((int)LIDARLite_ADDRESS, 2); // request 2 bytes from LIDAR-Lite

  if (2 <= Wire.available()) // if two bytes were received
  {
    reading = Wire.read(); // receive high byte (overwrites previous reading)
    reading = reading << 8; // shift high byte to be high 8 bits
    reading |= Wire.read(); // receive low byte as lower 8 bits
    reading = reading - 15 ;

  }
  return reading;

}

