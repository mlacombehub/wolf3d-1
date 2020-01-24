#!/usr/bin/python

import struct

fd = open("/dev/stdout", "wb")

px = struct.pack('d', 2.5)
py = struct.pack('d', 2.5)
pr = struct.pack('d', 0 * 3.14159265358979323846264338327950288 / 180)
mx = struct.pack('i', 16)
my = struct.pack('i', 16)


line = px + py + pr + mx + my

for y in range(256):
	for x in range(256):
		line = line + ' '
	line = line + '\n'

print(line)
