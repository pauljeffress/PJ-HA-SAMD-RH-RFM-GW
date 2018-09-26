//
//==============  rfSendMsg
//
//  sends messages over the radio network

#include "PJ-HA-SAMD-RH-RFM-GW.h" // My global defines and extern variables to help multi file comilation.

void rfSendMsg(int rfPackTo) {
  #ifdef DEBUGPJ2
    Serial.println();
    Serial.println("RF msg about to be sent >>>>");
    Serial.print("Outbound Message to Node:");Serial.println(rfPackTo);
    Serial.println("=========RF msg data===================");
    Serial.print("Src NodeID:");Serial.println(mes.nodeID);
    Serial.print("     devID:");Serial.println(mes.devID);
    Serial.print("       cmd:");Serial.println(mes.cmd);
    Serial.print("    intVal:");Serial.println(mes.intVal);
    Serial.print(" fltintVal:");Serial.println(mes.fltintVal);
    Serial.print("   payLoad:");
          for (int i=0; i<32; i++) Serial.print(mes.payLoad[i]);
    Serial.println(":");
    Serial.println("=======================================");
  #endif  // DEBUGPJ2
  
  Rstat = true;               // radio indicator on
  digitalWrite(R_LED, LOW);  // turn on radio LED
  onMillis = millis();        // store timestamp, so loop() can turn it off when the time is up.

  //RadioHead based sender code
  Serial.print("Sending to Mesh Addr: ");
  Serial.println(rfPackTo);  

  if (manager.sendtoWait((uint8_t*)&mes, sizeof(mes), rfPackTo) == RH_ROUTER_ERROR_NONE)
  {
    // It has been reliably delivered to the next node.
    Serial.println("Message reliably delivered.");
    rfToSend = false;
  }
  else
  {   
    //The send failed...
    Serial.println("sendtoWait failed. Are the intermediate mesh servers running?");
    sprintf(buff_topic, "home/sam_gw/nb/node%02d/dev90", NODEID); // construct MQTT topic and message
    sprintf(buff_mess, "connection lost to node %d", rfPackTo);    // for radio loss (device 90)
    mqttClient.publish(buff_topic,buff_mess);     // publish ...
    rfToSend = false;           // reset response needed flag
    #ifdef DEBUGPJ2
      Serial.print("No connection with RF node:");
      Serial.println(rfPackTo);
    #endif
  } 
  rfToSend = false;       // reset send trigger

  Serial.println("rfSendMsg()- Done"); 

} // ======== end rfSendMsg

