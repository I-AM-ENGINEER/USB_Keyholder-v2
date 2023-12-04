import sys
# from PyQt5 import QtWidgets, QtGui , QtCore
# from PyQt5.QtWidgets import QApplication, QSizePolicy, QDialog, QLabel, QLineEdit, QPushButton, QFileDialog, QHBoxLayout ,QVBoxLayout, QFrame, QToolBox, QScrollArea #, QMainWindow, QWidget,  QSpinBox, QSizePolicy, QMessageBox
# from PyQt5.QtGui import QRegExpValidator
# from PyQt5.QtCore import QRegExp, Qt#, QTimer
from PyQt5.QtWidgets import QApplication#, QHBoxLayout, QWidget, QPushButton
# from PyQt5.QtCore import Qt, QMimeData
# from PyQt5.QtGui import QDrag, QPixmap
# from pathlib import Path
# from PyQt5.QtGui import QPalette
# import os
# import setStyle_Black_Or_White
from ui import MainFrame, DragButton
# import win32api
# import win32con
# import win32gui
from PyQt5.QtGui import QWindow
from serialLib import seriall
# import ctypes
# from ctypes.wintypes import RGB
# from ctypes import byref, c_int
# from ctypes import WinDLL, byref, c_int
# from ctypes.wintypes import RGB

#
        # app.aboutToQuit.connect(self.clicked_close)

class MainWindow(MainFrame):

    def __init__(self, dict_monitor):
        self.countListPass = 0
        super().__init__(dict_monitor)
        self.num_read_pass = 0
        print(self.screenInfo())
        self.ser = seriall(1155, 21156, 9600)
        # print(win32api.MonitorFromWindow())
        # self.mas_btn[5].clicked.connect(self.close_Serial)
        #self.mas_btn[4].clicked.connect(self.read_serial)
        #self.mas_btn[3].clicked.connect(self.start_serial_after_read)
        self.mas_btn[4].clicked.connect(self.startSerial)

        self.ser.SignalOk.connect(lambda: print('SignalOk'))
        self.ser.SignalError.connect(lambda: print('SignalError'))
        self.ser.SignalInvalidComand.connect(lambda: print('SignalInvalidComand'))
        self.ser.countListSignal.connect(lambda count: self.updateCurrentCountListPass(count))
        self.ser.SignalPasswordGet.connect(lambda data: self.setNamePassToDRButt(data))

    def startSerial(self):
        # self.ser = seriall()
        self.mas_btn[0].clicked.connect(lambda: self.ser.write_('CRYPTO_SAVE'))
        self.mas_btn[2].clicked.connect(self.safeOpenPass)
        self.mas_btn[3].clicked.connect(self.addNewPass)
        self.SignalSetHotKey.connect(lambda num, num_hot_key: self.ser.write_('CRYPTO_HOTKEY_PASSWORD_SET_' + str(num) + ':' + str(num_hot_key)))
        # self.SignalMoveButton.connect(lambda num: self.ser.write_('CRYPTO_CMD_PASSWORD_MOVE_' + str(num).encode()))

        self.ser.startSerialAutoConnect()

    def setNamePassToDRButt(self, data):  # установка лейбла на др баттон
        self.getDataFromSerial()
        data = str(data).split(':')

        if (len(data) == 5):
              # 3 элемент login или coment
            self.mas_btn_dr[int(data[0])].setText(data[1])
            self.mas_btn_dr[int(data[0])].login = str(data[1])
            self.mas_btn_dr[int(data[0])].shortName = str(data[2])
            self.mas_btn_dr[int(data[0])].comm = str(data[4])[:-1]
            self.mas_btn_dr[int(data[0])].password = str(data[3])
            self.mas_btn_dr[int(data[0])].num = int(data[0])


    def updateCurrentCountListPass(self, count):
        self.countListPass = count
        self.distructorListPass()
        self.constrctorListPass(count)
        # for i in range(count):
        self.getDataFromSerial()
        # self.ser.write_('CRYPTO_PASSWORD_GET_' + str(2))
    def getDataFromSerial(self):
        if (self.num_read_pass < len(self.mas_btn_dr) + 1) :
            self.ser.write_('CRYPTO_PASSWORD_GET_' + str(self.num_read_pass))
            self.num_read_pass += 1
        else:
            self.num_read_pass = 0
    def addNewPass(self):
        if (self.countListPass == 0):
            self.ser.write_('CRYPTO_PASSWORD_INSERT')
        else:
            self.ser.write_('CRYPTO_PASSWORD_APPEND')
        self.countListPass += 1
        self.mas_btn_dr.append(DragButton("none"))
        self.laout_for_list_pass.addWidget(self.mas_btn_dr[self.countListPass - 1])
        self.setNumOpenPass(self.countListPass - 1)

        # for i in range(count):

    def safeOpenPass(self):
        self.ser.write_('CRYPTO_PASSWORD_SET_' + str(self.num_open_pass) + ':' + self.label_login.text() + ':' + self.label_shortName.text() + ':' + self.label_pass.text() + ':' + self.label_coment.text())

        self.mas_btn_dr[self.num_open_pass].login = self.label_login.text()
        self.mas_btn_dr[self.num_open_pass].password = self.label_pass.text()
        self.mas_btn_dr[self.num_open_pass].comm = self.label_coment.text()
        self.mas_btn_dr[self.num_open_pass].shortName = self.label_shortName.text()

    def close_Serial(self):
        self.ser.closeSerial()
        # self.comboBox.clear()



if __name__ == '__main__':
    app = QApplication(sys.argv)
    dict_monitor = {}
    for screen in app.screens():                # проверяем все мориторы
        window1 = QWindow()
        window1.setScreen(screen)
        dpi = screen.logicalDotsPerInch()
        scale_factor = dpi / 96.0
        dict_monitor[screen.name()] = scale_factor

    window = MainWindow(dict_monitor)           #создаем мейн окно
    window.editSize(window.windowHandle().screen().name())
    # print(window.windowHandle().screen().name())
    window.windowHandle().screenChanged.connect(lambda screen: window.editSize(screen.name()))      # подключаем тригер сманы эрана
    window.windowHandle().windowStateChanged.connect((lambda: print('windowStateChanged')))
    window.windowHandle().raise_()


    sys.exit(app.exec_())


