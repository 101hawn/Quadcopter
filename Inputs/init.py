import json
import sys

a = {}
a["RKp"] = 1
a["RKi"] = 1
a["RKd"] = 1
a["PKp"] = 1
a["PKi"] = 1
a["PKd"] = 1
a["YKp"] = 1
a["YKi"] = 1
a["YKd"] = 1
a["AKp"] = 1
a["AKi"] = 1
a["AKd"] = 1
a["Roll"] = 0
a["Yaw"] = 0
a["Pitch"] = 0
a["Altitude"] = 1000
	

print(json.dumps(a))
sys.stdout.flush()

	
