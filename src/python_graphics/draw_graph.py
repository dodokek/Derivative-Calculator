import numpy as np
import numpy as np
from scipy.interpolate import make_interp_spline
import matplotlib.pyplot as plt

# Dataset
f = open('data/graphic_data.txt','r')

x_1 = []
y_1 = []

for row in f:
    row = row.split(' ')
    x_1.append(int(row[0]))
    y_1.append(float(row[1]))

x = np.array(x_1)
y = np.array(y_1)

X_Y_Spline = make_interp_spline(x, y)

# Returns evenly spaced numbers
# over a specified interval.
X_ = np.linspace(x.min(), x.max(), 500)
Y_ = X_Y_Spline(X_)

# Plotting the Graph
plt.plot(X_, Y_)
plt.title("This is god damn it graphic, look at it fella!")
plt.xlabel("X")
plt.ylabel("Y")
plt.savefig("data/function_graph.png")
