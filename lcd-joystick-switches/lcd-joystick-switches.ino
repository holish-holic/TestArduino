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
//   X
// Y   A
//   B
int switchPinX=2;
int switchPinY=3;
int switchPinA=5;
int switchPinB=4;
void setup() {

  //VRxに繋いだピン(A0)をを入力ピンとして設定する(デフォルト設定なので省略しても良い)
  pinMode(A0,INPUT);

  //VRyに繋いだピン(A1)をを入力ピンとして設定する(デフォルト設定なので省略しても良い)
  pinMode(A1,INPUT);
  
  //SWに繋いだピン(2)をプルアップ付きの入力ピンとして設定する
  pinMode(2,INPUT_PULLUP);

  //タクトスイッチ用
  pinMode(switchPinA, INPUT_PULLUP);     // ボタンスイッチ用に入力に設定
  pinMode(switchPinB, INPUT_PULLUP);     // ボタンスイッチ用に入力に設定
  pinMode(switchPinX, INPUT_PULLUP);     // ボタンスイッチ用に入力に設定
  pinMode(switchPinY, INPUT_PULLUP);     // ボタンスイッチ用に入力に設定

  lcd.begin(16, 2);

  //シリアル通信の初期処理を行う
  Serial.begin(9600);
}

void loop() {
  
  //x軸方向とy軸方向の値を取得する
  x_value=analogRead(A0);
  y_value=analogRead(A1);

  //スイッチの状態を取得する
  switch_flag=digitalRead(2);

  bool bx=false, by=false, ba=false, bb=false;
  if (LOW == digitalRead(switchPinX))  // ピンよりデータ取得
  {
    bx=true;
  }
  if (LOW == digitalRead(switchPinY))  // ピンよりデータ取得
  {
    by=true;
  }
  if (LOW == digitalRead(switchPinA))  // ピンよりデータ取得
  {
    ba=true;
  }
  if (LOW == digitalRead(switchPinB))  // ピンよりデータ取得
  {
    bb=true;
  }


  //x軸方向とy軸方向の値、スイッチの状態を表示する
  Serial.print("X: ");
  Serial.print(x_value);
  Serial.print("  Y: ");
  Serial.print(y_value);
  Serial.print("  SW: ");
  Serial.println(switch_flag);

  lcd.clear();

  char buff[32];
  sprintf(buff, "%d, %d", x_value, y_value);
  //Serial.println(buff);
  lcd.setCursor(0, 0); //1列目1行目に表示
  lcd.print(buff); //測定した距離をディスプレイに表示

  sprintf(buff, "ABXY=%d%d%d%d", ba,bb,bx,by);
  lcd.setCursor(0, 1); //1列目2行目に表示
  lcd.print(buff); //測定した距離をディスプレイに表示
  //Serial.println(buff);
  
  delay(100);
}