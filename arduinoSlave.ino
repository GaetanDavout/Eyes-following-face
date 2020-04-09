#include <SoftwareSerial.h>
#include <Servo.h>

byte face_pos[3];

Servo xD;
Servo yD;
Servo xL;
Servo yL;

const int limInfx = 0;
const int limSupx = 180;
const int limInfy = 0;
const int limSupy = 180;

const int Dled = 13;
const int Lled = 3;

int count = 0;

void setup() {
  Serial.begin(9600);
  
  xD.attach(5);
  yD.attach(6);
  xL.attach(9);
  yL.attach(10);

  pinMode(Dled, OUTPUT);
  pinMode(Lled, OUTPUT);

  xD.write(90
  );
  yD.write(90);
  xL.write(90);
  yL.write(90);
  // informing that arduino is ready
  digitalWrite(Dled, HIGH);
  digitalWrite(Lled, HIGH);
  delay(3000);
  digitalWrite(Dled, LOW);
  digitalWrite(Lled, LOW);
}

void loop() {
  if(Serial.available()) {
    count = 0;
    Serial.readBytes(face_pos, 3);
    Serial.flush();

    if(face_pos[2] == 1) {
      digitalWrite(Dled, HIGH);
      digitalWrite(Lled, HIGH);
    } else {
      digitalWrite(Dled, LOW);
      digitalWrite(Lled, LOW);
    }
    
    xD.write(limSupx-orderToAngle(face_pos[0],limInfx,limSupx));
    yD.write(orderToAngle(face_pos[1],limInfy,limSupy));
    xL.write(limSupx-orderToAngle(face_pos[0],limInfx,limSupx));
    yL.write(orderToAngle(face_pos[1],limInfy,limSupy));
  } 
}

int orderToAngle(byte v, int limInf, int limSup) {
  int range = limSup - limInf;
  float angle = float(v)/255;
  angle = angle*range;
  angle = angle+limInf;
  return int(angle);
}
