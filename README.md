![Logo of the project](https://raw.githubusercontent.com/jehna/readme-best-practices/master/sample-logo.png)

# pcd-viewer

This project visualizes pcd files on Ubuntu. We could view pcd files either by double-clicking the pcd files or by using command line in terminal. This project is developed using c++ on Ubuntu 16.04 with PCL 1.8.1.

## Installing / Getting started

### Prerequisites
* pcl-1.9.1
* vtk-6.2.0
* glog
The versions of pcl and vtk are mandatory, if you download and install the released the package directly. If you compile and install from source, any versions of pcl and vtk are workable.
### Install
```shell
sudo dpkg -i pcd-viewer-1.0.2-Linux.deb
```
### Uninstall
```shell
sudo dpkg --purge pcd-viewer
```

### Usage

Some projects require initial configuration (e.g. access tokens or keys, `npm i`).
This is the section where you would document those requirements.

## Developing

### Prerequisites
* GNU-5.4.0
* cmake-3.5.1
* pcl-1.9.1
* vtk-6.2.0
* Boost-1.58.0
* glog
### Building

```shell
git clone git@github.com:YoungsonZhao/pcd-viewer.git
cd pcd-viewer/
mkdir build
cd build
cmake ..
make package
```
## Features

* Visualize a pcd file by double-clicking it.
* Visualize a pcd file by inputting its path in terminal using command line. 
* Visualize all the pcd files listed in a file by offering the file's path in terminal using command line.
* Visualize all the pcd files in a directory by offering the directory's path in terminal using command line.
* Support PointXYZ and PointXYZRGB format.

## Links
- Project homepage: https://github.com/YoungsonZhao/pcd-viewer
- Repository: https://github.com/YoungsonZhao/pcd-viewer
- Issue tracker: https://github.com/YoungsonZhao/pcd-viewer/issues
  - In case of sensitive bugs like security vulnerabilities, please contact
    zhaoyongsheng@zju.edu.cn directly instead of using issue tracker. We value your effort
    to improve the security and privacy of this project!
- Related projects:
  - create-projects: https://github.com/YoungsonZhao/scripts
  - PCL: https://github.com/PointCloudLibrary/pcl


## Licensing
The code in this project is licensed under MIT license.
