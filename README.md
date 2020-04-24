# SmartBin
 
![](https://github.com/williampaciaroni/SmartBin/blob/master/TrashIT_bb.png)

## Description ##
The SmartBin is composed by three ArduinoUNO.
One of them is the "Core" and it manages the bluetooth module, the barcode scanner and it communicates with the other two ArduinoUNOs.
The Core receives data from TrashIT application via BLE and validate them. If data are valid then it says to the right ArduinoUNO to move the Servo and after that it waits for a response. The ArduinoUNO moves the Servo and starts a timer; until timer runs out the ArduinoUNO waits for PIRSensors. It closes the Servo when it receives a Signal from one of the PIRSensors or when the timer runs out.