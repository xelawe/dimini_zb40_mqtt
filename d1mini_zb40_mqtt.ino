#include <cy_serdebug.h>
#include <cy_serial.h>

#include "cy_wifi.h"
#include "cy_ota.h"
#include "cy_mqtt.h"
#include "zb40.h"
//#include "zb40_pca9536.h"

const char* gc_hostname = "d1mzb40";

void setup() {
  // put your setup code here, to run once:

  cy_serial::start(__FILE__);
  wifi_init(gc_hostname);
  delay(500);

  init_ota(gc_hostname);

  init_zb40();

  init_mqtt(gc_hostname);

  Serial.println("done Setup");
}

void loop() {
  // put your main code here, to run repeatedly:

  check_ota();

  //check_mqtt();
  check_mqtt_reset();

  delay(100);
  //Serial.println("loop");
}
