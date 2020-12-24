#ifndef _COMPILER_HPP_
#define _COMPILER_HPP_

#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include <fstream>
#include <limits>
#include <pthread.h>
#include "parser.hpp"
#include "runtime.hpp"


//Statement indexes:
#define SET_LED_STATMNT		0
#define RETURN_LED_STATMNT	1
#define RETURN_KNOP_STATMNT	2
#define WHILE_STATEMENT		3
#define DELAY_STATEMENT		4
//Expression indexes:
#define LED1_STATE_EXPR		0
#define LED2_STATE_EXPR		1
#define LED3_STATE_EXPR		2
#define KNOP1_STATE_EXPR	3
#define KNOP2_STATE_EXPR	4
#define KNOP3_STATE_EXPR	5
#define KNOP4_STATE_EXPR	6
//Invalid input index:
#define INVALID_INPUT		999
//Programs that make use of the compiler:
#define PROG_1				0
#define PROG_2				1
#define PROG_NUM			2
//Output shared by programs:
extern bool newOutputAvailable;
extern char *progOutput[PROG_NUM];

class COMPILER_CLASS{
	public:
		void compilerInit(uint16_t prog);
		uint8_t executeInstruction(char *tokens[MAX_NUM_TOKENS], uint16_t lineNumber);
		void closeProgram(void);
		bool checkWhileConditions(char *expression, char *relationOperator, char *desiredState, uint16_t lineNumber);
		bool ledState;
		char compOutput[MAX_TOKEN_SIZE];
			
	private:
	//execute instruxtion function:
		bool executeReturnKnop(char *tokens[MAX_NUM_TOKENS]);
		bool executeReturnLed(char *tokens[MAX_NUM_TOKENS]);
		bool executeSetLed(char *tokens[MAX_NUM_TOKENS]);
		bool executeDelay(char *tokens[MAX_NUM_TOKENS]);
		//while:
		bool executeWhile(char *tokens[], uint16_t lineNumber);
		void skipToLine(uint16_t lineNumber);
		bool executeLoop_flag = false;
	//Statements:
		uint16_t instructionIndex(char *input);
		void getStatements(void);
		char *statements[MAX_NUM_TOKENS];
		
		char setLed_stmnt[MAX_TOKEN_SIZE];
		char returnLed_stmnt[MAX_TOKEN_SIZE];
		char returnKnop_stmnt[MAX_TOKEN_SIZE];
		char while_stmnt[MAX_TOKEN_SIZE];
		char delay_stmnt[MAX_TOKEN_SIZE];
	//Epressions:
		uint16_t expressionIndex(char *input);
		void getExpressions(void);
		char *expressions[MAX_NUM_TOKENS];
		char *expressionValues[MAX_NUM_TOKENS];
		
		char led1State_exp[MAX_TOKEN_SIZE]; 	char led1State_value[MAX_TOKEN_SIZE];
		char led2State_exp[MAX_TOKEN_SIZE]; 	char led2State_value[MAX_TOKEN_SIZE];
		char led3State_exp[MAX_TOKEN_SIZE]; 	char led3State_value[MAX_TOKEN_SIZE];
		char knop1State_exp[MAX_TOKEN_SIZE]; 	char knop1State_value[MAX_TOKEN_SIZE];
		char knop2State_exp[MAX_TOKEN_SIZE]; 	char knop2State_value[MAX_TOKEN_SIZE];
		char knop3State_exp[MAX_TOKEN_SIZE]; 	char knop3State_value[MAX_TOKEN_SIZE];
		char knop4State_exp[MAX_TOKEN_SIZE]; 	char knop4State_value[MAX_TOKEN_SIZE];
	//Program
		uint16_t oProgNum;
	//Classed:
		RUNTIME_CLASS 		runtimeObjct;
};

#endif