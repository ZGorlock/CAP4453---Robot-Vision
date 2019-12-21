My 'detected' image and report are included in this directory.
For my output image, vdetect detects 66 faces and it looks like there is 1 false positive.


To train with vboost, I used 2000 face images and 2000 non-face images.
If you want to run this yourself, you can navigate to AdaBoost/vboost/vboost and run:
    vboost.exe new.cls 2000 2000 1 100
    OR run.bat

Note that I have not included the training images in my submission, you must place these in the proper directories if you want to run the training program.


To detect faces with vdetect, I used a classifier.thresh value of 0.645 and a scale of 1.3.
If you want to run this yourself, you can navigate to AdaBoost/vdetect/Debug and run:
    vdetect.exe -i new.cls Germany.pgm
    OR run.bat