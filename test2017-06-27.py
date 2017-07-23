# -*- coding: utf-8 -*-
"""
Created on Tue Jun 27 12:03:01 2017

@author: Richard Lyu
"""

#!/usr/bin/env python
"""
 Blinks an LED on digital pin 13
 in 1 second intervals
"""

from Arduino import Arduino
import time
import mido
import pygame

"""
Arduino.Servos.attach(pin, min=544, max=2400) Create servo instance. Only 8 servos can be used at one time.
Arduino.Servos.read(pin) Returns the angle of the servo attached to the specified pin
Arduino.Servos.write(pin, angle) Move an attached servo on a pin to a specified angle
Arduino.Servos.writeMicroseconds(pin, uS) Write a value in microseconds to the servo on a specified pin
Arduino.Servos.detach(pin) Detaches the servo on the specified pin
"""

board = Arduino('9600') #plugged in via USB, serial com at rate 9600
board.Servos.attach(4, min=544, max=2400) 
board.Servos.attach(5, min=544, max=2400) 

mid = mido.MidiFile('004.MID')      #read midi file with mido, ready to extract information

playList =[]
timeDelay = []

for msg in mid:                 #extract relevant information, save them into lists
    if not msg.is_meta:
        #print(msg)
        msgList = str(msg).split(' ')
        if msgList[0] == 'note_on':
            channel = msgList[1][8:]
            note = msgList[2][5:]
            velocity = msgList[3][9:]
            delay = msgList[4][5:]
            playList.append([channel, note, velocity])
            timeDelay.append(delay)

pygame.init()
pygame.mixer_music.load('004.MID')  # load midi file with pygame.mixer_music, ready to play
pygame.mixer_music.play()     #play midi music

while True:
    try:
        print("stageA")
        board.digitalWrite(13,"HIGH")
        board.Servos.write(4, 0)
        time.sleep(1)
        print("stageB")
        board.Servos.write(4, 90)
        board.digitalWrite(13,"LOW")
        time.sleep(1)

    except KeyboardInterrupt:
        print("exit...")
        board.close()
        pygame.mixer_music.stop()

        