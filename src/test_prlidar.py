#!/usr/bin/env python

import turtle
from rplidar import RPLidar
import time


SYNC_BYTE = b'\xA5'
SYNC_BYTE2 = b'\x5A'

GET_INFO_BYTE = b'\x50'
GET_HEALTH_BYTE = b'\x52'

RESET_BYTE = b'\x40'
SCAN_BYTE = b'\x20'

lidar = RPLidar('COM41')
 
lidar._serial_port.read_all() 
lidar._send_cmd(RESET_BYTE)
time.sleep(.002)
print('test starting')

while 1:
    try:
        data = lidar._serial_port.read(1)
        if data == b'\xA5':
            print('[')
            print(data)
            print(']')
            data = lidar._serial_port.read(1)
            print('{')
            print(data)
            print('}')
    except :
        print('error')
        break




# for i, scan in enumerate(lidar.iter_scans()):
#     print('%d: Got %d measurments' % (i, len(scan)))
#     if i > 10:
#         break

lidar.disconnect()