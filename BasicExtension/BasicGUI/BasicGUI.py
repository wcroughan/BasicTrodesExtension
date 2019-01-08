import sys
from PyQt5.QtWidgets import QApplication, QWidget, QPushButton, QVBoxLayout, QLabel, QHBoxLayout, QLineEdit, QComboBox, QFileDialog, QInputDialog, QMessageBox, QLCDNumber
from PyQt5.QtGui import QIcon
from PyQt5.QtCore import QTimer, QTime
import ctypes as cy

CALLBACK = cy.CFUNCTYPE(None, cy.c_int)


class App(QWidget):
 
	def __init__(self):
		super().__init__()
		self.title = 'CHOIR: Computational Hippocampal Online Replay Detection'
		self.initCConnection()
		self.initUI()
		
	###
	# UI Layout functions

	def initUI(self):
		self.left = 100
		self.top = 100
		self.width = 800
		self.height = 400
		self.setWindowTitle(self.title)
		self.setGeometry(self.left, self.top, self.width, self.height)

		self.state_var = 0

		self.UIWelcome()
		self.show()

	def UIWelcome(self):
		vbox = QVBoxLayout()
		lbl_title = QLabel('Heres a simple lil program!')
		vbox.addWidget(lbl_title)

		hbox = QHBoxLayout()

		self.loadbtn = QPushButton("Start first collection", self)
		self.loadbtn.resize(self.loadbtn.sizeHint())
		self.loadbtn.clicked.connect(self.welcomeloadbtn)
		hbox.addStretch(1)
		hbox.addWidget(self.loadbtn)
		hbox.addStretch(1)

		vbox.addLayout(hbox)
		
		if self.layout():
			QWidget().setLayout(self.layout())
		self.setLayout(vbox)

	###
	# Button functions

	def welcomeloadbtn(self):
		print("button pressed")
		if self.state_var == 0:
			print("tryina call cfunc")
			self.pyfun_start()
			print("called cfunc")
			self.loadbtn.setText('Stop first collection')
			self.state_var = 1
		elif self.state_var == 1:
			self.pyfun_end_run()
			self.loadbtn.setText('Stop second collection')
			self.state_var = 2

	###
	# C++ interaction functions
	
	def pyfun_start(self):
		self.run_res = self.mydll.pyfun_start()
		print(self.run_res)

	def pyfun_end_run(self):
		self.mydll.pyfun_end_run()
		
	def initCConnection(self):
		self.mydll = cy.CDLL(r"libBasicExtension.so")
		self.mydll.pyfun_start.argtypes = []
		self.mydll.pyfun_start.restype = cy.c_int
		
		self.mydll.pyfun_end_run.argtypes = []
		self.mydll.pyfun_end_run.restype = None
		

if __name__ == '__main__':
	app = QApplication(sys.argv)
	ex = App()
	sys.exit(app.exec_())