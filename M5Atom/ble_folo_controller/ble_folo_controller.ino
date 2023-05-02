#include "BLEDevice.h"
#include "M5Atom.h "

static BLEUUID uuidEventService("E95D93AF-251D-470A-A062-FA1922DFA9A8");                            //event service
static BLEUUID uuidEventServiceCharacteristic_ClientEvent("E95D5404-251D-470A-A062-FA1922DFA9A8");  //client event

//static BLEAddress *pServerAddress = new BLEAddress("fc:de:2b:78:1d:4b");  //赤　自分のMicrobitのMACアドレス　赤はBLE＋加速度センサするとフリーズしてしまう
static BLEAddress serverAddress("fb:79:2c:d9:05:b2");  //緑　自分のMicrobitのMACアドレス

typedef struct event {
  uint16_t event_type;
  uint16_t event_value;
} event;

//  GAMEPADイベント
//  https://github.com/lancaster-university/microbit-dal/blob/master/inc/bluetooth/MESEvents.h#L94
static const int MES_DPAD_CONTROLLER_ID = 1104;     //  MES_DPAD_CONTROLLER_ID
static const int MES_DPAD_CONTROLLER_EVENT_A = 1;   //  MES_DPAD_BUTTON_A_DOWN  前進
static const int MES_DPAD_CONTROLLER_EVENT_B = 3;   //  MES_DPAD_BUTTON_B_DOWN  後退
static const int MES_DPAD_CONTROLLER_EVENT_3 = 13;  //  MES_DPAD_BUTTON_3_DOWN  右回転
static const int MES_DPAD_CONTROLLER_EVENT_4 = 15;  //  MES_DPAD_BUTTON_4_DOWN  左回転

//  ClientEventのキャラクタリスティック
BLERemoteCharacteristic* characteristicClientEvent = nullptr;

//  Client
BLEClient* pClient = nullptr;

//  atom led state
enum Led {
  Led_OFF,    //  初期状態
  Led_RED,    //  BLE 切断状態
  Led_WHITE,  //  BLE 接続状態
};
Led currentLed = Led_OFF;

void setLedColor(Led led) {
  if (led == currentLed) return;

  //  0xRRGGBB;
  switch (led) {
    case Led_OFF:
      {
        M5.dis.fillpix(0x0);
        break;
      }
    case Led_RED:
      {
        M5.dis.drawpix(0, 0, 0xff0000);
        break;
      }
    case Led_WHITE:
      {
        M5.dis.drawpix(0, 0, 0xffffff);
        break;
      }
  }
  currentLed = led;
}

void setup() {
  BLEDevice::init("m5atom");
  M5.begin(true, false, true);
  M5.IMU.Init();
  setLedColor(Led_OFF);

  Serial.begin(115200);
  Serial.print("Forming a connection to ");
  Serial.println(serverAddress.toString().c_str());

  pClient = BLEDevice::createClient();
  Serial.println(" - Created client");

  // Connect to the  BLE Server.
  pClient->connect(serverAddress, BLE_ADDR_TYPE_RANDOM);  //RANDOMにしないといけない
  Serial.println(" - Connected to server");

  // Obtain a reference to the service we are after in the remote BLE server.
  BLERemoteService* pRemoteService = pClient->getService(uuidEventService);
  if (pRemoteService == nullptr) {
    Serial.print("Failed to find our service UUID: ");
    Serial.println(uuidEventService.toString().c_str());
    return;
  }
  Serial.println(" - Found our service");

  BLERemoteCharacteristic* p = pRemoteService->getCharacteristic(uuidEventServiceCharacteristic_ClientEvent);
  if (p == nullptr) {
    Serial.print("Failed to find our characteristic UUID: ");
    Serial.println(uuidEventServiceCharacteristic_ClientEvent.toString().c_str());
  }
  Serial.println(" - Found our characteristic");

  characteristicClientEvent = p;
}


void loop() {

  if (pClient->isConnected()) {
    setLedColor(Led_WHITE);
  } else {
    setLedColor(Led_RED);
    return;
  }
  float y, p, r;
  M5.IMU.getAhrsData(&p, &r, &y);
  if (p < -45.0f || 45.0f < p) {
    //45.0f < p ? Serial.print("右回転") : Serial.print("左回転");
    event ev;
    ev.event_type = MES_DPAD_CONTROLLER_ID;
    ev.event_value = 45.0f < p
                       ? MES_DPAD_CONTROLLER_EVENT_3   //  右回転
                       : MES_DPAD_CONTROLLER_EVENT_4;  //  左回転
    characteristicClientEvent->writeValue((uint8_t*)&ev, sizeof(ev), true);
  } else if ((-135.0f < r && r < 0.0f) || (0.0f < r && r < 135.0f)) {
    //(0.0f < r && r < 135.0f) ? Serial.print("前進") : Serial.print("後退");
    event ev;
    ev.event_type = MES_DPAD_CONTROLLER_ID;
    ev.event_value = (0.0f < r && r < 135.0f)
                       ? MES_DPAD_CONTROLLER_EVENT_A   //  前進
                       : MES_DPAD_CONTROLLER_EVENT_B;  //  後退
    characteristicClientEvent->writeValue((uint8_t*)&ev, sizeof(ev), true);
  } else {
    event ev;
    ev.event_type = MES_DPAD_CONTROLLER_ID;
    ev.event_value = 0;  //  停止
    characteristicClientEvent->writeValue((uint8_t*)&ev, sizeof(ev), true);
  }
  //Serial.printf("pitch=%3.2f, roll=%3.2f, yaw=%3.2f\n", p, r, y);
  delay(50);
  M5.update();
}
