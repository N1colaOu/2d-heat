import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from mpl_toolkits.axes_grid1 import make_axes_locatable
from matplotlib import colormaps
from matplotlib import colors

def read_plate(name):
    with open(name, 'r') as f:
        nrows, ncols, nt = map(int, f.readline().split())
        data = np.loadtxt(f, dtype=np.float64) 
    return data.reshape(nt, nrows, ncols)

def visualize(plates, interval=50):
    frames = plates[::1]

    cmap = colormaps.get_cmap("magma")
    fig, ax = plt.subplots()

    divider = make_axes_locatable(ax)
    cax = divider.append_axes("right", size="5%", pad=0.05)

    vmin, vmax = plates[0].min(), plates[0].max()
    im = ax.imshow(frames[0], vmin=vmin, vmax=vmax, animated=True, cmap=cmap)
    fig.colorbar(mappable=im, cax=cax)
    def update(frame):
        im.set_data(frame)
        return [im] 

    ani = animation.FuncAnimation(fig, update, frames=frames,
                                  interval=interval, blit=True,
                                  repeat_delay=1000)
    return ani

i = int(input("Which Example to load? (0, 1, 2, 3): "))
if 0 <= i <= 3:
    data = read_plate(f"/home/nic/GithubRepos/2d-heat/build/data{i}.txt")
    ani = visualize(data, interval=50)
    ani.save(f"animation{i}.mp4", dpi=200, writer='ffmpeg')
else:
    print("Invalid Input!")