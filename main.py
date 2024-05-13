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

# Events functions take event as an argument. 
def generate(event):
    """Generates the artwork"""
    try:
        width = int(textBoxes[0].text())
    except ValueError as error:
        print("Invalid Input. Setting Width to 50")
        width = 50

    try:
        height = int(textBoxes[1].text())
    except ValueError as error:
        print("Invalid Input. Setting Height to 50")
        height = 50

    try:
        numImgs = int(textBoxes[4].text())
    except ValueError as error:
        print("Invalid Input. Generating 1 image")
        numImgs = 1

    try:
        seed = int(textBoxes[3].text())
        numImgs = 1
    except ValueError as error:
        print("No seed given. Setting seed to 70")
        seed = 0
        
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