# YAWIEL
[![Build Status](https://travis-ci.org/robertohueso/yawiel_colext.svg?branch=master)](https://travis-ci.org/robertohueso/yawiel_colext)

Yet Another WIkifier and Entity Linker.
This is a collocation extractor library for the YAWIEL project.

## Build

Dependencies:
* CMake >= 3.5
* Boost >= 1.67
  - unit\_test_framework
  - serialization
  - container
* OpenMP (optional)

How to build?

### Linux

```shell
$ cd yawiel
$ mkdir build
$ cd build
$ cmake ../
$ make
```

This results in a library stored in ```build/lib/``` and (if activated) tests stored in ```build/bin/yawiel_test```.
