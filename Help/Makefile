# MAC/LINUX ONLY !!! 
# if you know how to use the cli and want to use make you can try and
# use this.

MAINNAME= $(shell pwd | awk -F '/' '{print $$NF}')

run: $(MAINNAME).ino
	arduino-cli compile --fqbn adafruit:samd:adafruit_itsybitsy_m0 $(MAINNAME).ino && arduino-cli upload -p /dev/ttyACM0 --fqbn adafruit:samd:adafruit_itsybitsy_m0 $(MAINNAME).ino && arduino-cli monitor --port /dev/ttyACM0

build: $(MAINNAME).ino
	arduino-cli compile --fqbn adafruit:samd:adafruit_itsybitsy_m0 $(MAINNAME).ino

