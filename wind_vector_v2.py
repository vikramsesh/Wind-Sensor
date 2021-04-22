# 8 angles
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import math
import matplotlib.gridspec as gridspec
import array

angle_nos = 8

x_ang = np.zeros([1, angle_nos])
y_ang = np.zeros([1, angle_nos])

xtot = 0
ytot = 0


def angle_split(test_angle, x, y):
    global x_ang, y_ang
    if test_angle == 0:
        x_ang[0][0] += x
        y_ang[0][0] += y
    if test_angle == 45:
        x_ang[0][1] += x
        y_ang[0][1] += y
    if test_angle == 90:
        x_ang[0][2] += x
        y_ang[0][2] += y
    if test_angle == 135:
        x_ang[0][3] += x
        y_ang[0][3] += y
    if test_angle == 180:
        x_ang[0][4] += x
        y_ang[0][4] += y
    if test_angle == 225:
        x_ang[0][5] += x
        y_ang[0][5] += y
    if test_angle == 270:
        x_ang[0][6] += x
        y_ang[0][6] += y
    if test_angle == 315:
        x_ang[0][7] += x
        y_ang[0][7] += y


df = pd.read_csv('AIRFLOW3.csv')
time = df['Time (sec.)']
sample = df['Sample#']
magnitudeMS = df['WindSpeed (m/s)']
magnitudeMPH = df['Windspeed (mph)']
angle = df['Angle']

df = df.values
fig1, ax1 = plt.subplots()
ax1.set_title('Windspeed')

for i in range(0, len(df) - 1):
    x = df[i][2] * math.cos(math.radians(df[i][4]))
    y = df[i][2] * math.sin(math.radians(df[i][4]))
    angle_split(df[i][4], x, y)
    xtot += x
    ytot += y

for i in range(0, angle_nos):
    plt.quiver([0], [0], [-x_ang[0][i]], [-y_ang[0][i]],
               width=0.0040, angles='xy', scale_units='xy', scale=1, pivot='tip')

plt.quiver([0], [0], [-xtot], [-ytot],
           width=0.0040, angles='xy', color='r', scale_units='xy', scale=1, pivot='tip')
plt.xlim(-70, 70)
plt.ylim(-70, 70)
plt.show()
