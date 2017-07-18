#ifndef H_MEMORY
#define H_MEMORY

#include "memoryBase.h"

// This is just a simple memory with no virtual addresses.
// You will write your own memory with base and limit registers.

class memory: public MemoryBase {
public:
    memory(){mem = (uint8_t*) malloc(0x10000);}
	~memory(){ free(mem);}
    uint32_t getBase() {return base;}
    uint32_t getLimit(){return limit;}
    void setBase(uint32_t value){base = value;}
    void setLimit(uint32_t value){limit = value;}
	virtual uint8_t & at(uint32_t ind) { if(ind < limit) return  mem[ind + base];
        std::cout << "Segmentation fault." << std::endl;
        exit(1);
    }
	virtual uint8_t & physicalAt(uint32_t ind) { return mem[ind];}
private:
	uint8_t * mem;
    uint32_t base;
    uint32_t limit;
};

#endif
