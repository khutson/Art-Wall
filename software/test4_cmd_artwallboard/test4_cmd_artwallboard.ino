#include "LedControl.h"
/*
 * 
 * test2_artwallboard
 * Kent Hutson
 * this turns on each of the 64 channels in turn
 * as of 11/18/2017, up to 5 channels light up - this is on a board
 * that is not fully populated with lights, however.
 * as of 12/1/2017, 8 channels light up when one is set - board issue?
 */
// 20 is the number of led strips
// 3 is the number of leds per "led" (3 because tri color)


#include "lightgroup.h"

#include "CmdMessenger.h"

/* Define available CmdMessenger commands */
enum {
    cmd_who_are_you,
    cmd_my_name_is,
    cmd_set_intensity,
    cmd_set_matrix,
    cmd_delay,
    cmd_set_pixel,
    cmd_set_rgb,
    cmd_clear,
    cmd_start_recording,
    cmd_end_recording,
    cmd_play,
    cmd_ack,
    cmd_error,
};



const int MAXBOARDS = 4;
int cmdDelay = 100;
int intensity = 15;
// 13 data out. 12 LOAD. 11 CLOCK. 10 DATA IN.
int data = 11;
int load = 13;
int clk = 12;

LedControl lc=LedControl(data,clk,load,1);
String inputString = "";
boolean stringComplete = false;
int rowMax = 8;

/* Initialize CmdMessenger -- this should match PyCmdMessenger instance */
const int BAUD_RATE = 115200;
CmdMessenger c = CmdMessenger(Serial,',',';','/');

/*  callback functions to deal with incoming messages */

void on_who_are_you(void){
    struct LightGroupInfo lgi = get_LGInfo();
    c.sendCmd(cmd_my_name_is,lgi.name);
    lc.clearDisplay(0);
    for (int i=0;i<5;i++){
      for (int r=0;r<8;r++){
        lc.setRow(0,r,random(0,255));
      }
      delay(500);
    }
    lc.clearDisplay(0);
}

void on_set_intensity(void){
    int board = c.readInt16Arg();
    int intensity = c.readInt16Arg();
    lc.setIntensity(board,intensity);
}

void on_set_matrix(void){
  int board = c.readInt16Arg();
  byte rows[8];
  for (int i=0;i<8;i++){
    rows[i]= c.readInt16Arg() && 255;
    lc.setRow(board,i,rows[i]);
  }
  c.sendCmd(cmd_ack,"Command set_matrix executed.");
}

void on_delay(void){
  long delay_millis = c.readInt32Arg();
  delay(delay_millis);
}

void on_set_pixel(void){
   
    int board = c.readInt16Arg();
    int row = c.readInt16Arg();
    int col = c.readInt16Arg();
    int val = c.readInt16Arg();

    lc.setLed(board,row,col,val);
  c.sendCmd(cmd_ack,"Command set_pixel executed.");

}

void on_set_rgb(void){
   
    int board = c.readInt16Arg();
    int light_index = c.readInt16Arg();
    int r = c.readInt16Arg();
    int g = c.readInt16Arg();
    int b = c.readInt16Arg();

    /*need to know LED strip mapping*/
    //lc.setLed(board,row,col,val);
    c.sendCmd(cmd_error,"Command set_rgb not implemented.");

}

void on_clear(void){
  int board = c.readInt16Arg();
  lc.clearDisplay(board);
}

void on_start_recording(void){
  c.sendCmd(cmd_error,"Command on_start_recording not implemented.");
}

void on_end_recording(void){
  c.sendCmd(cmd_error,"Command on_end_recording not implemented.");
}
void on_play(void){
  c.sendCmd(cmd_error,"Command on_play not implemented.");
}

void on_unknown_command(void){
    c.sendCmd(cmd_error,"Command without callback.");
}


/*----------end of callbacks-----------*/

/* Attach callbacks for CmdMessenger commands */
void attach_callbacks(void) { 
  
    c.attach(cmd_who_are_you,on_who_are_you);
    c.attach(cmd_set_intensity,on_set_intensity);
    c.attach(cmd_set_matrix,on_set_matrix);
    c.attach(cmd_delay,on_delay);
    c.attach(cmd_set_pixel,on_set_pixel);
    c.attach(cmd_set_rgb,on_set_rgb);
    c.attach(cmd_clear,on_clear);
    c.attach(cmd_start_recording,on_start_recording);
    c.attach(cmd_end_recording,on_end_recording);
    c.attach(cmd_play,on_play);
    c.attach(on_unknown_command);
}

void setup()
{
  Serial.begin(115200);
  lc.shutdown(0,false);
  lc.setIntensity(0,intensity);
  lc.clearDisplay(0);
  lc.spiTransfer(0,9,0);//no digit decoding
  c.printLfCr();
  attach_callbacks();
  

}

byte rowdata=0;

void loop()
{
    c.feedinSerialData();
}
