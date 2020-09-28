#ifndef EXFUN_H_
#define EXFUN_H_
#pragma once
namespace ExFun
{
	float mean(float a[], int size);
	float rampProfile(float t, float T);
	float snrProfile(float t, float T, float Ni, float Ne);
	float linProfile(float t, float T);
	float sqProfile(float t, float T);
	float sigProfile(float m, float s,
	float t, float T);


	void cropImg(float imgIn[], int imgInW, int imgInH,
	float imgOut[], int imgOutW, int imgOutH);

	void byte2float(BYTE aIn[], int size, float aOut[]);

	void add(float aIn[], int size,
	float value, float aOut[]);

	void add(float aIn[], float bIn[], float aOut[], int size);

	void multiply(float aIn[], int size,
	float value, float aOut[]);

	void blendImg(float aIn1[], float aIn2[], int size,
	float aOut[], float alpha, float beta);

	void float2byte(float aIn[], int size, BYTE aOut[]);

	void makeSquare(BYTE imOut[], int size);

	void wnoise(float a[], int size);

	void clipImg(float aIn[],
	int size,
	float loBound,
	float upBound,
	float aOut[]);

	void normalize(float aIn1[],
	int size,
	float aOut[]);

	void normalize2(float aIn1[],
	float range,
	int size,
	float aOut[]);
	float norm2(float aIn1[],
	float range,
	int size);

	void swapUD(float imIn[], int width,
	int height, float imOut[]);

	float mini(float a[], int size);

	float maxi(float a[], int size);

	int locateMin(float aIn[], int size);

	int locateMax(float aIn[], int size);

	void RGB2gray(float imIn[],
	int size,
	float imOut[]);
	void gray2RGB(float imIn[],
	int size,
	float imOut[]);


	float percentile(float imIn[],
	int size,
	float prctl);

	void sortArray(float aIn[],
	int size,
	float aOut[]);

	int fread(const char *filename,
	float aOut[],
	int imW,
	int imH,
	bool flipUD = false);
	bool fexists(const char *filename);
	int readFpi(const char* filename, float* im);
	void createPseudoGrayImg(float* grayImg, float* rgbImg, int imgSize);
	void createDitheredPseudoGrayImg(float* grayImg, float* rgbImg, int imgSize);
}; // namespace
#endif

//class ExFun
//{
//public:
//
//	 float mean(float a[],int size);
//	 float rampProfile(float t, float T);
//	 float snrProfile(float t, float T, float Ni, float Ne);
//	 float linProfile(float t, float T);
//	 float sqProfile(float t, float T);
//	 float sigProfile(float m, float s,
//		float t, float T);
//	
//
//	 void cropImg(float imgIn[],int imgInW,int imgInH,
//					float imgOut[],int imgOutW,int imgOutH);
//
//	 void byte2float(BYTE aIn[],int size,float aOut[]);
//	
//	 void add(float aIn[],int size,
//		float value,float aOut[]);
//
//	 void add(float aIn[],float bIn[],float aOut[],int size);
//
//	 void multiply(float aIn[],int size,
//		float value,float aOut[]);
//
//	 void blendImg(float aIn1[], float aIn2[],int size, 
//					float aOut[],float alpha,float beta);
//	
//	 void float2byte(float aIn[],int size, BYTE aOut[]);	
//
//	 void makeSquare(BYTE imOut[], int size);
//
//	 void wnoise(float a[],int size);
//
//	 void clipImg(float aIn[],
//									int size,
//									float loBound,
//									float upBound, 
//									float aOut[]);
//
//	 void normalize(float aIn1[],
//									int size,
//									float aOut[]);
//
//	 void normalize2(float aIn1[],									
//									float range,
//									int size,
//									float aOut[]);
//	 float norm2(float aIn1[],									
//									float range,
//									int size);
//
//	 void swapUD(float imIn[],int width, 
//		int height, float imOut[]);
//
//	 float mini(float a[],int size);
//	
//	 float maxi(float a[],int size);
//	
//	 int locateMin(float aIn[],int size);
//
//	 int locateMax(float aIn[],int size);
//
//	 void RGB2gray(float imIn[],
//							int size, 
//							float imOut[]);
//	 void gray2RGB(float imIn[],
//							int size, 
//							float imOut[]);
//
//
//	 float percentile(float imIn[],
//									int size, 
//									float prctl);
//	
//	 void sortArray(float aIn[],
//								int size,
//								float aOut[]);
//
//	 int fread(const char *filename, 
//				float aOut[],
//				int imW,
//				int imH,
//				bool flipUD = false);
//	 bool fexists(const char *filename);
//	 int readFpi ( const char* filename, float* im);
//	 void createPseudoGrayImg( float* grayImg, float* rgbImg, int imgSize);
//	 void createDitheredPseudoGrayImg( float* grayImg, float* rgbImg, int imgSize);	
//};
