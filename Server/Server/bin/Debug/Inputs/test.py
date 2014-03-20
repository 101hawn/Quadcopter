import json
import sys
from time import sleep

i = 1

while 1==1:
	i+=1
	a = {}
	a["Stuff"] = "Hello world" + str(i)
	
	print(json.dumps(a))
	sys.stdout.flush()
	sleep(1)
	
