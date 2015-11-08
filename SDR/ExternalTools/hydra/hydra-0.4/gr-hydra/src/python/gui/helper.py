#! /usr/bin/env python
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

from hydra import gui

# Parameter needed for determining backend for matplotlib

# set backend for maplotlib
#USE_BACKEND='GTK'
#USE_BACKEND='WX'
USE_BACKEND='WXAgg'
#USE_BACKEND='GTKAgg'
import matplotlib
if   USE_BACKEND=='GTKAgg':
    matplotlib.use(USE_BACKEND)
    from matplotlib.backends.backend_gtkagg import FigureCanvasGTKAgg as FigCanvas
    from matplotlib.backends.backend_gtkagg import NavigationToolbar2GTKAgg as NavToolbar
elif USE_BACKEND=='GTK':
    matplotlib.use(USE_BACKEND)
    from matplotlib.backends.backend_gtk import FigureCanvasGTK as FigCanvas
    from matplotlib.backends.backend_gtk import NavigationToolbar2GTK as NavToolbar
elif USE_BACKEND=='WXAgg':
    matplotlib.use(USE_BACKEND)
    from matplotlib.backends.backend_wxagg import FigureCanvasWxAgg as FigCanvas
    from matplotlib.backends.backend_wxagg import NavigationToolbar2WxAgg as NavToolbar
elif USE_BACKEND=='WX':
    matplotlib.use(USE_BACKEND)
    from matplotlib.backends.backend_wx import Toolbar, FigureManager
    from matplotlib.backends.backend_wx import FigureCanvasWx as FigCanvas
    from matplotlib.backends.backend_wx import NavigationToolbar2Wx as NavToolbar
else:
    raise SystemExit, "[GUI/HELPER]: No valid matplotlib backend specified!"

""" Wrapper for gui.helper """
class GuiHelper(gui.helper):
    pass
