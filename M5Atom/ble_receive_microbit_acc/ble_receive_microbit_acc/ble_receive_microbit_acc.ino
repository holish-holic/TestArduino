#include "BLEDevice.h"
static BLEUUID ubit_serviceUUID("E95D0753-251D-470A-A062-FA1922DFA9A8");
static BLEUUID ubit_accUUID("E95DCA4B-251D-470A-A062-FA1922DFA9A8");
//static BLEAddress *pServerAddress = new BLEAddress("fc:de:2b:78:1d:4b");  //赤　自分のMicrobitのMACアドレス　赤はBLE＋加速度センサするとフリーズしてしまう
static BLEAddress *pServerAddress = new BLEAddress("fb:79:2c:d9:05:b2");  //赤　自分のMicrobitのMACアドレス
static BLERemoteCharacteristic *pRemoteCharacteristic;

static int count = 0;
static void Callback(
    BLERemoteCharacteristic *pBLERemoteCharacteristic,
    uint8_t *pData,
    size_t length,
    bool isNotify)
{
    if (count < 100)
    {
      ++count;
      return;
    }
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

Serial.begin(115200);
    Serial.print("Forming a connection to ");
    Serial.println((*pServerAddress).toString().c_str());

    BLEClient *pClient = BLEDevice::createClient();
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
        return;
    }
    Serial.println(" - abbbbbbbbbbbbbbbb");


    uint8_t data[2] = {0x01, 0x00}; //Notify
    //uint8_t data[2] = {0x02, 0x00}; //Indicate　
    pRD->writeValue(data, 2, false);
    Serial.println(" - bbbbbbbbbbbbbbbbbbbb");

    pRemoteCharacteristic->registerForNotify(Callback);
    Serial.println(" - ccccccccccccccccccccc");

}

void loop()
{

}
