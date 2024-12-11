#!/usr/bin/env bash

printf "*** COMPILING ***\n"
if arduino-cli compile --fqbn attiny:avr:ATtinyX5:cpu=attiny85,clock=internal1 $1; then
	printf "\n*** UPLOADING ***\n"
	arduino-cli upload -p /dev/ttyACM0 -b attiny:avr:ATtinyX5:cpu=attiny85,clock=internal1 --programmer arduinoasispatmega32u4 $1 && echo "Success."

fi
