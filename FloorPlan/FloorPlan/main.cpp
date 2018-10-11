#include "FloorPlan.h"



int main(const int argc, char* argv[])
{
	if(argc!=2)
	{
		printf("Invalid number of arguments\n");
		printf("Format: *.exe input.txt\n");
		exit(EXIT_FAILURE);
	}
	FloorPlan floorPlan(argv[1]);

	printf("cost: %f\n", floorPlan.Cost("12V3V4V5V6V7V8V9VaVbVcVdVeVfVgViVjVkVlV"));
	printf("NPE: %s\n\n",floorPlan.GetNPE().c_str());

	printf("cost: %f\n", floorPlan.Cost("12H3H4H5H6H7H8H9HaHbHcHdHeHfHgHiHjHkHlH"));
	printf("NPE: %s\n\n", floorPlan.GetNPE().c_str());

	printf("cost: %f\n", floorPlan.Cost("213546H7VHVa8V9HcVHgHibdHkVHfeHVlHVjHVH"));
	printf("NPE: %s\n\n", floorPlan.GetNPE().c_str());


	return 0;
}