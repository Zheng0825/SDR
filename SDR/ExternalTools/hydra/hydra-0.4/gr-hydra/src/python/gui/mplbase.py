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

from helper import *
from matplotlib.figure import Figure

##  @class MPLBase
#   @brief Base class to help create matplotlib plots
#
#   Call the appropriate plot method based on the USE_BACKEND
#   configuration in hydra.PyHydra.gui.helper. Call make_plot() method
#   and pass the appropriate arguments to it from the constructor of
#   inherited classes.
class MPLBase(object):
    def __init__(self):
        self.fig = None
        self.canvas = None
        self.toolbar = None

    ##  @brief Call appropriate backend to create fig, canvas, and toolbar.
    def make_plot(self, *args, **kwargs):
        if   USE_BACKEND=='GTKAgg':
            return self.make_plot_gtkagg(*args, **kwargs)
        elif USE_BACKEND=='GTK':
            return self.make_plot_gtk(*args, **kwargs)
        elif USE_BACKEND=='WXAgg':
            return self.make_plot_wxagg(*args, **kwargs)
        elif USE_BACKEND=='WX':
            return self.make_plot_wx(*args, **kwargs)

    def make_plot_gtkagg(self, win, figsize=(3.3,3.3), dpi=100 ):
        # FIXME: FigCanvas and NavToolbar don't instantiate correctly
        self.fig = Figure(figsize=figsize, dpi=dpi)
        self.canvas = FigCanvas(self.fig)
        self.toolbar = NavToolbar(self.canvas, win)
        self.toolbar.Realize()

    def make_plot_gtk(self, win, figsize=(3.3,3.3), dpi=100 ):
        # FIXME: FigCanvas and NavToolbar don't instantiate correctly
        self.fig = Figure(figsize=figsize, dpi=dpi)
        self.canvas = FigCanvas(self.fig, win)
        self.toolbar = NavToolbar(self.canvas)
        self.toolbar.Realize()

    def make_plot_wxagg(self, win, figsize=(3.3,3.3), dpi=100 ):
        self.fig = Figure(figsize=figsize, dpi=dpi)
        self.canvas = FigCanvas(win, -1, self.fig)
        self.toolbar = NavToolbar(self.canvas)
        self.toolbar.Realize()
        
    def make_plot_wx(self, win, figsize=(3.3,3.3), dpi=100 ):
        self.fig = Figure(figsize=figsize, dpi=dpi)
        self.canvas = FigCanvas(win, -1, self.fig)
        self.toolbar = NavToolbar(self.canvas)
        self.toolbar.Realize()

        # Create a figure manager to manage things
        self.figmgr = FigureManager(self.canvas, 1, win)

    def Draw(self):
        if not hasattr(self,'_first_draw'):
            self.canvas.draw()
            self._first_draw = None
            self._canvas_size = self.canvas.GetSize()
        elif self._canvas_size == self.canvas.GetSize():
            #self.canvas.SetSize(self._canvas_size )
            self.canvas.flush_events()
            self.canvas.draw_idle()
        else:
            self._canvas_size = self.canvas.GetSize()
            self.canvas.draw()
