#include <SoftwareSerial.h>
#include <Servo.h>

byte face_pos[2];

Servo xD;
Servo yD;
Servo xL;
Servo yL;

const int limInfx = 0;
const int limSupx = 180;
const int limInfy = 0;
const int limSupy = 180;

const int Dled = 13;
const int Lled = 4;

int count = 0;

void setup() {
  Serial.begin(9600);
  
  xD.attach(5);
  yD.attach(6);
  xL.attach(9);
  yL.attach(10);

  pinMode(Dled, OUTPUT);
  pinMode(Lled, OUTPUT);

  xD.write(90);
  yD.write(90);
  xL.write(90);
  yL.write(90);
  // informing that arduino is ready
  digitalWrite(Dled, HIGH);
  digitalWrite(Dled, HIGH);
  delay(3000);
  digitalWrite(Dled, LOW);
  digitalWrite(Dled, LOW);
}

void loop() {
  if(Serial.available()) {
    count = 0;
    digitalWrite(Dled, HIGH);
    digitalWrite(Dled, HIGH);
    Serial.readBytes(face_pos, 2);
    Serial.flush();
    digitalWrite(Dled, LOW);
    digitalWrite(Dled, LOW);
    /*
    Serial.print("x=");
    Serial.print(face_pos[0]);
    Serial.print("y=");
    Serial.print(face_pos[1]);
    */
    xD.write(orderToAngle(face_pos[0],limInfx,limSupx));
    yD.write(orderToAngle(face_pos[1],limInfy,limSupy));
    xL.write(orderToAngle(face_pos[0],limInfx,limSupx));
    yL.write(orderToAngle(face_pos[1],limInfy,limSupy));
  } 
}

int orderToAngle(byte v, int limInf, int limSup) {
  int angle = (((limSup - limInf)*int(v))/255)+limInf;
  return angle;
}
