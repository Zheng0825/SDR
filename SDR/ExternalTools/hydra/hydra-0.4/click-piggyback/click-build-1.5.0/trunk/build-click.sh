#! /bin/bash
# 
# Copyright (C) 2009  The University of Texas at Austin.
# 
# This file is part of Hydra: A wireless multihop testbed.
# 
# Permission is hereby granted, free of charge, to any person obtaining a
# copy of this software and associated documentation files (the "Software"),
# to deal in the Software without restriction, subject to the conditions
# listed in the Click LICENSE file. These conditions include: you must
# preserve this copyright notice, and you cannot mention the copyright
# holders in advertising related to the Software without their permission.
# The Software is provided WITHOUT ANY WARRANTY, EXPRESS OR IMPLIED. This
# notice is a summary of the Click LICENSE file; the license in that file is
# legally binding.
# 

CLICK_VERSION="1.5.0"
CLICKALLINONE_DIR=$PWD/../..
CLICKINSTALL_DIR=$CLICKALLINONE_DIR/click-install-$CLICK_VERSION

CUR_DIR=$PWD

echo "start installing click-$CLICK_VERSION at $CLICKALLINONE_DIR"
date


echo "unzip click-$CLICK_VERSION at $CLICKALLINONE_DIR"
tar -xzvf ./click-$CLICK_VERSION.tar.gz -C $CLICKALLINONE_DIR
CLICK_DIR=$CLICKALLINONE_DIR/click-$CLICK_VERSION


echo "replace some configuration files for click-$CLICK_VERSION"
rm -f $CLICK_DIR/configure
cp ./configure.$CLICK_VERSION $CLICK_DIR/configure
rm -f $CLICK_DIR/configure.in
cp ./configure.in.$CLICK_VERSION $CLICK_DIR/configure.in
rm -f $CLICK_DIR/etc/pkg-userlevel.mk
cp ./pkg-userlevel.mk.$CLICK_VERSION  $CLICK_DIR/etc/pkg-userlevel.mk
rm -f $CLICK_DIR/elements/grid/airoinfo.{hh,cc}
cp ./airoinfo.hh.$CLICK_VERSION $CLICK_DIR/elements/grid/airoinfo.hh
cp ./airoinfo.cc.$CLICK_VERSION $CLICK_DIR/elements/grid/airoinfo.cc
rm -f $CLICK_DIR/include/click/packet.hh
cp ./packet.hh.$CLICK_VERSION $CLICK_DIR/include/click/packet.hh
rm -f $CLICK_DIR/lib/packet.cc
cp ./packet.cc.$CLICK_VERSION $CLICK_DIR/lib/packet.cc
rm -f $CLICK_DIR/elements/grid/dsrroutetable.cc
cp ./dsrroutetable.cc.$CLICK_VERSION $CLICK_DIR/elements/grid/dsrroutetable.cc


echo "start compiling click-$CLICK_VERSION"
cd $CLICK_DIR

./configure --prefix=$CLICKINSTALL_DIR < configure.in
make elemlist
make
make install

cd $CUR_DIR 

echo "end of compiling click-$CLICK_VERSION"



