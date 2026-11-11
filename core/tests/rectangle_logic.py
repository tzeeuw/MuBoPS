import numpy as np
import matplotlib.pyplot as plt


# rectangle box with side lengts 2,3,4
dx = 2
dy = 3
dz = 4

max_length = np.sqrt(dx**2 + dy**2 + dz**2)/2
angles = np.linspace(0, 2 * np.pi, 100)

vec = -np.array([3, -3.1, 0])
scaled_vec = vec.copy()
sidelengths = np.array([dx/2, dy/2, dz/2])

# finds the scaling factor needed to scale the direction vector to the rectangle box
subtracted_vec = np.abs(vec) - sidelengths
max_index = np.argmax(subtracted_vec)
print(max_index)
scale = sidelengths[max_index] / vec[max_index]
scaled_vec *= scale * np.sign(vec[max_index])
        

plt.figure(figsize=(6, 6))
plt.plot(np.cos(angles) * max_length, np.sin(angles) * max_length, color='red', linewidth=2)
plt.plot([-dx/2, dx/2], [dy/2, dy/2], color='lightblue', linewidth=2)
plt.plot([dx/2, dx/2], [dy/2, -dy/2], color='lightblue', linewidth=2)
plt.plot([-dx/2, dx/2], [-dy/2, -dy/2], color='lightblue', linewidth=2)
plt.plot([-dx/2, -dx/2], [dy/2, -dy/2], color='lightblue', linewidth=2)
plt.plot([-dx/2 - 2, dx/2 + 2], [dy/2, dy/2], ls='--', color='lightblue', linewidth=2)
plt.plot([-dx/2 - 2, dx/2 + 2], [-dy/2, -dy/2], ls='--', color='lightblue', linewidth=2)
plt.plot([dx/2, dx/2], [dy/2 + 2, -dy/2 - 2], ls='--', color='lightblue', linewidth=2)
plt.plot([-dx/2, -dx/2], [dy/2 + 2, -dy/2 - 2], ls='--', color='lightblue', linewidth=2)
plt.plot([0, vec[0]], [0, vec[1]], color='green', linewidth=2)
plt.plot([0, scaled_vec[0]], [0, scaled_vec[1]], color='orange', linewidth=2)
plt.xlim(-dx -1, dx + 1)
plt.ylim(-dx -1, dx + 1)
plt.show()
