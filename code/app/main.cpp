#include <iostream>
#include "compiler.h"

int main(int argc, char *argv[])
{
	std::cout << "Starting up compiler...\n";
	return compilerMain(argc, argv);
}