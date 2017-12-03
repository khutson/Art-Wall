#ifndef lightgroup_h
#define lightgroup_h

#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#include <EEPROM.h>


#define MAGIC 1095914583 
//{'A','R','T','W'}
#define VERSION 1
#define MAX_NAME_LENGTH 20

struct LightGroupInfo {
  unsigned long magic;
  int version;
  char name[MAX_NAME_LENGTH];
  int num_boards;
};

void send_lgi_json(struct LightGroupInfo lgi){
  Serial.println("beginLGI");
  Serial.println("{");
  Serial.print("\"magic\":"); Serial.print(lgi.magic); Serial.println(",");
  Serial.print("\"version\": "); Serial.print(lgi.version); Serial.println(",");
  Serial.print("\"name\": \""); Serial.print(lgi.name);Serial.println("\",");
  Serial.print("\"num_boards\": "); Serial.print(lgi.num_boards);Serial.println("");
  Serial.println("}");
  Serial.println("endLGI");
}

struct LightGroupInfo get_LGInfo(){

  struct LightGroupInfo lgi;
  EEPROM.get(0, lgi);
  
  return lgi;
}

#endif

