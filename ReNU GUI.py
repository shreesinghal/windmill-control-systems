#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Fri Jan 10 22:39:50 2025

@author: sunil
"""


from tkinter import ttk
from tkinter import *

import threading
import time

import serial

import numpy as np

data = {
    "time": 0,
    "speed": 0,
    "wind": 0,
}
lock = threading.Lock()
# lock.release()




root = Tk()


can = Canvas(root, height=400, width=600
, bg="white", highlightbackground="gray", highlightcolor="blue", highlightthickness=0
)
can.pack()


    
entry = Entry(can
, bg="#5555ee", fg="white", justify="center", borderwidth=0, highlightbackground="gray", highlightcolor="#cccccc", highlightthickness=0, selectbackground="gray"
)
can.create_window(20, 70, window=entry, anchor="nw", width=210, height=40)

def set_angle():
    print(entry.get())

button = Button(can, bg="red", text="->", borderwidth=0, highlightbackground="black", highlightcolor="blue", highlightthickness=1, command=set_angle)
# can.create_window(240, 20, window=button, anchor="nw", width=40, height=40)
can.tag_bind(
    can.create_rectangle(240, 70, 280, 110, fill="#5555ee", outline=""),
    "<Button-1>", set_angle
)
can.create_text(260, 90, text=">")

button = Button(can
, bg="red", fg="black", borderwidth=0, text="E-STOP"
)
# can.create_window(20, 70, window=button, anchor="nw", width=260, height=40)

def stop(event):
    print()
can.tag_bind(
    can.create_rectangle(20, 20, 280, 60, fill="#ee5555", outline=""),
    "<Button-1>", stop
)
can.create_text(150, 40, text="E-STOP")



class Graph():
    def __init__(self, can, location, color):
        self.data = []
        self.location = location
        self.can = can
        self.first = 0
        self.lowest = 0
        self.highest = 0
        self.can.create_rectangle(location, fill="#eeeeee")
        self.line = self.can.create_line(0, 0, 0, 0, fill=color, width=2, 
        # arrow=LAST, arrowshape=(4, 4, 2)
        )
        self.can.create_rectangle(location, outline="#cccccc", width=0)
        self.zero_line = self.can.create_line(0, 0, 0, 0, fill="gray", smooth=True, width=2, dash=True)
        self.can.create_text(location[0], location[1] - 20, anchor="nw", text="Title", fill="black")
        self.current_text = self.can.create_text(location[2] - 100, location[1] - 20, anchor="nw", text="Title", fill="black", font="Monospace")

    def to_real(self, data):
        x, y, x2, y2 = self.location
        time, info = data
        padding = 10
        return (
            x2 + (time - self.data[-1][0]) * 10,
            y2 - ((info - self.lowest) / (self.highest - self.lowest) * (y2 - y - padding * 2) + padding)
        )

    def render(self):
        if (len(self.data) < 2):
            return
        x, y, x2, y2 = self.location
        
        
        # get rid of data points to left of graph
        while (self.to_real(self.data[self.first])[0] < x):
            self.first = self.first + 1
        
        self.can.coords(self.zero_line,
            [self.to_real(data) for data in [(self.data[self.first][0], 0), (self.data[-1][0], 0)]])

        self.can.coords(self.line,
            [self.to_real(data) for data in self.data[self.first:]])




    def add_data(self, time, info):
        self.can.itemconfigure(self.current_text, text=f"current: {info:.5f}")

        # find if this is new highest
        # if len(self.data) == 0:
        #     self.lowest = info
        #     self.highest = info
        if info < self.lowest:
            self.lowest = info
        if info > self.highest:
            self.highest = info

        self.data.append((time, info))
        
        self.render()

        
g = Graph(can, (300, 40, 580, 120), "red")
g2 = Graph(can, (300, 160, 580, 240), "blue")
g3 = Graph(can, (300, 280, 580, 360), "green")



    


can.create_polygon(90, 400, 110, 400, 100, 250, fill="#cccccc")
can.create_oval(90, 240, 110, 260, fill="#cccccc", width=0)
fan = [
    can.create_polygon(0, 0, 0, 0, fill="#00cccc", smooth=True),
    can.create_polygon(0, 0, 0, 0, fill="#cc00cc", smooth=True),
    can.create_polygon(0, 0, 0, 0, fill="#cccc00", smooth=True)]
    

def update(event):
    g.add_data(data["time"], data["speed"])
    g2.add_data(data["time"], 1 - data["speed"])
    g3.add_data(data["time"], 5 - data["speed"] ** 2)
    
    for i in range(0,3):
        angle = (np.pi * 2 / 3) * i + -data["time"]#data["speed"]
        a1 = data["speed"]
        blade = np.matmul(
            np.array([
                [0, -15, 10],
                [10, 50, 10],
                [0, 100, 10],
                [-10, 50, 10],
            ]),
            np.matmul(np.array([
                [np.cos(angle), -np.sin(angle), 0],
                [np.sin(angle), np.cos(angle), 0],
                [0, 0, 1],
            ]),
            np.array([
                [np.cos(a1), 0, np.sin(a1)],
                [0, 1, 0],
                [-np.sin(a1), 0, np.cos(a1)],
            ]))
        ) + np.array([
            [100, 250, 0],
            [100, 250, 0],
            [100, 250, 0],
            [100, 250, 0],
        ])


        can.coords(fan[i], blade[:,:2].flatten().tolist())

    

# can.bind("<B1-Motion>", stop)
# can.bind("<Button-1>", stop)
can.bind("<<read>>", update)

class A(threading.Thread):
    def __init__(self, data):
        threading.Thread.__init__(self)
        self.start_time=time.monotonic()
        self.data = data
        self.usb = serial.Serial("COM5", 9600)

    def run(self):
        while(True):
            # fake loop
            time.sleep(0.01)
            _time = time.monotonic()
            self.data["time"] = _time - self.start_time
            self.data["speed"] = np.sin(_time * 1)
            # print(data)

            # wait for a message
            while(self.usb.in_waiting == 0):
                pass

            # read
            line = self.usb.readline()
            print(line)

            # send data
            # if e-stop has been pushed, stop


            
            

            
            # update graph
            # can.event_generate("<<read>>")

            

a = A(data)
a.start()


print("hi")
root.mainloop()


"""

int main() {

    while(true) {
        if (Serial.avialable > 0) {
            // read the angle
            float angle = (float) Serial.read();
            set_angle(angle);
        }
    }
}

"""







#tkinter._test()