import serial
import json
import re
import time
import os

def get_lgi_configs():
    serialdir='/dev/serial/by-id'
    serdirs = os.listdir(serialdir)
    
    lgis=[]
    for serdir in serdirs:        
        ser = serial.Serial(serialdir+'/'+serdir,115200)
        s=""
        m=None
        lgi=None
        while m is None:
            ser.write(b'i\n')
            ser.flush()
            #need to add a timeout 
            s += ser.read_all().decode("utf-8")
            
            m=re.search(r'beginLGI.*(\{.*\}).*endLGI',s,re.S)
            if m:
                lgi=json.loads(m.groups()[0])
                lgi['device_file']=serdir
                lgis.append(lgi)
                print(lgi)
            #else:
                #print("no lightgroupinfo found")
        
           
        ser.close()
        
    return lgis

if __name__ == '__main__':
    print(get_lgi_configs())