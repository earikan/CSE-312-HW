#ifndef H_GTUOS
#define H_GTUOS

#include "8080emuCPP.h"

static const int MAX_NUMBER_OF_PROCESSES = 4;
static const int PROCESS_IS_READY = 0;
static const int PROCESS_IS_RUNNING = 1;
static const int PROCESS_IS_BLOCKED = 2;
static const int START_PROCESS_PID = 50;
static const int CYCLE = 100;
static const uint32_t BASE_REG  = 0x0000;
static const uint32_t LIMIT_REG = 0x4000;

typedef struct {
    std::string processName;  //the name of the process
    State8080 registerSet;    //the saved register set of 8080 chip
    uint32_t baseRegister;    //the base register
    uint32_t limitRegister;   //the limit register
    int processPID;           //pid
    int processParentPID;     //parent pid
    int startTimeOfProcess;   //start time of the process - the cycle number of CPU
    int numberOfCycles;       //how many cycles the process has used so far
    int stateOfProcess;       //ready, blocked or running
    uint32_t physicalAddress; //physical address of the memory location of the process
} ProcessInfo;


class GTUOS {
	public:
		//FUNCTIONS
		//constructor
		GTUOS();
		//for handle system calls
		uint64_t handleCall( CPU8080 & cpu);
		//get next process index
		int getNextProcessIndex();
		//go next process
		void goNextProcess();
		//get current process index from table
		int getCurrentProcessIndex();
		//before go next process set index in process table
		void setCurrentProcessIndex(int index);
		//get process info
		ProcessInfo getProcessInfo(int index);
		//change process info
		void setProcessInfoInProcessTable(int index, ProcessInfo & process);
		//is this index empty, running or blocked
		int availableIndex();
		//when a process create change flag in table
		void setFlagInTableIndex(int index, bool value);
		//create new process
		void processConstruct(ProcessInfo &process, char* fileName);
		//total number of currently runnig process
		int numberOfRunningProcess();
		//update cpu state
		void updateCPUState(CPU8080 & cpu, State8080 & registers);
		//store current process state
		void updateProcessState(CPU8080 & cpu, State8080 & registers);
		//find pid in process table for waitpid
		bool isDead(int pid);
		//debugs
		void debugProgramOne(const CPU8080 & cpu);
		void debugProgramTwo(const CPU8080 & cpu, ProcessInfo & process);
		void debugProgramThree(const CPU8080 & cpu, ProcessInfo & process);
		//writeMem
		void writeMemoryToFile(const CPU8080 & cpu);
    
	private:
		//FUNCTIONS
		int printB(const CPU8080 & cpu);
		int printMem(const CPU8080 & cpu);
		int readB(const CPU8080 & cpu);
		int readMem(const CPU8080 & cpu);
		int printStr(const CPU8080 & cpu);
		int readStr(const CPU8080 & cpu);
		int doFork(CPU8080 & cpu);
		int doExec(CPU8080 & cpu);
		int doWaitpid(CPU8080 & cpu);

		//VARIABLES
		//each process has a process table
		ProcessInfo processTable[MAX_NUMBER_OF_PROCESSES];
		//check table - is this location available or busy?
		bool checkTable[MAX_NUMBER_OF_PROCESSES];
		//each process has an index, it can be 1, 2, 3, 4
		int processIndex;
		//each process has diefferent pid
		int pidCounter;

};

#endif
