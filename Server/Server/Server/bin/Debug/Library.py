import os
def alive():
	ParentPID = os.environ.get('Parent')
	if ParentPID == None:
		return False
	try:
		os.kill(int(ParentPID),0)
	except OSError:
		return False
	except ValueError:
		return False
	else:
		return True




