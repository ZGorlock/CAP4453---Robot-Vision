To compile this program run:
	Windows:
		gcc sobel.c -o Sobel
		OR compile.bat
		
	Linux:
		gcc sobel.c -o Sobel -lm
		OR ./compile.sh
	
To run:
	Windows:
		Sobel.exe threshold
		OR run.bat
		
	Linux:
		./Sobel threshold
		OR ./run.sh
	
	where:
		threshold = the threshold percentage to use (.2 = 20.0%)
			    this parameter is optional and if not given a threshold of 20% will be used
		
	Recommended threshold = .2
	
This will run the program on image face05.pgm and produce the following output images:
	face05_1_gradientHorizontal.pgm
	face05_2_gradientVertical.pgm
	face05_3_gradientMagnitude.pgm
	face05_4_thresholdLow.pgm
	face05_5_thresholdHigh.pgm
	
Note: If you use my premade compile.sh and run.sh in Linux make sure you give it run permissions first:
	chmod +x <file>.sh