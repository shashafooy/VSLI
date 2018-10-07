#include "AVLProgram.h"

int main()
{
	auto* program = new AVLProgram();

	program->Run();

	delete program;

	return 0;

}