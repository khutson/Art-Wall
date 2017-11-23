
#include <EEPROM.h>

#define MAGIC {'A','R','T','W'}
#define VERSION 1
#define MAX_NAME_LENGTH 20

struct LightGroupInfo {
  char magic[4];
  int version;
  char name[MAX_NAME_LENGTH];
  int num_boards;
};

void print_config(struct LightGroupInfo lgi){
  Serial.println("{");
  Serial.print("\"magic\":\""); Serial.print(lgi.magic); Serial.println("\",");
  Serial.print("\"version\": "); Serial.print(lgi.version); Serial.println(",");
  Serial.print("\"name\": \""); Serial.print(lgi.name);Serial.println("\",");
  Serial.print("\"num_boards\": \""); Serial.print(lgi.num_boards);Serial.println("");
  Serial.println("}");
}

int wait_for_input( unsigned long timeout){ //milliseconds
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

  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  int eeAddress = 0;   //Location we want the data to be put.

  struct LightGroupInfo curlgi;
  EEPROM.get(eeAddress, curlgi);

  Serial.println("Current config:");
  print_config(curlgi);

  buf[0]='n';
  Serial.print("Change configuration? (y/n): ");
  while (!Serial.available());
  bytes_read = Serial.readBytesUntil('\n', buf, MAX_NAME_LENGTH);
  Serial.println();
  if ( buf[0] != 'y')  {
    Serial.println("Configuration left unchanged.");
    return;
  }

  struct LightGroupInfo lgi = {MAGIC, VERSION, "", 0  };

  Serial.print("\nEnter new name: ");
  while (!Serial.available());
  bytes_read = Serial.readBytesUntil('\n', lgi.name, MAX_NAME_LENGTH);
  Serial.println();
  if ( ! bytes_read){
    Serial.println("\nConfiguration left unchanged - no name given.");
    return;
  }
  Serial.print("Enter number of boards: ");
  char boardstr[3];
  while (!Serial.available());
  bytes_read = Serial.readBytesUntil('\n', boardstr, 3);
  Serial.println();
  if ( ! bytes_read){
    Serial.println("Configuration left unchanged - no number given.");
    return;
  }
  lgi.num_boards = atoi(boardstr);

  Serial.println("New configuration:");
  print_config(lgi);
  
  Serial.print("Update to new configuration? (y/n): ");
  buf[0]='n';
  while (!Serial.available());
  bytes_read = Serial.readBytesUntil('\n', buf, MAX_NAME_LENGTH);
  if ( !bytes_read || (buf[0] != 'y') ) {
    Serial.println("Configuration left unchanged.");
    return;
  }


  EEPROM.put(eeAddress, lgi);
  Serial.println();
  Serial.println("Updated with these values:");
  print_config(lgi);
  
}

void loop() {
  /* Empty loop */
}
