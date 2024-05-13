from qtpy.QtWidgets import (QApplication, QLabel, 
                            QPushButton, QGridLayout,
                            QWidget,QLineEdit)
import subprocess

app = QApplication([])

title = QLabel('Art Params')
labels = ["Width", "Height", "Filename", "Seed", "Number of Images to Generate"]
paramLabels = [QLabel(l) for l in labels ]

textBoxes = [QLineEdit() for i in range(5)]

# Buttons
button = QPushButton('Generate Artwork!')

def getWidth():
    try:
        return int(textBoxes[0].text())
    except ValueError as error:
        print("Invalid Input. Setting Width to 50")
        return  50
    
def getHeight():
    try:
        return int(textBoxes[1].text())
    except ValueError as error:
        print("Invalid Input. Setting Height to 50")
        return 50
    
def getNumImgs():
    try:
        return int(textBoxes[4].text())
    except ValueError as error:
        print("Invalid Input. Generating 1 image")
        return 1
def getSeed():
    try:
        seed = int(textBoxes[3].text())
        return seed
    except ValueError as error:
        print("No seed given. Setting seed to 70")
        return 0


# Events functions take event as an argument. 
def generate(event):
    """Generates the artwork"""
    width = getWidth()
    height = getHeight()
    seed = getSeed()
    numImgs = getNumImgs()
    if seed:
        numImgs = 1
        
    filename = textBoxes[2].text()
    filename = filename.rstrip(".png")
    if filename == "":
        filename = 'art'
    print(f"Generating {numImgs} images!")
    if numImgs== 1:
        subprocess.call(['./art-script.sh', filename+".png", str(seed), str(width), str(height) ])
    else:
        for img in range(numImgs):
            subprocess.call(['./art-script.sh', filename+"."+str(img)+".png",str(seed), str(width), str(height)])

    


# Layout is NOT a widget. Needs  parent widget
widget= QWidget()
layout = QGridLayout()
layout.addWidget(title, 0, 1)
for i, label in enumerate(paramLabels):
    layout.addWidget(label, i+1, 0)
for i, textbox in enumerate(textBoxes):
    layout.addWidget(textbox,i+1, 2)


layout.addWidget(button, 6, 1)
widget.setLayout(layout)


# Connect Button to event: 
# Must pass in a FUNCTION
button.clicked.connect(generate)

widget.show()

app.exec_()