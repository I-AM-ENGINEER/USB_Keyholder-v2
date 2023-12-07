import sys
from PyQt5 import QtWidgets, QtGui, QtCore
from PyQt5.QtWidgets import QVBoxLayout, QFrame, QScrollArea, QSizePolicy, QLineEdit#  QApplication, QDialog, QLabel, QPushButton, QFileDialog, QHBoxLayout , QToolBox , QMainWindow, QWidget,  QSpinBox, QSizePolicy, QMessageBox

from PyQt5.QtWidgets import QApplication, QHBoxLayout, QPushButton#, QWidget
from PyQt5.QtCore import Qt, QMimeData, pyqtSignal
from PyQt5.QtGui import QDrag, QPixmap, QPainter, QDragMoveEvent#, QMouseEvent, QDragLeaveEvent, QStyleHints
import setStyle_Black_Or_White
from PyQt5.QtCore import QIODevice, QTimer, Qt, pyqtSignal, pyqtSlot, QObject

dict_monitor_global = {}

old_screen_skale = 1
theme = int
class DragButton(QPushButton):     #класс драг кнопок
    SignalClicked = pyqtSignal(int)
    def __init__(self,l = '1',*args, **kargs):      #создам переменные кнопки и делаем emit сигнала при нажатии
        # self.setText(label)
        super().__init__(*args, **kargs)
        # self.setMouseTracking(True)
        self.setObjectName(l)

        self.setText(l)

        self.password = ''
        self.comm = ''
        self.shortName = ''
        self.login = ''
        self.num = int
        self.hotKey = ''
        self.clicked.connect(lambda : self.SignalClicked.emit(self.num))

    def mouseMoveEvent(self, e):          # ивент движения обрабатываеммый укнопкой - рисует знак кнопки под курсором
        if e.buttons() == Qt.LeftButton:
            # x = e.x()
            # y = e.y()
            # print(f"Mouse position: ({x}, {y})")
            drag = QDrag(self)
            mime_data = QMimeData()
            mime_data.setText(self.text())
            drag.setMimeData(mime_data)
            pixmap = QPixmap(self.size())
            painter = QPainter(pixmap)
            painter.drawPixmap(self.rect(), self.grab())
            painter.end()
            drag.setPixmap(pixmap)
            drag.setHotSpot(e.pos())
            drag.exec_(Qt.CopyAction | Qt.MoveAction)

class MainFrame(QtWidgets.QMainWindow):
    SignalMoveButton = pyqtSignal(str)
    SignalSetHotKey = pyqtSignal(str, str)
    # SignalMoveButton = pyqtSignal(int)

    def __init__(self, dict_monitor):
        # qwer =  QStyleHints()
        # qwer.useHoverEffectsChanged.connect(lambda : print(qwer.useHoverEffects()))
        super(MainFrame, self).__init__()
        # self.setWindowFlags(QtCore.Qt.FramelessWindowHint)
        self.setMouseTracking(True)
        global dict_monitor_global
        global theme
        dict_monitor_global = dict_monitor

        # cssStyle, themeBW = setStyle_Black_Or_White.setStyleBW()
        # self.setStyleSheet(cssStyle)
        # print(themeBW)
        # theme = themeBW
        self.updateStyleUI()
        frame1 = QFrame()
        self.setCentralWidget(frame1)

        main_laout = QHBoxLayout()
        frame1.setLayout(main_laout)
        self.frame_for_edit_pass = QFrame()
        main_laout.addWidget(self.frame_for_edit_pass)

        self.frame_for_list_pass = QFrame()
        self.comboBox_HotKey = QtWidgets.QComboBox()
        list1 = ['none', '1', '2', '3', '4', '5', '6', '7', '8']
        self.comboBox_HotKey.addItems(list1)
        self.comboBox_HotKey.setParent(self.frame_for_edit_pass)
        self.comboBox_HotKey.currentIndexChanged.connect(self.setNumHotKey)

        self.laout_for_list_pass = QVBoxLayout()
        self.frame_for_list_pass.setLayout(self.laout_for_list_pass)

        self.scroll = QScrollArea()
        self.scroll.setHorizontalScrollBarPolicy(Qt.ScrollBarAlwaysOff)
        self.scroll.setSizePolicy(QSizePolicy.Fixed, QSizePolicy.MinimumExpanding)

        self.scroll.setWidget(self.frame_for_list_pass)
        self.scroll.setWidgetResizable(True)
        main_laout.addWidget(self.scroll)

        self.frame_for_buttons = QFrame()
        main_laout.addWidget(self.frame_for_buttons)
        # laout_for_buttons = QVBoxLayout()
        # self.frame_for_buttons.setLayout(laout_for_buttons)
        self.setAcceptDrops(True)
        self.mas_btn_dr = []
        self.num_open_pass = int


        self.mas_btn  = []
        for i in range(5):
            self.mas_btn.append(QPushButton(self.frame_for_buttons))
            # laout_for_buttons.addWidget(self.mas_btn[i])
            self.mas_btn[i].setText(str(i))
        self.mas_btn[0].setText('safe')
        self.mas_btn[2].setText('wrine pass')
        self.mas_btn[3].setText('new')
        self.mas_btn[4].setText('start')
        # self.mas_btn[3].setText('start serial')
        # self.mas_btn[2].setText('auto start')
        self.label_login = QLineEdit()
        self.label_pass = QLineEdit()
        self.label_coment = QLineEdit()
        self.label_shortName = QLineEdit()
        self.label_login.setPlaceholderText('login')
        self.label_pass.setPlaceholderText('pass')
        self.label_coment.setPlaceholderText('coment')
        self.label_shortName.setPlaceholderText('short name')
        self.label_login.setParent(self.frame_for_edit_pass)
        self.label_pass.setParent(self.frame_for_edit_pass)
        self.label_coment.setParent(self.frame_for_edit_pass)
        self.label_shortName.setParent(self.frame_for_edit_pass)
        # self.comboBox = QtWidgets.QComboBox(self.frame_for_buttons)
        # laout_for_buttons.addWidget(self.comboBox)
        self.timer = QTimer()
        self.timer.timeout.connect(self.updateStyleUI)
        self.timer.setInterval(2500)
        self.timer.start()
        self.constrctorListPass(15)
        self.show()
    def updateStyleUI(self):
        global theme
        cssStyle, themeBW = setStyle_Black_Or_White.setStyleBW()
        if theme != themeBW:
            self.setStyleSheet(cssStyle)
            # pass
        # if self.flag_serial_run == 0:
        #     self.SignalUpDateSerial.emit()
    def setNumOpenPass(self, num):
        print(num)
        self.num_open_pass = num
        self.label_shortName.setText(self.mas_btn_dr[num].shortName)
        self.label_login.setText(self.mas_btn_dr[num].login)
        self.label_pass.setText(self.mas_btn_dr[num].password)
        self.label_coment.setText(self.mas_btn_dr[num].comm)
        self.comboBox_HotKey.setCurrentIndex(0)
        self.editSize(constrctorTable=True)


    # def showCurrentCountListPass(self, Count):
    #     for l in range(len(self.mas_btn_dr), Count):
    #         self.mas_btn_dr.append(DragButton(str(l)))
    #         self.laout_for_list_pass.addWidget(self.mas_btn_dr[l])
    def distructorListPass(self):
        print('UI: distructor list dr button')
        for _ in range(len(self.mas_btn_dr)):
            self.laout_for_list_pass.removeWidget(self.mas_btn_dr[0])
            self.mas_btn_dr[0].deleteLater()
            del self.mas_btn_dr[0]
    def constrctorListPass(self, len_):
        print('UI: constrctor list dr button')
        for l in range(len_):
            self.mas_btn_dr.append(DragButton(str(l)))
            self.laout_for_list_pass.addWidget(self.mas_btn_dr[l])
            self.mas_btn_dr[l].SignalClicked.connect(lambda num: self.setNumOpenPass(num))
        self.editSize(constrctorTable=True)


    def screenInfo(self):
        pass
    def setNumHotKey(self):
        if self.comboBox_HotKey.currentText() != 'none':
            self.SignalSetHotKey.emit(str(int(self.comboBox_HotKey.currentText()) - 1),str(self.num_open_pass))

    def editSize(self, set_screen_name = '', **kwargs): # отрисовка окна с учетем размера экрана
        global old_screen_skale
        if kwargs.get('constrctorTable', False):
            scale = old_screen_skale
        else:
            global dict_monitor_global
            scale = dict_monitor_global[set_screen_name]
            old_screen_skale = scale
        print('new window scale: ' + str(scale))
        # scale = 1.75

        font = QtGui.QFont()
        font.setFamily("Yu Gothic UI Semibold")
        font.setPointSize(int(12))
        self.setBaseSize(int(620 * scale), int(320 * scale))
        # self.setFixedWidth(int(620 * scale))
        for i in range(5):
            self.mas_btn[i].setFixedSize(int(scale * 100), int(scale * 40))
            self.mas_btn[i].setFont(font)
        for i in range(len(self.mas_btn_dr)):
            self.mas_btn_dr[i].setFixedSize(int(scale * 200) , int(scale * 40))
            self.mas_btn_dr[i].setFont(font)
            self.mas_btn_dr[i].num = i
        # self.comboBox.setFixedSize(int(scale * 100), int(scale * 40))
        # self.comboBox.move(int(scale * 0), int(scale * 300))
        # self.comboBox.setFont(font)
        self.label_login.setFixedSize(int(scale * 140), int(scale * 40))
        self.label_login.setFont(font)
        self.label_pass.setFixedSize(int(scale * 140), int(scale * 40))
        self.label_pass.setFont(font)
        self.label_coment.setFixedSize(int(scale * 140), int(scale * 40))
        self.label_coment.setFont(font)
        self.label_shortName.setFixedSize(int(scale * 140), int(scale * 40))
        self.label_shortName.setFont(font)
        self.comboBox_HotKey.setFixedSize(int(scale * 100), int(scale * 40))
        self.comboBox_HotKey.setFont(font)

        self.scroll.setFixedWidth(int((240 * scale)))
        self.frame_for_list_pass.setFixedWidth(int(scale * 220))
        self.frame_for_list_pass.setFixedHeight(int(scale * 50 * len(self.mas_btn_dr)) + 5)

        self.frame_for_list_pass.setFont(font)

        self.frame_for_buttons.setFixedWidth(int(scale * 160))
        self.frame_for_edit_pass.setFixedWidth(int(scale * 160))
        self.scroll.setFont(font)
        # self.frame_for_buttons.setFixedHeight(int(scale * 360))
        # self.frame_for_buttons.setFixedWidth(int(scale * 160))

        # self.label_login.parent(self.frame_for_edit_pass)

        self.window
        self.label_login.move(int(scale * 0), int(scale * 0))
        self.label_pass.move(int(scale * 0), int(scale * 65))
        self.label_coment.move(int(scale * 0), int(scale * 130))
        self.label_shortName.move(int(scale * 0), int(scale * 195))
        self.comboBox_HotKey.move(int(scale * 0), int(scale * 260))
        for i in range(5):
            self.mas_btn[i].move(int(scale * 0), int(scale * 50 * i))

        # self.comboBox.setParent(None)
        # self.frame_for_list_pass.setvisible(str(self.frame_for_list_pass.visibleRegion().boundingRect()).split('(')[1].strip(')','').split(',')[:2])
        self.show()
        # print(str(self.frame_for_list_pass.visibleRegion().boundingRect()).split('(')[1].split(',')[:2])
        # self.mas_btn_dr[0].move(int(scale * 15),int(scale * 15))
        # print(self.mas_btn_dr[0].pos(), self.mas_btn_dr[0].size())

    def dragEnterEvent(self, e):
        e.accept()

    def dragMoveEvent(self, e: QDragMoveEvent):
        global theme
        # print(e.pos().y())
        pos = e.pos()
        widget = e.source()
        # print(widget.objectName())
        if theme == 0:
            widget.setStyleSheet('QPushButton{'
                                 'color: #323232;'
                                 'background-color: #323232;'
                                 'border-width: 1px;'
                                 'border-color: #323232;'
                                 'border-style: solid;'
                                 'padding: 5px;'
                                 'border-radius: 0px;'
                                 'outline: none;}')
        if theme == 1:
            widget.setStyleSheet('QPushButton{'
                                 'color: #f5f5f5;'
                                 'background-color: #f5f5f5;'
                                 'border-width: 1px;'
                                 'border-color: #f5f5f5;'
                                 'border-style: solid;'
                                 'padding: 5px;'
                                 'border-radius: 0px;'
                                 'outline: none;}')

        visibleRegion = str(self.frame_for_list_pass.visibleRegion().boundingRect()).split('(')[1].split(',')[:2]
        if pos.y() < 57:
            position = 60
        else:
            position = pos.y()
        for n in range(self.laout_for_list_pass.count()):
            w = self.laout_for_list_pass.itemAt(n).widget()
            # w.setStyleSheet('color: #3C3C3C;')
            if (position + int(visibleRegion[1])) < w.y() + w.size().height() // 2:
                self.laout_for_list_pass.insertWidget(n - 1, widget)
                break
        e.accept()

    def dropEvent(self, e):
        print('dropEvent')
        pos = e.pos()
        widget = e.source()
        widget.setStyleSheet('')
        visibleRegion = str(self.frame_for_list_pass.visibleRegion().boundingRect()).split('(')[1].split(',')[:2]
        if pos.y() < 57:
            position = 60
        else:
            position = pos.y()
        num = int
        for n in range(self.laout_for_list_pass.count()):

            num = n
            w = self.laout_for_list_pass.itemAt(n).widget()
            if  (position + int(visibleRegion[1]) ) < w.y() + w.size().height() // 2:
                self.laout_for_list_pass.insertWidget(n-1, widget)
                break
        e.accept()
        num = str(widget.num) + '|' + str(num - 1)
        self.SignalMoveButton.emit(num)
        print(num)

        for i in range(len(self.mas_btn_dr)):
            self.laout_for_list_pass.itemAt(i).widget().num = i

    def dragLeaveEvent(self,e):

        for i in range(len(self.mas_btn_dr)):
            self.mas_btn_dr[i].setStyleSheet('')





