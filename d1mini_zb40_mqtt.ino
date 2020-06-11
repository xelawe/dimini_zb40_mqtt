#include <cy_serdebug.h>
#include <cy_serial.h>

#include "cy_wifi.h"
#include "cy_ota.h"

//#include "zb40.h"
#include "zb40_pca9536.h"
#include "ldr_tool.h"
#include "mqtt_tool.h"

#include "schedule.h"

const char* gc_hostname = "d1mzb40";

void setup() {
  cy_serial::start(__FILE__);

  init_zb40();
  wifi_init(gc_hostname);
  delay(500);

  init_ota(gv_clientname);

  init_mqtt_local( );

  init_sched();

  Serial.println(F("done Setup"));
}

void loop() {
  check_ota();

  //check_mqtt();
  check_mqtt_reset();

  if ( gv_1s ) {
    get_LDR_val();

    gv_1s = false;
  }

  if ( gv_1m ) {
    pub_sens(get_LDR_val());

    gv_1m = false;
  }


  delay(100);
}
