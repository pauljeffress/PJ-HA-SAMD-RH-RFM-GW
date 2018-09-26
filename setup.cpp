//
//==============  SETUP
//

#include "PJ-HA-SAMD-RH-RFM-GW.h" // My global defines and extern variables to help multi file comilation.

void setup() {

  //while (!Serial);  // On a SAMD based board you need this or you may miss first bits of output. 
                    // But if no USB is connected then it will hang here.
  delay(5000);  // if we have commented out the above "while (!Serial);" then wait a bit before getting going to give Serial time to init.

  Serial.begin(SERIAL_BAUD); // Initialise the 1st hw serial port for Arduino IDE Serial Monitor
  Serial.println("PJ RFM Gateway");
  Serial.print("N ");
  Serial.print(NODEID);
  Serial.print(" ");
  Serial.println(VERSION);
  
  Serial.println("RF Config");
  Serial.print("Freq ");
  Serial.print(915);
  Serial.println(" Mhz");
  
  Serial.println("ETH Config");
  Serial.print("MAC ");
    for (int i = 0; i < 6; i++)
      {
      Serial.print(mac[i], HEX);
      Serial.print(":");
      }
  Serial.println();  
  Serial.print("Node IP Addr: ");
    for (int i = 0; i < 4; i++)
      {
      Serial.print(ip[i]);
      Serial.print(".");
      }
  Serial.println();  
  Serial.print("MQTT Broker: ");
    for (int i = 0; i < 4; i++)
      {
      Serial.print(mqtt_server[i]);
      Serial.print(".");
      }
  Serial.println();
  
  #ifdef DEBUGPJ2
    //Serial.begin(SERIAL_BAUD);
    Serial.println("DEBUGPJ2 ON");
  #endif
  
  #ifdef DEBUGPJ
    //Serial.begin(SERIAL_BAUD);
    Serial.println("DEBUGPJ ON");
  #endif
  
  #ifdef DEBUG
    Serial.println("DEBUG ON");
  #endif

  #if defined(WIZ_RESET)
    pinMode(WIZ_RESET, OUTPUT);
    digitalWrite(WIZ_RESET, HIGH);
    delay(100);
    digitalWrite(WIZ_RESET, LOW);
    delay(100);
    digitalWrite(WIZ_RESET, HIGH);
  #endif

  // setup LED Pins etc.
  pinMode(R_LED, OUTPUT);          // set pin of radio indicator
  pinMode(MQCON, OUTPUT);         // set pin for MQTT connection indicator
  pinMode(P_LED, OUTPUT);         // set pin for Power/Startup indicator
  digitalWrite(MQCON, HIGH);         // switch off MQTT connection indicator
  digitalWrite(R_LED, HIGH);       // switch off radio indicator
  digitalWrite(P_LED, HIGH);        // switch OFF Power/Startup indicator

  // test all LEDS - flash all LEDS 3 times. Then leave just PWR LED on.
  delay(1000);
  digitalWrite(MQCON, LOW);         // switch all LEDS ON
  digitalWrite(R_LED, LOW);       
  digitalWrite(P_LED, LOW);        
  delay(1000);
  digitalWrite(MQCON, HIGH);         // switch all LEDS OFF
  digitalWrite(R_LED, HIGH);       
  digitalWrite(P_LED, HIGH);        
  delay(1000);
  digitalWrite(MQCON, LOW);         // switch all LEDS ON
  digitalWrite(R_LED, LOW);       
  digitalWrite(P_LED, LOW);        
  delay(1000);
  digitalWrite(MQCON, HIGH);         // switch all LEDS OFF
  digitalWrite(R_LED, HIGH);       
  digitalWrite(P_LED, HIGH);        
  delay(1000);
  digitalWrite(MQCON, LOW);         // switch all LEDS ON
  digitalWrite(R_LED, LOW);       
  digitalWrite(P_LED, LOW);        
  delay(1000);
  digitalWrite(MQCON, HIGH);         // switch all LEDS OFF but Power
  digitalWrite(R_LED, HIGH);             
  delay(1000);

  Serial.println("LED Flashes done");
    
  #ifdef DEBUGPJ2
    Serial.println("radio initialisation starting.");
  #endif

  // RadioHead radio initialisation code
  if (!manager.init())
    Serial.println("init FAILED");
  else
    Serial.println("init succeded");
  // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM (for low power module)
  // No encryption
  
  if (!driver.setFrequency(915.0))
    Serial.println("setFrequency FAILED");
  else
    Serial.println("setFrequency succeded");
    
  // If you are using a high power RF69 eg RFM69HW, you *must* set a Tx power with the
  // ishighpowermodule flag set like this:
  driver.setTxPower(13, true);
    
  // The encryption key has to be the same as the one in the client
  uint8_t key[] = { 0x02, 0x03, 0x07, 0x02, 0x06, 0x06, 0x01, 0x01,
                    0x02, 0x03, 0x07, 0x02, 0x06, 0x06, 0x01, 0x01};
  driver.setEncryptionKey(key); Serial.println("Encryption ENABLED");
  //Serial.println("Encryption DISABLED");

  #ifdef DEBUGPJ2
    Serial.println("radio initialisation is done.");
  #endif
  
  digitalWrite(P_LED, HIGH);     // flash P_LED twice to say we have completed radio setup.      
  delay(1000);
  digitalWrite(P_LED, LOW);        
  delay(1000);
  digitalWrite(P_LED, HIGH);        
  delay(1000);
  digitalWrite(P_LED, LOW);        
  delay(1000);
  
  // Initialise the Ethernet module
  Ethernet.init(WIZ_CS);  
  // give the ethernet module time to boot up:
  delay(1000);
  Ethernet.begin(mac, ip); // start the Ethernet connection with static IP
  
  Serial.println("Ethernet.begin is done");

  digitalWrite(P_LED, HIGH);        
  delay(1000);
  digitalWrite(P_LED, LOW);     // flash P_LED three times to say Ethernet setup statements done.      
  delay(1000);
  digitalWrite(P_LED, HIGH);        
  delay(1000);
  digitalWrite(P_LED, LOW);        
  delay(1000);
  digitalWrite(P_LED, HIGH);        
  delay(1000);
  digitalWrite(P_LED, LOW);        
  delay(1000);
  
  mqttCon = 0;          // reset connection flag
  int numtries = 5;
  while((mqttCon != 1) && (numtries > 0))        // retry MQTT connection 'numtries' times if nescesary
      {
      Serial.println("Try mqtt connect...");
      mqttCon = mqttClient.connect(clientName); // retry connection to broker
      delay(2000);          // wait 2 secs between retries
      }

  if(mqttCon){          // Connected !
    Serial.println("Connected with MQTT server");
    digitalWrite(MQCON, LOW);      // switch on MQTT connection indicator LED
    mqttClient.subscribe(subTopic);     // subscribe to all southbound messages
    }
  else Serial.println("No con with MQTT server");

  #ifdef DEBUGPJ2
    Serial.println("Setup Done");
  #endif
  
} // end setup
