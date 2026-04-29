from mubops import mubops_core as mc
import random
import math

    
sim = mc.Simulation()

minR = 0.1
maxR = 0.8
for i in range(5000):
    body = mc.ClassicalBody()
    angle = random.uniform(0, 2 * 3.14159)
    radius = math.sqrt(minR**2 + random.uniform(0, 1) * (maxR**2 - minR**2))
    x = radius * math.cos(angle)
    y = radius * math.sin(angle)
    body.setPosition(x, y, 0.0)

    vel_size = math.sqrt(6.67430e-11 * 5.972e8 / radius)/radius
    body.setVelocity(-y * vel_size, x * vel_size, 0.0)
    body.setAcceleration(0.0, 0.0, 0.0)
    sim.addBody(body)

renderer = mc.Renderer(sim)
renderer.setupWindow(800, 800)
renderer.setupShaders()
renderer.setupObjects()
renderer.setupCamera()
renderer.startRenderLoop()
