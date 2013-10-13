import re
from os import listdir
from os.path import *

def run(origin, destination):
	for path in find(origin, '.obj'):
		source = open(join(origin, path))
		out = open(join(destination, path), 'w')

		for line in source.read().splitlines():
				match = re.match('f\s* (\d+/\d+/\d+)\s* (\d+/\d+/\d+)\s* (\d+/\d+/\d+)\s* (\d+/\d+/\d+)\s*', line)
				if match:
					out.write('f ' + match.group(1) + ' ' + match.group(2) + ' ' + match.group(3) + '\n' +
							  'f ' + match.group(3) + ' ' + match.group(4) + ' ' + match.group(1) + '\n')
				else:
					out.write(line + '\n')

		source.close()
		out.close()

def find(directory, extension):
	return [entry for entry in listdir(directory)
				if splitext(entry)[1] == extension]

if __name__ == "__main__":
	from sys import argv
	run(argv[1], argv[2])