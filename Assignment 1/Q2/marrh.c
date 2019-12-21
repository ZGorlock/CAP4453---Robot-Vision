/* Zachary Gill
 * CAP4453-16Fall 0001
 * Assignment 1
 * Marr-Hildreth.c  (or marrh.c)
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define  PICSIZE 256
#define  COLOR   255
#define  MAXMASK 100
#define  PI      3.1415926535


int    pic[PICSIZE][PICSIZE];
double outpicx[PICSIZE][PICSIZE], outpicy[PICSIZE][PICSIZE];
double xmask[MAXMASK][MAXMASK], ymask[MAXMASK][MAXMASK];
double ival[PICSIZE][PICSIZE], conv[PICSIZE][PICSIZE];
int    cand[PICSIZE][PICSIZE], flags[PICSIZE][PICSIZE];
int    hist[COLOR];
int    thresholdLo, thresholdHi;


void doubleThresholding(int i, int j);
void printHeader(FILE *f);
double to_rad(double degrees);

main(int argc, char** argv)
{
    int     i, j, p, q, sig, mr, centx, centy;
    double  sumx, sumy, maxival, thresholdPerc;
    FILE    *fo1, *fo2, *fo3, *fp1, *fopen();
    char    *foobar;
    char    throwaway[80];


    //get sigma width
    argc--; argv++;
    foobar = *argv;
    sig = atoi(foobar);

    //get threshold percentage
    argc--; argv++;
    foobar = *argv;
    thresholdPerc = atof(foobar);
    

    //open image files
    fp1 = fopen("garb34.pgm", "rb");

    sprintf(foobar, "garb34_%d_1_gradient.pgm", sig);
    fo1 = fopen(foobar, "wb");

    sprintf(foobar, "garb34_%d_2_candidates.pgm", sig);
    fo2 = fopen(foobar, "wb");

    sprintf(foobar, "garb34_%d_3_doubleThreshold.pgm", sig);
    fo3 = fopen(foobar, "wb");


    //set up variables
    mr = (int)(sig * 3);
    centx = (MAXMASK / 2);
    centy = (MAXMASK / 2);


    //read header
    fgets(throwaway, 80, fp1);
    fgets(throwaway, 80, fp1);
    fgets(throwaway, 80, fp1);
    if (!((throwaway[0] == '2') && (throwaway[1] == '5') && (throwaway[2] == '5'))) {
        fgets(throwaway, 80, fp1);
    }

    //read image
    for (i = 0; i < PICSIZE; i++)
    {
        for (j = 0; j<PICSIZE; j++)
        {
            pic[i][j] = getc(fp1);
        }
    }


    //PART ONE
    //SMOOTHENING


    //calculate Gaussian mask
    for (p = -mr; p <= mr; p++) //y
    {
        for (q = -mr; q <= mr; q++) //x
        {
            xmask[p + centy][q + centx] = ((-1 * q) / (2 * PI * (sig*sig*sig*sig))) * exp(-1 * (((q*q) + (p*p)) / (2 * (sig*sig))));
            ymask[p + centy][q + centx] = ((-1 * p) / (2 * PI * (sig*sig*sig*sig))) * exp(-1 * (((q*q) + (p*p)) / (2 * (sig*sig))));
        }
    }

    //apply mask
    for (i = mr; i <= PICSIZE - 1 - mr; i++)
    {
        for (j = mr; j <= PICSIZE - 1 - mr; j++)
        {
            sumx = 0;
            sumy = 0;
            for (p = -mr; p <= mr; p++)
            {
                for (q = -mr; q <= mr; q++)
                {
                    sumx += pic[i + p][j + q] * xmask[p + centy][q + centx];
                    sumy += pic[i + p][j + q] * ymask[p + centy][q + centx];
                }
            }
            outpicx[i][j] = sumx;
            outpicy[i][j] = sumy;
        }
    }

    //produce output picture (gradient)
    maxival = 0;
    for (i = mr; i<PICSIZE - mr; i++)
    {
        for (j = mr; j<PICSIZE - mr; j++)
        {
            ival[i][j] = sqrt((double)((outpicx[i][j] * outpicx[i][j]) +
                (outpicy[i][j] * outpicy[i][j])));
            conv[i][j] = ival[i][j];

            if (ival[i][j] > maxival) {
                maxival = ival[i][j];
            }
        }
    }

    printHeader(fo1);
    for (i = 0; i<PICSIZE; i++)
    {
        for (j = 0; j<PICSIZE; j++)
        {
            ival[i][j] = ival[i][j] / maxival * COLOR;
            fprintf(fo1, "%c", (char)((int)ival[i][j]));
        }
    }


    //PART TWO
    //NON-MAXIMA SUPPRESSION


    for (i = mr; i < PICSIZE - mr; i++) {
        for (j = mr; j < PICSIZE - mr; j++) {

            if (outpicx[i][j] == 0) {
                outpicx[i][j] = .000001;
            }

            double tandir = outpicy[i][j] / outpicx[i][j];

            if ((tan(to_rad(337.5))) < tandir && tandir <= (tan(to_rad(22.5)))) {
                //horizontal
                if ((conv[i][j] > conv[i][j - 1]) && (conv[i][j] > conv[i][j + 1])) {
                    cand[i][j] = 1;
                }
            }
            else if ((tan(to_rad(22.5))) < tandir && tandir <= (tan(to_rad(67.5)))) {
                //diagonal /*
                if ((conv[i][j] > conv[i - 1][j - 1]) && (conv[i][j] > conv[i + 1][j + 1])) {
                    cand[i][j] = 1;
                }
            }
            else if ((tan(to_rad(67.5))) < tandir && tandir <= (tan(to_rad(112.5)))) {
                //vertical
                if ((conv[i][j] > conv[i - 1][j]) && (conv[i][j] > conv[i + 1][j])) {
                    cand[i][j] = 1;
                }
            }
            else { //if ((tan(to_rad(112.5))) < tandir && tandir <= (tan(to_rad(157.5)))) {
                //diagonal \*
                if ((conv[i][j] > conv[i + 1][j - 1]) && (conv[i][j] > conv[i - 1][j + 1])) {
                    cand[i][j] = 1;
                }
            }
        }
    }

    printHeader(fo2);
    for (i = 0; i<PICSIZE; i++)
    {
        for (j = 0; j<PICSIZE; j++)
        {
            fprintf(fo2, "%c", (char)((int)cand[i][j] * COLOR));
        }
    }


    //PART FOUR
    //COMPUTE THRESHOLD


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
    for (i = maxMag; i >= 0; i--) {   
        areaOfTops += hist[i];
        if (areaOfTops > cutOff) {
            break;
        }
    }
    thresholdHi = i;
    thresholdLo = thresholdHi * .35;

    printf("Sigma: %d  Threshold Percentage: %.4f%c\n", sig, (thresholdPerc * 100), '%');
    printf("Hi Threshold: %d\n", thresholdHi);
    printf("Lo Threshold: %d\n", thresholdLo);
    printf("\n");


    //PART THREE
    //HYSTERESIS THRESHOLDING


    //filter hi and lo candidates
    for (i = 0; i < PICSIZE; i++) {
        for (j = 0; j < PICSIZE; j++) {
            if (cand[i][j]) {
                if (ival[i][j] > thresholdHi) {
                    cand[i][j] = 0;
                    flags[i][j] = 1;
                }
                else if (ival[i][j] < thresholdLo) {
                    cand[i][j] = 0;
                    flags[i][j] = 0;
                }
            }
        }
    }

    //process mid candidates
    for (i = 0; i < PICSIZE; i++) {
        for (j = 0; j < PICSIZE; j++) {
            if (cand[i][j]) {
                doubleThresholding(i, j);
            }
        }
    }

    printHeader(fo3);
    for (i = 0; i<PICSIZE; i++)
    {
        for (j = 0; j<PICSIZE; j++)
        {
            fprintf(fo3, "%c", (char)((int)flags[i][j] * COLOR));
        }
    }


    //close file streams
    fclose(fp1);

    fclose(fo1);
    fclose(fo2);
    fclose(fo3);


    //system("PAUSE");
}

void doubleThresholding(int i, int j)
{
    if (i < 1 || i > PICSIZE - 1 ||
        j < 1 || j > PICSIZE - 1) {
        return;
    }
    if (cand[i][j] == 0 || flags[i][j] == 1) {
        return;
    }

    int p, q;
    int done = 0;
    for (p = -1; p <= 1; p++) {
        for (q = -1; q <= 1; q++) {
            
            if (flags[i + p][j + q]) {
                cand[i][j] = 0;
                flags[i][j] = 1;

                doubleThresholding(i - 1, j - 1);
                doubleThresholding(i - 1, j);
                doubleThresholding(i - 1, j + 1);
                doubleThresholding(i, j - 1);
                doubleThresholding(i, j + 1);
                doubleThresholding(i + 1, j - 1);
                doubleThresholding(i + 1, j);
                doubleThresholding(i + 1, j + 1);

                done = 1;
                break;
            }
        }

        if (done) {
            break;
        }
    }
}

void printHeader(FILE *f)
{
    fprintf(f, "%s\n%d %d\n%d\n", "P5", PICSIZE, PICSIZE, COLOR);  /* Print out the .pgm header */
}

double to_rad(double degrees) {
    return degrees * (PI / 180.0);
}