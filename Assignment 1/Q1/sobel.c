/* Zachary Gill
 * CAP4453-16Fall 0001
 * Assignment 1
 * Sobel.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define  PICSIZE 256
#define  COLOR   255


int    pic[PICSIZE][PICSIZE];
int    outpicx[PICSIZE][PICSIZE];
int    outpicy[PICSIZE][PICSIZE];
int    maskx[3][3] = { { -1, 0, 1 }, { -2, 0, 2 }, { -1, 0, 1 } };
int    masky[3][3] = { { 1, 2, 1 }, { 0, 0, 0 }, { -1, -2, -1 } };
double ival[PICSIZE][PICSIZE], maxival;
int    hist[COLOR];
int    thresholdLo, thresholdHi;

void printHeader(FILE *f);

main(int argc, char** argv)
{
    int    i, j, p, q, mr, sumx, sumy;
    double thresholdPerc;
    FILE   *fo1, *fo2, *fo3, *fo4, *fo5, *fp1, *fopen();
    char   *foobar;
    char   throwaway[80];


    //get threshold
    if (argc == 2) {
        argc--; argv++;
        foobar = *argv;
        thresholdPerc = atof(foobar);
    }
    else {
        thresholdPerc = .2;
    }


    //open image files
    fp1 = fopen("face05.pgm", "rb");
    fo1 = fopen("face05_1_gradientHorizontal.pgm", "wb");
    fo2 = fopen("face05_2_gradientVertical.pgm", "wb");
    fo3 = fopen("face05_3_gradientMagnitude.pgm", "wb");
    fo4 = fopen("face05_4_thresholdLow.pgm", "wb");
    fo5 = fopen("face05_5_thresholdHigh.pgm", "wb");


    //read image
    fgets(throwaway, 80, fp1);
    fgets(throwaway, 80, fp1);
    fgets(throwaway, 80, fp1);
    if (!((throwaway[0] == '2') && (throwaway[1] == '5') && (throwaway[2] == '5'))) {
        fgets(throwaway, 80, fp1);
    }

    for (i = 0; i<PICSIZE; i++)
    {
        for (j = 0; j<PICSIZE; j++)
        {
            pic[i][j] = getc(fp1);
            pic[i][j] &= 0377;
        }
    }


    //sobel filter
    mr = 1;
    for (i = mr; i<PICSIZE - mr; i++)
    {
        for (j = mr; j<PICSIZE - mr; j++)
        {
            sumx = 0;
            sumy = 0;
            for (p = -mr; p <= mr; p++)
            {
                for (q = -mr; q <= mr; q++)
                {
                    sumx += pic[i + p][j + q] * maskx[p + mr][q + mr];
                    sumy += pic[i + p][j + q] * masky[p + mr][q + mr];
                }
            }
            outpicx[i][j] = sumx;
            outpicy[i][j] = sumy;
        }
    }


    //produce output picture (gradient x)
    int minXGrad = 100000;
    int maxXGrad = -100000;
    for (i = mr; i < PICSIZE - mr; i++) {
        for (j = mr; j < PICSIZE - mr; j++) {
            if (outpicx[i][j] < minXGrad) {
                minXGrad = outpicx[i][j];
            }
            if (outpicx[i][j] > maxXGrad) {
                maxXGrad = outpicx[i][j];
            }
        }
    }
    int xGradRange = maxXGrad - minXGrad;

    printHeader(fo1);
    for (i = 0; i<PICSIZE; i++)
    {
        for (j = 0; j<PICSIZE; j++)
        {
            fprintf(fo1, "%c", (char)(int)((double)(outpicx[i][j] - minXGrad) / xGradRange * COLOR));
        }
    }


    //produce output picture (gradient y)
    int minYGrad = 100000;
    int maxYGrad = -100000;
    for (i = mr; i < PICSIZE - mr; i++) {
        for (j = mr; j < PICSIZE - mr; j++) {
            if (outpicy[i][j] < minYGrad) {
                minYGrad = outpicy[i][j];
            }
            if (outpicy[i][j] > maxYGrad) {
                maxYGrad = outpicy[i][j];
            }
        }
    }
    int yGradRange = maxYGrad - minYGrad;

    printHeader(fo2);
    for (i = 0; i<PICSIZE; i++)
    {
        for (j = 0; j<PICSIZE; j++)
        {
            fprintf(fo2, "%c", (char)(int)((double)(outpicy[i][j] - minYGrad) / yGradRange * COLOR));
        }
    }


    //produce output picture (gradient magnitude)
    maxival = 0;
    for (i = mr; i<PICSIZE - mr; i++)
    {
        for (j = mr; j<PICSIZE - mr; j++)
        {
            ival[i][j] = sqrt((double)((outpicx[i][j] * outpicx[i][j]) +
                (outpicy[i][j] * outpicy[i][j])));
            
            if (ival[i][j] > maxival) {
                maxival = ival[i][j];
            }
        }
    }

    printHeader(fo3);
    for (i = 0; i<PICSIZE; i++)
    {
        for (j = 0; j<PICSIZE; j++)
        {
            ival[i][j] = ival[i][j] / maxival * COLOR;
            fprintf(fo3, "%c", (char)((int)ival[i][j]));
        }
    }


    //create histogram
    int maxMag = 0;
    for (i = 0; i < PICSIZE; i++) {
        for (j = 0; j < PICSIZE; j++) {
            int mag = (int)ival[i][j];
            hist[mag]++;

            if (mag > maxMag) {
                maxMag = mag;
            }
        }
    }

    //get thresholds
    int cutOff = (int)(thresholdPerc * PICSIZE * PICSIZE);
    int areaOfTops = 0;
    for (i = maxMag; i >= 1; i--) {
        areaOfTops += hist[i];
        if (areaOfTops > cutOff) {
            break;
        }
    }
    thresholdHi = i;
    thresholdLo = thresholdHi * .35;


    //produce output picture (low threshold)
    printHeader(fo4);
    for (i = 0; i<PICSIZE; i++)
    {
        for (j = 0; j<PICSIZE; j++)
        {
            double thisIval = ival[i][j] / maxival * COLOR;
            if (thisIval < thresholdLo) {
                thisIval = 0;
            }
            fprintf(fo4, "%c", (char)((int)thisIval));
        }
    }


    //produce output picture (high threshold)
    printHeader(fo5);
    for (i = 0; i<PICSIZE; i++)
    {
        for (j = 0; j<PICSIZE; j++)
        {
            double thisIval = ival[i][j] / maxival * COLOR;
            if (thisIval < thresholdHi) {
                thisIval = 0;
            }
            fprintf(fo5, "%c", (char)((int)thisIval));
        }
    }


    //close file streams
    fclose(fp1);

    fclose(fo1);
    fclose(fo2);
    fclose(fo3);
    fclose(fo4);
    fclose(fo5);


    //system("PAUSE");
}

void printHeader(FILE *f)
{
    fprintf(f, "%s\n%d %d\n%d\n", "P5", PICSIZE, PICSIZE, COLOR);  /* Print out the .pgm header */
}