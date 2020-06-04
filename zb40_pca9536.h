#include <PCA9536.h>

PCA9536 pca9536; // construct a new PCA9536 instance

//ZB40 config via PCA9536

// pinmapping HCS361 keeloq chip<->PCA9536 (0,1,2,3)
//const int HCS361_PINS[] = {14, 5, 4, 15}; //  D5, D1, D2, D8 {16, 5, 4, 15};

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

// Ausgabe auf Pins - sind harwaretechnisch vertausch: 1 3 2 4 --> Bits bei Ausgabe vertauschen
  pca9536.setState((HCS361_bits[0] ? IO_HIGH : IO_LOW), (HCS361_bits[2] ? IO_HIGH : IO_LOW), (HCS361_bits[1] ? IO_HIGH : IO_LOW), (HCS361_bits[3] ? IO_HIGH : IO_LOW));
  
  DebugPrint("Sending... [");
  //set the outputs accordingly
  for (int i = 3; i >= 0; i--) {
    //digitalWrite(HCS361_PINS[i], HCS361_bits[i]);
    DebugPrint(HCS361_bits[i]);
  }
  DebugPrintln("]");

  //wait
  delay(100);

  //and set them back to 0
  pca9536.setState(IO_LOW, IO_LOW, IO_LOW, IO_LOW);

  delay(500);
}



void init_zb40() {

  Wire.begin();

  pca9536.reset(); // make sure device testing starts with default settings

  //pca9536.ping() ? Serial.print(F("PCA9536 Not Found\n")) : Serial.print(F("PCA9536 Found!\n"));

  //DebugPrintln(F("\nSetting all pins as OUTPUT..."));
  pca9536.setMode(IO_OUTPUT);

  pca9536.setState(IO_LOW, IO_LOW, IO_LOW, IO_LOW);

}

