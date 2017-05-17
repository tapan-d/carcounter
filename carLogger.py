# Read data from Arduino on the serial port and both write to screen and to file
#   default baud is 9600, default port is /dev/ttyUSB0
# exectute with "sudo python3 carLogger.py"
# toggle button starts/stops/restarts recording. 0 = On, 1 = off

# TODO
#   make run automatically on start in bash script

# BUGS

import sys
import argparse
import serial
import datetime
# try to load GPIO module
try:
    import RPi.GPIO as GPIO
except Exception as e:
    print("Error loading GPIO. Possibly need to be in superuser mode.\n")
    print(str(e))
    sys.exit(1)

# set the gpio pin to input for switch
GPIO.setmode(GPIO.BCM)
GPIO.setup(18,GPIO.IN,pull_up_down=GPIO.PUD_UP)

# set up the argument parser
parser = argparse.ArgumentParser(description="Read from serial port")
parser.add_argument("-o","--output",type=str,default="carCount",help="output file for car count data")
parser.add_argument("-d","--debugging",type=int,default=1,help="Flag to create output file for debug info")
parser.add_argument("-p","--port",type=str,default="/dev/ttyUSB0",help="port to read from ex: /dev/ttyUSB0")

args = parser.parse_args()

#read arguments
thePort = args.port
outFile = args.output
isDebug = args.debugging
debugFile = "carDebug"

print("Reading from: "+thePort+" and writing count to: "+outFile)
if isDebug == True:
    print("Debugging enabled\n")
else:
    print("No debugging\n")

#open the serial port to the Arduino
try:
    serialport = serial.Serial(str(thePort), 9600, timeout=0.5)
except:
    print("serial port "+str(thePort)+" not found "+str(sys.exc_info()))
    sys.exit()

print("Opened serial port\n")
isCounting = False
print("Note: Control-C terminates program")
print(" ")
print("Waiting for counting switch to be turned on")

#open both the car count and the debug log files


try:
# loop and only record when physical switch is toggled to record
#    loop forever until program cancelled with control-C
    while True:
        switchValue = GPIO.input(18)
# note: button low/False means switch is flipped to record. 0 = on, 1=off
        if switchValue == 0 and isCounting == False:
# Open files for writing
            datenow = datetime.datetime.now()
            fileDate = datenow.strftime("%m%d%y_%H%M%S")
            fout= open(outFile+fileDate+".log",'w')

            if isDebug:
                fdebugout = open(debugFile+fileDate+".log",'w')
                fdebugout.write("Serial Data from Arduino\n")
            isCounting = True
# Reset count number on Arduino
            serialport.write(b'R')

        elif switchValue == 0 and isCounting == True:
# loop to keep getting data from Arduino until user does Ctrl-C
            line = serialport.readline()
#         convert from byte array to string. remove  cr-lf and prefix b'
            aline = str(line)[2:-5]
# actual car count lines contain the word "COUNT". Write date/time
            if "COUNT" in aline:
                datenow = datetime.datetime.now()
                print(str(datenow)+" "+aline)
                fout.write(str(datenow)+" "+aline+"\n")
                if isDebug:
                    fdebugout.write(str(datenow)+" "+aline+"\n")
            else:
                if len(aline)>4 :
                    if isDebug:
                        print(aline);
                        fdebugout.write(aline+"\n")
        elif switchValue == 1 and isCounting == True:
# physical counting switch has been turned off
            isCounting = False
            fout.close()
            if isDebug:
                fdebugout.close()

# handle exceptions to close all files
except KeyboardInterrupt:
    datenow = datetime.datetime.now()
    if isDebug == True and isCounting == True:
        fdebugout.write("User terminated program at:"+str(datenow)+" \n")
    print("\nUser terminated program at:"+str(datenow)+" \n")
except: 
    if isDebug == True and isCounting == True:
        fdebugout.write("Something bad happened "+str(sys.exc_info())+"\n")
    print("\nSomething bad happened "+str(sys.exc_info()))
       
finally:
# close everything
    if isCounting == True:
        fout.close()
        if isDebug:
            fdebugout.close()

    serialport.close()
    GPIO.cleanup(18)

print("Everything shut down")
