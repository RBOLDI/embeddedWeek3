
#include "compiler.hpp"

using namespace std;

//Shared values:
char *progOutput[PROG_NUM];
bool newOutputAvailable = false;

/*
	initializes all statement instructions known to the program
	to their own index in an array of pointers.
*/
void COMPILER_CLASS::getStatements(void){
	//statements:
	strcpy(setLed_stmnt, "setLed");
	strcpy(returnLed_stmnt, "returnLed");
	strcpy(returnKnop_stmnt, "returnKnop");
	strcpy(while_stmnt, "while{");
	strcpy(delay_stmnt, "delay");
	
	statements[SET_LED_STATMNT] 	= setLed_stmnt;
	statements[RETURN_LED_STATMNT] 	= returnLed_stmnt;
	statements[RETURN_KNOP_STATMNT] = returnKnop_stmnt;
	statements[WHILE_STATEMENT] 	= while_stmnt;
	statements[DELAY_STATEMENT]		= delay_stmnt;
}
/*
	initializes all expressions and their values known to the program
	to their own index in an array of pointers.
*/
void COMPILER_CLASS::getExpressions(void){
	//Expressions and they're starting states
	strcpy(led1State_exp,"led1-state"); 	strcpy(led1State_value,"LOW");
	strcpy(led2State_exp,"led2-state"); 	strcpy(led2State_value, "LOW");
	strcpy(led3State_exp, "led3-state"); 	strcpy(led3State_value, "LOW");
	strcpy(knop1State_exp, "knop1-state"); 	strcpy(knop1State_value, "LOW");
	strcpy(knop2State_exp, "knop2-state"); 	strcpy(knop2State_value, "LOW");
	strcpy(knop3State_exp, "knop3-state"); 	strcpy(knop3State_value, "LOW");
	strcpy(knop4State_exp, "knop4-state"); 	strcpy(knop4State_value, "LOW");
	
	expressions[LED1_STATE_EXPR] 	= led1State_exp;
	expressions[LED2_STATE_EXPR] 	= led2State_exp;
	expressions[LED3_STATE_EXPR] 	= led3State_exp;
	expressions[KNOP1_STATE_EXPR] 	= knop1State_exp;
	expressions[KNOP2_STATE_EXPR] 	= knop2State_exp;
	expressions[KNOP3_STATE_EXPR] 	= knop3State_exp;
	expressions[KNOP4_STATE_EXPR] 	= knop4State_exp;
	
	//Starting values:
	expressionValues[LED1_STATE_EXPR] 	= led1State_value;
	expressionValues[LED2_STATE_EXPR] 	= led2State_value;
	expressionValues[LED3_STATE_EXPR] 	= led3State_value;
	expressionValues[KNOP1_STATE_EXPR] 	= knop1State_value;
	expressionValues[KNOP2_STATE_EXPR] 	= knop2State_value;
	expressionValues[KNOP3_STATE_EXPR] 	= knop3State_value;
	expressionValues[KNOP4_STATE_EXPR] 	= knop4State_value;
}
/*
	Initializes all statement instructions and expression known to the program
*/
void COMPILER_CLASS::compilerInit(uint16_t progNum){
	oProgNum = progNum;
	getStatements();
	getExpressions();

}
/*
	Loops through the array of pointers to statements and compares them to the program input.
	If there is a match, it returns the index of the place where the statement is stored
*/
uint16_t COMPILER_CLASS::instructionIndex(char *input){
	for(uint8_t i = 0; i < sizeof statements; i++){
		if(statements[i] == NULL)
			return INVALID_INPUT;
		if(!strcmp(input, statements[i]))
			return i;
	}
	return INVALID_INPUT;
}
/*
	Loops through the array of pointers to expressions and compares them to the program input.
	If there is a match, it returns the index of the place where the statement is stored
*/
uint16_t COMPILER_CLASS::expressionIndex(char *input){
	for(uint8_t i =0; i < MAX_NUM_TOKENS; i++){
		if(expressions[i] == NULL)
			return INVALID_INPUT;
		if(!strcmp(input, expressions[i]))
			return i;
	}
	return INVALID_INPUT;
}
/*
	Executes the setLed statement.
	BNF: <set-led-statement> ::= setLed <led-num> <led-state-exp>
	Example: setLed 1 HIGH
	this will turn on Led 1
*/
bool COMPILER_CLASS::executeSetLed(char *tokens[MAX_NUM_TOKENS]){
	uint8_t ledGPIO;
	uint8_t ledIndex;
	int state;
	//determine led hardware
	if (!strcmp(tokens[1], "1"))
		ledGPIO = LED1;
	else if (!strcmp(tokens[1], "2"))
		ledGPIO = LED2;
	else if (!strcmp(tokens[1], "3"))
		ledGPIO = LED3;
	else{
		//pthread_mutex_lock(&oLockOutput);
		sprintf(compOutput,"Undefined led...");
		progOutput[oProgNum] = compOutput;
		newOutputAvailable = true;
		//pthread_mutex_unlock(&oLockOutput);
		//cout<<"Undefined led...\n"<<endl;
		return false;
	}
	ledIndex = atoi(tokens[1]) - 1;
	//determine desired state
	if (!strcmp(tokens[2], "HIGH")){
		state = HIGH;
		strcpy(expressionValues[ledIndex], "HIGH");
	}
	else if(!strcmp(tokens[2], "LOW")){
		state = LOW;
		strcpy(expressionValues[ledIndex], "LOW");
	}
	else{
		//pthread_mutex_lock(&oLockOutput);
		sprintf(compOutput,"not a valid led-state...");
		progOutput[oProgNum] = compOutput;
		newOutputAvailable = true;
		//pthread_mutex_unlock(&oLockOutput);
		//cout<<"not a valid led-state...\n"<<endl;
		return false;
	}
	runtimeObjct.setLed(ledGPIO, state, compOutput);
	//pthread_mutex_lock(&oLockOutput);
	progOutput[oProgNum] = compOutput;
	newOutputAvailable = true;
	//cout << progOutput[oProgNum] << endl;
	//pthread_mutex_unlock(&oLockOutput);
	return true;
}
/*
	Executes the returnLed statement.
	BNF: <return-led-statement> ::= returnLed <led-num>
	Example: returnLed 2
	Returns the state of led 2, HIGH if it's turned on, LOW if it's turned off.
*/
bool COMPILER_CLASS::executeReturnLed(char *tokens[MAX_NUM_TOKENS]){
	uint8_t led;
	//determine led hardware
	if (!strcmp(tokens[1], "1"))
		led = LED1;
	else if (!strcmp(tokens[1], "2"))
		led = LED2;
	else if (!strcmp(tokens[1], "3"))
		led = LED3;
	else{
		//pthread_mutex_lock(&oLockOutput);
		sprintf(compOutput,"Undefined led...");
		progOutput[oProgNum] = compOutput;
		newOutputAvailable = true;
		//pthread_mutex_unlock(&oLockOutput);
		//cout<<"Undefined led...\n"<<endl;
		return false;
	}
	runtimeObjct.pollLed(led, compOutput);
	//pthread_mutex_lock(&oLockOutput);
	progOutput[oProgNum] = compOutput;
	newOutputAvailable = true;
	//cout << progOutput[oProgNum] << endl;
	//pthread_mutex_unlock(&oLockOutput);
	return true;
}
/*
	Executes the returnKnop statement.
	BNF: <return-knop-statement> ::= returnKnop <knop-num>
	Example: returnKnop 4
	Returns the state of knop 4, HIGH if it's pressed in, LOW if not.
*/
bool COMPILER_CLASS::executeReturnKnop(char *tokens[MAX_NUM_TOKENS]){
	uint8_t buttonGPIO;
	uint8_t buttonIndex;
	uint8_t state;
	//determine knop hardware
	if (!strcmp(tokens[1], "1"))
		buttonGPIO = BUTTON1;
	else if (!strcmp(tokens[1], "2"))
		buttonGPIO = BUTTON2;
	else if (!strcmp(tokens[1], "3"))
		buttonGPIO = BUTTON3;
	else if (!strcmp(tokens[1], "4"))
		buttonGPIO = BUTTON4;
	else{
		//pthread_mutex_lock(&oLockOutput);
		sprintf(compOutput,"Undefined knop...");
		progOutput[oProgNum] = compOutput;
		//pthread_mutex_unlock(&oLockOutput);
		newOutputAvailable = true;
		//cout<<"Undefined knop...\n"<<endl;
		return false;
	}
	buttonIndex = atoi(tokens[1])+2;
	state = runtimeObjct.pollButton(buttonGPIO, compOutput);
	//pthread_mutex_lock(&oLockOutput);
	progOutput[oProgNum] = compOutput;
	newOutputAvailable = true;
	//cout << progOutput[oProgNum] << endl;
	//pthread_mutex_unlock(&oLockOutput);
	
	if(state == HIGH)
		strcpy(expressionValues[buttonIndex], "HIGH");
	else
		strcpy(expressionValues[buttonIndex], "LOW");
	return true;
}
/*
	Checks if the conditions set for the while loop are true. 
	BNF: 	<while-statement> ::= while{ <expression> <relation-operators> <desired-state> do
			<statement> 
			}
	First it finds out what statement it has to compare. then it finds out how to compare it through
	the relation operator. Then it compares the expression acording to the relation operator to the 
	desired state
*/
bool COMPILER_CLASS::checkWhileConditions(char *expression, char *relationOperator, char *desiredState, uint16_t lineNumber){	
	//cout<<"knop1-state = "<<expressionValues[KNOP1_STATE_EXPR]<<endl;
	uint16_t expIndex = expressionIndex(expression);
	if(!strcmp(relationOperator, "==")){
		if(!strcmp(expressionValues[expIndex], desiredState)){
			//pthread_mutex_lock(&oLockOutput);
			sprintf(compOutput,"Expression is the desired state! (==)");
			progOutput[oProgNum] = compOutput;
			newOutputAvailable = true;
			//pthread_mutex_unlock(&oLockOutput);
			//cout<<"Expression is the desired state! (==)"<<endl;
			return true;
		}
		else return false;
	}
	if(!strcmp(relationOperator, "!=")){
		if(strcmp(expressionValues[expIndex], desiredState)){
			//pthread_mutex_lock(&oLockOutput);
			sprintf(compOutput,"Expression is not the desired state! (!=)");
			progOutput[oProgNum] = compOutput;
			newOutputAvailable = true;
			//pthread_mutex_unlock(&oLockOutput);
			//cout<<"Expression is not the desired state! (!=)"<<endl;
			return true;
		}
		else return false;
	}
	//If expression should (not) be the same as desired state, just compare them as strings, else convert them to intergers for computing
	int currentState_int = atoi(expressionValues[expIndex]);
	int desiredState_int = atoi(desiredState);
	if(!strcmp(relationOperator, "<=")){
		if(currentState_int <= desiredState_int){
			//pthread_mutex_lock(&oLockOutput);
			sprintf(compOutput,"Expression is not the desired state! (<=)");
			progOutput[oProgNum] = compOutput;
			newOutputAvailable = true;
			//pthread_mutex_unlock(&oLockOutput);
			//cout<<"expression is smaller than or equal to desired state! (<=)"<<endl;
			return true;
		}
		else return false;
	}
	if(!strcmp(relationOperator, ">=")){
		if(currentState_int >= desiredState_int){
			//pthread_mutex_lock(&oLockOutput);
			sprintf(compOutput,"Expression is not the desired state! (>=)");
			progOutput[oProgNum] = compOutput;
			newOutputAvailable = true;
			//pthread_mutex_unlock(&oLockOutput);
			//cout<<"expression is bigger than or equal to desired state! (>=)"<<endl;
			return true;
		}
		else return false;
	}
	if(!strcmp(relationOperator, "<")){
		if(currentState_int < desiredState_int){
			//pthread_mutex_lock(&oLockOutput);
			sprintf(compOutput,"Expression is not the desired state! (<)");
			progOutput[oProgNum] = compOutput;
			newOutputAvailable = true;
			//pthread_mutex_unlock(&oLockOutput);
			//cout<<"expression is smaller than desired state! (<)"<<endl;
			return true;
		}
		else return false;
	}
	if(!strcmp(relationOperator, ">")){
		if(currentState_int > desiredState_int){
			//pthread_mutex_lock(&oLockOutput);
			sprintf(compOutput,"Expression is not the desired state! (>)");
			progOutput[oProgNum] = compOutput;
			newOutputAvailable = true;
			//pthread_mutex_unlock(&oLockOutput);
			//cout<<"expression is bigger than desired state! (>)"<<endl;
			return true;
		}
		else return false;
	}
	//pthread_mutex_lock(&oLockOutput);
	sprintf(compOutput,"incorrect usage of while on line: %u", lineNumber);
	progOutput[oProgNum] = compOutput;
	newOutputAvailable = true;
	//pthread_mutex_unlock(&oLockOutput);
	//cout << "incorrect usage of while on line: " << (unsigned)lineNumber<< endl;
	return false;
}
/*
	Executes delay statement
	BNF: <delay-statemnt> ::> delay <miliseconds>
	delays program with certain amount of miliseconds
*/
bool COMPILER_CLASS::executeDelay(char *tokens[]){
	uint16_t ms = atoi(tokens[1]);
	runtimeObjct.delayRPI(ms, compOutput);
	//pthread_mutex_lock(&oLockOutput);
	//cout << progOutput[oProgNum] << endl;
	progOutput[oProgNum] = compOutput;
	newOutputAvailable = true;
	//pthread_mutex_unlock(&oLockOutput);
	
	return true;
}
/*
	Executes the input of the program.
	First it finds out which statement it has to execute. Then it checks if it has 
	the right amount of arguments en gives error messages if not.
*/
uint8_t COMPILER_CLASS::executeInstruction(char *tokens[MAX_NUM_TOKENS], uint16_t lineNumber){
	uint16_t instruction = instructionIndex(tokens[0]);
	if(!strcmp(tokens[0], "}")){
		//pthread_mutex_lock(&oLockOutput);
		sprintf(compOutput,"Error... unexpected end of while function, check line: %u", lineNumber);
		progOutput[oProgNum] = compOutput;
		newOutputAvailable = true;
		//pthread_mutex_unlock(&oLockOutput);
		//cout<<"Error... unexpected end of while function, check line: "<<(unsigned) lineNumber<<endl;
		return 0;
	}
	if(instruction == INVALID_INPUT){
		//pthread_mutex_lock(&oLockOutput);
		sprintf(compOutput,"Error...No valid input, check line:  %u", lineNumber);
		progOutput[oProgNum] = compOutput;
		newOutputAvailable = true;
		//pthread_mutex_unlock(&oLockOutput);
		//cout<<"Error...No valid input, check line: "<<(unsigned) lineNumber<<endl;
		return 0;
	}
	if(instruction == SET_LED_STATMNT){
		if(tokens[1] == NULL){
			//pthread_mutex_lock(&oLockOutput);
			sprintf(compOutput,"Error...Check line:  %u", lineNumber);
			progOutput[oProgNum] = compOutput;
			newOutputAvailable = true;
			//pthread_mutex_unlock(&oLockOutput);
			//cout<<"Error...Check line: "<<(unsigned) lineNumber<<endl;
			return 0;
		}
		executeSetLed(tokens);
		return instruction;
	}
	if(instruction == RETURN_LED_STATMNT){
		if(tokens[1] == NULL){
			//pthread_mutex_lock(&oLockOutput);
			sprintf(compOutput,"Error...Check line:  %u", lineNumber);
			progOutput[oProgNum] = compOutput;
			newOutputAvailable = true;
			//pthread_mutex_unlock(&oLockOutput);
			//cout<<"Error...Check line: "<<(unsigned) lineNumber<<endl;
			return 0;
		}
		executeReturnLed(tokens);
		return instruction;
	}
	if(instruction == RETURN_KNOP_STATMNT){
		if(tokens[1] == NULL){
			//pthread_mutex_lock(&oLockOutput);
			sprintf(compOutput,"Error...Check line:  %u", lineNumber);
			progOutput[oProgNum] = compOutput;
			newOutputAvailable = true;
			//pthread_mutex_unlock(&oLockOutput);
			//cout<<"Error...Check line: "<<(unsigned) lineNumber<<endl;
			return 0;
		}
		executeReturnKnop(tokens);
		return instruction;
	}
	if(instruction == WHILE_STATEMENT){
		if(tokens[1] == NULL){
			//pthread_mutex_lock(&oLockOutput);
			sprintf(compOutput,"Error...Check line:  %u", lineNumber);
			progOutput[oProgNum] = compOutput;
			newOutputAvailable = true;
			//pthread_mutex_unlock(&oLockOutput);
			//cout<<"Error...Check line: "<<(unsigned) lineNumber<<endl;
			return 0;
		}
		//executeWhile(tokens, lineNumber);
		return instruction;
	}
	if(instruction == DELAY_STATEMENT){
		if(tokens[1] == NULL){
			//pthread_mutex_lock(&oLockOutput);
			sprintf(compOutput,"Error...no time to delay, check line: %u", lineNumber);
			progOutput[oProgNum] = compOutput;
			newOutputAvailable = true;
			//pthread_mutex_unlock(&oLockOutput);
			//cout<<"Error...no time to delay, check line: "<<(unsigned) lineNumber<<endl;
			return 0;
		}
		executeDelay(tokens);
		return instruction;
	}
	return false;
}

void COMPILER_CLASS::closeProgram(void){
	runtimeObjct.closeBcm();
}