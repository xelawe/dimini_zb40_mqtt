#include <PCA9536.h>

PCA9536 pca9536; // construct a new PCA9536 instance

//char mqtt_topic[50] = "/ZB40_GATEWAY";

const char* device_name = "ZB40_GATEWAY";


//ZB40 config via PCA9536

// pinmapping HCS361 keeloq chip<->PCA9536 (0,1,2,3)
const int HCS361_PINS[] = {14, 5, 4, 15}; //  D5, D1, D2, D8 {16, 5, 4, 15};
const int CMD_UP = 1;
const int CMD_DOWN = 2;
const int CMD_STOP = 3;
const int SHUTTER_ALL = 0;
const int SHUTTER_1 = 1;
const int SHUTTER_2 = 2;
const int SHUTTER_3 = 3;

//ZB40 functions
void send_ZB40_command(int shutter, int command) {
  int HCS361_bits[4];

  //shutter is encoded in the bit 3 and 2
  HCS361_bits[3] = shutter >> 1 & 0x01;
  HCS361_bits[2] = shutter & 0x01;
  //command is encoded in bit 1 and 0
  HCS361_bits[1] = command >> 1 & 0x01;
  HCS361_bits[0] = command & 0x01;

  pca9536.setState((HCS361_bits[0] ? IO_HIGH : IO_LOW), (HCS361_bits[1] ? IO_HIGH : IO_LOW), (HCS361_bits[2] ? IO_HIGH : IO_LOW), (HCS361_bits[3] ? IO_HIGH : IO_LOW));
  Serial.print("Sending... [");
  //set the outputs accordingly
  for (int i = 3; i >= 0; i--) {
    //digitalWrite(HCS361_PINS[i], HCS361_bits[i]);
    Serial.print(HCS361_bits[i]);
  }
  Serial.println("]");

  //wait
  delay(1000);

  //and set them back to 0
  pca9536.setState(IO_LOW, IO_LOW, IO_LOW, IO_LOW);

  delay(1000);
}

void callback_mqtt_index(int shutter_index, byte* payload, unsigned int length) {
  bool cmd_valid = true;
  String message_string = "";

  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    //fill up the message string
    message_string.concat((char)payload[i]);
  }
  Serial.println();

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
    Serial.print("Received illegal command message: ");
    Serial.println(message_string.c_str());
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

void init_zb40() {

  Wire.begin();

  pca9536.reset(); // make sure device testing starts with default settings

  pca9536.ping() ? Serial.print(F("PCA9536 Not Found\n")) : Serial.print(F("PCA9536 Found!\n"));

  DebugPrintln(F("\nSetting all pins as OUTPUT..."));
  pca9536.setMode(IO_OUTPUT);

  pca9536.setState(IO_LOW, IO_LOW, IO_LOW, IO_LOW);

  //  while (true) {
  //    pca9536.setState(IO_HIGH, IO_HIGH, IO_HIGH, IO_HIGH);
  //    delay(50);
  //    pca9536.setState(IO_LOW, IO_HIGH, IO_HIGH, IO_HIGH);
  //    delay(50);
  //    pca9536.setState(IO_LOW, IO_LOW, IO_HIGH, IO_HIGH);
  //    delay(50);
  //    pca9536.setState(IO_LOW, IO_LOW, IO_LOW, IO_HIGH);
  //    delay(50);
  //    pca9536.setState(IO_LOW, IO_LOW, IO_LOW, IO_LOW);
  //    delay(50);
  //  }


  add_subtopic("ATSH28/UG/Z2/GW60/0/set", callback_mqtt_0);
  add_subtopic("ATSH28/UG/Z2/GW60/1/set", callback_mqtt_1);
  add_subtopic("ATSH28/UG/Z2/GW60/2/set", callback_mqtt_2);
  add_subtopic("ATSH28/UG/Z2/GW60/3/set", callback_mqtt_3);
}

