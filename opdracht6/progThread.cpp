// w3.1.3hwlayer

#include "progThread.hpp"

using namespace std;

//Compiler class objects
COMPILER_CLASS compProg[PROG_NUM];
ifstream file[PROG_NUM];

//Program filenames.
char program_1[255] = "program1.txt";
char program_2[255] = "program2.txt";
char *programList[PROG_NUM];

void setProgList(void){
	programList[PROG_1] = program_1;
	programList[PROG_2] = program_2;
	
	activeProgramIndex = PROG_1;
};

void progThreadsInit(void){
	setProgList();
};

bool getCurrProgOutput(char *src){
	cout << src[0];
	return false;
};

progThread::progThread(char *filename, uint8_t i) {
	// Initialiseer de thread instellingen
	pthread_attr_init(&progAttr_);
	pthread_attr_setdetachstate(&progAttr_, PTHREAD_CREATE_JOINABLE);
	// Maak de thread zelf en koppel de class instantie via het dataveld
	int nRet = pthread_create(&progThread_, &progAttr_, thread, filename); 
	if (nRet > 0) {
		printf("progThread kon niet gemaakt worden!\n");
		return;
	};
};

progThread::~progThread() {

  // Vrijgeven thread instellingen
  pthread_attr_destroy(&progAttr_);
};

void progThread::join() {

  void *pStatus;

  // Wacht tot de thread klaar is
  pthread_join(progThread_, &pStatus); 
};

uint16_t progThread::programIndex(char *input){
	for(uint8_t i = 0; i < sizeof programList; i++){
		if(programList[i] == NULL)
			return INVALID_INPUT;
		if(!strcmp(input, programList[i]))
			return i;
	}
	return INVALID_INPUT;
};

void progThread::delay(long nMSec) {
	
  struct timespec oTime, oTimeLeft;
  // Handige delay-functie in msec
  oTime.tv_sec = 0; 
  oTime.tv_nsec = nMSec * 1000000L;
  nanosleep(&oTime, &oTimeLeft);
};

void progThread::skipToLine(uint16_t lineNumber, char *fileName, uint8_t currProg){
	char input[MAX_TOKEN_SIZE];
	file[currProg].close();
	file[currProg].open(fileName);
	for(uint16_t i = 0; i < lineNumber; i++)
		file[currProg].getline(input, sizeof input);
};

void *progThread::thread(void *pThreadData) {
// Maak een referentie naar het class object
	progThread *pThis;
	pThis = (progThread*)pThreadData;
	//Program information
	char *filename = (char*)pThreadData;
	uint16_t currProg = pThis->programIndex(filename);
	cout << filename << endl;
//Program input
	uint16_t lineNumber = 0;
	uint16_t lineNumber_cpy;
	uint8_t instruction = 0;
	char *tokens[MAX_NUM_TOKENS];
	char input[MAX_TOKEN_SIZE];
	char while_expression[MAX_TOKEN_SIZE];
	char while_relationOperator[MAX_TOKEN_SIZE];
	char while_desiredState[MAX_TOKEN_SIZE];
	bool executeLoop_flag = false;
//initialize compiler
	compProg[currProg].compilerInit(currProg);
//Get first line of program
	if(filename != NULL && currProg != INVALID_INPUT){
		pthread_mutex_lock(&oLockInput);
		file[currProg].open(filename);
		file[currProg].getline(input, sizeof input);
		pthread_mutex_unlock(&oLockInput);
		pThis->delay(40);
		lineNumber++;
	}
	while(filename != NULL && currProg != INVALID_INPUT){
		/************************
		CHECK STOP BUTTON
		*************************/
		if(stopPushed) {
			pthread_mutex_lock(&oLockInput);
			file[currProg].close();
			pthread_mutex_unlock(&oLockInput);
			pThis->delay(40);
			compProg[currProg].closeProgram();
			pthread_exit(NULL);
		}
		/************************
		RUN PROGRAM
		*************************/
		if(pThis->parseObjct.tokenizer(input, tokens) && currProg == activeProgramIndex){
			pthread_mutex_lock(&oLockOutput);
			instruction = compProg[currProg].executeInstruction(tokens, lineNumber);
			pthread_mutex_unlock(&oLockOutput);
			pThis->delay(40);
			/************************
			WHILE LOOP
			*************************/
			if(instruction == WHILE_STATEMENT){
				strcpy(while_expression, tokens[1]);
				strcpy(while_relationOperator, tokens[2]);
				strcpy(while_desiredState, tokens[3]);
				lineNumber_cpy = lineNumber;
				//Check if conditions for loop are true
				pthread_mutex_lock(&oLockOutput);
				if(compProg[currProg].checkWhileConditions(while_expression, while_relationOperator, while_desiredState, lineNumber))
					executeLoop_flag = true;
				pthread_mutex_unlock(&oLockOutput);
				pThis->delay(40);
				//If conditons are true, execute loop until they are not true anymore
				while(executeLoop_flag == true){
					pthread_mutex_lock(&oLockInput);
					file[currProg].getline(input, sizeof input);
					pthread_mutex_unlock(&oLockInput);
					pThis->delay(40);
					lineNumber_cpy++;
					if(pThis->parseObjct.tokenizer(input, tokens)){
						if(!strcmp(tokens[0], "}")){
							pthread_mutex_lock(&oLockOutput);
							if(!compProg[currProg].checkWhileConditions(while_expression, while_relationOperator, while_desiredState, lineNumber))
								executeLoop_flag = false;
							
							else{
								lineNumber_cpy = lineNumber;
								pthread_mutex_lock(&oLockInput);
								pThis->skipToLine(lineNumber, filename, currProg);
								pthread_mutex_unlock(&oLockInput);
								pThis->delay(40);
							}
							pthread_mutex_unlock(&oLockOutput);
							pThis->delay(40);
						}
						else{
							/************************
							CHECK STOP BUTTON
							*************************/
							if(stopPushed) {
								pthread_mutex_lock(&oLockInput);
								file[currProg].close();
								pthread_mutex_unlock(&oLockInput);
								pThis->delay(40);
								//pthread_mutex_lock(&oLockOutput);
								//compProg[currProg].closeProgram();
								//pthread_mutex_unlock(&oLockOutput);
								//pThis->delay(40);
								pthread_exit(NULL);
							}
							pthread_mutex_lock(&oLockOutput);
							compProg[currProg].executeInstruction(tokens, lineNumber_cpy);
							pthread_mutex_unlock(&oLockOutput);
							pThis->delay(40);
						}
					}
				}
				sprintf(progOutput[currProg],"End of while!");
				newOutputAvailable = true;
				pthread_mutex_lock(&oLockInput);
				pThis->skipToLine(lineNumber_cpy, filename, currProg);
				pthread_mutex_unlock(&oLockInput);
			}
			/************************
			END OF WHILE LOOP
			*************************/
			pthread_mutex_lock(&oLockInput);
			file[currProg].getline(input, sizeof input);
			pthread_mutex_unlock(&oLockInput);
			lineNumber++;
		}
		pthread_mutex_unlock(&oLockOutput);
		pThis->delay(40);
	}
	// Thread vrijgeven
	pthread_exit(NULL);
};

