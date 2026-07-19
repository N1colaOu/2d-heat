import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from matplotlib import colormaps

def read_plate(name):
    with open(name, 'r') as f:
        nx, ny, nt = map(int, f.readline().split())
        data = np.loadtxt(f, dtype=np.float64) 
    return data.reshape(nt, ny, nx)

def visualize(plates, step=1, interval=50):
    nt, ny, nx = plates.shape
    frames = plates[::step]                   
    colors = colormaps.get_cmap("magma")
    fig, ax = plt.subplots()
    vmin, vmax = plates[0].min(), plates[0].max()
    im = ax.imshow(frames[0], vmin=vmin, vmax=vmax, animated=True, cmap=colors)
    fig.colorbar(mappable=im)
    def update(frame):
        im.set_data(frame)
        return [im] 

    ani = animation.FuncAnimation(fig, update, frames=frames,
                                  interval=interval, blit=True,
                                  repeat_delay=1000)
    return ani

i = int(input("Which Example to load? (0 - 1) "))
if i == 0 or i == 1:
    data = read_plate(f"/home/nic/GithubRepos/2d-heat/build/data{i}.txt")
    ani = visualize(data, step=1, interval=50)
    ani.save(f"animation{i}.gif", dpi=80, writer='pillow')  
else:
    print("Invalid Input!")