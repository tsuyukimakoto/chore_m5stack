#include <M5Stack.h>

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>

// UUID1
#define SERVICE_UUID "3f7b3ec2-0e6d-11ea-82fc-f40f242254e3"
#define CHARACTERISTIC_UUID "4067918c-0e6d-11ea-82fc-f40f242254e3"

// UUID4
// #define SERVICE_UUID "20759c32-8c07-494f-bc01-bc802e843dac"
// #define CHARACTERISTIC_UUID "31e87af5-3d86-4371-9fd1-2bb4fa956743"


BLEServer* pServer = NULL;
BLECharacteristic* pCharacteristic = NULL;
bool deviceConnected = false;

class BLEServerSampleCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      M5.Lcd.println("connect");
      deviceConnected = true;
    };
    void onDisconnect(BLEServer* pServer) {
      M5.Lcd.println("disconnect");
      deviceConnected = false;
    }
};

class SampleCallbacks: public BLECharacteristicCallbacks {
  void onRead(BLECharacteristic *pCharacteristic) {
    M5.Lcd.println("read");
    pCharacteristic->setValue("message from onRead.");
    pCharacteristic->notify();
  }

  void onWrite(BLECharacteristic *pCharacteristic) {
    M5.Lcd.println("write");
    std::string value = pCharacteristic->getValue();
    M5.Lcd.println(value.c_str());
  }
};

void setup(){

  Serial.begin(115200);
  M5.begin();
  m5.Speaker.mute();

  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextColor(GREEN , BLACK);
  M5.Lcd.setTextSize(3);

  M5.Lcd.println("BLE start.");
  BLEDevice::init("M5Stack");
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new BLEServerSampleCallbacks());
  BLEService *pService = pServer->createService(SERVICE_UUID);
  pCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE |
                                         BLECharacteristic::PROPERTY_NOTIFY |
                                         BLECharacteristic::PROPERTY_INDICATE
                    );
  pCharacteristic->setCallbacks(new SampleCallbacks());
  pCharacteristic->addDescriptor(new BLE2902());

  pService->start();
  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->start();
}

void buttonPushed(std::string button_message) {
  if(deviceConnected == true) {
      M5.Lcd.println(button_message.c_str());
      pCharacteristic->setValue(button_message.c_str());
      pCharacteristic->notify();
  } else {
    M5.Lcd.println("doesn't connect.");
  }
}

void loop() {
  M5.update();
  if (M5.BtnA.wasReleased()) {
    buttonPushed("Button A");
  } else if (M5.BtnB.wasReleased()) {
    buttonPushed("Button B");
  } else if (M5.BtnC.wasReleased()) {
    buttonPushed("Button C");
  }
}
