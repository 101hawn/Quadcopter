import json
import sys
from time import sleep
import Library #Quadcopter helper library

#init variables
i = 1


#control loop
while Library.alive():
	#update variables
	i+=1
	
	#create an array
	a = {}
	#add data to array for transformation to c#
	a["Roll"] = 0
	a["Yaw"] = 0
	a["Pitch"] = 0
	a["Altitude"] = 1
	a["Altitude_timeout"] = 3000
	
	#print the json dumps to stdout
	print(json.dumps(a))
	
	#flush to allow for reading at c# end
	sys.stdout.flush()

	#give time back to cpu
	sleep(2)
	
