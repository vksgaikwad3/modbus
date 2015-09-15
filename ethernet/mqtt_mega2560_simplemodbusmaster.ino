#include <SimpleModbusMaster.h>

#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>

#define baud 9600
#define timeout 1000
#define polling 1000 
#define retry_count 10
unsigned long time;
char message_buff[100];

byte mac[]    = {  0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
IPAddress ip(192, 168, 30, 90);
IPAddress netmask(255, 255,255, 0);
IPAddress gateway(192, 168, 30, 1);
IPAddress server(190, 97, 168, 236);


void callback(char* topic, byte* payload, unsigned int length);


EthernetClient ethClient;
PubSubClient client(server,1883,callback,ethClient);



// used to toggle the receive/transmit pin on the driver
#define TxEnablePin 2 

#define LED 9
long randNumber; 
// The total amount of available memory on the master to store data
#define TOTAL_NO_OF_REGISTERS 1

// This is the easiest way to create new packets
// Add as many as you want. TOTAL_NO_OF_PACKETS
// is automatically updated.
enum
{
  PACKET1,
  PACKET2,
  TOTAL_NO_OF_PACKETS // leave this last entry
};

// Create an array of Packets to be configured
Packet packets[TOTAL_NO_OF_PACKETS];

// Masters register array
unsigned int regs[TOTAL_NO_OF_REGISTERS];

void setup()
{
  Serial.begin(9600);
  // Initialize each packet
  modbus_construct(&packets[PACKET1], 1, READ_HOLDING_REGISTERS, 0, 1, 0);
  modbus_construct(&packets[PACKET2], 1, PRESET_MULTIPLE_REGISTERS, 1, 1, 0);
  
  // Initialize the Modbus Finite State Machine
  modbus_configure(&Serial3, baud, SERIAL_8N2, timeout, polling, retry_count, TxEnablePin, packets, TOTAL_NO_OF_PACKETS, regs);
  


Ethernet.begin(mac, ip);
  if (client.connect("arduinoClient")) {
    client.publish("test/temp2","hello world");
    client.subscribe("test/temp3");
  }


}


void callback(char* topic, byte* payload, unsigned int length) {


  int i = 0;
  for(i=0; i<length; i++) {
    message_buff[i] = payload[i];
  }
  message_buff[i] = '\0';
  
String msgString = String(message_buff);  
Serial.println("Payload: " + msgString);


  
}


void loop()
{

 // randNumber = random(10, 25); 
 //modbus_update();
  
  //regs[0] =  randNumber;
  //regs[0] =  x;
  client.loop();
 String msgString = String(message_buff);  
int z = msgString.toInt();
modbus_update(); 
regs[0] = z; 
  
   //delay(100);

}