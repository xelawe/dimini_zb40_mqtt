#include "cy_mqtt.h"

void callback_mqtt_index(int shutter_index, byte* payload, unsigned int length) {
  bool cmd_valid = true;
  String message_string = "";

  for (int i = 0; i < length; i++) {
    DebugPrint((char)payload[i]);
    //fill up the message string
    message_string.concat((char)payload[i]);
  }
  DebugPrintln();

  //map payload / commands
  int shutter_cmd = 0;
  if (message_string.equalsIgnoreCase("up")) {
    shutter_cmd = CMD_UP;
  }
  else if (message_string.equalsIgnoreCase("down")) {
    shutter_cmd = CMD_DOWN;
  }
  else if (message_string.equalsIgnoreCase("stop")) {
    shutter_cmd = CMD_STOP;
  }
  else {
    DebugPrint("Received illegal command message: ");
    DebugPrintln(message_string.c_str());
    cmd_valid = false;
  }

  if (cmd_valid) {
    send_ZB40_command(shutter_index, shutter_cmd);
    // To be sure, send it a second time
    send_ZB40_command(shutter_index, shutter_cmd);
  }

}

void callback_mqtt_0(char* topic, byte* payload, unsigned int length) {
  DebugPrintln("Callback 0 - SHUTTER_ALL");
  callback_mqtt_index(SHUTTER_ALL, payload, length);
}

void callback_mqtt_1(char* topic, byte* payload, unsigned int length) {
  DebugPrintln("Callback 1 - SHUTTER_1");
  callback_mqtt_index(SHUTTER_1, payload, length);
}

void callback_mqtt_2(char* topic, byte* payload, unsigned int length) {
  DebugPrintln("Callback 2 - SHUTTER_2");
  callback_mqtt_index(SHUTTER_2, payload, length);
}

void callback_mqtt_3(char* topic, byte* payload, unsigned int length) {
  DebugPrintln("Callback 3 - SHUTTER_3");
  callback_mqtt_index(SHUTTER_3, payload, length);
}

void init_mqtt_local( ) {

  init_mqtt(gv_clientname);

  add_subtopic("ATSH28/UG/Z2/GW60/0/set", callback_mqtt_0);
  add_subtopic("ATSH28/UG/Z2/GW60/1/set", callback_mqtt_1);
  add_subtopic("ATSH28/UG/Z2/GW60/2/set", callback_mqtt_2);
  add_subtopic("ATSH28/UG/Z2/GW60/3/set", callback_mqtt_3);

}

