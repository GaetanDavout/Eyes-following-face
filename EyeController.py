# -*- coding: utf-8 -*-
"""
Created on Sun Apr  5 15:07:26 2020

@author: Gaetan Davout

"""
from threading import Timer

import warnings
import serial
import serial.tools.list_ports

import cv2
import face_recognition
import time


# main function
def main():
    # define baud rate
    dbt = 9600
    
    # search Arduino Port 
    arduino_ports = [
        p.device
        for p in serial.tools.list_ports.comports()
        if 'Arduino' in p.description
    ]
    if not arduino_ports:
        raise IOError("No Arduino found")
    if len(arduino_ports) > 1:
        warnings.warn('Multiple Arduinos found - using the first')
    
    # use previous data to set serial communication
    global ser
    ser = serial.Serial(arduino_ports[0],dbt)
    #Timer(1, readSerial).start()
    
    # camera settings
    cap = cv2.VideoCapture(0)
    
    Xsize = 320
    Ysize = 240
    cap.set(3,Xsize)
    cap.set(4,Ysize)
    
    # init some variables
    face_location = []
    noface_time = 0
    tmp_time = 0
    
    while True:
        ret,im = cap.read()
        
        cv2.imshow("Robert Eyes", im)
        
        face_location = face_recognition.face_locations(im)
        
        print("Found {} faces in image.".format(len(face_location)))
        # on prendra uniquement le premier visage en compte pour le mouvement des yeux
        #
        #       x           y
        #  255--+---0   0---0---0
        #   |   |   |   |   |   |
        #  255--+---0   +---+---+
        #   |   |   |   |   |   |
        #  255--+---0  255-255-255
        #
        if len(face_location) > 0:
            x = ((face_location[0][1]+face_location[0][3])/2)
            y = ((face_location[0][0]+face_location[0][2])/2)
            x = int(x*255/Xsize)
            y = int(y*255/Ysize)
            print("x="+str(x)+"\ty="+str(y))
            ser.write(bytes([x,y,1]))
            noface_time = 0
        else:
            if noface_time == 0:
                print("pass")
                tmp_time = time.time()
                noface_time = 0.0000001
                
            noface_time = noface_time + (time.time()-tmp_time)
            
            if noface_time > 50 and noface_time < 30:
                print("noface_time = " + str(noface_time))
                ser.write(bytes([127,127,0]))
            

        key = cv2.waitKey(1)
    
"""    
def readSerial():
    while ser.inWaiting():
        print(str(ser.read()))
    
    Timer(0.1, readSerial).start() # autocall this function in 100ms
"""
if __name__ == "__main__":
    main()