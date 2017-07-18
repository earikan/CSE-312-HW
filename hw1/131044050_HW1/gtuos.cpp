#include <iostream>
#include "8080emuCPP.h"
#include "gtuos.h"

#define FILE_NAME "exe.mem"
#define LAST_MEMORY_ADDRESS 65535


uint64_t GTUOS::handleCall(const CPU8080 & cpu) {

 int stateA = 0;
 stateA = static_cast<int>(cpu.state->a);

    switch (stateA) {

        //PRINT_B
        case 1: return printB(cpu);

        //PRINT_MEM
        case 2: return printMem(cpu);

        //READ_B
        case 3: return readB(cpu);

        //READ_MEM
        case 4: return readMem(cpu);

        //PRINT_STR
        case 5: return printStr(cpu);

        //READ_STR
        case 6: return readStr(cpu);

        //UNKNOWN CALL
        default: std::cout << "Unknown System Call" << std::endl;
                 throw -1;
                 break;
    }

    return 0;
}


////////////////////////////////////////////////////////////////////////////////
//Prints the contents of register B on the screen as a decimal number         //
////////////////////////////////////////////////////////////////////////////////
int GTUOS::printB(const CPU8080 & cpu){

    int stateB;
    stateB = static_cast<int>(cpu.state->b);

    std::cout << "Value of B: " << stateB << std::endl;
    std::cout << "System Call: PRINT_B\n" << std::endl;

    return 10;
}


////////////////////////////////////////////////////////////////////////////////
//Prints the contents of the memory pointed by registers B and C              //
//as a decimal number                                                         //
////////////////////////////////////////////////////////////////////////////////
int GTUOS::printMem(const CPU8080 & cpu){

    int value;
    uint16_t offset;
    offset = (cpu.state->b<<8) | cpu.state->c;

    value = static_cast<int>(cpu.memory[offset]);

    std::cout << "Value of Memory[BC]: " << value << std::endl;
    std::cout << "System Call: PRINT_MEM\n" << std::endl;

    return 10;
}


////////////////////////////////////////////////////////////////////////////////
//Reads an integer from keyboard and puts it in register B                    //
////////////////////////////////////////////////////////////////////////////////
int GTUOS::readB(const CPU8080 & cpu){

    int value;

    std::cout << "Enter a value to B: ";
    std::cin >> value;

    cpu.state->b = static_cast<uint8_t>(value);

    std::cout << "System Call: READ_B\n" << std::endl;

    return 10;
}


////////////////////////////////////////////////////////////////////////////////
//Reads an integer from keyboard and puts in the memory location pointed by   //
//registers B and C                                                           //
////////////////////////////////////////////////////////////////////////////////
int GTUOS::readMem(const CPU8080 & cpu){

    int value;
    uint16_t offset;

    offset = (cpu.state->b<<8) | cpu.state->c;

    std::cout << "Enter a value to Memory[BC]: ";
    std::cin >> value;

    cpu.memory[offset] = static_cast<uint8_t>(value);

    std::cout << "System Call: READ_MEM\n" << std::endl;

    return 10;
}


////////////////////////////////////////////////////////////////////////////////
//Prints the null terminated string at the memory location pointed            //
//by registers B and C                                                        //
////////////////////////////////////////////////////////////////////////////////
int GTUOS::printStr(const CPU8080 & cpu){

    char value;
    uint16_t offset;

    offset = (cpu.state->b<<8) | cpu.state->c;

    while(cpu.memory[offset] != '\0')
    {
      value = static_cast<char>(cpu.memory[offset]);
      std::cout << value;
      ++offset;
    }

    std::cout << "System Call: PRINT_STR\n" << std::endl;

    return 100;
}


////////////////////////////////////////////////////////////////////////////////
//Reads the null terminated string from the keyboard and puts the result      //
//at the memory location pointed by registers B and C                         //
////////////////////////////////////////////////////////////////////////////////
int GTUOS::readStr(const CPU8080 & cpu){

    std::string word;
    int i = 0;
    uint16_t offset;

    offset = (cpu.state->b<<8) | cpu.state->c;

    std::cout << "Enter a word to Memory[BC]: ";
    std::cin.ignore();
    getline(std::cin, word);

    while( i < word.length())
    {
      cpu.memory[offset] = static_cast<uint8_t>(word.at(i));
      ++offset;
      ++i;
    }

    cpu.memory[offset] = '\0';

    std::cout << "System Call: READ_STR\n" <<  std::endl;

    return 100;
}


////////////////////////////////////////////////////////////////////////////////
//At the end of the simulation, the whole memory will be saved to exe.mem as  //
//a text file of hexadecimal numbers                                          //
////////////////////////////////////////////////////////////////////////////////
void GTUOS::writeMemoryToFile(const CPU8080 & cpu){

  uint32_t offset = 0;
  FILE *myfile = fopen(FILE_NAME, "w");

  while(offset < LAST_MEMORY_ADDRESS){

    fprintf(myfile, "%.4x:    ", offset);

    for(int inCount = 0; inCount < 0x10; ++inCount)
      fprintf(myfile, "%.2x ", cpu.memory[offset + inCount]);

    fprintf(myfile, "\n");
    offset = offset + 16;

  }

  fclose(myfile);

}
