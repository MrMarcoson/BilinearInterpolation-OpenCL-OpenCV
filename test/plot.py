import pandas as pd
import matplotlib.pyplot as plt
import csv
  
pixels = []
gpu_time = []
cpu_time = []
  
gpu_info = pd.read_csv('gpu.csv', sep=';', decimal='.', header=None)
cpu_info = pd.read_csv('cpu.csv', sep=';', decimal='.', header=None)

info = pd.concat([gpu_info, cpu_info], axis=1, join='inner')
info.columns = ['image_size', 'gpu', 'to_delete', 'cpu']
info.drop(info.columns[2], axis=1, inplace=True)

#gpu_info.plot.scatter(x=0, y=1)

fig, ax = plt.subplots()
ax.plot(info["image_size"], info["gpu"], label="GPU", color='r')
ax.plot(info["image_size"], info["cpu"], label="CPU", color='b')

ax.set_xlabel("Image size (pixels x pixels)")
ax.set_ylabel("Time of execution (s)")
ax.legend(loc='best')

plt.show()
  
#plot_gpu = plt.scatter(pixels, gpu_time, label='GPU', color='r')
#plot_cpu = plt.scatter(pixels, cpu_time, label='CPU', color='b')
