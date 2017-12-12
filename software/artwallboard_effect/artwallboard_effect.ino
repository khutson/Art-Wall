#include "LedControl.h"
/*
 * 
 * test5_artwallboard
 * Kent Hutson
 * this turns on each of the 64 channels in turn
 * as of 11/18/2017, up to 5 channels light up - this is on a board
 * that is not fully populated with lights, however.
 * as of 12/1/2017, 8 channels light up when one is set - board issue?
 * 
 * test5 using cmdmessenger and pycmdmessenger with binary communication
 */
// 20 is the number of led strips
// 3 is the number of leds per "led" (3 because tri color)


#include "lightgroup.h"
#include "effect.h"
#include "CmdMessenger.h"

#define USE_TEXT

#ifdef USE_BINARY
#define READINT readBinArg<int>

#endif
#ifdef USE_TEXT
#define READINT readInt16Arg
#endif

int debug = 0;

/* Define available CmdMessenger commands */
enum {
    cmd_who_are_you, //0
    cmd_my_name_is, //1
    cmd_set_intensity,  //2,<board>,<intensity 0..15>
    cmd_set_matrix,     //3,<board>,<row0byte>,...,<row7byte>
    cmd_delay,          //4,<milliseconds>
    cmd_set_pixel,      //5,<board>,<row 0..7>,<col 0..7>
    cmd_set_rgb,        //6
    cmd_clear,          //7,<board>
    cmd_start_recording,
    cmd_end_recording,
    cmd_play,
    cmd_ack,
    cmd_debug,
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

int num_lights=3;
//lights[light_strip_index][rgb][rowcol]
byte lights[3][3][2] = { { {0,0},{0,1}, {0,2} },
                         { {0,3},{0,4},{0,5} },
                         { {0,6},{0.7}, {1,0} } };
int num_colors=6;
byte wheel_colors[6][3] = { {1,0,0}, {1,1,0},{0,1,0},{0,1,1},{0,0,1},{1,0,1} };
/* effects */
class ColorWheelEffect: public Effect{
  //all the lights change color together, rotating through the wheel_colors
  public:
    ColorWheelEffect() {Effect();};
    update(){
        float phase = float(millis()-start_time) / cycle_time;
        int wheel_start = int(phase * 6) % 6; //make sure we are in 0..numcolors-1
        lc.clearDisplay(0);
        for (int i=0;i<num_lights;i++){
          for (int rgb=0;rgb<3;rgb++){
            byte row = lights[i][rgb][0];
            byte col = lights[i][rgb][1];
            byte rgbval = wheel_colors[wheel_start][rgb];
            lc.setLed(0,row,col,rgbval);
            Serial.print(row); Serial.print(", ");
            Serial.print(col); Serial.print("= ");
            Serial.println(rgbval);
          }
        }

    }
};

ColorWheelEffect color_wheel;

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
    int board = c.READINT();
    int intensity = c.READINT();
    lc.setIntensity(board,intensity);
    if( debug ){
        c.sendCmd(cmd_ack,"set_intensity="+String(intensity));
    }
}

void on_set_matrix(void){
  int board = c.READINT();
  byte rows[8];
  for (int i=0;i<8;i++){
    rows[i]= c.READINT() & 255;
    lc.setRow(board,i,rows[i]);
  }
  if( debug ){
    c.sendCmd(cmd_ack,"set_matrix");
  }
}

void on_delay(void){
  long delay_millis = c.READINT();
  if( debug && delay_millis > 1000){
    //only give message if more than a second delay
    c.sendCmd(cmd_ack,"delay for "+String(delay_millis)+" milliseconds.");
  }
  delay(delay_millis);

}

void on_set_pixel(void){
   
    int board = c.READINT();
    int row = c.READINT();
    int col = c.READINT();
    int val = c.READINT();

    lc.setLed(board,row,col,val);
  if( debug > 1 ){
      c.sendCmd(cmd_ack," set_pixel.board=" + String(board)+
                        "row=" + String(row) +
                        " col=" + String(col) +
                        " val=" +String(val));
  }
}

void on_set_rgb(void){
   
    int board = c.READINT();
    int light_index = c.READINT();
    int r = c.READINT();
    int g = c.READINT();
    int b = c.READINT();

    /*need to know LED strip mapping*/
    //lc.setLed(board,row,col,val);
    c.sendCmd(cmd_error,"set_rgb not implemented.");

}

void on_clear(void){
  int board = c.READINT();
  lc.clearDisplay(board);
  if (debug>0){
    c.sendCmd(cmd_ack,"clear executed.");
  }
}

void on_start_recording(void){
  c.sendCmd(cmd_error,"on_start_recording not implemented.");
}

void on_end_recording(void){
  c.sendCmd(cmd_error,"on_end_recording not implemented.");
}
void on_play(void){
  c.sendCmd(cmd_error,"on_play not implemented.");
}

void on_debug(void){
  debug = c.READINT();
  c.sendCmd(cmd_ack,"debug is "+String(debug));
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
    c.attach(cmd_debug,on_debug);
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
    color_wheel.update();
}

/*
 * test messages that can be sent via serial monitor:
0;
2,0,15;
3,0,255,255,255,255,255,255,255,255;
4,500;
5,0,3,3,1;
7,0;

 */
