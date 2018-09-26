#ifndef PJ-HA-RFM_GW_H
#define PJ-HA-RFM_GW_H

#include <arduino.h>
#include <RHMesh.h>
#include <RH_RF69.h>
#include <SPI.h>
#include <Ethernet2.h>        // Ethernet2 library, Adafruits Fork. Required for Feather Ethernet Wing.
#include <PubSubClient.h>


#define RH_MESH_MAX_MESSAGE_LEN 80  // Max size of packet for RadioHead
#define HARFPACKSIZE 52 // Proper packet size for one of my home automation RF packets

// RadioHead Mesh Addressing
#define CLIENT_ADDRESS 1

/* DEBUG CONFIGURATION PARAMETERS */
#define DEBUG // uncomment for debugging
//#define DEBUGPJ1 // uncomment for debugging
#define DEBUGPJ2 // uncomment for debugging
#ifdef DEBUG
  #define DEBUGPJ1 // if DEBUG thern ensure DEBUGPJ1 is enabled as it includes things DEBUG needs.
  #define DEBUGPJ2 // if DEBUG thern ensure DEBUGPJ2 is enabled as it includes things DEBUG needs.
#endif

#define SERIAL_BAUD 115200

#define VERSION "SAMD-RFM_GWvGitHub"   // this value can be queried as device 3

#define STRPAYLOADSIZE 32   // How many chars in the String Payload we send? (must match in GW and Node!!!!!)
#define RFTXRETRIES 5       // RF TX reties.

/* RFM_GW CORE CONFIGURATION PARAMETERS 
****************************************************/
// Wireless settings
#define NODEID 01				// unique node ID in the closed radio network;
//PJ WAS HERE - RFM_SS is very specific per hardware platform.
//              I changed RFM_SS to 10 when testing some of this on a Moteino
//              I used pin 8 on a real Uno (native SPI SS pin on this board is 10 but Eth shield hardwired to it
//                                          so as per forums etc, go with pin 8)
//              I used pin 53 for Mega2560 (native SPI SS pin on this board...see mega pinout diag)
//                  Note: Mega has hw SPI port on 50,51,52,53...so need to make sure all wiring to the RFM69 is correct.
#define PJ_RFM_SS      8				// Slave Select RFM69 is connected to.  See comments above, as this varies. MY_RF69_SPI_CS is a SenseBender pin name.
#define PJ_RFM_RESET   4
#define PJ_RFM_IRQ_PIN 3
#define PJ_RFM_IRQ_NUM 3
// define PJ_RFM_IS_RFM69HCW false (pre RH lib)

#define NETWORKID 111				// closed radio network ID

// Ethernet settings
// for use with Adafruit Ethernet FeatherWing
#if defined(ESP8266)
  // default for ESPressif
  #define WIZ_CS 15
#elif defined(ESP32)
  #define WIZ_CS 33
#elif defined(ARDUINO_STM32_FEATHER)
  // default for WICED
  #define WIZ_CS PB4
#elif defined(TEENSYDUINO)
  #define WIZ_CS 10
#elif defined(ARDUINO_FEATHER52)
  #define WIZ_CS 11
#else   // default for 328p, 32u4, M4 and M0
  #define WIZ_CS 10
#endif


// PIN settings
#define MQCON A3					// GREEN LED - MQTT Connection indicator.  LED_GREEN is a SenseBender pin name.
#define R_LED A4	    			// RED LED - Radio activity indicator
#define P_LED A5          // BLUE LED - Power/Startup LED

#define COMMS_LED_ON_PERIOD 1000 // How long we keep it on for, in mSec.

// ==============================================
// 'struct's that I had to place here not in main ino file, to assist compilation.
//
typedef struct {			// Radio packet structure max 66 bytes (only transmitted between RF GW <> Nodes, not over IP)
  long  nodeID;			// FROM node. Using a long as its 32bits on AVR and ARM. 
  long	devID;			// device identifier 0 is node; 31 is temperature, 32 is humidity
  long	cmd;			  // read or write
  long	intVal;			// integer payload
  long	fltintVal;	// floating point payload, but multiplied by 100 and transported as a long.  Works on ARM and AVR the same.
  char	payLoad[STRPAYLOADSIZE];	// char array, String payload
  } Message;

// ============================================
// Function prototypes when required;
void mqtt_subs();
void processRfPacket(int rfPackLen, int rfPackFrom);
void rfSendMsg(int target);


// =============================================
// Global variables as 'externs' so individual files can compile if they use them.
extern byte mac[];
extern byte mqtt_server[];
extern byte ip[];
extern Message mes;
extern int dest;
extern int DID;
extern int 	error;
extern long	lastMinute;
extern long	upTime;
extern bool	Rstat;
extern bool	mqttCon;
//extern bool	respNeeded;
extern bool	rfToSend;
extern bool	promiscuousMode;
extern bool	verbose;
extern bool	IntMess, RealIntMess, StatMess, StrMess;
extern long	onMillis;
extern char	*subTopic;
extern char	*clientName;
extern char	buff_topic[30];
extern char	buff_mess[32];
extern uint8_t radioDataBuf[RH_MESH_MAX_MESSAGE_LEN];
//extern RFM69 radio;
extern RH_RF69 driver;  
extern RHMesh manager;
extern EthernetClient ethClient;
extern PubSubClient mqttClient;

#endif // PJ-HA-RFM_GW_H
