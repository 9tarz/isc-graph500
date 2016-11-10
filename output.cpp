/** 
  * Hybrid MPI and OpenMP Graph500 implementation 
  * based on Boston University High Performance Computing's implementation (https://github.com/buhpc/isc16-graph500)
  * filename: output.cpp
  */ 

#include "output.h"

void printOutput(int SCALE, int edgefactor, int NBFS, double kernel_1_time, double *kernel_2_time, long long int *kernel_2_nedge, double *kernel_2_TEPS) {
	cout << "SCALE: "<< SCALE << endl;
	cout << "edgefactor: "<< edgefactor << endl;
	cout << "NBFS: "<< NBFS << endl;
	cout << "construction_time: "<< kernel_1_time << endl;

	//kernel_2_time
	cout << "min_time: "<< *min_element(kernel_2_time,kernel_2_time + NBFS ) << endl;

	sort(kernel_2_time,kernel_2_time + NBFS);
	double medianTime;
	double q1Time;
	double q3Time;
	int mid = NBFS/2; 

	if (NBFS  % 2 == 0) {
		medianTime = (kernel_2_time[mid] + kernel_2_time[mid-1]) / 2;
		if ((mid-1)/2 == 0) {
			q1Time = (kernel_2_time[(mid)/2] + kernel_2_time[((mid)/2)-1])/2;
			q3Time = (kernel_2_time[(3*mid)/2] + kernel_2_time[(3*mid)/2-1])/2;
		} else {
			q1Time = kernel_2_time[(mid-1)/2];
			q3Time = kernel_2_time[mid + mid/2];
		}
	} else {
		medianTime = kernel_2_time[mid];
		q1Time = kernel_2_time[mid/2];
		q3Time = kernel_2_time[mid+1+mid/2];
	}

	cout << "firstquartile_time: "<< q1Time << endl;
	cout << "median_time: "<< medianTime << endl;
	cout << "thirdquartile_time: "<< q3Time << endl;

	cout << "max_time: "<< *max_element(kernel_2_time,kernel_2_time + NBFS ) << endl;
	double sumTime = accumulate(kernel_2_time,kernel_2_time + NBFS,0.0);
	double meanTime = sumTime / NBFS;
	cout << "mean_time: "<< meanTime << endl;
	double sqSumTime = inner_product(kernel_2_time,kernel_2_time + NBFS, kernel_2_time, 0.0);
	double stdevTime = sqrt(sqSumTime / NBFS - meanTime * meanTime);
	cout << "stddev_time: "<< stdevTime << endl;

	cout << "min_nedge: "<< *min_element(kernel_2_nedge,kernel_2_nedge + NBFS ) << endl;

	sort(kernel_2_nedge,kernel_2_nedge + NBFS);
	double medianEdges;
	double q1Edges;
	double q3Edges;

	if (NBFS  % 2 == 0) {
		medianEdges = (kernel_2_nedge[mid] + kernel_2_nedge[mid-1]) / 2;
		if ((mid-1)/2 == 0) {
			q1Edges = (kernel_2_nedge[(mid)/2] + kernel_2_nedge[((mid)/2)-1])/2;
			q3Edges = (kernel_2_nedge[(3*mid)/2] + kernel_2_nedge[(3*mid)/2-1])/2;
		} else {
			q1Edges = kernel_2_nedge[(mid-1)/2];
			q3Edges = kernel_2_nedge[mid + mid/2];
		}
	} else {
		medianEdges = kernel_2_nedge[mid];
		q1Edges = kernel_2_nedge[mid/2];
		q3Edges = kernel_2_nedge[mid+1+mid/2];
	}
	cout << "firstquartile_nedge: "<< q1Edges << endl;
	cout << "median_nedge: "<< medianEdges << endl;
	cout << "thirdquartile_nedge: "<< q3Edges << endl;

	cout << "max_nedge: "<< *max_element(kernel_2_nedge,kernel_2_nedge + NBFS ) << endl;
	long long int sumEdges = accumulate(kernel_2_nedge,kernel_2_nedge + NBFS ,0.0);
	double meanEdges = sumEdges / NBFS;
	cout << "mean_nedge: "<< meanEdges << endl;
	double sqSumEdges = inner_product(kernel_2_nedge,kernel_2_nedge + NBFS, kernel_2_nedge, 0.0);
	double stdevEdges = sqrt(sqSumEdges/NBFS - meanEdges * meanEdges);
	cout << "stddev_nedge: "<< stdevEdges << endl;

	cout << "min_TEPS: "<< *min_element(kernel_2_TEPS,kernel_2_TEPS + NBFS )  << endl;
	sort(kernel_2_TEPS,kernel_2_TEPS + NBFS);
	double medianTEPS;
	double q1TEPS;
	double q3TEPS;

	if (NBFS  % 2 == 0) {
		medianTEPS = (kernel_2_TEPS[mid] + kernel_2_TEPS[mid-1]) / 2;
		if ((mid-1)/2 == 0) {
			q1TEPS = (kernel_2_TEPS[(mid)/2] + kernel_2_TEPS[((mid)/2)-1])/2;
			q3TEPS = (kernel_2_TEPS[(3*mid)/2] + kernel_2_TEPS[(3*mid)/2-1])/2;
		} else {
			q1TEPS = kernel_2_TEPS[(mid-1)/2];
			q3TEPS = kernel_2_TEPS[mid + mid/2];
		}
	} else {
		medianTEPS = kernel_2_nedge[mid];
		q1TEPS = kernel_2_TEPS[mid/2];
		q3TEPS = kernel_2_TEPS[mid+1+mid/2];
	}
	cout << "firstquartile_time: "<< kernel_1_time << endl;
	cout << "median_time: "<< medianTEPS << endl;
	cout << "thirdquartile_time: "<< kernel_1_time << endl;
	cout << "max_TEPS: "<< *max_element(kernel_2_TEPS,kernel_2_TEPS + NBFS ) << endl;

	double sumHMTEPS = 0.0;
	for (int i = 0; i < NBFS; i++){
		sumHMTEPS = sumHMTEPS + (1/kernel_2_TEPS[i]);
	}
	double hmMeanTEPS = NBFS/sumHMTEPS;
	cout << "harmonic_mean_TEPS: "<< hmMeanTEPS << endl;

	double muMeanTEPS = sumHMTEPS/NBFS;
	double sqSumHMTEPS = 0.0;
	for (int i = 0; i < NBFS; i++){
		sqSumHMTEPS = sumHMTEPS + ((1/kernel_2_TEPS[i]) - muMeanTEPS)*((1/kernel_2_TEPS[i]) - muMeanTEPS);
	}
	double stdevHMTEPS = sqrt(sqSumHMTEPS/NBFS);
	stdevHMTEPS = stdevHMTEPS/sqrt(NBFS*muMeanTEPS*muMeanTEPS*muMeanTEPS*muMeanTEPS);

	cout << "harmonic_stddev_TEPS: "<< stdevHMTEPS << endl;
	
}