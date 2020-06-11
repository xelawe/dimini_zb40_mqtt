#include "Ticker.h"

boolean gv_1s;
boolean gv_1m;
int gv_1m_cnt = 0;

Ticker sched_1s;

void do_sched_1s() {
  gv_1s = true;
  gv_1m_cnt += 1;
  if ( gv_1m_cnt >= 60 ) {
    gv_1m = true;
    gv_1m_cnt = 0;
  }

}

void init_sched() {
  sched_1s.attach(1, do_sched_1s);
}

