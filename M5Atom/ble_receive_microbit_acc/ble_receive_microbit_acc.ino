#include "BLEDevice.h"
#include "M5Atom.h "
//static BLEUUID ubit_serviceUUID("E95D0753-251D-470A-A062-FA1922DFA9A8");  //acc service
//static BLEUUID ubit_serviceUUID("E95D9882-251D-470A-A062-FA1922DFA9A8");  //button service
//static BLEUUID ubit_serviceUUID("E95DD91D-251D-470A-A062-FA1922DFA9A8");  //led service
static BLEUUID ubit_serviceUUID("E95D93AF-251D-470A-A062-FA1922DFA9A8");  //event service


//static BLEUUID ubit_accUUID("E95DCA4B-251D-470A-A062-FA1922DFA9A8");  //acc
//static BLEUUID ubit_accUUID("E95DDA90-251D-470A-A062-FA1922DFA9A8");  //button a
//static BLEUUID ubit_accUUID("E95D93EE-251D-470A-A062-FA1922DFA9A8");  //led text
//static BLEUUID ubit_accUUID("E95DB84C-251D-470A-A062-FA1922DFA9A8");  //microbit requirements
//static BLEUUID ubit_accUUID("E95D9775-251D-470A-A062-FA1922DFA9A8"); //microbit event
//static BLEUUID ubit_accUUID("E95D23C4-251D-470A-A062-FA1922DFA9A8"); //Client Requirements
static BLEUUID ubit_accUUID("E95D5404-251D-470A-A062-FA1922DFA9A8"); //client event


//static BLEAddress *pServerAddress = new BLEAddress("fc:de:2b:78:1d:4b");  //赤　自分のMicrobitのMACアドレス　赤はBLE＋加速度センサするとフリーズしてしまう
static BLEAddress *pServerAddress = new BLEAddress("fb:79:2c:d9:05:b2");  //緑　自分のMicrobitのMACアドレス
static BLERemoteCharacteristic *pRemoteCharacteristic;

static int count = 0;
static void Callback(
    BLERemoteCharacteristic *pBLERemoteCharacteristic,
    uint8_t *pData,
    size_t length,
    bool isNotify)
{
  /*
    if (count < 100)
    {
      ++count;
      return;
    }
    */
    Serial.print("ababa Notify/Indicate Callback for characteristic: ");
    Serial.print(pBLERemoteCharacteristic->getUUID().toString().c_str());
    Serial.print(" data length ");
    Serial.print(length);
    Serial.print(" data: ");
    for(int i = 0;i < length ;i++)
    {
        Serial.printf("%02x", pData[i]);
    }
    Serial.print("\n");
    count = 0;
}


void setup()
{
    BLEDevice::init("");
    M5.begin(true, false, true);

    Serial.begin(115200);
    Serial.print("Forming a connection to ");
    Serial.println((*pServerAddress).toString().c_str());

    BLEClient *pClient = BLEDevice::createClient();
    //pClient->
    Serial.println(" - Created client");

    // Connect to the  BLE Server.
    pClient->connect(*pServerAddress, BLE_ADDR_TYPE_RANDOM); //RANDOMにしないといけない
    Serial.println(" - Connected to server");

    // Obtain a reference to the service we are after in the remote BLE server.
    BLERemoteService *pRemoteService = pClient->getService(ubit_serviceUUID);
    if (pRemoteService == nullptr)
    {
        Serial.print("Failed to find our service UUID: ");
        Serial.println(ubit_serviceUUID.toString().c_str());
        return;
    }
    Serial.println(" - Found our service");

    // Obtain a reference to the characteristic in the service of the remote BLE server.
    pRemoteCharacteristic = pRemoteService->getCharacteristic(ubit_accUUID);
    if (pRemoteCharacteristic == nullptr)
    {
        Serial.print("Failed to find our characteristic UUID: ");
        Serial.println(ubit_accUUID.toString().c_str());
        return;
    }
    Serial.println(" - Found our characteristic");

    //enable Notify/Indicate　
    BLERemoteDescriptor *pRD = pRemoteCharacteristic->getDescriptor(BLEUUID((uint16_t)0x2902));
    if (pRD == nullptr)
    {
        Serial.print("Failed to find our descriptor UUID: ");
        Serial.println(BLEUUID((uint16_t)0x2902).toString().c_str());
        //return;
    }
    else
    {

        Serial.println(" - abbbbbbbbbbbbbbbb");


        //uint8_t data[2] = {0x01, 0x00}; //Notify
        //uint8_t data[2] = {0x02, 0x00}; //Indicate　
        //pRD->writeValue(data, 2, false);
        Serial.println(" - bbbbbbbbbbbbbbbbbbbb");
    }

    pRemoteCharacteristic->registerForNotify(Callback);
    
    Serial.printf("canNotify:%d\n", pRemoteCharacteristic->canNotify());
    Serial.printf("canRead:%d\n", pRemoteCharacteristic->canRead());
    Serial.printf("canWrite:%d\n", pRemoteCharacteristic->canWrite());
    Serial.println(" - ccccccccccccccccccccc");

}
static bool fa=false;
typedef struct event {
  uint16_t event_type;
  uint16_t event_value;
} event;
int eventCount=0;
void loop()
{
  if (!fa)
  {
    Serial.println(" - fa");
    fa=true;  
  }
  if (M5.Btn.wasReleased())
  {
    //delay(100);
    //uint8_t data[] = {1, 1, 1, 0};
    //char data[] = "nagaho";
    event ev;
    switch (eventCount % 4)
    {
      case 0:
      {
        ev.event_type = 1104;
        ev.event_value = 0; //any
        break;
      }
      case 1:
      {
        ev.event_type = 1104;
        ev.event_value = 1; //any
        break;
      }
      case 2:
      {
        ev.event_type = 1104;
        ev.event_value = 13; //any
        break;
      }
      case 3:
      {
        ev.event_type = 2;
        ev.event_value = 0; //any
        break;
      }
    }
    //Serial.printf("size=%d\n", (int)(sizeof(data)));
    Serial.printf("size=%d, cnt=%d, id=%d, event=%d\n", (int)(sizeof(ev)), eventCount, ev.event_type, ev.event_value);
    ++eventCount;
        //pRemoteCharacteristic->writeValue((uint8_t*)data, sizeof(data), false);
    pRemoteCharacteristic->writeValue((uint8_t*)&ev, sizeof(ev), true);
    //BLERemoteDescriptor *pRD = pRemoteCharacteristic->getDescriptor(BLEUUID((uint16_t)0x2902));
    //pRD->writeValue((uint8_t*)&ev, sizeof(ev), false);
    
    Serial.println(ubit_accUUID.toString().c_str());
  }

  delay(50);
    M5.update();  // Read the press state of the key.  读取按键按下状态

    //if ()
}
