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
CLICKHYDRA_DIR=$CLICKALLINONE_DIR/click-hydra-$CLICK_VERSION

CUR_DIR=$PWD

echo "start compiling click-hydra-$CLICK_VERSION"
cd $CLICKHYDRA_DIR/trunk

./configure --prefix=$CLICKINSTALL_DIR
make elemlist
make package
make install

cd $CUR_DIR 

echo "end of compiling click-hydra-$CLICK_VERSION"



