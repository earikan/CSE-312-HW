#ifndef H_GTUOS
#define H_GTUOS

#include "8080emuCPP.h"
#include <fstream>

class GTUOS{
	public:
		uint64_t handleCall(const CPU8080 & cpu);
		void writeMemoryToFile(const CPU8080 & cpu);
	private:
		int printB(const CPU8080 & cpu);
		int printMem(const CPU8080 & cpu);
		int readB(const CPU8080 & cpu);
		int readMem(const CPU8080 & cpu);
		int printStr(const CPU8080 & cpu);
		int readStr(const CPU8080 & cpu);

};

#endif
