#!/usr/bin/python3
import subprocess
import re
import serial

def get_tempr():
	out = subprocess.run(['sensors'], stdout=subprocess.PIPE)
	lines = out.stdout.decode('utf-8')
	# Core 0:       +73.0°C  (crit = +125.0°C)
	m = re.search('Core 0:.*?\+([0-9][0-9])', lines)
	temp = m.group(1)
	return int(temp)

def calc_speed(tempr):
	thr = 66
	if tempr > thr+9:
		return 9
	elif tempr > thr:
		return tempr-thr
	else:
		return 0

def set_speed(speed_ch):
	ser=serial.Serial('/dev/ttyUSB0', 115200)
	line = ser.readline()
	print(line)
	ser.write(speed_ch.encode('ascii'))
	line = ser.readline()
	print(line)
	ser.close()

t = get_tempr()
s = calc_speed(t)
print(t, s)
set_speed(str(s))

