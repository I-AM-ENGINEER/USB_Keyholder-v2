from PyQt5.QtSerialPort import QSerialPort, QSerialPortInfo
from PyQt5.QtCore import QIODevice, QTimer, Qt, pyqtSignal, pyqtSlot, QObject
import time
# from main import MainWindow
# CRYPTO_CMD_PASSWORD_GET_COUNT			"PASSWORD_COUNT"
# CRYPTO_CMD_PASSWORD_GET						"PASSWORD_GET_"
# CRYPTO_CMD_PASSWORD_SET						"PASSWORD_SET_"						// PASSWORD_INSERT_idx:login \t password \t comment
# CRYPTO_CMD_PASSWORD_REMOVE				"PASSWORD_REMOVE_"				// PASSWORD_MOVE_idx Remove with shift
# CRYPTO_CMD_PASSWORD_APPEND				"PASSWORD_APPEND"				// PASSWORD_INSERT_idx	// create new password
# CRYPTO_CMD_PASSWORD_INSERT				"PASSWORD_INSERT"				// PASSWORD_INSERT_idx	// create new password
# CRYPTO_CMD_PASSWORD_MOVE					"PASSWORD_MOVE_"					// PASSWORD_MOVE:from:to
# ##CRYPTO_CMD_PASSWORD_SWAP					"PASSWORD_SWAP_"					// PASSWORD_SWAP:firs:second
# CRYPTO_CMD_SAVE										"SAVE"
# CRYPTO_CMD_HOTKEY_PASSWORD_SET		"HOTKEY_PASSWORD_SET_"		// HOTKEY_PASSWORD_SET_hotkey:password

# CRYPTO_REPLY_GET_PASSWORD					"PASSWORD_"

# CRYPTO_REPLY_PASSWORD_APPEND			"NEW_PASSWORD_IDX:"
# CRYPTO_REPLY_OK										"OK"
# CRYPTO_REPLY_ERROR								"ERROR"
#  CRYPTO_REPLY_INVALID_COMMAND			"INVALID_COMMAND"
class seriall(QObject):
    productIdentifier = 0
    vendorIdentifier = 0
    BaudRate = 0
    SignalReadPort = pyqtSignal(list)
    countListSignal = pyqtSignal(int)
    SignalOk = pyqtSignal()
    SignalError = pyqtSignal()
    SignalInvalidComand = pyqtSignal()
    SignalPasswordGet = pyqtSignal(str)
    SignalSerialStartOk = pyqtSignal()

    def __init__(self, vendorIdentifier_ = 0, productIdentifier_ = 0, BaudRate_ = 0):
        super().__init__()
        global vendorIdentifier
        global productIdentifier
        global BaudRate
        BaudRate = BaudRate_
        vendorIdentifier = vendorIdentifier_
        productIdentifier = productIdentifier_
        self.serial = QSerialPort()
    def readPort(self):                            #запускаем сериал и счтываем порты(записываем в comboBox)
        ports = QSerialPortInfo().availablePorts()
        ports_name = [port.portName() for port in ports]
        self.SignalReadPort.emit(ports_name)
        return ports

    def startSerialAutoConnect(self):                            #запускаем сериал и авто коннект
        portList = self.readPort()
        for port in portList:

            if port.productIdentifier() == productIdentifier and port.vendorIdentifier() == vendorIdentifier:
                self.openSerialAndStartMessage(port.portName())


    def openSerialAndStartMessage(self, currertPort):                   #начинаем общение с сериалом
        print(currertPort, BaudRate)
        self.serial.setBaudRate(BaudRate)
        self.serial.setPortName(currertPort)
        self.serial.open(QIODevice.ReadWrite)
        self.serial.readyRead.connect(self.readInpurAndOutput)
        self.SignalSerialStartOk.emit()
        self.write_("CRYPTO_PASSWORD_COUNT")

    def readInpurAndOutput(self):                        #оброботчик всех приходящих пакетов

        inputStr = self.serial.readLine()

        inputStr = str(inputStr, 'utf-8')
        print('serial resive: ' + inputStr)

        if inputStr.find("count: ") != -1:
            self.countListSignal.emit(int(inputStr[7:]))

        elif inputStr.find("OK:") != -1:
            self.SignalOk.emit()
            # self.readinputStr_OK(inputStr)

        elif (inputStr.find("ERROR") != -1):
            self.SignalError.emit()
            # self.readinputStr_ERROR(inputStr)

        elif inputStr.find("PASSWORD_") != -1:
            self.SignalPasswordGet.emit(inputStr[9:])
            # self.readinputStr_PASSWORD_(inputStr)

        elif inputStr.find("NEW_PASSWORD_IDX") != -1:
            self.readinputStr_NEW_PASSWORD_IDX(inputStr)

        elif inputStr.find("INVALID_COMMAND") != -1:
            self.SignalInvalidComand.emit()

    def readinputStr_PASSWORD_(self, inputStr):
        pass
    def readinputStr_NEW_PASSWORD_IDX(self, inputStr):
        pass

    def closeSerial(self):                 #закрываем serial и стираем все состояния кнопок в приложении
        self.serial.close()

    def write_(self, str_):
        print('serial send : ' + str_)

        if self.BaudRate > 6000:
            self.serial.write(str(str_).encode())
        else:
            start_time = time.time()
            while time.time() - start_time < 0.5:
                self.serial.write(str(str_).encode())
                time.sleep(0.05)
            time.sleep(0.3)
            self.serial.write(str(str_).encode())

