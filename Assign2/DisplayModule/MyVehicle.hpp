#ifndef __MYVEHICLE_HPP__
#define __MYVEHICLE_HPP__


#include "Vehicle.hpp"

class MyVehicle : public Vehicle
{
private:
	int* numRanges;
	double* rangeX;
	double* rangeY;
	void plotLaser();


public:
	MyVehicle();
	MyVehicle(int* numData, double rangeX[], double rangeY[]);
	void setLaser(int* numData, double rangeX[], double rangeY[]);
	virtual void draw();

};

#endif