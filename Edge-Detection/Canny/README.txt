To compile this program run:
	Windows:
		gcc marrh.c -o Canny
		OR compile.bat
		
	Linux:
		gcc marrh.c -o Canny -lm
		OR ./compile.sh
	
To run:
	Windows:
		Canny.exe sigma threshold
		OR run.bat
		
	Linux:
		./Canny sigma threshold
		OR ./run.sh
	
	where:
		sigma     = the width of one sigma in pixels
		threshold = the threshold percentage to use (.05 = 5.0%)
		
	Recommended threshold with sig 1 = .05
						  with sig 2 = .09

This will run the program on image garb34.pgm and produce the following output images:
	garb34_%d_1_gradient.pgm
	garb34_%d_2_candidates.pgm
	garb34_%d_3_doubleThreshold.pgm
	
	Where %d is the sigma value
	
Note: If you use my premade compile.sh and run.sh in Linux make sure you give it run permissions first:
	chmod +x <file>.sh