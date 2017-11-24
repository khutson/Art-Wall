import serial
import json
import re
import time

ser = serial.Serial('/dev/ttyUSB0',115200)

ser.write(b'i\n')
#need to wait for response or keep checking for it
time.sleep(10)
s = ser.read_all().decode("utf-8")

m=re.search(r'\{.*\}',s,re.S)

s=json.loads(m.group())

print(s)
ser.close()
