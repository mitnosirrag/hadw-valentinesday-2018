import serial
import platform
import random
from subprocess import call

ser = serial.Serial("/dev/cu.usbmodemFD1361", 9600, parity=serial.PARITY_NONE, timeout=1) 

phrases = [
	"Lookin' good, Jonas, have you been riding your bike?",
	"Jonas, will you be my Valentine?",
	"You look as good as a sackagni's chicken parmesan sandwich, Jonas",
]

def speak():
	max = len(phrases) - 1
	random.seed()
	rand = random.randint(0, max)
	phrase = phrases[rand]
	cmd = "say"
	if "Darwin" == platform.system():
		cmd = "say"
	if "Linux" == platform.system():
		cmd = "espeak"

	call([cmd, phrase])

while ser.isOpen():
	line = ser.readline().strip()
	if "SCAN_COMPLETE" == line:
		speak()


ser.close()