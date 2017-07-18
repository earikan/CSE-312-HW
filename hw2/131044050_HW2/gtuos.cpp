#include <iostream>
#include <string.h>
#include "8080emuCPP.h"
#include "gtuos.h"

#define FILE_NAME "exe.mem"
#define LAST_MEMORY_ADDRESS 65535


//constructor
GTUOS::GTUOS(){
        for(int i=0; i < MAX_NUMBER_OF_PROCESSES; ++i){
            checkTable[i] = false;
            memset(&processTable[i].registerSet, 0, sizeof(State8080)); 
            processTable[i].processName = std::string("");
        }
        pidCounter = START_PROCESS_PID;
}




//get a process info
ProcessInfo GTUOS::getProcessInfo(int index) {
		return processTable[index];
}



//set a process info
void GTUOS::setProcessInfoInProcessTable(int index, ProcessInfo &process){
		if (index > -1)
			processTable[index] = process;
}



//is available index for process
int GTUOS::availableIndex() {

	for(int i=0; i < MAX_NUMBER_OF_PROCESSES; ++i)
		if(checkTable[i] == false)
			return i;

	return -1;
}



//if a process create or end change the available index
void GTUOS::setFlagInTableIndex(int index, bool value){
		checkTable[index] = value;

}



//create an empty process
void GTUOS::processConstruct(ProcessInfo &process, char* fileName){
		std::string nameOfProcess = std::string(fileName);
		process.processName = nameOfProcess;
		process.baseRegister = BASE_REG;
		process.limitRegister = BASE_REG;
		process.processPID = -1;
		process.processParentPID = -1;
		process.startTimeOfProcess = 0 ;
		process.numberOfCycles = 0;
		process.stateOfProcess = PROCESS_IS_READY;
		process.physicalAddress = BASE_REG;
}



//number of running process
int GTUOS::numberOfRunningProcess(){
	int runningProcessCounter = 0;

	for(int i=0; i < MAX_NUMBER_OF_PROCESSES; ++i){
		if(checkTable[i] == true)
			++runningProcessCounter;
	}

	return runningProcessCounter;
}



//get next running process
int GTUOS::getNextProcessIndex() {

	int nextProcessIndexTemp = 0;
	nextProcessIndexTemp = processIndex;
	//if there is no running process end loop
	if(numberOfRunningProcess() < 1){
		nextProcessIndexTemp = -1;
		return -1;
	}
	//if there is more than one process then find next process index
	else{
		nextProcessIndexTemp = (++nextProcessIndexTemp) % MAX_NUMBER_OF_PROCESSES;
		while(nextProcessIndexTemp != processIndex){
			if(checkTable[nextProcessIndexTemp] == true)
				return nextProcessIndexTemp;
			nextProcessIndexTemp = (++nextProcessIndexTemp) % MAX_NUMBER_OF_PROCESSES;
		}
	}


	return processIndex;
}



//go next process
void GTUOS::goNextProcess(){
	processIndex = getNextProcessIndex();
}



//return process index
int GTUOS::getCurrentProcessIndex() {
		return processIndex;
}



//set process index
void GTUOS::setCurrentProcessIndex(int index){
		processIndex = index;
}



//update cpu state
void GTUOS::updateCPUState(CPU8080 & cpu, State8080 & registers){
		*(cpu.state) = registers;
}



//update process state
void GTUOS::updateProcessState(CPU8080 & cpu, State8080 & registers){
		registers = *(cpu.state);
}



//handle call
uint64_t GTUOS::handleCall(CPU8080 & cpu){

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

			//FORK
			case 7: return doFork(cpu);

			//EXEC
			case 8: return doExec(cpu);

			//waitpid
			case 9: return doWaitpid(cpu);

			//UNKNOWN CALL
			default: std::cout << "Unknown System Call" << std::endl;
							 throw -1;
							 break;
	}

	return 0;
}



//fork function
int GTUOS::doFork(CPU8080 & cpu){

	uint32_t copyIndex = BASE_REG;
	int nextIndex = availableIndex();
	int currentIndex = getCurrentProcessIndex();

	//is there any space for child process
	if(numberOfRunningProcess() < MAX_NUMBER_OF_PROCESSES)
	{
		while(copyIndex < LIMIT_REG){
			cpu.memory->physicalAt(nextIndex * LIMIT_REG + copyIndex ) = cpu.memory->physicalAt(currentIndex * LIMIT_REG + copyIndex);
			copyIndex++;
		}

		checkTable[nextIndex] = true;
		processTable[nextIndex] = processTable[currentIndex];
		processTable[nextIndex].processPID = ++pidCounter;
		processTable[nextIndex].processParentPID = processTable[currentIndex].processPID;

		processTable[nextIndex].registerSet.b = 0x00;
		processTable[currentIndex].registerSet.b = processTable[nextIndex].processPID;

	}

	updateCPUState(cpu,processTable[currentIndex].registerSet);

	return 50;

}



//exec function
int GTUOS::doExec(CPU8080 & cpu){

		int index = 0;
		index = getCurrentProcessIndex() * LIMIT_REG;
		std::string tempStringForFileName = "";
		char value;
    uint16_t offset;

    offset = (cpu.state->b<<8) | cpu.state->c;

    while(cpu.memory->at(offset) != '\0')
    {
      value = static_cast<char>(cpu.memory->at(offset));
      tempStringForFileName += value;
      ++offset;
    }

		tempStringForFileName += '\0';
		cpu.ReadFileIntoMemoryAt(tempStringForFileName.c_str(), index);
		processTable[getCurrentProcessIndex()].registerSet.pc = 0;
		updateCPUState(cpu,processTable[getCurrentProcessIndex()].registerSet);

		return 80;
}


void GTUOS::debugProgramOne(const CPU8080 & cpu){

		std::cout << "cpu.state->a " << (int)cpu.state->a << "\n";
		std::cout << "cpu.state->b " << (int)cpu.state->b << "\n";
		std::cout << "cpu.state->c " << (int)cpu.state->c << "\n";
		std::cout << "cpu.state->d " << (int)cpu.state->d << "\n";
		std::cout << "cpu.state->e " << (int)cpu.state->e << "\n";
		std::cout << "cpu.state->h " << (int)cpu.state->h << "\n";
		std::cout << "cpu.state->l " << (int)cpu.state->l << "\n";
		std::cout << "cpu.state->sp " << (int)cpu.state->sp << "\n";
		std::cout << "cpu.state->pc " << (int)cpu.state->pc << std::endl;

}

void GTUOS::debugProgramTwo(const CPU8080 & cpu, ProcessInfo & process ){

		std::cout << "Process Name: " << process.processName << std::endl;

}

void GTUOS::debugProgramThree(const CPU8080 & cpu, ProcessInfo & process ){

		printf("process name 	    base      limit 	pid 	ppid  	sto	state	  pAddess\n" );
		printf("%-18s   %-7ld  %-7ld    %-4d   %-4d 	%-6d  %-6d    	%-6ld\n",
			process.processName.c_str(), (long)process.baseRegister,
			(long)process.limitRegister,process.processPID,process.processParentPID,
			process.startTimeOfProcess, process.stateOfProcess, (long)process.physicalAddress);


}


//waitpid function
int GTUOS::doWaitpid(CPU8080 & cpu){

		processTable[getCurrentProcessIndex()].stateOfProcess = PROCESS_IS_BLOCKED;
		updateCPUState(cpu,processTable[getCurrentProcessIndex()].registerSet);

		return 80;
}



//is child process dead
bool GTUOS::isDead(int pid){

	for(int i=0; i<MAX_NUMBER_OF_PROCESSES; ++i){
			if(processTable[i].processPID == pid)
				if(checkTable[i] == true)
					return false;
	}
	return true;
}



//Prints the contents of register B on the screen as a decimal number
int GTUOS::printB(const CPU8080 & cpu){

    int stateB;
    stateB = static_cast<int>(cpu.state->b);

    std::cout << (int)stateB << std::endl;
    std::cout << "System Call: PRINT_B" << std::endl;

    return 10;
}



//Prints the contents of the memory pointed by registers B and C
//as a decimal number
int GTUOS::printMem(const CPU8080 & cpu){

    int value;
    uint16_t offset;
    offset = (cpu.state->b<<8) | cpu.state->c;

    value = static_cast<int>(cpu.memory->at(offset));

    std::cout << (int)value << std::endl;
    std::cout << "System Call: PRINT_MEM" << std::endl;

    return 10;
}



//Reads an integer from keyboard and puts it in register B
int GTUOS::readB(const CPU8080 & cpu){

    int value;

    std::cout << "Enter a value to B: ";

    std::cin >> value;

		std::cout << " ";
    cpu.state->b = static_cast<uint8_t>(value);

    std::cout << "System Call: READ_B" << std::endl;
		std::cin.ignore();
    return 10;
}



//Reads an integer from keyboard and puts in the memory location pointed by
//registers B and C
int GTUOS::readMem(const CPU8080 & cpu){

    int value;
    uint16_t offset;

    offset = (cpu.state->b<<8) | cpu.state->c;

    std::cout << "Enter a value to Memory[BC]: ";
    std::cin >> value;

    cpu.memory->at(offset) = static_cast<uint8_t>(value);

    std::cout << "System Call: READ_MEM" << std::endl;
		std::cin.ignore();
    return 10;
}



//Prints the null terminated string at the memory location pointed
//by registers B and C
int GTUOS::printStr(const CPU8080 & cpu){

    char value;
    uint16_t offset;

    offset = (cpu.state->b<<8) | cpu.state->c;

    while(cpu.memory->at(offset) != '\0')
    {
      value = static_cast<char>(cpu.memory->at(offset));
      std::cout << value;
      ++offset;
    }

    std::cout << "System Call: PRINT_STR" << std::endl;

    return 100;
}



//Reads the null terminated string from the keyboard and puts the result
//at the memory location pointed by registers B and C
int GTUOS::readStr(const CPU8080 & cpu){

    std::string word;
    int i = 0;
    uint16_t offset;

    offset = (cpu.state->b<<8) | cpu.state->c;

    std::cout << "Enter a word to Memory[BC]: " << std::endl;
		std::cin.clear();
    getline(std::cin, word);
		std::cout << " ";

    while( i < word.length())
    {
      cpu.memory->at(offset) = static_cast<uint8_t>(word.at(i));
      ++offset;
      ++i;
    }

    cpu.memory->at(offset) = '\0';

    std::cout << "System Call: READ_STR" <<  std::endl;
    return 100;
}



//write memory to file
void GTUOS::writeMemoryToFile(const CPU8080 & cpu){

  uint32_t offset = 0;
  FILE *myfile = fopen(FILE_NAME, "w");

  while(offset < LAST_MEMORY_ADDRESS){

    fprintf(myfile, "%.4x:    ", offset);

    for(int inCount = 0; inCount < 0x10; ++inCount)
      fprintf(myfile, "%.2x ", cpu.memory->physicalAt(offset + inCount));

    fprintf(myfile, "\n");
    offset = offset + 16;

  }

  fclose(myfile);

}
