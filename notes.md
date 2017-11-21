max7219

64 channel using 8x8 multiplexing
16 level pwm for entire board (intensity)
500ma per channel

SPI interface using arduino


configuration
need flexible light config 
location/size of each light in the artwork
## group: group name of light [required] does not have to be unique
## index: index # for this light in group name 
## bounds: bounding box in 3d [optional] assume point source of not present
## loc: center of box [required] in this group of lights
## channels: channels used ( 3 for rgb, 1 for monochrome)
## default_color: initial color (after reset)
## color: current color
## pulse_rate: float: hertz
## pulse_type: "up","down","sin","flat" : shape of light intesity waveform
            as time progresses through a pulse
            
project:
    [ lightgroups ]

lightgroup:
{ "name": "thermometer",
    "lights":[light]
    }
    
light:
{   "name": "thermometer0"} #if not specified, created as lightgroup['name']+'index'
    "index": 0,
    "loc":[0,0,0],
    "channels": [0,1,2]
    "default_color": "white",
    "color":"white",
    "intensity":1.0,
    "pulse_rate":0.0,
    "pulse_type":"sin",
    }
    
    
    
pi:
* sends config data to arduino for the groups controlled by that arduino


arduino commands:
x is hexadecimal digit (0..F)
color : 0..7 for rgb, 0..1 for bw, xxx need name translation to use on pi

i<board>,<d>; - set intensity - decimal
m<board>,<x16>; - set board <board (0..3)> matrix x16 is 64 bits 
d<int>; - delay for <int> milliseconds decimal
s<board>,<row>,<col>,<color>; - set pixel
r,<sequence_name>; - start named sequence recording
e; - end current sequence recording
p,<sequence_name>; - play sequence

all commands can be sent through main web site using the form 
http://<piaddress>/<lightgroup>/<board>/<board_command>
