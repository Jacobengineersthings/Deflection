#
#
# @Version 1.0
# @Author Jacob Hardy
# Reference
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from matplotlib import style
import numpy as np
import random
import serial

#initialize serial port
ser = serial.Serial(port="COM8", baudrate=9600, bytesize=8, timeout=10, stopbits=serial.STOPBITS_ONE)
#initializeSerialPort()
ser.timeout = 10  # specify timeout when using readline()
if ser.is_open == True:
    print("\nAll right, serial port now open. Configuration:\n")
    print(ser, "\n")  # print serial parameters
else:
    ser.open()

# Create figure for plotting
fig = plt.figure()
ax = fig.add_subplot(1, 1, 1)
xs = [] #store trials here (n)
ys = [] #store relative frequency here
rs = [] #for theoretical probability

# Set up plot to call animate() function periodically


# This function is called periodically from FuncAnimation
def animate(i, xs, ys,):

    #Aquire and parse data from serial port
    line = ser.readline()      #ascii
    #print(line)
    lineAsList = line.split(b',') #typecast "b" changes a string to a byte data type
    print(lineAsList)
    i = int(lineAsList[0])
    deflection = lineAsList[len(lineAsList)-1]
    deflectionAsList = deflection.split(b'\n')
    deflectionAsList = deflection.split(b'\r')
    deflectionAsFloat = float(deflectionAsList[0])
    print(deflectionAsFloat)
	# Add x and y to lists
    xs.append(i)
    ys.append(deflectionAsFloat)
    rs.append(0.5)

    # Limit x and y lists to 20 items
    #xs = xs[-20:]
    #ys = ys[-20:]

    # Draw x and y lists
    ax.clear()
    ax.plot(xs, ys, label="Deflection (mm)")
    #ax.plot(xs, rs, label="time")

    # Format plot
    plt.xticks(rotation=45, ha='right')
    plt.subplots_adjust(bottom=0.30)
    plt.title('Liveplot of deflection values over time')
    plt.ylabel('Relative frequency')
    plt.legend()
    plt.axis([1, None, -150, 150]) #Use for arbitrary number of trials
    #plt.axis([1, 100, 0, 1.1]) #Use for 100 trial demo

ani = animation.FuncAnimation(fig, animate, fargs=(xs, ys), interval=2)

plt.show()
ser.close()