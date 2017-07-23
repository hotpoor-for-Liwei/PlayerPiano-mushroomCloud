# -*- coding: utf-8 -*-
"""
Created on Fri Jun 16 10:18:46 2017

@author: Richard
"""

import mido
import pygame
import serial
from time import sleep

#######################################################################

#-------------    Piano Connection Initialization     ----------------
"""
piano1 = serial.Serial('COM9',115200)
piano1.write("1a1b")
if piano1.readline():
    print("Piano1 connected")
"""
"""
piano2 = serial.Serial('COM3',115200)
piano2.write("1a1b")
if piano2.readline():
    print("piano2 connected")
"""

#-------------    Drum connection initialization      -----------------
"""
drum = serial.Serial('COM3',115200)
drum.write("1a1b")
if drum.readline():
    print("Durm connected")
"""
#-------------    Guitar Connection Initialization     ----------------
"""
guitar = serial.Serial('COM10',115200)
guitar.write("1a1b")
if guitar.readline():
    print("Guitar connected")
"""

######################################################################
pygame.init()
mid = mido.MidiFile('the_entertainer.MID')      #read midi file with mido, ready to extract information

channel =[]
note =[]  
velocity =[]
timeDelay = []

for msg in mid:                 #extract relevant information, save them into lists
    if not msg.is_meta:
#        print(msg)
        msgList = str(msg).split(' ')
        if msgList[0] == 'note_on' or msgList[0] == 'note_off':
            channel.append(msgList[1][8:])
            note.append(msgList[2][5:])
            velocity.append(msgList[3][9:])
            timeDelay.append(msgList[4][5:]) 
        elif msgList[0] == 'pitchwheel':
            channel.append(0)
            note.append(0)
            velocity.append(0)
            timeDelay.append(msgList[3][5:])
        elif msgList[0] == 'control_change':
            channel.append(0)
            note.append(0)
            velocity.append(0)
            timeDelay.append(msgList[4][5:])

print("the mid file lasts " + str(mid.length) + " seconds")

######################################################################

pygame.mixer_music.load('the_entertainer.MID')  # load midi file with pygame.mixer_music, ready to play
pygame.mixer_music.play()     #play midi music
"""

for i in range(len(timeDelay)):       # sending midi infomation to corresponding ports 
    try:
        if int(channel[i]) == 0:    
            #print(note[i] + 'a' + velocity[i] + 'b'
            piano1.write(str(note[i]) + 'a' + str(velocity[i]) + 'b')
            
            drum.write(str(note[i]) + 'a' + str(velocity[i]) + 'b')        
            guitar.write(str(note[i]) + 'a' + str(velocity[i]) + 'b')        
        sleep(float(timeDelay[i]))
    except KeyboardInterrupt:
        pygame.mixer_music.stop()
        guitar.close()
        drum.close()
        piano1.close()
#       piano2.close()

guitar.close()       
drum.close()       
piano1.close()
#piano2.close()
"""