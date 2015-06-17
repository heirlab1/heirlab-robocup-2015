# heirlab-robocup-2015

##How to test setup and test your CM-700 connection

First, plug in the LN-101 to your computer, and into the CM-700. Then, type in your terminal
''' 
$ dmesg | grep tty
'''

The last device on the list should be the LN-101, which should appear as "ttyUSBx" where x is some number.

To ensure the CM-700 is connected properly, use
'''
$ screen /dev/ttyUSBx 57600
'''
where x is the number in the device address you saw in the output dmesg

Switch your CM-700 to play mode, and press start. You should get a solid green light, and some output should appear on the terminal

(To quit out of screen use ''ctrl-a+d'')