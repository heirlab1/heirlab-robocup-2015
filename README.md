# heirlab-robocup-2015

###How to test setup and test your CM-700 connection

First, plug in the LN-101 to your computer, and into the CM-700. Then, type in your terminal
``` 
$ dmesg | grep tty
```

The last device on the list should be the LN-101, which should appear as ``ttyUSBx`` where x is some number.

To ensure the CM-700 is connected properly, use
```
$ screen /dev/ttyUSBx 57600
```
where x is the number in the device address you saw in the output dmesg

Switch your CM-700 to play mode, and press start. You should get a solid green light, and some output should appear on the terminal

(To quit out of screen use ``ctrl-a+d``)

###LN-101 Troubleshooting

If you can't connect to the CM-700 with a LN-101 from Windows, follow the steps in [this guide](https://www.dropbox.com/s/84g7pvjwn235afe/FT232R%20USB%20UART%20Recovery%20Instruction.pdf?dl=0) to recover your drivers

If you can't connect from Linux, your LN-101 product ID may have been erased. In this case, plug your LN-101 into a Windows computer and download the FT_PROG utility from [this list](http://www.ftdichip.com/Support/Utilities.htm#FT_PROG)

For information on how to reset the product ID, watch [this video at 6:00](https://www.youtube.com/watch?v=WIF9f4NsEvk#t=359)