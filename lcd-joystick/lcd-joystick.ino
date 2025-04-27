#include <LiquidCrystal.h>


// 変数の生成とPINの割り当て
// rs:7 en:8 d4:9 d5:10 d6:11 d7:12　に対応
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);


//x軸方向の値
int x_value;

//y軸方向の値
int y_value;

//スイッチの状態を見る変数(押されていない:1 押されている:0)
bool switch_flag;

void setup() {

  //VRxに繋いだピン(A0)をを入力ピンとして設定する(デフォルト設定なので省略しても良い)
  pinMode(A0,INPUT);

  //VRyに繋いだピン(A1)をを入力ピンとして設定する(デフォルト設定なので省略しても良い)
  pinMode(A1,INPUT);
  
  //SWに繋いだピン(2)をプルアップ付きの入力ピンとして設定する
  pinMode(2,INPUT_PULLUP);

  //シリアル通信の初期処理を行う
  Serial.begin(9600);
}

void loop() {
  
  //x軸方向とy軸方向の値を取得する
  x_value=analogRead(A0);
  y_value=analogRead(A1);

  //スイッチの状態を取得する
  switch_flag=digitalRead(2);

  //x軸方向とy軸方向の値、スイッチの状態を表示する
  Serial.print("X: ");
  Serial.print(x_value);
  Serial.print("  Y: ");
  Serial.print(y_value);
  Serial.print("  SW: ");
  Serial.println(switch_flag);

  char buff[32];
  sprintf(buff, "%d, %d", x_value, y_value);
  Serial.println(buff);
  lcd.clear();
  lcd.setCursor(0, 0); //1列目1行目に表示
  //lcd.print("%d,%d", x_value, y_value); //ディスプレイ表示
  //lcd.setCursor(6, 0); //6列目1行目に表示
  lcd.print(buff); //測定した距離をディスプレイに表示
  delay(100);
}