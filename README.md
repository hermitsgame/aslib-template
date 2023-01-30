# aslib-template

## 0. Requirements

* cmake >= 3.16

## 1. Install Dependencies

Install gtest
```
$ git clone https://github.com/google/googletest
$ cd googletest
$ mkdir build
$ cd build
$ cmake ..
$ make
$ sudo make install
```

Install glog
```
$ git clone https://github.com/google/glog
cd glog
cmake -S . -B build -G "Unix Makefiles"
cmake --build build
cmake --build build --target install
```

## 2. Build and Install

```
$ mkdir build -p
$ cd build
$ cmake ..
$ make
$ make install
```
