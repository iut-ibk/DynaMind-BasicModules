"""
@file
@author  Heiko Kinzel <kinzel@hydro-it.com>
@version 0.1
@section LICENSE

This file is part of dynamind
Copyright (C) 2011  Christian Urich

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
"""

from PyQt4.QtCore import *
from PyQt4.QtGui import *
from rain_gui import *
from pydynamind import *
from numpy import *
from os import  *
import netCDF4
from matplotlib import *
import matplotlib.pyplot as plt
import tempfile



class Rain(Module):
        def __init__(self):
            Module.__init__(self)
            self.createParameter("FileName", FILENAME, "filename")
            self.FileName = ""
#            self.createParameter("Rain",DOUBLE,"Sample Description")
#            self.Rain = 0

            views = []
            self.r = View("SUPERBLOCK", FACE, READ)
            self.r.addAttribute("Rain")
            
            views.append(self.r)
            self.addData("Catchment", views)

    
        def run(self):
            dataflow = self.getData("Catchment")
            catchments = dataflow.getUUIDsOfComponentsInView(self.r)              
            a = netCDF4.Dataset(self.FileName,'r',format='NETCDF4')
            data = a.variables['rain'][:][125][125]
            plt.plot(data)
            filename = "plot"
            filename+= "rain"
            filename+=".png"
            print tempfile.gettempdir()
            plt.savefig(tempfile.gettempdir()+'/'+filename)
            plt.close()
            
            for catch in catchments:                
                c = dataflow.getComponent(catch)  
                rainattr = Attribute("Rain")
                rain = rainattr.getDoubleVector()
                for i in data:
                    rain.append(float(i))                
                rainattr.setDoubleVector(rain)
                c.addAttribute(rainattr)
            for catch in catchments:                
                c = dataflow.getComponent(catch) 
                d = c.getAttribute("Rain").getDoubleVector()
                print "huhu"
                for v in d:                    
                    print v
        def createInputDialog(self):
            form = RainGui(self, QApplication.activeWindow())
            form.show()
            return True 
                    

                         


