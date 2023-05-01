#include "BLEDevice.h"
#include "M5Atom.h "
static BLEUUID ubit_event_serviceUUID("E95D93AF-251D-470A-A062-FA1922DFA9A8");  //event service


static BLEUUID ubit_event_characteristic_microbit_requirementsUUID("E95DB84C-251D-470A-A062-FA1922DFA9A8");  //microbit requirements
static BLEUUID ubit_event_characteristic_microbit_eventUUID("E95D9775-251D-470A-A062-FA1922DFA9A8"); //microbit event
static BLEUUID ubit_event_characteristic_client_requirementsUUID("E95D23C4-251D-470A-A062-FA1922DFA9A8"); //Client Requirements
static BLEUUID ubit_event_characteristic_client_eventUUID("E95D5404-251D-470A-A062-FA1922DFA9A8"); //client event
static BLEUUID* characteristicUUIDs[4] = {
  &ubit_event_characteristic_microbit_requirementsUUID,
  &ubit_event_characteristic_microbit_eventUUID,
  &ubit_event_characteristic_client_requirementsUUID,
  &ubit_event_characteristic_client_eventUUID,
};

//static BLEAddress *pServerAddress = new BLEAddress("fc:de:2b:78:1d:4b");  //赤　自分のMicrobitのMACアドレス　赤はBLE＋加速度センサするとフリーズしてしまう
static BLEAddress *pServerAddress = new BLEAddress("fb:79:2c:d9:05:b2");  //緑　自分のMicrobitのMACアドレス
//static BLERemoteCharacteristic *pRemoteCharacteristic_microbitrequirements;
//static BLERemoteCharacteristic *pRemoteCharacteristic_microbitevent;
//static BLERemoteCharacteristic *pRemoteCharacteristic_clientrequirements;
//static BLERemoteCharacteristic *pRemoteCharacteristic_clientevent;
BLERemoteCharacteristic* remoteCharacteristics[4] = {
  nullptr,
  nullptr,
  nullptr,
  nullptr,
};
typedef struct event {
  uint16_t event_type;
  uint16_t event_value;
} event;

static void CallbackNotify(
    BLERemoteCharacteristic *pBLERemoteCharacteristic,
    uint8_t *pData,
    size_t length,
    bool isNotify)
{

    Serial.print("Notify/Indicate Callback for characteristic: ");
    Serial.print(pBLERemoteCharacteristic->getUUID().toString().c_str());
    Serial.print(" data length ");
    Serial.print(length);
    Serial.print(" data: ");
    event* ev = (event*)pData;
    /*
    for(int i = 0;i < length ;i++)
    {
      Serial.printf("%02x", pData[i]);
    }
    */
    Serial.printf("%0d, %0d", ev->event_type, ev->event_value);
    Serial.printf(" isNotify=%d", isNotify);
    Serial.print("\n");
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
    BLERemoteService *pRemoteService = pClient->getService(ubit_event_serviceUUID);
    if (pRemoteService == nullptr)
    {
        Serial.print("Failed to find our service UUID: ");
        Serial.println(ubit_event_serviceUUID.toString().c_str());
        return;
    }
    Serial.println(" - Found our service");

    // Obtain a reference to the characteristic in the service of the remote BLE server.    
    for (int i=0; i< 4; ++i)
    {
      BLEUUID* uuid = characteristicUUIDs[i];
      Serial.printf("require [%s].\n", uuid->toString().c_str());

      BLERemoteCharacteristic* p = pRemoteService->getCharacteristic(*uuid);
      if (p == nullptr)
      {
          Serial.print("Failed to find our characteristic UUID: ");
          Serial.println(uuid->toString().c_str());
      }
      /*
      //enable Notify/Indicate　
      BLERemoteDescriptor *pRD = p->getDescriptor(BLEUUID((uint16_t)0x2902));
      if (pRD == nullptr)
      {
          Serial.print("Failed to find our descriptor UUID: ");
          Serial.println(BLEUUID((uint16_t)0x2902).toString().c_str());
      }
      else
      {
          uint8_t data[2] = {0x01, 0x00}; //Notify
          //uint8_t data[2] = {0x02, 0x00}; //Indicate　
          pRD->writeValue(data, 2, false);
      }
      */
      p->registerForNotify(CallbackNotify);

      Serial.printf("canNotify:%d\n", p->canNotify());
      Serial.printf("canRead:%d\n", p->canRead());
      Serial.printf("canWrite:%d\n", p->canWrite());
    
      remoteCharacteristics[i] = p;
    }
    Serial.println(" - Found our characteristic");

}



int eventCount = 0;

void loop()
{

  if (M5.Btn.wasReleased())
  {
    event ev;
    int e = eventCount % 6;
    switch (e)
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
        ev.event_value = 10; //button 1 up
        break;
      }
      case 2:
      {
        ev.event_type = 2; // button b
        ev.event_value = 1; //down
        break;
      }
      case 3:
      {
        ev.event_type = 2; //button b
        ev.event_value = 0; //any
        break;
      }
      case 4:
      {
        ev.event_type = 4649; //test 1
        ev.event_value = 4649; //any
        break;
      }
      case 5:
      {
        ev.event_type = 5963; //test 2
        ev.event_value = 5963; //any
        break;
      }
    }
    //Serial.printf("size=%d\n", (int)(sizeof(data)));
    Serial.printf("size=%d, cnt=%d, id=%d, event=%d\n", (int)(sizeof(ev)), e, ev.event_type, ev.event_value);
    ++eventCount;
        
    remoteCharacteristics[2]->writeValue((uint8_t*)&ev, sizeof(ev), true);    
    
  }

    delay(50);
    M5.update();    
}
