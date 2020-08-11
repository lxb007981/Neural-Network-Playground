# A Neural Network Based Image Classifier Using C++

## Overview
- A toy Neural Network-based linear image classifier. It is designed to be trained on any grayscale dataset.
  
## Dependencies and Installation
- C++ >= 17
- OpenCV >= 4.1.1
- Visual Studio>=2017
- Qt 5.12.5 MSVC 2017 64-bit

- install the extension "Qt VS Tools" in Visual Studio marketplace
- add Qt 5.12.5 MSVC 2017 64-bit in Qt Maintenance Tool
- set the C++ language standard in Visual Studio by Visual Studio:  `Debug->project properties->C/C++->Language->C++ Language - Standard->ISO C++17 Standard (/std:c++17)`

## How to use It
- **[Part 1] Train the networks**
    1. Open the file "widgetTest.pro" in Qt VS Tools, Visual Studio will create a .sln file based on the .pro file, save the .sln file and open it afterwards.
    2. Use Ctrl + Alt + L to open Solution Explorer, click Add->Existing Items, add DataLoader.cpp, DataLoader.h, FullyConnectedNet.cpp, FullyConnectedNet.h, Solver.cpp, Solver.h
    3. Upload a dataset by pressing the Upload button. The dataset should be organized in the following folder.
 
        ```
        root/dog/xxx.png
        root/dog/xxy.png
        root/dog/xxz.png
        root/cat/123.png
        root/cat/124.png
        root/cat/125.png
        ```

    4. Fill in hyperparameters: 
            `Number of Epoch`,
            `Learning Rate`,
            `Regularation Strength`,
            `Batchsize`,
            `Weight Scale`,
            `Learning Rate Decay `
    
    5. Add hidden layer manually.

    6. Press Run button to train the networks

- **[Part 2] Get Prediction**
    1. trun on **test mode**
   
    2. press **run** button to get the prediction

**Special Notes**: Inappropriate hyperparameters might cause gradient vanishing.

## External Libraries
[OpenCV](https://opencv.org/)

[QCustomPlot](https://www.qcustomplot.com/)

[other miscellaneous classes](https://stackoverflow.com/questions/14780517/toggle-switch-in-qt)

## Contributing
We appreciate all contributions. Please carefully read the details of our code of conduct.

## Authors
[lxb007981](https://github.com/lxb007981)

[Equinox-sy](https://github.com/Equinox-sy)

## Acknowledgements
Get Inspiration from Stanford online course [cs231n: Convolutional Neural Networks for Visual Recognition](https://cs231n.github.io/).