# Image File Reader QT Application Documentation
## Intro
The following document provides the information about main functions and abilities of the "Image File Reader", about installing and using it.This cross-platform QT application allows to read files of the following extensions: .jpg, .gif, .tif, .bmp, .png. The second ability is to define several basic properties: name, size, resolution, color depth and compression of the image.

## Resources and technologies
### Used libraries

**Common:** \
• QWidget (main application) \
• QPushPutton(button) \
• QLabel(labels) \
• QListView(model display) \
• QTableWidget(output of the model in a user-friendly form) \
\
**Working with files:** \
• QDir(working with the file system) \
• QFileSystemModel(standard Qt file system model) \
• QFile (getting a single file) \
• QFileDialog(for multiple image selection) \
\
**Working with images:** \
• QImage (getting image file) \
• QImageWriter(getting image parameters) 
### Main technologies for image properties defining

**Name:** QFile::fileName()  
**Size:** QImage::height(), QImage::width()\
**Resolution:** QImage::physicalDpiY()\
**Color depth:** QImage::bitPlaneCount()\
**Compression:** QImageWriter::compression()

## Installing
To start using "Image File Picker" you should just download the file "ImageReaderRelease.rar" from this repozitory to your computer, unzip it and run the .exe file. You can check the program with datasets "Test" and "Check Reader".

## Interface
After starting the app, you'll be presented with an interface consisting of the following elements:

• File system table\
• Button for multiple image selection\
• Text fields for information output\
• A text field for displaying the file 

After selecting multiple files  you'll be presented with a table, including the required information.
