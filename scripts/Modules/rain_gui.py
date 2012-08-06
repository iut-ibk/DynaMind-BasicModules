from PyQt4 import QtCore, QtGui
from pydynamind import *
from Ui_Rain_Dialog import Ui_P8Rain_GUI
import netCDF4
from matplotlib import *
import matplotlib.pyplot as plt

class RainGui(QtGui.QDialog):
    def __init__(self, m, parent=None):
        self.module = Module
        self.module = m
        QtGui.QDialog.__init__(self, parent)
        self.ui = Ui_P8Rain_GUI()
        self.ui.setupUi(self)
        self.ui.le_r.setText(self.module.getParameterAsString("FileName"))
        QtCore.QObject.connect(self.ui.pb_preview, QtCore.SIGNAL("released()"), self.preview)
        QtCore.QObject.connect(self.ui.buttonBox, QtCore.SIGNAL("accepted()"), self.save_values)
        QtCore.QObject.connect(self.ui.pb_r, QtCore.SIGNAL("released()"), self.load)
        
    def preview(self):    
        filename = str(self.ui.le_r.text())
        a = netCDF4.Dataset(filename,'r',format='NETCDF4')
        data = a.variables['rain'][:][125][125]
        plt.plot(data)
        plt.show()
        a.close()

    def save_values(self):
        filename = str(self.ui.le_r.text())
        self.module.setParameterValue("FileName", filename)

    def load(self):
        filename = QtGui.QFileDialog.getOpenFileName(self, "Open rain file", "Open new file", self.tr("Text Files (*.nc)"))
        self.ui.le_r.setText(filename)
        self.save_values


