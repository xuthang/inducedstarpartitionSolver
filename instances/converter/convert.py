import os
from os import listdir
from os.path import isfile, join
import sys

if len(sys.argv) == 1 :
	print(f"usage: python {os.path.basename(__file__)} [filelocation]")
	sys.exit()

mypath = os.path.realpath(os.path.dirname(__file__)) + sys.argv[1]
onlyfiles = [join(mypath, f)
			 for f in listdir(mypath) if isfile(join(mypath, f))]
onlyfiles = list(filter(lambda f: f[-2:] != 'gr', onlyfiles))

for f in onlyfiles:
	print(f)
	with open(f) as instance:
		data = instance.readlines()

	data = list(map(lambda l: l if l[0] != 'e' else l[2:], data))

	with open(f+'.gr', 'w') as file:
		file.writelines( data )
		
	os.remove(f)
