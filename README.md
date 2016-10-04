# MacWarmer
Monitoring temperature of macbook and calculating PI to keep it warm.

### Screenshots

![](https://github.com/XJBTech/MacWarmer/raw/master/img/1.jpeg)
![](https://github.com/XJBTech/MacWarmer/raw/master/img/2.jpeg)
![](https://github.com/XJBTech/MacWarmer/raw/master/img/3.jpeg)

### Build

```sh
$ make
```

### Usage

```sh
$ ./warmer -s [set temp] -p [maximum number of processes]
```

If don't set number of processes, it will use maximum number of processes automatically.

### Special thanks to

* [OSX CPU Temp](https://github.com/lavoiesl/osx-cpu-temp)
* [HWSensors](https://github.com/kozlek/HWSensors)
* [smcFanControl](https://github.com/hholtmann/smcFanControl/tree/master/smc-command)
* [Arduino-PID-Library](https://github.com/br3ttb/Arduino-PID-Library)