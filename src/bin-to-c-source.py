#!/usr/bin/env python

from __future__ import print_function

import sys, struct

def generate(data_in, name):
	print("static const unsigned char %s[] PROGMEM = {" % (name))

	for i in range(0, len(data), 20):
		line = bytearray(data[i:i+20])

		for val in line:
			print("0x%02x," % (val), end="")
		print("")

	print("};")

if __name__ == "__main__":
	if len(sys.argv) < 3 or (len(sys.argv) - 1) % 2 != 0:
		print("Usage: bin-to-source.py <file> <base-name> [<file2> <base-name2>]")
		sys.exit(1)

	print("#include <avr/pgmspace.h>")

	for i in range(1,len(sys.argv), 2):
		file = sys.argv[i]
		base_name = sys.argv[i + 1]

		f = open(file, "rb")
		data = f.read()
		f.close()

		generate(data, base_name)
