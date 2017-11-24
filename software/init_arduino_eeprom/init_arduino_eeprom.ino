
#include <EEPROM.h>
#include "lightgroup.h"

int wait_for_input( unsigned long timeout){ //seconds
  timeout *=1000;
  unsigned long start = millis();
  int bytes=0;
  while ((millis()-start) < timeout && !bytes){
    bytes = Serial.available();
  }
  return bytes;
}
void setup() {
  char c;
  char buf[MAX_NAME_LENGTH];
  int bytes_read=0;

  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  int eeAddress = 0;   //Location we want the data to be put.

  struct LightGroupInfo curlgi = get_LGInfo();

  Serial.println("Current config:");
  send_lgi_json(curlgi);

  buf[0]='n';
  Serial.print("Change configuration? (y/n): ");
  wait_for_input(15);
  bytes_read = Serial.readBytesUntil('\n', buf, MAX_NAME_LENGTH);
  Serial.println();
  if ( buf[0] != 'y')  {
    Serial.println("Configuration left unchanged.");
    return;
  }

  struct LightGroupInfo lgi = {MAGIC, VERSION, "", 0  };

  Serial.print("\nEnter new name: ");
  wait_for_input(60);
  bytes_read = Serial.readBytesUntil('\n', lgi.name, MAX_NAME_LENGTH);
  Serial.println();
  if ( ! bytes_read){
    Serial.println("\nConfiguration left unchanged - no name given.");
    return;
  }
  Serial.print("Enter number of boards: ");
  char boardstr[3];
  wait_for_input(60);
  bytes_read = Serial.readBytesUntil('\n', boardstr, 3);
  Serial.println();
  if ( ! bytes_read){
    Serial.println("Configuration left unchanged - no number given.");
    return;
  }
  lgi.num_boards = atoi(boardstr);

  Serial.println("New configuration:");
  send_lgi_json(lgi);
  
  Serial.print("Update to new configuration? (y/n): ");
  buf[0]='n';
  wait_for_input(60);
  bytes_read = Serial.readBytesUntil('\n', buf, MAX_NAME_LENGTH);
  if ( !bytes_read || (buf[0] != 'y') ) {
    Serial.println("Configuration left unchanged.");
    return;
  }


  EEPROM.put(eeAddress, lgi);
  Serial.println();
  Serial.println("Updated with these values:");
  send_lgi_json(lgi);
  
}

void loop() {
  /* Empty loop */
}
