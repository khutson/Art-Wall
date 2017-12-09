# ------------------------------------------------------------------------------
# Python program using the library to interface with the arduino sketch above.
# ------------------------------------------------------------------------------

import PyCmdMessenger
import os, time

# List of commands and their associated argument formats. These must be in the
# same order as in the sketch.
commands = [["who_are_you",""],
            ["my_name_is","s"],
            ["set_intensity","ii"],
            ["set_matrix","iiiiiiiii"], #<boardnum>,<byte>*8 - one byte per row
            ["delay","i"], #milliseconds
            ["set_pixel","iiii"], #<boardnum>,<row>,<col>,<value>
            ["set_rgb","iiiii"], #<boardnum>,<light_index>,<r>,<g>,<b>
            ["clear","i"], #<boardnum>
            ["start_recording","s"], #<sequence_name>
            ["end_recording",""],
            ["play","s"], #sequence_name
            ["ack","s"],
            ["debug","i"],
            ["error","s"]]

def find_arduinos():
    serialdir='/dev/serial/by-id'
    serdirs = os.listdir(serialdir)
    arduinos = {}
    
    for serdir in serdirs:        
    
        arduino = PyCmdMessenger.ArduinoBoard(os.path.join(serialdir,serdir),
                                              baud_rate=115200,
                                              timeout=0.0)
    
        c = PyCmdMessenger.CmdMessenger(arduino,commands)
    
        c.send("who_are_you")
        # Receive. Should give ["my_name_is",["<name_of_arduino>"],TIME_RECIEVED]
        # arduino needs to be set up with init_arduino_eeprom program
        # arduino needs to be running latest artwall arduino program
        c.board.comm.timeout = 3.0
        msg = c.receive()
        c.board.comm.timeout = 0.5
        print(msg)
        if msg:
            if msg[0] == 'my_name_is':
                arduinos[msg[1][0]] = c
        
    return arduinos

def await_response(c,delay = 5.0):
    if delay > 0:
        print("waiting {} seconds.".format(delay))
        time.sleep(delay)
    while c.board.comm.in_waiting:
        response = c.receive()
        if response:
            print(response)
    
def run_arduino_test(c):
    
    c.send("debug",1)
    await_response(c)

    print("set intensity test")
    c.send("set_intensity",0,15)
    await_response(c)
    
    print("set pixel test 8x8")
    for row in range(8):
        for col in range(8):
            print("row {}, col {}".format(row,col))
            c.send("set_pixel",0,row,col,1)
            c.send('delay',100)
            c.send("set_pixel",0,row,col,0)
            
            await_response(c,0)
    c.send('delay',5000)
    await_response(c)
    
    c.send("debug",2)
    await_response(c)
    
    print("set matrix test all 255's")
    c.send('set_matrix',0,255,255,255,255,255,255,255,255)
    await_response(c)
    c.send('delay',5000)
    await_response(c)
    
    
    print("set matrix test unpacking 255 array")
    c.send('set_matrix',0,*[0b10101010]*8)
    await_response(c)
    c.send('delay',5000)
    await_response(c)

    print("test clear")
    c.send("clear")
    await_response(c)

if __name__ == "__main__":
    arduinos = find_arduinos()
    for lightgroup_name, arduino in arduinos.items():
        print("Testing lightgroup {}".format(lightgroup_name))
        run_arduino_test(arduino)
        await_response(arduino)
