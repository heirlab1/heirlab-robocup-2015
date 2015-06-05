#!/usr/bin/env python
#coding: utf8

import time
import serial
import os
import fnmatch
import functools

from tkinter import *
#Tkinter with python 2

directory='/dev'
UP    ="\xFF\x55\x01\xFE\x00\xFF"
DOWN  ="\xFF\x55\x02\xFD\x00\xFF"
LEFT  ="\xFF\x55\x04\xFB\x00\xFF"
RIGHT ="\xFF\x55\x08\xF7\x00\xFF"

_6    ="\xFF\x55\x00\xFF\x00\xFF"
_5    ="\xFF\x55\x00\xFF\x01\xFE"
_4    ="\xFF\x55\x80\x7F\x00\xFF"
_3    ="\xFF\x55\x40\xBF\x00\xFF"
_2    ="\xFF\x55\x20\xDF\x00\xFF"
_1    ="\xFF\x55\x10\xEF\x00\xFF"

_1U    ="\xFF\x55\x11\xEE\x00\xFF"
_1D    ="\xFF\x55\x12\xED\x00\xFF"
_1L    ="\xFF\x55\x14\xEB\x00\xFF"
_1R    ="\xFF\x55\x18\xEF\x00\xFF"

_2U    ="\xFF\x55\x21\xDE\x00\xFF"
_2D    ="\xFF\x55\x22\xDD\x00\xFF"
_2L    ="\xFF\x55\x24\xDB\x00\xFF"
_2R    ="\xFF\x55\x28\xD7\x00\xFF"

_3U    ="\xFF\x55\x41\xBE\x00\xFF"
_3D    ="\xFF\x55\x42\xBD\x00\xFF"
_3L    ="\xFF\x55\x44\xBB\x00\xFF"
_3R    ="\xFF\x55\x48\xB7\x00\xFF"

_4U    ="\xFF\x55\x81\x7E\x00\xFF"
_4D    ="\xFF\x55\x82\x7D\x00\xFF"
_4L    ="\xFF\x55\x84\x7B\x00\xFF"
_4R    ="\xFF\x55\x88\x77\x00\xFF"


STOP  ="\xFF\x55\x00\xFF\x00\xFF"

def move(event, key):
    ser.write(key.encode('iso-8859-1'))
    print(key)


# Get Port name
for root, dirs, files in os.walk(directory):
    for filename in files:
        for filename in fnmatch.filter(files,'tty.usbserial'+'*'):          
            usbserialport=directory+'/'+filename
            pass

# Check Port        
try:        
    print (usbserialport) 
except NameError:
    print ("Robot Not Connected. Exit.")
    exit(0)


# Connect Port
ser = serial.Serial(
    port=usbserialport,
    baudrate=57600,
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.EIGHTBITS,
    timeout=0,
    xonxoff=1
)

ser.close()
ser.open()
ser.isOpen()

#DRAW GUI
root = Tk()

root.title("Simple GUI")
root.geometry("600x600+100+100")
label1= Label(root, text='Robot Controller GUI',fg='darkgrey').pack()

#UP DOWN LEFT RIGHT BUTTONS

buttonUp=Button(root, text='UP')
buttonUp.place(x="150",y="150")
buttonUp.bind("<Button-1>",functools.partial(move, key=UP))
buttonUp.bind("<ButtonRelease-1>",functools.partial(move, key=STOP))

buttonDown=Button(root, text='Down')
buttonDown.place(x="140",y="250")
buttonDown.bind("<Button-1>",functools.partial(move, key=DOWN))
buttonDown.bind("<ButtonRelease-1>",functools.partial(move, key=STOP))

buttonLeft=Button(root, text='Left')
buttonLeft.place(x="70",y="200")
buttonLeft.bind("<Button-1>",functools.partial(move, key=LEFT))
buttonLeft.bind("<ButtonRelease-1>",functools.partial(move, key=STOP))

buttonRight=Button(root, text='Right')
buttonRight.place(x="210",y="200")
buttonRight.bind("<Button-1>",functools.partial(move, key=RIGHT))
buttonRight.bind("<ButtonRelease-1>",functools.partial(move, key=STOP))

root.bind('<KeyPress-Up>',functools.partial(move, key=UP))
root.bind('<KeyPress-Down>',functools.partial(move, key=DOWN))
root.bind('<KeyPress-Left>',functools.partial(move, key=LEFT))
root.bind('<KeyPress-Right>',functools.partial(move, key=RIGHT))
root.bind('<KeyRelease>',functools.partial(move, key=STOP))

# 1 to 6 BUTTONS

button1=Button(root, text='1')
button1.place(x="450",y="150")
button1.bind("<Button-1>",functools.partial(move, key=_1U))
button1.bind("<ButtonRelease-1>",functools.partial(move, key=STOP))

button2=Button(root, text='2')
button2.place(x="380",y="200")
button2.bind("<Button-1>",functools.partial(move, key=_2))
button2.bind("<ButtonRelease-1>",functools.partial(move, key=STOP))

button3=Button(root, text='3')
button3.place(x="450",y="250")
button3.bind("<Button-1>",functools.partial(move, key=_3))
button3.bind("<ButtonRelease-1>",functools.partial(move, key=STOP))

button4=Button(root, text='4')
button4.place(x="510",y="200")
button4.bind("<Button-1>",functools.partial(move, key=_4))
button4.bind("<ButtonRelease-1>",functools.partial(move, key=STOP))

button5=Button(root, text='5')
button5.place(x="450",y="50")
button5.bind("<Button-1>",functools.partial(move, key=_5))
button5.bind("<ButtonRelease-1>",functools.partial(move, key=STOP))

button6=Button(root, text='6')
button6.place(x="155",y="50")
button6.bind("<Button-1>",functools.partial(move, key=_6))
button6.bind("<ButtonRelease-1>",functools.partial(move, key=STOP))

root.bind('1',functools.partial(move, key=_1))
root.bind('2',functools.partial(move, key=_2))
root.bind('3',functools.partial(move, key=_3))
root.bind('4',functools.partial(move, key=_4))
root.bind('5',functools.partial(move, key=_5))
root.bind('6',functools.partial(move, key=_6))

# 1 + arrows BUTTONS

button1U=Button(root, text='[1+U]  Q')
button1U.place(x="100",y="350")
button1U.bind("<Button-1>",functools.partial(move, key=_1U))
button1U.bind("<ButtonRelease-1>",functools.partial(move, key=STOP))

button1D=Button(root, text='[1+D]  W')
button1D.place(x="200",y="350")
button1D.bind("<Button-1>",functools.partial(move, key=_1D))
button1D.bind("<ButtonRelease-1>",functools.partial(move, key=STOP))

button1L=Button(root, text='[1+L]  E')
button1L.place(x="300",y="350")
button1L.bind("<Button-1>",functools.partial(move, key=_1L))
button1L.bind("<ButtonRelease-1>",functools.partial(move, key=STOP))

button1R=Button(root, text='[1+R]  R')
button1R.place(x="400",y="350")
button1R.bind("<Button-1>",functools.partial(move, key=_1R))
button1R.bind("<ButtonRelease-1>",functools.partial(move, key=STOP))

root.bind('q',functools.partial(move, key=_1U))
root.bind('w',functools.partial(move, key=_1D))
root.bind('e',functools.partial(move, key=_1L))
root.bind('r',functools.partial(move, key=_1R))

# 2 + arrows BUTTONS

button1U=Button(root, text='[2+U]  Y')
button1U.place(x="100",y="400")
button1U.bind("<Button-1>",functools.partial(move, key=_2U))
button1U.bind("<ButtonRelease-1>",functools.partial(move, key=STOP))

button1D=Button(root, text='[2+D]  U')
button1D.place(x="200",y="400")
button1D.bind("<Button-1>",functools.partial(move, key=_2D))
button1D.bind("<ButtonRelease-1>",functools.partial(move, key=STOP))

button1L=Button(root, text='[2+L]  I')
button1L.place(x="300",y="400")
button1L.bind("<Button-1>",functools.partial(move, key=_2L))
button1L.bind("<ButtonRelease-1>",functools.partial(move, key=STOP))

button1R=Button(root, text='[2+R]  O')
button1R.place(x="400",y="400")
button1R.bind("<Button-1>",functools.partial(move, key=_2R))
button1R.bind("<ButtonRelease-1>",functools.partial(move, key=STOP))

root.bind('y',functools.partial(move, key=_2U))
root.bind('u',functools.partial(move, key=_2D))
root.bind('i',functools.partial(move, key=_2L))
root.bind('o',functools.partial(move, key=_2R))


# 3 + arrows BUTTONS

button1U=Button(root, text='[3+U]  A')
button1U.place(x="100",y="450")
button1U.bind("<Button-1>",functools.partial(move, key=_3U))
button1U.bind("<ButtonRelease-1>",functools.partial(move, key=STOP))

button1D=Button(root, text='[3+D]  S')
button1D.place(x="200",y="450")
button1D.bind("<Button-1>",functools.partial(move, key=_3D))
button1D.bind("<ButtonRelease-1>",functools.partial(move, key=STOP))

button1L=Button(root, text='[3+L]  D')
button1L.place(x="300",y="450")
button1L.bind("<Button-1>",functools.partial(move, key=_3L))
button1L.bind("<ButtonRelease-1>",functools.partial(move, key=STOP))

button1R=Button(root, text='[3+R]  F')
button1R.place(x="400",y="450")
button1R.bind("<Button-1>",functools.partial(move, key=_3R))
button1R.bind("<ButtonRelease-1>",functools.partial(move, key=STOP))

root.bind('a',functools.partial(move, key=_3U))
root.bind('s',functools.partial(move, key=_3D))
root.bind('d',functools.partial(move, key=_3L))
root.bind('f',functools.partial(move, key=_3R))


# 4 + arrows BUTTONS

button1U=Button(root, text='[4+U]  H')
button1U.place(x="100",y="500")
button1U.bind("<Button-1>",functools.partial(move, key=_4U))
button1U.bind("<ButtonRelease-1>",functools.partial(move, key=STOP))

button1D=Button(root, text='[4+D]  J')
button1D.place(x="200",y="500")
button1D.bind("<Button-1>",functools.partial(move, key=_4D))
button1D.bind("<ButtonRelease-1>",functools.partial(move, key=STOP))

button1L=Button(root, text='[4+L]  K')
button1L.place(x="300",y="500")
button1L.bind("<Button-1>",functools.partial(move, key=_4L))
button1L.bind("<ButtonRelease-1>",functools.partial(move, key=STOP))

button1R=Button(root, text='[4+R]  L')
button1R.place(x="400",y="500")
button1R.bind("<Button-1>",functools.partial(move, key=_4R))
button1R.bind("<ButtonRelease-1>",functools.partial(move, key=STOP))

root.bind('h',functools.partial(move, key=_4U))
root.bind('j',functools.partial(move, key=_4D))
root.bind('k',functools.partial(move, key=_4L))
root.bind('l',functools.partial(move, key=_4R))

