import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import math
import matplotlib.gridspec as gridspec
import array
from matplotlib import cm

x0 = y0 = 0
x22 = y22 = 0
x45 = y45 = 0
x67 = y67 = 0
x90 = y90 = 0
x112 = y112 = 0
x135 = y135 = 0
x157 = y157 = 0
x180 = y180 = 0
xtot = 0
ytot = 0

df = pd.read_csv('AIRFLOW7.csv')
time = df['Time (sec.)']
sample = df['Sample#']
magnitudeMS = df['WindSpeed (m/s)']
magnitudeMPH = df['Windspeed (mph)']
angle = df['Angle']

df = df.values
fig1, ax1 = plt.subplots()
ax1.set_title('Windspeed')
# x = magnitudeMS * math.cos(math.radians(angle))
# y = magnitudeMS * math.sin(math.radians(angle))

# x = df[:,2] * math.cos(math.radians(df[:,4]))
# y = df[:,2] * math.sin(math.radians(df[:,4]))
def angle_split(test_angle):
    global x0,y0,x22,y22,x45,y45,x67,y67,x90,y90,x112,y112,x135,y135,x157,y157,x180,y180
    if test_angle==0:
        x0 = x0+x
        y0 = y0+y
    if test_angle==22:
        x22 = x22+x
        y22 = y22+y
    if test_angle ==45:
        x45 += x
        y45 += y
    if test_angle ==67:
        x67 += x
        y67 += y
    if test_angle ==90:
        x90 += x
        y90 += y
    if test_angle ==112:
        x112 += x
        y112 += y
    if test_angle ==135:
        x135 += x
        y135 += y
    if test_angle ==157:
        x157 += x
        y157 += y
    if test_angle ==180:
        x180 += x
        y180 += y

for i in range(0,len(df)-1):
    x = df[i][2] * math.cos(math.radians(df[i][4]))
    y = df[i][2] * math.sin(math.radians(df[i][4]))
    angle_split(df[i][4])
    xtot += x
    ytot += y

print (x0,y0)
print (x22,y22)
print (x45,y45)
print (x67,y67)
print (x90,y90)
print (x112,y112)
print (x135,y135)
print (x157,y157)
print (x180,y180)

xfin = x0+x22+x45+x67+x90+x112+x135+x157+x180
yfin = y0+y22+y45+y67+y90+y112+y135+y157+y180

print (xfin, yfin)
# mag = math.sqrt(xtot**2+ytot**2)
# ang = math.atan2(y,x)
#
# print(mag,ang)
# V = np.array([xtot,ytot])
# origin = [0],[0]
# plt.quiver(*origin,xtot,ytot,color = 'r', scale = 21)
#
# plt.show()


# V = np.array([[x0,y0],[x22,y22],[x45,y45],[x67,y67],[x90,y90],[x112,y112],[x135,y135],[x157,y157],[x180,y180]])
# origin = [0], [0] # origin point
# # set_sketch_params(scale = None, Length = 2, randomness = 2)
# # plt.quiver(*origin,x135,y135, color=['r','b','g'],pivot = 'tail', width = 0.0030,headwidth = 2.5,headaxislength =4, scale_units = 'height' )
# # plt.quiver(*origin,xfin,yfin,pivot = 'tail',  width = 0.0030,headwidth = 2.5,minshaft = 0.5, scale_units = 'inches',cmap = 'autumn')
# # plt.contourf(x,20)
# # plt.streamplot(0,0,xfin,yfin,cmap = 'autumn')
# # plt.quiver(*origin,V[:,0], V[:,1], color=['r','b','g'],pivot = 'tail')
# v12 = V[0]+V[1]+V[2]+V[3]+V[4]
# plt.quiver(*origin, V[:5,0], V[:5,1],color = 'green',pivot = 'tip',width = 0.0030,angles='xy', scale_units='xy')
# plt.quiver(*origin, v12[0], v12[1],pivot = 'tip',width = 0.0030, angles='xy', scale_units='xy')
#
# plt.show()

plt.quiver([0, 0, 0,0,0,0,0,0,0], [0, 0, 0,0,0,0,0,0,0], [x0, x22, x45, x67, x90,x112,x135,x157,x180], [y0, y22, y45, y67, y90,y112,y135,y157,y180], angles='xy', scale_units='xy', scale=1, color = ['r','b','g'])
plt.quiver([0],[0],[xfin],[yfin], angles='xy', scale_units='xy', scale=1)
plt.xlim(-5100,2500)
plt.ylim(-10, 6500)
plt.show()
# Q = ax1.quiver(xtot,ytot,color = 'red',units='inches')
# plt.show()

# fig = plt.figure(figsize=(7, 9))
# gs = gridspec.GridSpec(nrows=3, ncols=2, height_ratios=[1, 1, 2])
# ax1 = fig.add_subplot(gs[0, 1])
# u = 1
# v = 1
# Q = ax1.streamplot(2,2,2,2,linewidth=2, density = 0.6,cmap = 'autumn')
# # qk = ax1.quiverkey(Q, 0.1, 0.1, 2, r'$2 \frac{m}{s}$', labelpos='E',coordinates='figure')
# plt.show()

# sample, time, magnitudeMS, magnitudeMPH, angle = np.loadtxt('AIRFLOW7.csv', delimiter = ',' , unpack = True, skiprows = 1)
# plt.grid(True, lw=2, ls='--', c='.75')
# plt.plot(sample,magnitudeMS,label = "windspeedM/S")
# # plt.plot(sample,magnitudeMPH,label = "windspeedMPH")
# plt.legend()
# plt.show()

# print (sample[0])
# x = 7.43 * math.cos(math.radians(0))
# y = 7.43 * math.sin(math.radians(0))
#
# x1 = 7.43 * math.cos(math.radians(45))
# y1 = 7.43 * math.sin(math.radians(45))
#
# x2 = 7.43 * math.cos(math.radians(157))
# # y2 = 7.43 * math.sin(math.radians(157))
#
# for i in sample:
#     x = magnitudeMS[i] * math.cos(math.radians(angle[i]))
#     y = magnitudeMS[i] * math.sin(math.radians(angle[i]))
#
#     V = np.array([x[i],y[i]])
#     origin = [0],[0]
#     plt.quiver(*origin, V[:,0], V[:,1], color=['r','b','g'])
#
# plt.show()

# print (x,y)
