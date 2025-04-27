
#include <LiquidCrystal.h>

//LCD
// 変数の生成とPINの割り当て
// rs:7 en:8 d4:9 d5:10 d6:11 d7:12　に対応
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

//Button
//   X
// Y   A
//   B
int switchPinX=2;
int switchPinY=3;
int switchPinA=5;
int switchPinB=4;

//Loop内のステート管理用
int loopState=0;

//オブジェクトの姿勢
struct Transform
{
  int posx;
  int posy;
  bool dirr;
  bool active;

  //  画面外かどうか？
  bool outside(const Transform& t) {
    if (t.posx < 0 || t.posx > 16
    || t.posy < 0 || t.posy > 2
    ) {
      return true;
    }
    return false;
  }
};

//ロゴ
class Logo {
public: 
  Transform transform;
  int updateInterval;
  int logoLength;
  const char* logo = nullptr;
  char displogo[32];
  Logo(const char* l)
    : transform()
    , updateInterval(0)
    , logo(l)
    , logoLength(0)
  {
    logoLength = 0;
    memset(displogo, 0, sizeof(displogo));
    updateInterval = 4;
  }

  void updateLogo() {
    if (!transform.active) return;
    
    updateInterval -= 1;
    if (updateInterval < 0) {

      logoLength += 1;
      strncpy(displogo, logo, logoLength);    

      updateInterval=2;
    }
  }
  const char* displayLogo() {return displogo;}
};
Logo logoNintendo("Nintendo");
Logo logoSwitch("Switch");
Logo logo3("3");

//プレイヤー
class Player
{
public:
  Player() 
  : transform()
  , bulletInterval(0)
  {

  }
  Transform transform;
  int bulletInterval;
  void update() {
    if (bulletInterval > 0) {
      --bulletInterval;
    }
  }
  bool canFire() {
    return bulletInterval == 0;
  }
};
Player player;

//弾
class Bullet
{
public:
  Bullet()
    : transform()
  {}

  Transform transform;
  void update(){
    if (transform.active)
    {
      transform.posx += transform.dirr ? 1 : -1;        
      if (transform.outside(transform)){
        transform.active=false;
      }
    }
  }
};
Bullet bullet[32];
//次発射できる弾インデックス
int bulletFireCount=0;

//敵
class Enemy{
public:
  Enemy()
  :transform()
  ,moveInterval(0)
  {
    
  }
  Transform transform;
  int moveInterval;
  void update() {
    if (transform.active) {
      moveInterval = constrain(moveInterval -1, 0, 10);
      if (moveInterval == 0) {
        transform.posx += transform.dirr ? 1 : -1;
        moveInterval = 4;
      }
      if (transform.outside(transform)){
        transform.active=false;
      }
    }
  }
};
Enemy enemy[16];

//配列サイズ取得マクロ
#define ARRAYSIZE(array) (sizeof(array)/sizeof(array[0]))

//初期化
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

  //LCD初期化
  lcd.begin(16, 2); //表示サイズを設定
  lcd.clear();  //画面をクリアする

  //シリアル通信の初期処理を行う
  Serial.begin(9600);
  
  //プレイヤー初期化
  player.transform.posx = 7;
  player.transform.posy = 1;
  player.transform.active = true;
  player.transform.dirr = true;
  //弾初期化
  for (int i=0; i< ARRAYSIZE(bullet); ++i){
    Bullet& b = bullet[i];
    b.transform.posx=0;
    b.transform.posy=0;
    b.transform.active=false;
    b.transform.dirr=false;
    //Serial.println(i);
  }
  bulletFireCount=0;
  //敵初期化
  for (int i=0; i< ARRAYSIZE(enemy); ++i){
    Enemy& e = enemy[i];
    e.transform.posx=0;
    e.transform.posy=0;
    e.transform.active=false;
    e.transform.dirr=false;
    e.moveInterval = 0;
    //Serial.println(i);
  }
  //ロゴ初期化
  logoNintendo.transform.posx=4;
  logoNintendo.transform.posy=0;
  logoNintendo.transform.active=false;
  logoNintendo.transform.dirr=false;
  logoSwitch.transform.posx=4;
  logoSwitch.transform.posy=1;
  logoSwitch.transform.active=false;
  logoSwitch.transform.dirr=false;
  logo3.transform.posx=11;
  logo3.transform.posy=1;
  logo3.transform.active=false;
  logo3.transform.dirr=false;
}

//ジョイスティックのアナログ値を-1.0～+1.0で返す
float normalizeJoystick(float src)
{
  const float resolution = 1024.0f;
  src = src * 2 - resolution; //-1024.0 ~ +1024.0
  src /= resolution; //-1.0f ~ 1.0f;
  return src;
}

//弾発射
void fire(int posx, int posy, bool mover)
{
  Bullet& b = bullet[bulletFireCount];
  b.transform.posx = posx;
  b.transform.posy = posy;
  b.transform.dirr = mover;
  b.transform.active=true;
  ++bulletFireCount;
  if (bulletFireCount >= ARRAYSIZE(bullet)) bulletFireCount=0;

  player.bulletInterval = 4;
}

//タイトルロゴ
void loopShowLogo() {

  if (!logoNintendo.transform.active) {
    logoNintendo.transform.active=true;
  }
  logoNintendo.updateLogo();

  if (strlen(logoNintendo.displayLogo()) > 4 
    && !logoSwitch.transform.active) {
    logoSwitch.transform.active=true;
  }
  logoSwitch.updateLogo();

  if (strlen(logoSwitch.displayLogo()) > 5
    && !logo3.transform.active) {
    logo3.transform.active=true;
  }
  logo3.updateLogo();

  lcd.clear();
  
  lcd.setCursor(logoNintendo.transform.posx, logoNintendo.transform.posy);
  lcd.print(logoNintendo.displayLogo());

  lcd.setCursor(logoSwitch.transform.posx, logoSwitch.transform.posy);
  lcd.print(logoSwitch.displayLogo());

  lcd.setCursor(logo3.transform.posx, logo3.transform.posy);
  lcd.print(logo3.displayLogo());
  
  delay(50);

  //ボタン入力によって次のステートへ遷移する
  if (LOW == digitalRead(switchPinA))  // ピンよりデータ取得
  {
    ++loopState;
    Serial.println("next");
  }
}

//敵の登場インターバル
int enemyAppearCount = 80;
//敵登場処理
void appearEnemy() {
  --enemyAppearCount;
  if (enemyAppearCount < 0) {
    for (int i=0; i< ARRAYSIZE(enemy); ++i){
      Enemy& e = enemy[i];
      if (e.transform.active) continue;
      e.transform.active = true;
      if (random(0, 2) == 0) {
        e.transform.posx = 0;
        e.transform.dirr = true;
      } else {
        e.transform.posx = 16;
        e.transform.dirr = false;
      }      
      e.transform.posy = random(0, 2);
      e.moveInterval = 8;
      break;
    }
    enemyAppearCount=30;
  }
}

//ゲームメイン
void loopGame() {
  
  //プレイヤー入力処理
  //x軸方向とy軸方向の値を取得する
  float nml_x = normalizeJoystick(analogRead(A0));
  float nml_y = normalizeJoystick(analogRead(A1));
  if (nml_x > 0.5f) {
    ++player.transform.posx;
    player.transform.dirr=true;
  }
  else if (nml_x < -0.5f) {
    --player.transform.posx;
    player.transform.dirr=false;
  }
  player.transform.posx = constrain(player.transform.posx, 0, 15);
  if (nml_y > 0.5f) ++player.transform.posy;
  else if (nml_y < -0.5f) --player.transform.posy;
  player.transform.posy = constrain(player.transform.posy, 0, 1);
  //弾発射入力処理
  if (LOW == digitalRead(switchPinA))  // ピンよりデータ取得
  {
    if (player.canFire()) {
      fire(player.transform.posx, player.transform.posy, player.transform.dirr);
      //Serial.println("fire");
    }
  }
  
  //各更新
  player.update();
  for (int i=0; i< ARRAYSIZE(bullet); ++i)
  {
    Bullet& b = bullet[i];
    b.update();
  }
  appearEnemy();
  for (int i=0; i< ARRAYSIZE(enemy); ++i){
    Enemy& e = enemy[i];
    e.update();
  }
  //弾と敵の当たり判定
  for (int bi=0; bi< ARRAYSIZE(bullet); ++bi)
  {
    Bullet& b = bullet[bi];
    if (!b.transform.active) continue;

    for (int ei=0; ei< ARRAYSIZE(enemy); ++ei){
      Enemy& e = enemy[ei];
      if (!e.transform.active) continue;

      if (e.transform.posx == b.transform.posx
      && e.transform.posy == b.transform.posy){
        b.transform.active = e.transform.active = false;
      }
    }
  }
  
  //描画
  lcd.clear();
  
  for (int i=0; i< ARRAYSIZE(enemy); ++i){
    Enemy& e = enemy[i];
    if (e.transform.active){
      lcd.setCursor(e.transform.posx, e.transform.posy);
      lcd.print("E");
    }
  }
  for (int i=0; i< ARRAYSIZE(bullet); ++i){
    Bullet& b = bullet[i];
    if (b.transform.active){
      lcd.setCursor(b.transform.posx, b.transform.posy);
      lcd.print("-");
    }
  }
  lcd.setCursor(player.transform.posx, player.transform.posy); //1列目1行目に表示
  lcd.print(player.transform.dirr ? ">" : "<"); //測定した距離をディスプレイに表示

  delay(100);
}
//何も表示しない状態
void loopClear(){
  lcd.clear();
  delay(1000);
  ++loopState;
}
//メインループ
void loop(){
  switch (loopState){
    case 0: loopClear();
    case 1: loopShowLogo(); break;
    case 2: loopClear(); break;
    case 3: loopGame(); break;
  }
}