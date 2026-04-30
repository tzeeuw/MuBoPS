from mubops import mubops_core as mc
import random
import math
import numpy as np

    
sim = mc.Simulation()

minR = 0.1
maxR = 0.8
for i in range(5000):
    body = mc.ClassicalBody()
    angle = random.uniform(0, 2 * 3.14159)
    phi = random.uniform(0, 3.14159)
    radius = math.sqrt(minR**2 + random.uniform(0, 1) * (maxR**2 - minR**2))
    x = radius * math.cos(angle) 
    y = radius * math.sin(angle)
    z = random.uniform(-0.8, 0.8)
    body.setPosition(x, y, 0.3)

    # phi_tangent = random.uniform(0, 2*3.14159)
    # r_norm = radius/np.linalg.norm([x, y, z])
    # if abs(x) <= abs(y) and abs(x) <= abs(z):
    #     ref = np.array([1, 0, 0])
    # else:
    #     ref = np.array([0,1,0])
    
    # t1 = np.cross(ref, [x, y, z])
    # t1 = t1/np.linalg.norm(t1)
    # t2 = np.cross([x, y, z], t1)

    # tangent_dir = math.cos(phi_tangent) * t1 + math.sin(phi_tangent) * t2
    # tangent_dir = tangent_dir/np.linalg.norm(tangent_dir)

    vel_size = math.sqrt(6.67430e-11 * 5.972e8 / radius) / radius
    body.setVelocity(-y * vel_size, x * vel_size, 0)
    body.setAcceleration(0.0, 0.0, 0.0)
    sim.addBody(body)
# body = mc.QuantumBody(50,50,50)
# sim.addBody(body)

renderer = mc.Renderer(sim)
renderer.setupWindow(2560, 1440)
renderer.setupShaders()
renderer.setupObjects()
renderer.setupCamera()
renderer.startRenderLoop()
