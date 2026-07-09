import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation

def read_plate(name):
    with open(name, 'r') as f:
        nx, ny, nt = map(int, f.readline().split())
        data = np.loadtxt(f, dtype=np.float64)   # more robust than fromstring
    return data.reshape(nt, ny, nx)

def visualize(plates, step=1, interval=50):
    nt, ny, nx = plates.shape
    frames = plates[::step]                    # optional time downsampling

    fig, ax = plt.subplots()
    # use global min/max for consistent colormap
    vmin, vmax = plates.min(), plates.max()
    im = ax.imshow(frames[0], vmin=vmin, vmax=vmax, animated=True)

    def update(frame):
        im.set_data(frame)
        return [im]   # return the artist for blitting (optional)

    ani = animation.FuncAnimation(fig, update, frames=frames,
                                  interval=interval, blit=True,
                                  repeat_delay=1000)
    return ani

i = int(input("Which Example to load? (0 - 1) "))
if i < 0 or i > 1:
    print("Invalid Input!")
else:
    data = read_plate(f"/home/nic/GithubRepos/2d-heat/build/data{i}.txt")
    ani = visualize(data, step=1, interval=50)
    ani.save(f"animation{i}.gif", dpi=80, writer='pillow')  # lower dpi for smaller file