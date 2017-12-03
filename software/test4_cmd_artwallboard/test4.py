# ------------------------------------------------------------------------------
# Python program using the library to interface with the arduino sketch above.
# ------------------------------------------------------------------------------

import PyCmdMessenger
import timeit

# Initialize an ArduinoBoard instance.  This is where you specify baud rate and
# serial timeout.  If you are using a non ATmega328 board, you might also need
# to set the data sizes (bytes for integers, longs, floats, and doubles).  
arduino = PyCmdMessenger.ArduinoBoard("/dev/ttyACM0",baud_rate=115200)

# List of commands and their associated argument formats. These must be in the
# same order as in the sketch.
commands = [["who_are_you",""],
            ["my_name_is","s"],
            ["set_intensity","ii"],
            ["set_matrix","iiiiiiiii"], #<boardnum>,<byte>*8 - one byte per row
            ["delay","l"], #milliseconds
            ["set_pixel","iiii"], #<boardnum>,<row>,<col>,<value>
            ["set_rgb","iiiii"], #<boardnum>,<light_index>,<r>,<g>,<b>
            ["clear","i"], #<boardnum>
            ["start_recording","s"], #<sequence_name>
            ["end_recording",""],
            ["play","s"], #sequence_name
            ["ack","s"],
            ["error","s"]]


# Initialize the messenger
c = PyCmdMessenger.CmdMessenger(arduino,commands)

# Send
c.send("who_are_you")
# Receive. Should give ["my_name_is",["fish"],TIME_RECIEVED]
msg = c.receive()
print(msg)

print("set intensity test")
c.send("set_intensity",0,15)
print(c.receive())

print("set pixel test 8x8")
for row in range(8):
    for col in range(8):
        c.send("set_pixel",0,row,col,1)
        #print(c.receive())

print("set matrix test all 255's")
c.send('set_matrix',0,255,255,255,255,255,255,255,255)

print("set matrix test unpacking 255 array")
c.send('set_matrix',0,*[255]*8)

print("test clear")
c.send("clear")
