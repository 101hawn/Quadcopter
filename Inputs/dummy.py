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
	a["Roll"] = 10
	a["Roll_timeout"] = 1000
	a["Yaw"] = 10
	a["Yaw_timeout"] = 1000
	a["Pitch"] = 10
	a["Pitch_timeout"] = 1000
	a["Altitude"] = 10
	a["Altitude_timeout"] = 3000
	
	#print the json dumps to stdout
	print(json.dumps(a))
	
	#flush to allow for reading at c# end
	sys.stdout.flush()

	#give time back to cpu
	sleep(2)
	