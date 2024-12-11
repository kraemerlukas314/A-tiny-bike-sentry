@echo off
echo *** COMPILING ***
arduino-cli compile --fqbn attiny:avr:ATtinyX5:cpu=attiny85,clock=internal1 %1
if %errorlevel%==0 (
    echo *** UPLOADING ***
    arduino-cli upload -p COM5 -b attiny:avr:ATtinyX5:cpu=attiny85,clock=internal1 --programmer arduinoasispatmega32u4 %1
    if %errorlevel%==0 (
        echo Success.
    ) else (
        echo Upload failed.
    )
) else (
    echo Compilation failed.
)
