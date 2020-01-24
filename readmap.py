#!/usr/bin/python

import struct

with open('/dev/stdin', 'rb') as f:
	bs = f.read()

ppx , ppy , pr = struct.unpack_from('ddd', bs, 0)
pr = int(pr / 3.14159265358979323846264338327950288 * 180)

mx , my = struct.unpack_from('ii', bs, 24)

print(ppx, ppy, pr)
print(mx , my)

for y in range(my):
	line = ''
	for x in range(mx):
		block ,  = struct.unpack_from('c', bs, 32 + x + y * 257)
		line = line + block
	print(line)
