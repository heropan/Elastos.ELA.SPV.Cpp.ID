# Elastos.ELA.SPV.Cpp.ID

## Summary
ID Chain.

## Build on Ubuntu/MacOs
### Check the required tools
Make sure your computer have installed the required packages below:
* [git](https://www.git-scm.com/downloads)
* [cmake](https://cmake.org/download)
* [wget](https://www.gnu.org/software/wget)

### Clone source code
Open terminal, go to `/home/xxx/dev`
```shell
$ cd /home/xxx/dev/
$ git clone git@github.com:elastos/Elastos.ELA.SPV.Cpp.ID.git
$ cd /home/xxx/dev/Elastos.ELA.SPV.Cpp.ID
$ git submodule init
$ git submodule update --recursive
```

### Build Elastos.ELA.SPV.Cpp.ID
Create a build directory `cmake-build-ndk-release`
```shell
$ cd /home/xxx/dev/Elastos.ELA.SPV.Cpp.ID
$ mkdir cmake-build-ndk-release
```

Execute cmake command to generate Makefile, and make
```shell
$ cd cmake-build-ndk-release
$ cmake ..
$ make
```


## Build for Android
### Check the required tools
Make sure your computer have installed the required packages below:
* [git](https://www.git-scm.com/downloads)
* [cmake](https://cmake.org/download)
* [wget](https://www.gnu.org/software/wget)
* [ndk](https://developer.android.com/ndk/downloads/)

### Install ndk
`NDK` version: r16+

Unzip to somewhere, for example `/Users/xxx/dev/android-ndk-r16`

Set system environment variable `ANDROID_NDK` to `/Users/xxx/dev/android-ndk-r16`

### Clone source code
Open terminal, go to `/Users/xxx/dev`
```shell
$ cd /Users/xxx/dev/
$ git clone git@github.com:elastos/Elastos.ELA.SPV.Cpp.ID.git
$ cd /Users/xxx/dev/Elastos.ELA.SPV.Cpp.ID
$ git submodule init
$ git submodule update --recursive
```

### Build Elastos.ELA.SPV.Cpp.ID
Create a build directory `cmake-build-ndk-release`
```shell
$ cd /Users/xxx/dev/Elastos.ELA.SPV.Cpp.ID
$ mkdir cmake-build-ndk-release
```

Execute cmake command to generate Makefile, and make
```shell
$ cd cmake-build-ndk-release
$ cmake -DIDCHAIN_FOR_ANDROID=ON ..
$ make
```
