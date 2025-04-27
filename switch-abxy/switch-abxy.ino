int state=0;
//   X
// Y   A
//   B
int switchPinX=2;
int switchPinY=3;
int switchPinA=5;
int switchPinB=4;
void setup() {
  // put your setup code here, to run once:
  pinMode(switchPinA, INPUT_PULLUP);     // ボタンスイッチ用に入力に設定
  pinMode(switchPinB, INPUT_PULLUP);     // ボタンスイッチ用に入力に設定
  pinMode(switchPinX, INPUT_PULLUP);     // ボタンスイッチ用に入力に設定
  pinMode(switchPinY, INPUT_PULLUP);     // ボタンスイッチ用に入力に設定
  state=0;
  //シリアル通信の初期処理を行う
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
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
  char buff[32];
  sprintf(buff, "abxy=%d%d%d%d", ba,bb,bx,by);
  Serial.println(buff);       // シリアルモニタに出力
  delay(500);
}
