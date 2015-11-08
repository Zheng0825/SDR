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
Database for MAC protocols to log side information; contains `MACDB` class.

Logs side information and maintains timestamps. Also keeps track of a timeout
value used to clean up information in the database.

:author: Ketan Mandke <mandke@ece.utexas.edu>

:var MACDB_VERBOSE:
    Constant enumeration to control verbose thresholds in this file.
  
    Setting the verbose level of a `MACDB` above this threshold will enable the
    corresponding output in this file.
"""

MACDB_VERBOSE = 4

import time

# FIXME: this version only keeps last piece of information for each key
class MACDB(object):
    """
    Database for logging side information to be used by MAC protocols.

    Timed out data will be thrown away if an attempt to access it is made by
    calling `read()`.

    :IVariables:
     * `timeout`: Timeout value for information in database.
    """
    DBTIMEOUT = 100.0
    def __init__(self, timeout=None):
        if timeout is None: timeout = MACDB.DBTIMEOUT
        self.timeout = timeout
        self.__db = {}

    def write(self, key, data):
        """Insert new information into database using a key."""
        ts = time.time()
        self.__db[key] = (ts, data)
        print "--> database contains data for", self.__db.keys()

    def read(self, key):
        """
        Read information out of database using a key.

        :param key: Key to index item in database.

        If timeout has been exceeded since the information was written, this
        method will return None. Also, if the key is not in the data base, this
        method will return None.
        """
        rval = None
        if key in self.__db:
            now = time.time()
            (ts, data) = self.__db[key]
            if (now-ts)>self.timeout:
                # timeout occurred
                del self.__db[key]
            else:
                rval = data
        return rval

    def __contains__(self, key):
        """Dictionary interface to enable 'in' operation."""
        return (key in self.__db)

    def __getitem__(self, key):
        """Dictionary interface that accesses `read()`."""
        return self.read(key)

    def __setitem__(self, key, val):
        """Dictionary interface that accesses `write()`."""
        return self.write(key, val)

    def __delitem__(self, key):
        """Dictionary interface to remove item from database."""
        del self.__db[key]

