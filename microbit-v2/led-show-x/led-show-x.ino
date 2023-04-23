#include "pins_arduino.h"

//  https://github.com/sandeepmistry/arduino-nRF5/blob/master/variants/BBCmicrobitV2/variant.cpp
//  を参照すること
int row1 = 21;
int row2 = 22;
int row3 = 23;
int row4 = 24;
int row5 = 25;
int col1 = 4;
int col2 = 7;
int col3 = 3;
int col4 = 6;
int col5 = 10;



void setup()
{
  //  シリアル初期化（デバッグ用）
  Serial.begin(9600);
  while (!Serial);

  //  pinはすべて出力モードにする
  pinMode(row1, OUTPUT);
  pinMode(row2, OUTPUT);
  pinMode(row3, OUTPUT);
  pinMode(row4, OUTPUT);
  pinMode(row5, OUTPUT);
  pinMode(col1, OUTPUT);
  pinMode(col2, OUTPUT);
  pinMode(col3, OUTPUT);
  pinMode(col4, OUTPUT);
  pinMode(col5, OUTPUT);
}

//  rowはHIGHで灯火、LOWで消灯
//  colはHIGHで消灯、LOWで灯火
//  任意の形のLED点灯のやり方としては、
//  1.列（colX、縦）で点灯させるところをONにする
//  2.delay(n)で少しの間点灯させる
//  3.1でONにした箇所をOFFにする
void loop() {
  int d = 1;
  //  ｘを描く
  //  x---x
  //  -x-x-
  //  --x--
  //  -x-x-
  //  x---x
  
  //  col1
  digitalWrite(col1, LOW);
  digitalWrite(row1, HIGH);
  digitalWrite(row5, HIGH);
  delay(d);
  {
    digitalWrite(col1, HIGH);
    digitalWrite(row1, LOW);
    digitalWrite(row5, LOW);
  }
  
  //  col2
  digitalWrite(col2, LOW);
  digitalWrite(row2, HIGH);
  digitalWrite(row4, HIGH);
  delay(d);
  {
    digitalWrite(col2, HIGH);
    digitalWrite(row2, LOW);
    digitalWrite(row4, LOW);
  }

  //  col3
  digitalWrite(col3, LOW);
  digitalWrite(row3, HIGH);
  delay(d);
  {
    digitalWrite(col3, HIGH);
    digitalWrite(row3, LOW);
  }

  //  col4
  digitalWrite(col4, LOW);
  digitalWrite(row2, HIGH);
  digitalWrite(row4, HIGH);
  delay(d);
  {
    digitalWrite(col4, HIGH);
    digitalWrite(row2, LOW);
    digitalWrite(row4, LOW);
  }

  //  col5
  digitalWrite(col5, LOW);
  digitalWrite(row1, HIGH);
  digitalWrite(row5, HIGH);
  delay(d);
  {
    digitalWrite(col5, HIGH);
    digitalWrite(row1, LOW);
    digitalWrite(row5, LOW);
  }
}
