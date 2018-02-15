import sys
import glob
import serial
import platform
import random
from subprocess import call
from threading import Timer

class HaLoveBot():

    phrases = []

    serial_ports = []

    recent_phrases = [];

    def __init__(self):
        print "Starting setup"
        self.setup()

    def __del__(self):
        print "Shutting down..."
        if self.ser:
            self.ser.close()

    def load_phrases(self):
        phrase_file = open("phrases.txt", "r")
        self.phrases = phrase_file.read().split("\n")

    def setup(self):
        print "Loading phrases...\n"
        self.load_phrases()

        print "Select a serial port to connect to:"
        """ Lists serial port names

            :raises EnvironmentError:
                On unsupported or unknown platforms
            :returns:
                A list of the serial ports available on the system
        """
        if sys.platform.startswith('win'):
            ports = ['COM%s' % (i + 1) for i in range(256)]
        elif sys.platform.startswith('linux') or sys.platform.startswith('cygwin'):
            # this excludes your current terminal "/dev/tty"
            ports = glob.glob('/dev/tty[A-Za-z]*')
        elif sys.platform.startswith('darwin'):
            ports = glob.glob('/dev/tty.*')
        else:
            raise EnvironmentError('Unsupported platform')

        result = []
        for port in ports:
            try:
                s = serial.Serial(port)
                s.close()
                result.append(port)
            except (OSError, serial.SerialException):
                pass
        
        for index, port in enumerate(ports):
            print "{}: {}".format(index, port)

        port_number = input("Enter the number of the port to connect to: ")
        self.connect(ports[port_number])

    def connect(self, device):
        self.ser = serial.Serial(device, 9600, parity=serial.PARITY_NONE, timeout=1) 

        print "Connected to {}".format(device)

        self.run()



    def speak(self, phrase):
        if not phrase: return

        cmd = "say"
        if "Darwin" == platform.system():
            cmd = "say"
        if "Linux" == platform.system():
            cmd = "espeak"

        call([cmd, phrase])

    def scan_complete(self):
        phrase = "Facial recognition scan complete."
        self.speak(phrase)
        self.speak_random_phrase()

    def speak_random_phrase(self):
        max = len(self.phrases) - 1
        random.seed()
        rand = random.randint(0, max)

        if rand in self.recent_phrases:
            return self.speak_random_phrase()

        self.recent_phrases.append(rand)
        if 3 < len(self.recent_phrases):
            self.recent_phrases.pop(0)

        phrase = self.phrases[rand]
        self.speak(phrase)

    def startup(self):
        phrase = "Hello, I am Deb, bot. Here to provide a personalized Valentine. Please stick your head in my box to commence facial recognition."
        self.speak(phrase)
        self.send("1")

    def send(self, str):
        if not str: return
        if self.ser.isOpen():
            print "Sending string: {}".format(str)
            self.ser.write(str)

    def run(self):
        print "\nReady...\n"
        while self.ser.isOpen():
            line = self.ser.readline().strip()
            if line:
                print "============= Command recieved: {}".format(line)
            if "STARTUP" == line:
                self.startup()
            if "SCAN_COMPLETE" == line:
                self.scan_complete()

if __name__ == "__main__":
    HaLoveBot()
