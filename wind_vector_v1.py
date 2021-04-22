import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import math
import matplotlib.gridspec as gridspec
import array

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

xfin = x0+x22+x45+x67+x90+x112+x135+x157+x180
yfin = y0+y22+y45+y67+y90+y112+y135+y157+y180

print (xfin, yfin)

plt.quiver([0, 0, 0,0,0,0,0,0,0],
[0, 0, 0,0,0,0,0,0,0],
[x0, x22, x45, x67, x90,x112,x135,x157,x180],
[y0, y22, y45, y67, y90,y112,y135,y157,y180],
width = 0.0040, angles='xy', scale_units='xy', scale=1, color = ['r','b','g'], pivot = 'tip')
# plt.quiver([0],[0],[xfin],[yfin], angles='xy', width = 0.0040, scale_units='xy', scale=1, pivot = 'tip')
plt.xlim(-200,2600)
plt.ylim(-2600, 20)
plt.show()
