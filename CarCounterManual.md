#Car Counter Instructions

Last revision June 1, 2017

0. Write down which car counter (A,B,C,D) will be at which location, such as East Washington bridge entrance. Make sure all the power bricks are charged. If using a Macintosh or Windows laptop to check the counters in the field, be sure to install the Prolific or Silabs CP210X chipset drivers (both drivers if unsure). Be sure to test each counter the day before the event to make sure it still works.
Then for each of the car counters to be used, do the following steps:

1. First plug in PI+Arduino counter to power brick.
Wait about 2 minutes until the PI finishes booting

2. Connect your laptop to the serial console cable.   
Use your favorite terminal emulation program at 115200 baud.  
Log in as user: "pi" with the right password.

3. You need to set the date time  Example:  
sudo date -s "Sat Jun 03 017 08:14:00"

4. cd into carLogs directory.  
Make sure the toggle switch on the car counter is off

5. Start the car counting program  
 sudo python3 carLogger.py  
It doesn't start counting until the toggle switch is on.
Unplug the serial console cable.

6. Take the car counter out to an entrance and set up.  
Level the tripod and then bungee or velcro tie one leg to a garden stake or post. The tripod can be left out overnight.

7. Toggle the switch to on (Be sure to do this or all your effort is wasted.)

During the day, connect to the raspberry by connecting the serial console
cable and logging in. The debug stream should scroll by.

To move the car counter during the day, toggle the switch off, which closes
the current log file. Then move the counter, then toggle the switch on again. 
This starts a new log file with new date time in the file name.

8. At the end of the day turn the toggle switch off. 
Terminate the python carLogger program with Control-C
Move the car counter back to the IT room still powered on.

9. Transfer the files from the car counter to a laptop or desktop. It is fastest to plug the raspberry pi into a router
with an ethernet cable, then use the serial terminal to find the IP address assigned to the pi. Then use software like
FileZilla or scp to transfer the files. Once the files are transfered to the laptop delete them from the raspberry pi with "sudo rm *.log"

10. Power off the raspberry pi with:  
sudo poweroff

11. After a minute when the green light on the pi stops blinking, unplug the car counter from the power brick. 
Be sure to recharge the power brick for the next day.

##Troubleshooting
Turn off the toggle switch, and back on. This closes the current data files so you can examine them.

To examine Arduino serial output, disconnect cable from raspberry pi
and use a USB extender cable to connect to your laptop. Then
you can look at the serial stream with a terminal program 9600 baud.

To modify arduino code, have a project set up on your laptop.
Then disconnect cable from raspberry pi, and connect to laptop.
Download a new sketch.

