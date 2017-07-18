#include <iostream>
#include "8080emuCPP.h"
#include "gtuos.h"
#include "memory.h"


int main (int argc, char**argv)
{
	if (argc != 3){
		std::cerr << "Usage: prog exeFile debugOption\n";
		exit(1);
	}

	int DEBUG = atoi(argv[2]);

	int counterOfCycle 	= 0;	//cycle counter for each process
	int totalCycleCPU 	= 0;	//total cycle of CPU
	int processIndex 	= 0;	//index for check table
	ProcessInfo process;		//for create a process
	bool flag = false;
	memory mem;					//memory, CPU, OS variables
	CPU8080 theCPU(&mem);
	GTUOS	theOS;
	int oneInstruction = 0;

	//take program to memory
	theCPU.ReadFileIntoMemoryAt(argv[1], BASE_REG);

	//set first flag as filled
	theOS.setFlagInTableIndex(0,true);

	//create a new process
	theOS.processConstruct(process, argv[1]);

	//set initial values
	process.baseRegister 	= BASE_REG;
	process.limitRegister   = LIMIT_REG;
	process.processPID 		= START_PROCESS_PID;

	//set boolean array for p1, p2, p3, p4
	theOS.setCurrentProcessIndex(0);

	//set initial start time of process
	process.startTimeOfProcess = 0; 
	process.registerSet.pc = 0;
	theOS.setProcessInfoInProcessTable(0, process);

	//refresh process variable for current process
	process = theOS.getProcessInfo(theOS.getCurrentProcessIndex());

	do{

		//set cpu state
		theOS.updateCPUState(theCPU, process.registerSet);
		if(process.processPID != -1 && process.stateOfProcess != PROCESS_IS_BLOCKED)
		{
			//set state of process and register set
			process.stateOfProcess = PROCESS_IS_RUNNING;

			//set memory base and limits
			mem.setBase(theOS.getCurrentProcessIndex() * LIMIT_REG);
			mem.setLimit(LIMIT_REG);

			//each process take 100 cycle in one time
			while(counterOfCycle < CYCLE && process.stateOfProcess == PROCESS_IS_RUNNING)
			{
				//add cycles
				oneInstruction += theCPU.Emulate8080p(DEBUG);
				if(theCPU.isSystemCall()){
					theOS.updateProcessState(theCPU, process.registerSet);
					theOS.setProcessInfoInProcessTable(theOS.getCurrentProcessIndex(), process);
					oneInstruction += theOS.handleCall(theCPU);
					process = theOS.getProcessInfo(theOS.getCurrentProcessIndex());
				}
				//if current process is halted, make current flag as false
				if(theCPU.isHalted()){
					theOS.setFlagInTableIndex(theOS.getCurrentProcessIndex(), false);
					theOS.updateProcessState(theCPU, process.registerSet);
					theOS.setProcessInfoInProcessTable(theOS.getCurrentProcessIndex(), process);

					flag = true;
					break;
				}

				//calculate cycles
				counterOfCycle += oneInstruction;
				totalCycleCPU += oneInstruction;
				process.numberOfCycles += oneInstruction;

				oneInstruction = 0;
				//don't lose the information of process
				theOS.updateProcessState(theCPU, process.registerSet);
				//save current process status at the end of instructions
				theOS.setProcessInfoInProcessTable(theOS.getCurrentProcessIndex(), process);

			}
		}else{
			theOS.updateProcessState(theCPU, process.registerSet);
			if(DEBUG == 1)
				theOS.debugProgramOne(theCPU);

			if(theOS.isDead(process.registerSet.b)){
				process.stateOfProcess = PROCESS_IS_READY;
				theOS.updateProcessState(theCPU, process.registerSet);

			}
		}
		if(!flag){
			if(process.stateOfProcess != PROCESS_IS_BLOCKED)
				process.stateOfProcess = PROCESS_IS_READY;

			theOS.setProcessInfoInProcessTable(theOS.getCurrentProcessIndex(), process);

			//check next process if there is other process
			if(theOS.getNextProcessIndex()!=-1){
				theOS.goNextProcess();
			}
			counterOfCycle = 0;

			//refresh process variable for current process
			process = theOS.getProcessInfo(theOS.getCurrentProcessIndex());
		}else{
			if(theOS.getNextProcessIndex()!=-1){
				theOS.goNextProcess();
				if(DEBUG == 2)
					theOS.debugProgramTwo(theCPU, process);
				if(DEBUG == 3)
					theOS.debugProgramThree(theCPU, process);
			}

			counterOfCycle = 0;

			//refresh process variable for current process
			process = theOS.getProcessInfo(theOS.getCurrentProcessIndex());
			flag = false;

		}
	}	while (theOS.numberOfRunningProcess())
;

	theOS.writeMemoryToFile(theCPU);
	
	return 0;
}
