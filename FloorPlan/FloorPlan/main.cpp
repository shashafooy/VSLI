#include "FloorPlan.h"
#include <iostream>


int main(const int argc, char* argv[])
{
	if(argc!=2)
	{
		printf("Invalid number of arguments\n");
		printf("Format: *.exe input.txt\n");
		exit(EXIT_FAILURE);
	}
	FloorPlan floorPlan(argv[1]);

/*
	floorPlan.Run("12V3V4V5V6V7V8V9VaVbVcVdVeVfVgViVjVkVlH");
	floorPlan.Run("12H3H4H5H6H7H8H9HaHbHcHdHeHfHgHiHjHkHlH");*/
	floorPlan.Run("213546H7VHVa8V9HcVHgHibdHkVHfeHVlHVjHVH");


	return 0;
}
