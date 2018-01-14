#include <cy_serdebug.h>
#include <cy_serial.h>

#include "cy_wifi.h"
#include "cy_ota.h"
#include "cy_mqtt.h"
#include "zb40.h"

const char* gc_hostname = "d1minizb40";

void setup() {
  // put your setup code here, to run once:

  cy_serial::start("__FILE__");
  wifi_init(gc_hostname);
  delay(500);

  init_ota(gc_hostname);
  
  init_zb40();

  add_subtopic("ATSH28/UG/Z2/GW60/0/set", callback_mqtt_0);
  add_subtopic("ATSH28/UG/Z2/GW60/1/set", callback_mqtt_1);
  add_subtopic("ATSH28/UG/Z2/GW60/2/set", callback_mqtt_2);
  add_subtopic("ATSH28/UG/Z2/GW60/3/set", callback_mqtt_3);
  init_mqtt(gc_hostname);
}

void loop() {
  // put your main code here, to run repeatedly:

  check_ota();

  check_mqtt();

  delay(100);
}
