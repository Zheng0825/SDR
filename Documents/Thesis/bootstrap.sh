#!/usr/bin/env bash

apt-get update

#Everything below here is mentioned as being a prereq for 
#GNURadio except Vim, Vim is just swell. Love Vim.
apt-get --yes --force-yes install python
apt-get --yes --force-yes install vim
apt-get --yes --force-yes install cmake
apt-get --yes --force-yes install python-pip
apt-get --yes --force-yes install libboost-all-dev
apt-get --yes --force-yes install libcppunit-dev
apt-get --yes --force-yes install git
apt-get --yes --force-yes install libfftw3-dev libfftw3-doc
apt-get --yes --force-yes install libgsl0ldbl
apt-get --yes --force-yes install swig
apt-get --yes --force-yes install libsdl2-2.0
apt-get --yes --force-yes install python-wxgtk2.8
apt-get --yes --force-yes install build-essential
apt-get --yes --force-yes install mesa-common-dev
apt-get --yes --force-yes install qt-sdk
apt-get --yes --force-yes install libqwt-dev
apt-get --yes --force-yes install python-numpy python-scipy python-matplotlib ipython ipython-notebook python-pandas python-sympy python-nose
apt-get --yes --force-yes install python-setuptools
apt-get --yes --force-yes install python-gtk2-dev
apt-get --yes --force-yes install doxygen
apt-get --yes --force-yes install doxygen-gui
apt-get --yes --force-yes install graphviz
apt-get --yes --force-yes install git-core
apt-get --yes --force-yes install g++
apt-get --yes --force-yes install python-dev
apt-get --yes --force-yes install pkg-config
apt-get --yes --force-yes install python-cheetah
apt-get --yes --force-yes install python-lxml
apt-get --yes --force-yes install libxi-dev
apt-get --yes --force-yes install python-sip
apt-get --yes --force-yes install libqt4-opengl-dev
apt-get --yes --force-yes install libfontconfig1-dev
apt-get --yes --force-yes install libxrender-dev
apt-get --yes --force-yes install python-sip
apt-get --yes --force-yes install python-sip-dev
apt-get --yes --force-yes install libboost-all-dev
apt-get --yes --force-yes install libusb-1.0-0-dev
apt-get --yes --force-yes install python-mako
apt-get --yes --force-yes install python-docutils
apt-get --yes --force-yes install xinitx
apt-get --yes --force-yes install batctl
apt-get --yes --force-yes install gnome
apt-get --yes --force-yes install xinit

sudo pip install pyside

#These were to allow for qt to work, but seem to be errors
export PYTHONPATH=$PYTHONPATH:/opt/qt/lib/python2.7/dist-package
export PYTHONPATH=$PYTHONPATH:/usr/bin/python


#remove standard packages of UHD if they are there
sudo apt-get --yes --force-yes remove libuhd-dev
sudo apt-get --yes --force-yes remove libuhd003
sudo apt-get --yes --force-yes remove uhd-host

#Install UHD Using Ettus Repos
sudo bash -c 'echo "deb http://files.ettus.com/binaries/uhd/repo/uhd/ubuntu/`lsb_release -cs` `lsb_release -cs` main" > /etc/apt/sources.list.d/ettus.list'
sudo apt-get update
sudo apt-get --yes --force-yes install -t `lsb_release -cs` uhd

git clone --recursive http://git.gnuradio.org/git/gnuradio.git

#Here we build gnuradio from sources
#This will be the longest process
cd gnuradio
mkdir build
cd build
cmake ..
sudo make
sudo make install

sudo ldconfig


##Now we will download gr-mac
cd ..
cd ..
git clone https://github.com/balint256/gr-mac.git
cd gr-mac
mkdir build
cd build
cmake ..
sudo make
sudo make install

sudo ldconfig

##Which also needs gr-foo
cd ..
cd ..
git clone https://github.com/bastibl/gr-foo.git
cd gr-foo
mkdir build
cd build
cmake ..
make
sudo make install
sudo ldconfig


#RTL Driver
cd ..
cd ..
git clone git://git.osmocom.org/rtl-sdr.git
cd rtl-sdr
mkdir build
cd build
cmake ../ -DINSTALL_UDEV_RULES=ON
sudo make
sudo make install
sudo ldconfig

#RTL-GNU Radio Pluging
cd ..
cd ..
git clone git://git.osmocom.org/gr-osmosdr
cd gr-osmosdr/
mkdir build
cd build/
cmake ../
make
sudo make install
sudo ldconfig


cd ..
cd ..
git clone https://github.com/btrowbridge/alfred-ubuntu.git
cd alfred-ubuntu
sudo bash ubuntuAlfredInstall.sh




