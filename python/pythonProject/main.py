import sys
from PyQt5 import QtCore, QtGui, QtWidgets
from PyQt5.QtGui import QIcon
from ui import Ui_MainWindow
import serial



class pogram(QtWidgets.QMainWindow):
    def __init__(self):
        super(pogram, self).__init__()
        self.ui = Ui_MainWindow()
        self.ui.setupUi(self)
        self.init_UI()
    def init_UI(self):
        self.setWindowTitle('artem lox')

        self.ui.linePass.setPlaceholderText('вводим')
        self.ui.lineLogin.setPlaceholderText('вводим')
        self.ui.lineComment.setPlaceholderText('вводим')
        self.ui.pushButton.clicked.connect(self.WriteUsbFl)

        self.ReadUsbFlLogins()

    def ReadUsbFlLogins(self):
        for i in range(20):
            global num
            num = i
            ser.write(b'b\n')
            q = ser.readline()

            text = str(num) + '\n'
            ser.write(text.encode('utf-8'))

            w = ser.readline()
            e = ser.readline()
            r = ser.readline()
            text = "%s" % w.decode('utf-8')[0:-1]

            self.ui.listWidget.addItem(text)


        self.ui.listWidget.itemDoubleClicked.connect(self.ReadUsbFlLodPassCom)

    def ReadUsbFlLodPassCom(self):
        num = self.ui.listWidget.currentRow()
        #passFL = self.ui.linePass.text()
        #loginFL = self.ui.lineLogin.text()
        #commentFL = self.ui.lineComment.text()

        ser.write(b'b\n')
        q = ser.readline()

        text = str(num) + '\n'
        ser.write(text.encode('utf-8'))

        w = ser.readline()
        e = ser.readline()
        r = ser.readline()
        text = "%s" % w.decode('utf-8')[0:-1]
        self.ui.lineLogin.setText(text)
        text = "%s" % e.decode('utf-8')[0:-1]
        self.ui.linePass.setText(text)
        text = "%s" % r.decode('utf-8')[0:-1]
        self.ui.lineComment.setText(text)
    def WriteUsbFl(self):
        print('доделать')


ser = serial.Serial('COM3', 38400, timeout=1)
app = QtWidgets.QApplication([])
application = pogram()
application.show()

sys.exit(app.exec())