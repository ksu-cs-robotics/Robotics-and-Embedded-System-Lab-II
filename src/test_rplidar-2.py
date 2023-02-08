from rplidar import RPLidar
import time

lidar = RPLidar('COM41')

print('starting...0')
info = lidar.get_info()
print(info)
print('starting...1')
time.sleep(2.0)
print('starting...2')
health = lidar.get_health()
print(health)

for i, scan in enumerate(lidar.iter_scans()):
    print('%d: Got %d measures' % (i, len(scan)))
    if i > 10:
        break
lidar.stop()
lidar.stop_motor()
lidar.disconnect()