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
            
project:{
    "name":<string>,
    "lightgroups":[ lightgroups ]
    }

lightgroup: #each lightgroup is run by one arduino
{ "name": "thermometer", #name of the arduino that controls this lightgroup
    "connection": "usb" | "net", # how the arduino connects to the unix host
    "ip": <ip_address>, #only useful for arduinos with wifi
    "lights": [ light ]
    "num_boards":<int> #number of boards attached to arduino
}


light:
{   "name": "thermometer0", #if not specified, created as lightgroup['name']+'index'
    "boardnum":0,
    "index": 0,
    "loc":[[0,0,0],[1,1,1]], #top left and bottom right coordinates of light
    "channels": [0,1,2]
    "default_color": "white",
    "color":"white",
    "intensity":1.0,
    "pulse_rate":0.0,
    "pulse_type":"sin",
    }
    
    
    
pi:
* requests config data from arduino for the boards controlled by that arduino

arduino:
sends lightgroup name and board config info to pi
lightgroup.name

num_boards



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



-------
            
project:
    [ lightgroups ]

lightgroup:
{ "name": "thermometer", #name of the arduino that controls this lightgroup
    "connection": "usb" | "net", # how the arduino connects to the unix host
    "ip": <ip_address>, #only useful for arduinos with wifi
    "lights": [ light ]
}

light:
{   "name": "phase0"} #if not specified, created as lightgroup['name']+'index'
    "board": <int>, #which board is this plugged in to
    "index": <int>, #logical index - not related to channels
    "loc":[[0,0,0],[1,1,1]], #top left and bottom right coordinates of light
    "channels": [<r>,<g>,<b>] | [<w>], #channels 0..63 mapped as <digit> * 8 + <segment>
    "default_color": "black" | "white" | "red" | "green" | "blue" | "yellow" |
        "magenta" | "cyan" | [<r>,<g>,<b>],
}
    
url example:
https://art-wall-server/moon/light/0/1/red would 
set light 1 of board 0 of lightgroup "moon" to red 
If that light is named "phase0", the url could be
https://art-wall-server/moon/phase0/red
    
pi:
* sends config data to arduino for the groups controlled by that arduino


arduino commands:
x is hexadecimal digit (0..F)
color : 0..7 for rgb, 0..1 for bw
colors = {"black" :[0,0,0],
            "white" :[1,1,1],
            "red" : [1,0,0],
            "green": [0,1,0],
            "blue" : [0,0,1],
            "yellow": [1,1,0],
            "magenta":[1,0,1],
            "cyan": [0,1,1] }
            

i<board>,<d>; - set intensity - decimal
m<board>,<x16>; - set board <board (0..3)> matrix x16 is 64 bits 
d<int>; - delay for <int> milliseconds decimal
s<board>,<row>,<col>,<color>; - set pixel
r,<sequence_name>; - start named sequence recording
e; - end current sequence recording
p,<sequence_name>; - play sequence

all commands can be sent through main web site using the form 
http://<piaddress>/<lightgroup>/<board>/<board_command>


effects: update project lights
effect required data:
    cycle_time:how long each cycle lasts. millisecond resolution
    lights: list of lights to be updated
    
    internals:
    cycle_start_time: when the current cycle started
    
example:
    effect_moonphase: update phase of the moon lights
    effect_rainbow: create a traveling rainbow of color along the lights
    