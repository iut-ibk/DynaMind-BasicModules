# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'rain_gui.ui'
#
# Created: Fri Aug 03 14:40:58 2012
#      by: PyQt4 UI code generator 4.8.6
#
# WARNING! All changes made in this file will be lost!

from PyQt4 import QtCore, QtGui

try:
    _fromUtf8 = QtCore.QString.fromUtf8
except AttributeError:
    _fromUtf8 = lambda s: s

class Ui_P8Rain_GUI(object):
    def setupUi(self, P8Rain_GUI):
        P8Rain_GUI.setObjectName(_fromUtf8("P8Rain_GUI"))
        P8Rain_GUI.resize(456, 143)
        P8Rain_GUI.setWindowTitle(QtGui.QApplication.translate("P8Rain_GUI", "Rain", None, QtGui.QApplication.UnicodeUTF8))
        self.verticalLayout = QtGui.QVBoxLayout(P8Rain_GUI)
        self.verticalLayout.setObjectName(_fromUtf8("verticalLayout"))
        self.groupBox = QtGui.QGroupBox(P8Rain_GUI)
        self.groupBox.setTitle(QtGui.QApplication.translate("P8Rain_GUI", "Data setup", None, QtGui.QApplication.UnicodeUTF8))
        self.groupBox.setObjectName(_fromUtf8("groupBox"))
        self.gridLayout = QtGui.QGridLayout(self.groupBox)
        self.gridLayout.setObjectName(_fromUtf8("gridLayout"))
        self.label = QtGui.QLabel(self.groupBox)
        self.label.setText(QtGui.QApplication.translate("P8Rain_GUI", "Rain (netCDF)", None, QtGui.QApplication.UnicodeUTF8))
        self.label.setObjectName(_fromUtf8("label"))
        self.gridLayout.addWidget(self.label, 0, 0, 1, 1)
        self.le_r = QtGui.QLineEdit(self.groupBox)
        self.le_r.setObjectName(_fromUtf8("le_r"))
        self.gridLayout.addWidget(self.le_r, 0, 1, 1, 1)
        self.pb_r = QtGui.QPushButton(self.groupBox)
        self.pb_r.setText(QtGui.QApplication.translate("P8Rain_GUI", "...", None, QtGui.QApplication.UnicodeUTF8))
        self.pb_r.setObjectName(_fromUtf8("pb_r"))
        self.gridLayout.addWidget(self.pb_r, 0, 2, 1, 1)
        self.pb_preview = QtGui.QPushButton(self.groupBox)
        self.pb_preview.setText(QtGui.QApplication.translate("P8Rain_GUI", "Preview", None, QtGui.QApplication.UnicodeUTF8))
        self.pb_preview.setObjectName(_fromUtf8("pb_preview"))
        self.gridLayout.addWidget(self.pb_preview, 1, 2, 1, 1)
        self.verticalLayout.addWidget(self.groupBox)
        self.buttonBox = QtGui.QDialogButtonBox(P8Rain_GUI)
        self.buttonBox.setOrientation(QtCore.Qt.Horizontal)
        self.buttonBox.setStandardButtons(QtGui.QDialogButtonBox.Cancel|QtGui.QDialogButtonBox.Ok)
        self.buttonBox.setObjectName(_fromUtf8("buttonBox"))
        self.verticalLayout.addWidget(self.buttonBox)

        self.retranslateUi(P8Rain_GUI)
        QtCore.QObject.connect(self.buttonBox, QtCore.SIGNAL(_fromUtf8("accepted()")), P8Rain_GUI.accept)
        QtCore.QObject.connect(self.buttonBox, QtCore.SIGNAL(_fromUtf8("rejected()")), P8Rain_GUI.reject)
        QtCore.QMetaObject.connectSlotsByName(P8Rain_GUI)

    def retranslateUi(self, P8Rain_GUI):
        pass

