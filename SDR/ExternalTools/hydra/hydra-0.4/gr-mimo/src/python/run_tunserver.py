#!  /usr/bin/env python
# 
# Copyright (C) 2009  The University of Texas at Austin.
# 
# This file is part of Hydra: A wireless multihop testbed.
# 
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
# 

"""
Run `TunServer` to manage tun/tap interface.

The congfiguration installed from gr-hydra that can be used with `TunServer` can
be found in prefix/share/hydra (i.e. in the install tree).

:author: Ketan Mandke <mandke@ece.utexas.edu>
"""

from hydra.PyHydra import tunserver_mainloop as ts_mainloop

def main():
    ts_mainloop()

if __name__=="__main__":
    main()
