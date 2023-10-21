# Prerequisite

## Check your kernel version
```
$ uname -r
```
if the version < 5.4, please update  linux kernel.

## Install linux-headers package
```
$ sudo apt install linux-headers-`uname -r`
```

## Confirm linux-headers package installed correctly
```
$ dpkg -L linux-headers-`uname -r` | grep "/lib/modules/.*/build"
```

The output of perspective:
```
/lib/modules/<your version>/build
```


## Reference
[Linux 核心模組運作原理](https://hackmd.io/@sysprog/linux-kernel-module)