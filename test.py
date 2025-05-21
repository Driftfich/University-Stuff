from PyQt6.QtWidgets import QApplication, QMainWindow, QVBoxLayout, QPushButton, QWidget
from multicombobox import MultiComboBox
import sys


class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()

        self.setWindowTitle("MultiComboBox Test")

        # Create MultiComboBox instance
        self.multiComboBox = MultiComboBox()
        self.multiComboBox.addItems(["Option 1", "Option 2", "Option 3", "Option 4"])

        # Create a button to show selected items
        self.showSelectionButton = QPushButton("Show Selected Items")
        self.showSelectionButton.clicked.connect(self.showSelection)

        # Layout
        layout = QVBoxLayout()
        layout.addWidget(self.multiComboBox)
        layout.addWidget(self.showSelectionButton)

        # Set the central widget
        centralWidget = QWidget()
        centralWidget.setLayout(layout)
        self.setCentralWidget(centralWidget)

    def showSelection(self):
        selectedItems = self.multiComboBox.lineEdit().text()
        print("Selected items:", selectedItems)

def main():
    app = QApplication(sys.argv)
    mainWindow = MainWindow()
    mainWindow.show()
    sys.exit(app.exec())

if __name__ == "__main__":
    main()