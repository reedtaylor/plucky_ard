
// config: ////////////////////////////////////////////////////////////

/*************************  General Config *******************************/

#define bufferSize 1024

/*************************  UART Config *******************************/

#define UART_BAUD 115200
#define SERIAL_PARAM SERIAL_8N1

#define SERIAL_DE_UART_NUM 1 // could be 2; is 1 weirdly reserved?
#define SERIAL_DE_RX_PIN 16
#define SERIAL_DE_TX_PIN 17

#define SERIAL_BLE_UART_NUM 2 // could be 2; is 1 weirdly reserved?
#define SERIAL_BLE_RX_PIN 13
#define SERIAL_BLE_TX_PIN 27
#define SERIAL_BLE_CTS_PIN 12
#define SERIAL_BLE_RTS_PIN 33


/*************************  WiFi & TCP Config *******************************/

#define WIFI

const char* ssid     = "myssid";
const char* password = "wpa2passwd";
const char* host = "de1";

#define MAX_TCP_CLIENTS 10
#define TCP_PORT 9090

/// end config ///////////////////////////////////////////////////////////

// Enumerate the hardware serial devices
#include "driver/uart.h"
HardwareSerial & Serial_USB = Serial;
HardwareSerial Serial_DE(SERIAL_DE_UART_NUM);
HardwareSerial Serial_BLE(SERIAL_BLE_UART_NUM);


#ifdef WIFI
#include <WiFi.h>
WiFiServer TCPServer(TCP_PORT);
WiFiClient TCPClient[MAX_TCP_CLIENTS];
#endif

void setup() {
  delay(500);
  
  Serial_USB.begin(UART_BAUD);
  Serial_DE.begin(UART_BAUD, SERIAL_PARAM, SERIAL_DE_RX_PIN, SERIAL_DE_TX_PIN);
  Serial_BLE.begin(UART_BAUD, SERIAL_PARAM, SERIAL_BLE_RX_PIN, SERIAL_BLE_TX_PIN);

  // Need to make a couple of "low level" esp-idf-esque calls to setup HW flow
  // contol for the BLE adaptor.  I think this is doable because it's provided
  // in the driver here:
  // https://github.com/espressif/arduino-esp32/blob/master/tools/sdk/include/driver/driver/uart.h
  // 
  // For now only enabling CTS because it's only the BLE adaptor side that might 
  // use this.  May need to force the RTS pin low, we'll see
  // Reference: esp_err_t uart_set_hw_flow_ctrl(uart_port_t uart_num, uart_hw_flowcontrol_t flow_ctrl, uint8_t rx_thresh);
  uart_set_hw_flow_ctrl(UART_NUM_2, UART_HW_FLOWCTRL_CTS_RTS, 0);

  // Reference: esp_err_t uart_set_pin(uart_port_t uart_num, int tx_io_num, int rx_io_num, int rts_io_num, int cts_io_num);
  uart_set_pin(UART_NUM_2, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, SERIAL_BLE_RTS_PIN, SERIAL_BLE_CTS_PIN);


  delay(10);

  Serial_DE.println("Hello World");


  // Connect to WiFi -- TODO make this all based on user config
  // TODO wrap this in debug
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());


}

void loop() {
  Serial.println("Hello World");
  Serial_DE.println("Hello World");
  Serial_BLE.println("Hello World");
  delay(500);
  Serial.println("Goodbye World");
  Serial_DE.println("Goodbye World");
  Serial_BLE.println("Goodbye World");
  delay(500);
}
