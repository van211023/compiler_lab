from PyQt5 import QtCore, QtGui
from PyQt5 import QtWidgets
try:
    _fromUtf8 = QtCore.QString.fromUtf8
except AttributeError:
    _fromUtf8 = lambda s: s

class Ui_MainWindow(object):

    def setupUi(self, MainWindow):
        MainWindow.setObjectName(_fromUtf8('MainWindow'))
        MainWindow.resize(800, 600)
        #sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Fixed, QtGui.QSizePolicy.Fixed)
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Fixed, QtWidgets.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(MainWindow.sizePolicy().hasHeightForWidth())
        MainWindow.setSizePolicy(sizePolicy)
        MainWindow.setMinimumSize(QtCore.QSize(800, 600))
        MainWindow.setMaximumSize(QtCore.QSize(800, 600))
        icon = QtGui.QIcon()
        icon.addPixmap(QtGui.QPixmap(_fromUtf8(':/icon.png')), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        MainWindow.setWindowIcon(icon)
        #self.centralwidget = QtGui.QWidget(MainWindow)
        self.centralwidget = QtWidgets.QWidget(MainWindow)
        self.centralwidget.setObjectName(_fromUtf8('centralwidget'))
        #self.layoutWidget = QtGui.QWidget(self.centralwidget)
        self.layoutWidget = QtWidgets.QWidget(self.centralwidget)
        self.layoutWidget.setGeometry(QtCore.QRect(20, 10, 761, 490))
        self.layoutWidget.setObjectName(_fromUtf8('layoutWidget'))
        self.horizontalLayout = QtWidgets.QHBoxLayout(self.layoutWidget)
        #self.horizontalLayout.setMargin(0)
        self.horizontalLayout.setObjectName(_fromUtf8('horizontalLayout'))
        self.verticalLayout = QtWidgets.QVBoxLayout()
        self.verticalLayout.setObjectName(_fromUtf8('verticalLayout'))
        self.label = QtWidgets.QLabel(self.layoutWidget)
        font = QtGui.QFont()
        font.setFamily(_fromUtf8('Verdana'))
        font.setBold(True)
        font.setWeight(75)
        self.label.setFont(font)
        self.label.setObjectName(_fromUtf8('label'))
        self.verticalLayout.addWidget(self.label)
        self.codeList = QtWidgets.QListWidget(self.layoutWidget)
        self.codeList.setEnabled(True)
        font = QtGui.QFont()
        font.setFamily(_fromUtf8('Arial'))
        self.codeList.setFont(font)
        self.codeList.setEditTriggers(QtWidgets.QAbstractItemView.NoEditTriggers)
        self.codeList.setSelectionBehavior(QtWidgets.QAbstractItemView.SelectRows)
        self.codeList.setObjectName(_fromUtf8('codeList'))
        self.verticalLayout.addWidget(self.codeList)
        self.horizontalLayout.addLayout(self.verticalLayout)
        spacerItem = QtWidgets.QSpacerItem(40, 20, QtWidgets.QSizePolicy.Preferred, QtWidgets.QSizePolicy.Minimum)
        self.horizontalLayout.addItem(spacerItem)
        self.verticalLayout_2 = QtWidgets.QVBoxLayout()
        self.verticalLayout_2.setObjectName(_fromUtf8('verticalLayout_2'))
        self.label_2 = QtWidgets.QLabel(self.layoutWidget)
        font = QtGui.QFont()
        font.setFamily(_fromUtf8('Verdana'))
        font.setBold(True)
        font.setWeight(75)
        self.label_2.setFont(font)
        self.label_2.setObjectName(_fromUtf8('label_2'))
        self.verticalLayout_2.addWidget(self.label_2)
        self.watchTable = QtWidgets.QTableWidget(self.layoutWidget)
        font = QtGui.QFont()
        font.setFamily(_fromUtf8('Arial'))
        self.watchTable.setFont(font)
        self.watchTable.setEditTriggers(QtWidgets.QAbstractItemView.NoEditTriggers)
        self.watchTable.setSelectionMode(QtWidgets.QAbstractItemView.SingleSelection)
        self.watchTable.setSelectionBehavior(QtWidgets.QAbstractItemView.SelectRows)
        self.watchTable.setHorizontalScrollMode(QtWidgets.QAbstractItemView.ScrollPerPixel)
        self.watchTable.setObjectName(_fromUtf8('watchTable'))
        self.watchTable.setColumnCount(2)
        self.watchTable.setRowCount(0)
        item = QtWidgets.QTableWidgetItem()
        self.watchTable.setHorizontalHeaderItem(0, item)
        item = QtWidgets.QTableWidgetItem()
        self.watchTable.setHorizontalHeaderItem(1, item)
        self.verticalLayout_2.addWidget(self.watchTable)
        spacerItem1 = QtWidgets.QSpacerItem(20, 40, QtWidgets.QSizePolicy.Minimum, QtWidgets.QSizePolicy.Preferred)
        self.verticalLayout_2.addItem(spacerItem1)
        self.label_3 = QtWidgets.QLabel(self.layoutWidget)
        font = QtGui.QFont()
        font.setFamily(_fromUtf8('Verdana'))
        font.setBold(True)
        font.setWeight(75)
        self.label_3.setFont(font)
        self.label_3.setObjectName(_fromUtf8('label_3'))
        self.verticalLayout_2.addWidget(self.label_3)
        self.console = QtWidgets.QTextEdit(self.layoutWidget)
        font = QtGui.QFont()
        font.setFamily(_fromUtf8('Serif'))
        self.console.setFont(font)
        self.console.setReadOnly(True)
        self.console.setObjectName(_fromUtf8('console'))
        self.verticalLayout_2.addWidget(self.console)
        self.horizontalLayout.addLayout(self.verticalLayout_2)
        MainWindow.setCentralWidget(self.centralwidget)
        self.menubar = QtWidgets.QMenuBar(MainWindow)
        self.menubar.setGeometry(QtCore.QRect(0, 0, 800, 25))
        self.menubar.setObjectName(_fromUtf8('menubar'))
        self.menuFile = QtWidgets.QMenu(self.menubar)
        self.menuFile.setObjectName(_fromUtf8('menuFile'))
        self.menuRun = QtWidgets.QMenu(self.menubar)
        self.menuRun.setObjectName(_fromUtf8('menuRun'))
        self.menuAbout = QtWidgets.QMenu(self.menubar)
        self.menuAbout.setObjectName(_fromUtf8('menuAbout'))
        MainWindow.setMenuBar(self.menubar)
        self.statusbar = QtWidgets.QStatusBar(MainWindow)
        self.statusbar.setObjectName(_fromUtf8('statusbar'))
        MainWindow.setStatusBar(self.statusbar)
        self.toolBar = QtWidgets.QToolBar(MainWindow)
        self.toolBar.setObjectName(_fromUtf8('toolBar'))
        MainWindow.addToolBar(QtCore.Qt.TopToolBarArea, self.toolBar)
        self.action_Open = QtWidgets.QAction(MainWindow)
        icon1 = QtGui.QIcon()
        icon1.addPixmap(QtGui.QPixmap(_fromUtf8(':/filenew.png')), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.action_Open.setIcon(icon1)
        self.action_Open.setObjectName(_fromUtf8('action_Open'))
        self.actionQuit = QtWidgets.QAction(MainWindow)
        icon2 = QtGui.QIcon()
        icon2.addPixmap(QtGui.QPixmap(_fromUtf8(':/filequit.png')), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.actionQuit.setIcon(icon2)
        self.actionQuit.setObjectName(_fromUtf8('actionQuit'))
        self.actionRun = QtWidgets.QAction(MainWindow)
        self.actionRun.setEnabled(True)
        icon3 = QtGui.QIcon()
        icon3.addPixmap(QtGui.QPixmap(_fromUtf8(':/run.png')), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.actionRun.setIcon(icon3)
        self.actionRun.setObjectName(_fromUtf8('actionRun'))
        self.actionStop = QtWidgets.QAction(MainWindow)
        self.actionStop.setEnabled(True)
        icon4 = QtGui.QIcon()
        icon4.addPixmap(QtGui.QPixmap(_fromUtf8(':/stop.png')), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.actionStop.setIcon(icon4)
        self.actionStop.setObjectName(_fromUtf8('actionStop'))
        self.actionStep = QtWidgets.QAction(MainWindow)
        self.actionStep.setEnabled(True)
        icon5 = QtGui.QIcon()
        icon5.addPixmap(QtGui.QPixmap(_fromUtf8(':/step.png')), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.actionStep.setIcon(icon5)
        self.actionStep.setObjectName(_fromUtf8('actionStep'))
        self.actionAbout = QtWidgets.QAction(MainWindow)
        self.actionAbout.setIcon(icon)
        self.actionAbout.setObjectName(_fromUtf8('actionAbout'))
        self.menuFile.addAction(self.action_Open)
        self.menuFile.addSeparator()
        self.menuFile.addAction(self.actionQuit)
        self.menuRun.addAction(self.actionRun)
        self.menuRun.addAction(self.actionStop)
        self.menuRun.addAction(self.actionStep)
        self.menuAbout.addAction(self.actionAbout)
        self.menubar.addAction(self.menuFile.menuAction())
        self.menubar.addAction(self.menuRun.menuAction())
        self.menubar.addAction(self.menuAbout.menuAction())
        self.toolBar.addAction(self.action_Open)
        self.toolBar.addSeparator()
        self.toolBar.addAction(self.actionRun)
        self.toolBar.addAction(self.actionStep)
        self.toolBar.addAction(self.actionStop)
        self.toolBar.addSeparator()
        self.toolBar.addAction(self.actionQuit)
        self.retranslateUi(MainWindow)
        QtCore.QMetaObject.connectSlotsByName(MainWindow)
        MainWindow.setTabOrder(self.codeList, self.watchTable)
        MainWindow.setTabOrder(self.watchTable, self.console)

    def retranslateUi(self, MainWindow):
        #MainWindow.setWindowTitle(QtWidgets.QApplication.translate('MainWindow', 'IR Simulator', None, QtWidgets.QApplication.UnicodeUTF8))
        MainWindow.setWindowTitle(QtWidgets.QApplication.translate('MainWindow', 'IR Simulator', None))
        #self.label.setText(QtWidgets.QApplication.translate('MainWindow', 'Code', None, QtWidgets.QApplication.UnicodeUTF8))
        self.label.setText(QtWidgets.QApplication.translate('MainWindow', 'Code', None))
        #self.label_2.setText(QtWidgets.QApplication.translate('MainWindow', 'Watch', None, QtWidgets.QApplication.UnicodeUTF8))
        self.label_2.setText(QtWidgets.QApplication.translate('MainWindow', 'Watch', None))
        item = self.watchTable.horizontalHeaderItem(0)
        #item.setText(QtWidgets.QApplication.translate('MainWindow', 'Variable', None, QtWidgets.QApplication.UnicodeUTF8))
        item.setText(QtWidgets.QApplication.translate('MainWindow', 'Variable', None))
        item = self.watchTable.horizontalHeaderItem(1)
        item.setText(QtWidgets.QApplication.translate('MainWindow', 'Value', None))
        self.label_3.setText(QtWidgets.QApplication.translate('MainWindow', 'Console', None))
        self.menuFile.setTitle(QtWidgets.QApplication.translate('MainWindow', 'File', None))
        self.menuRun.setTitle(QtWidgets.QApplication.translate('MainWindow', 'Run', None))
        self.menuAbout.setTitle(QtWidgets.QApplication.translate('MainWindow', 'About', None))
        self.toolBar.setWindowTitle(QtWidgets.QApplication.translate('MainWindow', 'toolBar', None))
        self.action_Open.setText(QtWidgets.QApplication.translate('MainWindow', '&Open...', None))
        self.action_Open.setStatusTip(QtWidgets.QApplication.translate('MainWindow', 'Open an existing IR file', None))
        self.action_Open.setWhatsThis(QtWidgets.QApplication.translate('MainWindow', 'Open', None))
        self.action_Open.setShortcut(QtWidgets.QApplication.translate('MainWindow', 'Ctrl+O', None))
        self.actionQuit.setText(QtWidgets.QApplication.translate('MainWindow', '&Quit', None))
        self.actionQuit.setStatusTip(QtWidgets.QApplication.translate('MainWindow', 'Quit the application', None))
        self.actionQuit.setShortcut(QtWidgets.QApplication.translate('MainWindow', 'Esc', None))
        self.actionRun.setText(QtWidgets.QApplication.translate('MainWindow', '&Run', None))
        self.actionRun.setStatusTip(QtWidgets.QApplication.translate('MainWindow', 'Run the code', None))
        self.actionRun.setShortcut(QtWidgets.QApplication.translate('MainWindow', 'F5', None))
        self.actionStop.setText(QtWidgets.QApplication.translate('MainWindow', 'St&op', None))
        self.actionStop.setStatusTip(QtWidgets.QApplication.translate('MainWindow', 'Stop the current running process', None))
        self.actionStop.setShortcut(QtWidgets.QApplication.translate('MainWindow', 'F4', None))
        self.actionStep.setText(QtWidgets.QApplication.translate('MainWindow', 'St&ep...', None))
        self.actionStep.setStatusTip(QtWidgets.QApplication.translate('MainWindow', 'Run the code step by step', None))
        self.actionStep.setShortcut(QtWidgets.QApplication.translate('MainWindow', 'F8', None))
        self.actionAbout.setText(QtWidgets.QApplication.translate('MainWindow', '&About...', None))
        self.actionAbout.setStatusTip(QtWidgets.QApplication.translate('MainWindow', 'About this application', None))
        return


import resources_rc
