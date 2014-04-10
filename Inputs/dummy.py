import json
import sys
from time import sleep

#init variables
i = 1


#control loop
while 1==1:
	#update variables
	i+=1
	
	#create an array
	a = {}
	#add data to array for transformation to c#
	a["x"] = 1+i
	a["y"] = 2+i
	a["z"] = 3+i
	
	#print the json dumps to stdout
	print(json.dumps(a))
	
	#flush to allow for reading at c# end
	sys.stdout.flush()

	#give time back to cpu
	sleep(2)
	
