
#pragma once
#include <emil/EMIL.hpp>
#include <queue>

#include "ExFun.h"
#include "stdafx.h"

#include <iostream>
#include <sstream>
#include <fstream>

#include <cstdlib>
#include <time.h>
#include <sys/stat.h>


using namespace std;
//using namespace ExFun;


//float rampProfile(float t, float T)
//{
//	float y = 0;
//	if( t < T ){ y = t / T;}
//	return(y);
//}
//
//float snrProfile(float t, float T, float Ni, float Ne)
//{
//	float y = ( Ni*t/T)/( Ne*(1-t/T));
//	if ( y>1) y=1;
//	return( y);
//}
//
//float sqProfile(float t, float T)
//{
//	float y = (t>0&&t<=T);
//	return(y);
//}
//
//// this bit of code implements a logistic function 
//// with t:(0;1). preferred values for this function
//// are m=.5 and s=.05
//float sigProfile(float m, float s, float t, float T)
//{
//	float y = 1 / ( 1+exp((m-(t/T))/s) );
//	return(y);
//}

namespace ExFun
{
	void wnoise(float a[], int size)
	{
		for (int j = 0; j < size; j++)
		{
			a[j] = float(rand()) / float(RAND_MAX);
		}
	}

	float mean(float a[], int size)
	{
		float m = 0;
		for (int j = 0; j < size; j++)
		{
			m += a[j] / size;
		}
		return (m);
	}


	float mini(float a[], int size)
	{
		float min = a[0];
		for (int j = 0; j<size; j++)
		{
			if (min > a[j]) min = a[j];
		}
		return (min);
	}

	float maxi(float a[], int size)
	{
		float max = a[0];
		for (int j = 0; j < size; j++)
		{
			if (max < a[j]) max = a[j];
		}
		return (max);
	}


	int locateMin(float aIn[], int size)
	{
		int iMin = 0;
		for (int j = 0; j < size; j++)
		{
			if (aIn[j] < aIn[iMin]) iMin = j;
		}
		return(iMin);
	}
	int locateMax(float aIn[], int size)
	{
		int iMax = 0;
		for (int j = 0; j<size; j++)
		{
			if (aIn[j]>aIn[iMax]) iMax = j;
		}
		return(iMax);
	}

	//add two matrices of the same size
	void add(float aIn[], float bIn[], float aOut[], int size)
	{
		float* aTemp = new float[size];
		for (int j = 0; j < size; j++)
		{
			aTemp[j] = aIn[j] + bIn[j];
		}
		memcpy(aOut, aTemp, sizeof(float)*size);
		delete[] aTemp;
	}

	void add(float aIn[],
		int size,
		float value,
		float aOut[])
	{
		float* aTemp = new float[size];
		for (int j = 0; j < size; j++)
		{
			aTemp[j] = aIn[j] + value;
		}
		memcpy(aOut, aTemp, sizeof(float)*size);
		delete[] aTemp;
	}

	void multiply(float aIn[],
		int size,
		float value,
		float aOut[])
	{
		float* aTemp = new float[size];
		for (int j = 0; j < size; j++)
		{
			aTemp[j] = aIn[j] * value;
		}
		memcpy(aOut, aTemp, sizeof(float)*size);
		delete[] aTemp;
	}

	void clipImg(float aIn[],
		int size,
		float loBound,
		float upBound,
		float aOut[])
	{
		float* aTemp = new float[size];
		for (int j = 0; j<size; j++)
		{
			if (aIn[j] > upBound)
			{
				aTemp[j] = upBound;
			}
			else if (aIn[j] < loBound)
			{
				aTemp[j] = loBound;
			}
			else
			{
				aTemp[j] = aIn[j];
			}
		}
		memcpy(aOut, aTemp, sizeof(float)*size);
		delete[] aTemp;
	}

	void sortArray(float aIn[],
		int size,
		float aOut[])
	{
		float* aTemp = new float[size];
		memcpy(aTemp, aIn, sizeof(float)*size);

		for (int h = 0; h < size; h++)
		{
			float vTemp = aTemp[h];

			int iMin = h + locateMin(aTemp + h, size - h);

			aTemp[h] = aTemp[iMin];

			aTemp[iMin] = vTemp;
		}
		memcpy(aOut, aTemp, sizeof(float)*size);
		delete[] aTemp;
	}


	float percentile(float imIn[],
		int size,
		float prctl)
	{
		float value;
		int pos = prctl / 100 * size;

		float* aTemp = new float[size];
		memcpy(aTemp, imIn, sizeof(float)*size);

		if (pos < (size / 2))
		{
			for (int h = 0; h < pos + 1; h++)
			{
				// store this value to swap it later on
				float vTemp = aTemp[h];
				int iMin = h + locateMin(aTemp + h, size - h);
				// swap positions
				aTemp[h] = aTemp[iMin];
				aTemp[iMin] = vTemp;
			}
		}
		else
		{
			pos = size - pos; // !!
			for (int h = 0; h < pos + 1; h++)
			{
				float vTemp = aTemp[h];
				int iMax = h + locateMax(aTemp + h, size - h);
				aTemp[h] = aTemp[iMax];
				aTemp[iMax] = vTemp;
			}
		}
		//note that for the upper side percentiles, 
		//position is now size-pos
		value = aTemp[pos];
		delete[] aTemp;
		return (value);
	}



	void blendImg(float imIn1[],
		float imIn2[],
		int size,
		float imOut[],
		float alpha,
		float beta)
	{
		float* aTemp = new float[size];
		for (int j = 0; j < size; j++)
		{
			aTemp[j] = imIn1[j] * alpha + imIn2[j] * beta;
		}
		memcpy(imOut, aTemp, sizeof(float)*size);
		delete[] aTemp;
	}


	void normalize(float imIn[],
		int size,
		float imOut[])
	{
		float* aTemp = new float[size];
		float loBound = mini(imIn, size);
		float upBound = maxi(imIn, size);

		float scale = 255 / (upBound - loBound);
		for (int j = 0; j < size; j++)
		{
			aTemp[j] = scale * (imIn[j] - loBound);
		}
		memcpy(imOut, aTemp, sizeof(float)*size);
		delete[] aTemp;
	}

	/* given an array of non-zero mean, this function
	returns the coefficient by which the vector itself
	needs to be multiplied (after havign subtracted
	the mean) to have it in the range passed as
	argument*/
	float norm2(float imIn[],
		float range,
		int size)
	{
		float loBound = mini(imIn, size);
		float upBound = maxi(imIn, size);
		float m = mean(imIn, size);

		float norm = (abs(upBound - m) > abs(loBound - m))
			? abs(upBound - m) : abs(loBound - m);

		norm = range / norm;
		return(norm);
	}

	void normalize2(float imIn[],
		float range,
		int size,
		float imOut[])
	{
		float norm = norm2(imIn, range, size);
		float* aTemp = new float[size];
		for (int j = 0; j < size; j++)
		{
			aTemp[j] = norm * imIn[j];
		}
		memcpy(imOut, aTemp, sizeof(float)*size);
		delete[] aTemp;
	}

	void byte2float(BYTE imIn[], int size, float imOut[])
	{
		for (int j = 0; j < size; j++)
		{
			imOut[j] = float(imIn[j]);
		}
	}


	void float2byte(float imIn[], int size, BYTE imOut[])
	{
		for (int j = 0; j < size; j++)
		{
			imOut[j] = BYTE(imIn[j]);
		}
	}

	void RGB2gray(float imIn[], int size, float imOut[])
	{
		for (int j = 0; j < size; j + 3)
		{
			imOut[j / 3] = 1 / 3 * (imIn[j] +
				imIn[1 + j] +
				imIn[2 + j]);
		}
	}

	void gray2RGB(float imIn[], int size, float imOut[])
	{
		for (int j = 0; j < size * 3; j++)
		{
			imOut[j] = imIn[j / 3];
		}
	}

	void cropImg(float imgIn[], int imgInW, int imgInH,
		float imgOut[], int imgOutW, int imgOutH)
	{
		for (int j = 0; j < imgOutH; j++)
			for (int h = 0; h < imgOutW; h++)
			{
				imgOut[h + j*imgOutW] =
					imgIn[(h + (imgInW - imgOutW) / 2) +
					((j + (imgInH - imgOutH) / 2)*imgInW)];
			}

	}

	void swapUD(float imIn[], int width,
		int height, float imOut[])
	{

		float* aTemp = new float[width*height];

		for (int h = 0; h < height; h++)
			for (int j = 0; j < width; j++)
			{
				aTemp[j + h*width] =
					imIn[j + (height - (h + 1))*width];
			}
		memcpy(imOut, aTemp, sizeof(float)*(width*height));
		delete[] aTemp;
	}

	void makeSquare(BYTE imOut[], int size)
	{
		int side = int(sqrt(double(size)));

		for (int h = 0; h < 3; h++)
			for (int j = 0; j < side; j++)
				for (int i = 0; i < side; i++)
				{
					int index = (i + j*side) * 3 + h;
					if (i < side / 2 && j < side / 2)
						imOut[index] = BYTE(255);

					else
						imOut[index] = BYTE(0);
				}
	}


	bool fexists(const char *filename)
	{
		ifstream ifile(filename);
		bool fileExists = ifile.good();
		ifile.close();
		//return ifile;
		return fileExists;
	}



	int fread(const char *filename,
		float imOut[],
		int imW,
		int imH,
		bool flipUD)
	{
		int c = 0; // colums
		int r = 0; // rows
		string line;
		ifstream ifile(filename);

		if (ifile){
			while (ifile.good())
			{
				c = 0;
				getline(ifile, line);
				char* token;
				char* tLine = new char[line.length() + 1];
				strcpy(tLine, line.c_str());
				token = strtok(tLine, "  ");

				while (token != NULL)
				{
					if (flipUD) {
						imOut[(imH - (1 + r))*imW + c++] = atof(token);
					}
					else imOut[imW*r + c++] = atof(token);
					token = strtok(NULL, "  ");
				}
				delete[] tLine;
				r++;
			}
		}
		else return(0);

	}

	int readFpi(const char* filename, float* im){

		ifstream in(filename, ios::in | ios::binary | ios::ate);
		if (in){
			int fsize = int(in.tellg());
			char* buff;
			if (in.is_open()) {
				buff = new char[fsize];
				in.seekg(0, ios::beg);
				in.read(buff, fsize);
				in.close();
				memcpy(im, buff, fsize);
				delete[] buff;
				return(1);
			}
			else return(0);
		}
		else return(0);
	}

	void createPseudoGrayImg(float* grayImg, float* rgbImg, int imgSize){

		// i'm assuming ratios of 1:2:4 (BRG)
		int LUT[21] = {
			//R  G  B
			0, 0, 0,
			0, 0, 1,
			1, 0, 0,
			1, 0, 1,
			0, 1, 0,
			0, 1, 1,
			1, 1, 0
		};

		for (int i = 0; i < imgSize; i++){

			int intPart = int(grayImg[i]);
			float decPart = grayImg[i] - intPart;

			int step = int(7 * decPart) % 7;

			rgbImg[i * 3] = intPart + LUT[3 * step];
			rgbImg[1 + i * 3] = intPart + LUT[1 + 3 * step];
			rgbImg[2 + i * 3] = intPart + LUT[2 + 3 * step];
		}
	}

	void createDitheredPseudoGrayImg(float* grayImg, float* rgbImg, int imgSize){

		// i'm assuming ratios of 1:2:4 (BRG)
		int LUT[24] = {
			//R  G  B
			0, 0, 0,
			0, 0, 1,
			1, 0, 0,
			1, 0, 1,
			0, 1, 0,
			0, 1, 1,
			1, 1, 0,
			1, 1, 1
		};

		for (int i = 0; i < imgSize; i++){

			int lumInt = int(grayImg[i]);
			float lumDec = grayImg[i] - lumInt;

			float step = 7 * lumDec;
			int stepInt = int(step);
			float stepDec = step - stepInt;
			int stepRnd = stepInt + int(rand() < int(stepDec *RAND_MAX));

			rgbImg[i * 3] = lumInt + LUT[3 * stepRnd];
			rgbImg[1 + i * 3] = lumInt + LUT[1 + 3 * stepRnd];
			rgbImg[2 + i * 3] = lumInt + LUT[2 + 3 * stepRnd];
		}
	}
}; // Exfun