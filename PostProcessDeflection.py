#This code seeks to generate figures, graphs, and data from general acceleromoter and IR sensor data. This effort was done to better understand the
#deflection, vibration, and dynamic response of objects to impulse or step responses
# @author Jacob Hardy
# @version V1.1
#11/02/2022
#--------------------------------------------#
def plotRawAndFilter(xvalue,yvalue, Title, xLabel,yLabel): #This function filters and plots raw + filtered data
    measurements = np.asarray(yvalue)
    kf = KalmanFilter(transition_matrices=[1],
                      observation_matrices=[1],
                      initial_state_mean=measurements[0],
                      initial_state_covariance=1,
                      observation_covariance=5,
                      transition_covariance=1)  # 0.01)
    state_means, state_covariances = kf.filter(measurements)
    state_std = np.sqrt(state_covariances[:, 0])
    plt.xlabel(xLabel)
    plt.ylabel(yLabel)
    plt.plot(xvalue, measurements, '-r', label= 'raw ' + yLabel + ' data')
    plt.plot(xvalue, state_means, '-b', label='kalman-filter output of ' + yLabel)
    plt.title(Title)
    plt.legend(loc='best')
    #plt.show()
def plotFilter(xvalue,yvalue, Title, xLabel,yLabel,): #This function filters and plots raw + filtered data
    measurements = np.asarray(yvalue)
    kf = KalmanFilter(transition_matrices=[1],
                      observation_matrices=[1],
                      initial_state_mean=measurements[0],
                      initial_state_covariance=1,
                      observation_covariance=5,
                      transition_covariance=1)  # 0.01)
    state_means, state_covariances = kf.filter(measurements)
    state_std = np.sqrt(state_covariances[:, 0])
    plt.xlabel(xLabel)
    plt.ylabel(yLabel)
    plt.plot(xvalue, state_means, '-g', label='kalman-filter output of ' + yLabel)
    plt.title(Title)
    plt.legend(loc='best')
from matplotlib import pyplot as plt    #To plot in python
from pykalman import KalmanFilter       #For filtering Data
import numpy as np                      #For Filtering Data //
#initiate lists needed
ddx = []          # [m/s^2]
dx = []           # [mm]
Min = []          # [Min]
Sec = []          # [Sec]
mass = []         # [kg]
Force = []        # [N]
K = []            # [N/m]
test = []
fileName = "DEFLECT.TXT"
i = 0 #iterator for
G = 1 #temporary storage of previous time instance
ignore_rows = 1 #number of the first rows to ignore (useful when data has transients)
fid = open(fileName, 'r')
for row_num, line in enumerate(fid):
    if row_num <= ignore_rows:
        Min.insert(row_num, 0.0)
        Sec.insert(row_num, 0.0)
        ddx.insert(row_num, 0.0)
        mass.insert(row_num, 0.0)
        dx.insert(row_num, 0.0)
        Force.insert(row_num, 0.0)
        K.insert(row_num, 0.0)
    else:
        values = line.strip().split(',')
        Sec.insert(row_num, float(values[0])/1000)
        ddx.insert(row_num,  abs(float(values[1])))
        mass.insert(row_num,  float(values[2]))
        dx.insert(row_num,  float(values[3]))
        Force.insert(row_num, (mass[row_num] / ddx[row_num]))
        K.insert(row_num, Force[row_num] / dx[row_num])
#-----------------------------2+2 quick maths-----------------------
g = [] #first N data points of the Sec list
for i in range(0,len(dx),1) :
    g.append(Sec[i])
#--------------------------now, lets do some cool shit-------------------
fig = plt.figure(figsize=(12, 4))
plt.subplot(1,3,1) #subplot with 1 row, 3 columns the 1st one
plotRawAndFilter(Sec,K,'graph of K versus time','time[s]','stiffness [N/mm]')
plt.subplot(1,3,2) #subplot with 1 row, 3 columns the 2nd one
plotRawAndFilter(Sec,Force, 'graph of F over time', 'time[s]', 'Force [N]')
plt.subplot(1,3,3) #subplot with 1 row, 3 columns the 3rd one
plotRawAndFilter(Sec, dx, 'Deflection of bracket over time', 'time[s]', 'deflection [mm]')
plt.show()

fig = plt.figure(figsize=(16, 4))
plt.subplot(1,3,1) #subplot with 1 row, 3 columns the 1st one
plotFilter(Sec,K,'graph of K versus time','time[s]','stiffness [N/mm]')
plt.subplot(1,3,2) #subplot with 1 row, 3 columns the 2nd one
plotFilter(Sec,Force, 'graph of F over time', 'time[s]', 'Force [N]')
plt.subplot(1,3,3) #subplot with 1 row, 3 columns the 3rd one
plotFilter(Sec, dx, 'Deflection of bracket over time', 'time[s]', 'deflection [mm]')
plt.show()
fid.close()




