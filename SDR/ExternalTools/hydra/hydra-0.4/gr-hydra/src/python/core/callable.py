#!/usr/bin/env python
#
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
# 

"""
  Class Callable:
    Use this wrapper to enable static member functions.

    Example:
      class A:
        def foo(name):
          print "hello ", str(name)
        foo = Callable(foo)

      A.foo("Alice")
      >>> Alice
"""
class Callable:
  def __init__(self, anycallable):
    self.__call__ = anycallable
