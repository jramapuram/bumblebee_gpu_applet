#bumblebee_gpu_applet
A simple tool that can be used with i3blocks in order to provide GPU usage / GPU memory usage and GPU temp for NVIDIA cards using bumblebee

## Building
Edit the `Makefile` to setup your CUDA dir and also make sure `libnvidia-ml` in in `/usr/lib`, otherwise you will have to change that too. This is currently only tested on ARCH & I might eventually get to other distros, but no guarentees. 

After this simply:
```bash
make
```

## Usage
```conf
[script]
command=<PATH>/bumblebee_gpu_applet
interval=10
```

## Screenshots
GPU ON:
![GPU On](gpu_on.png)

GPU OFF:
![GPU Off](gpu_off.png)
