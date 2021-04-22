import matplotlib.pyplot as plt
import numpy as np

X, Y = np.meshgrid(np.arange(0, 3 * np.pi, .2), np.arange(0, 3 * np.pi, .2))
print (X,Y)
U = np.cos(X)
V = np.sin(Y)


# fig1, ax1 = plt.subplots()
# ax1.set_title('Arrows scale with plot width, not view')
# Q = ax1.quiver(U, V, units='width')
# qk = ax1.quiverkey(Q, 0.9, 0.9, 2, r'$2 \frac{m}{s}$', labelpos='E',
#                    coordinates='figure')


# fig1, ax1 = plt.subplots()
# ax1.set_title('Arrows scale with plot width, not view')
# Q = ax1.quiver(X, Y, U, V, units='width')
# qk = ax1.quiverkey(Q, 0.9, 0.9, 2, r'$2 \frac{m}{s}$', labelpos='E',
#                    coordinates='figure')

fig2, ax2 = plt.subplots()
ax2.set_title("pivot='mid'; every third arrow; units='inches'")
Q = ax2.quiver(X[::3, ::3], Y[::3, ::3], U[::3, ::3], V[::3, ::3],
               pivot='mid', units='inches')
qk = ax2.quiverkey(Q, 0.9, 0.9, 1, r'$1 \frac{m}{s}$', labelpos='E',
                   coordinates='figure')
ax2.scatter(X[::3, ::3], Y[::3, ::3], color='r', s=5)
#
# fig3, ax3 = plt.subplots()
# ax3.set_title("pivot='tip'; scales with x view")
# M = np.hypot(U, V)
# Q = ax3.quiver(X, Y, U, V, M, units='x', pivot='tip', width=0.022,
#                scale=1 / 0.15)
# qk = ax3.quiverkey(Q, 0.9, 0.9, 1, r'$1 \frac{m}{s}$', labelpos='E',
#                    coordinates='figure')
# ax3.scatter(X, Y, color='k', s=5)

plt.show()

# import numpy as np
# import matplotlib.pyplot as plt
# import math
#
# # print ("cos(3) : ",  math.cos(3))
# # print ("cos(-3) : ",  math.cos(-3))
# # print ("cos(0) : ",  math.cos(0))
# # print ("cos(math.pi) : ",  math.cos(math.pi))
# # print ("cos(2*math.pi) : ",  math.cos(2*math.pi))
#
# x = 7.53*math.cos(135)
# y = 7.53*math.sin(135)
#
# x1 = 9.2*math.cos(157)
# y1 = 9.2*math.sin(157)
#
# x2 = 0.58*math.cos(45)
# y2 = 0.58*math.sin(45)
#
# print (x,y)
# print(x1,y1)
# print(x2,y2)
#
# V = np.array([[x,y],[x1,y1],[x2,y2]])
# origin = [0],[0]
# plt.quiver(*origin, V[:,0], V[:,1], color=['r','b','g'])
# plt.show()
# # V = np.array([[1,1],[-2,2],[4,-7]])
# #
# # origin = [0], [0] # origin point
# #
# # plt.quiver(*origin, V[:,0], V[:,1], color=['r','b','g'], scale=40)
# # # v12 = V[0] + V[1] # adding up the 1st (red) and 2nd (blue) vectors
# # # v13 = V[0] + V[2]
# # # v23 = V[1] + V[2]
# # v123 = V[0] + V[1] + V[2]
# # # plt.quiver(*origin, v12[0], v12[1])
# # # plt.quiver(*origin, v13[0], v13[1])
# # # plt.quiver(*origin, v23[0], v23[1])
# # plt.quiver(*origin, v123[0], v123[1])
# # plt.show()
