import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as anim

def read_plate(name):

    file = open(name, 'r')

    first_line = file.readline()
    nx, ny, nt = map(int, first_line.split())

    rest = file.read()
    all_values = np.fromstring(rest, sep=' ', dtype=np.float64)

    plates = all_values.reshape((nt, ny, nx))

    file.close()
    return plates;

def visualize(plates):
    nt = np.size(plates, 0)
    fig, ax = plt.subplots()
    ims = []
    for i in range(nt):
        im = ax.imshow(plates[i], animated=True)
        ims.append([im])
    ani = anim.ArtistAnimation(fig, ims, interval=50, blit=True,
                                repeat_delay=1000)
    return ani
data = read_plate("/home/nic/GithubRepos/2d-heat/build/data.txt")
animation = visualize(data)
animation.save("animation.gif")