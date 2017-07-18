#include <iostream>
#include "8080emuCPP.h"
#include "gtuos.h"

int main (int argc, char**argv)
{
	if (argc != 3){
		std::cerr << "Usage: prog exeFile debugOption\n";
		exit(1);
	}
	int DEBUG = atoi(argv[2]);
	int counterOfCycle = 0;
	CPU8080 theCPU;
	GTUOS	theOS;

	theCPU.ReadFileIntoMemoryAt(argv[1], 0x0000);

	do
	{
		counterOfCycle += theCPU.Emulate8080p(DEBUG);
		if(theCPU.isSystemCall())
			counterOfCycle += theOS.handleCall(theCPU);
		if(DEBUG == 2)
			std::cin.get();
	}	while (!theCPU.isHalted());

	theOS.writeMemoryToFile(theCPU);
	fprintf(stdout, "Total Cycles = %d\n", counterOfCycle);

	return 0;
}
