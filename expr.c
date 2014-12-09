#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define STR_LENGTH 100
#define PREFIX 1
#define INFIX 2
#define POSTFIX 3

char* clearBrackets( char* );
char* infixToPostfix( char* );
char* infixToPrefix( char* );
char* postfixToX( char*, int );
char* prefixToX( char*, int );
char* removeSpaces( char* );
char* reverse( char* );
char* stackPop( void );
int checkStringNotation( char* );
int imbalancedParentheses( char* );
int precedence( char );
int stackLength( void );
void stackDisplay( void );
void stackPush( char* );

typedef struct node {
	char* string;
	struct node* next;
} node;

node* top;

int main(void) {
	char* inputString;
	int choice;
	int type;
	inputString = (char*)malloc(sizeof(char) * STR_LENGTH);

	printf("Enter an expression (use single letters for variables): ");
	fgets(inputString, STR_LENGTH, stdin);

	// remove spaces to normalize string
	inputString = removeSpaces(inputString);

	// check for imba parentheses
	if ( imbalancedParentheses(inputString) ) {
		printf("\n");
		return 1;
	}

	switch( checkStringNotation(inputString) ) {
		case INFIX:
			printf("--------------------------------\n");
			printf("You entered an infix string. Options:\n");
			printf("1) Convert to postfix string\n");
			printf("2) Convert to prefix string\n");
			printf("3) Convert to both postfix and prefix strings\n");
			printf("Enter you choice: ");

			scanf("%d", &choice);

			if ( choice == 1 ) {
				printf("Postfix notation: %s\n", infixToPostfix(inputString));
			}
			else if ( choice == 2 ) {
				printf("Prefix notation: %s\n", infixToPrefix(inputString));
			}
			else if ( choice == 3) {
				printf("Postfix notation: %s\n", infixToPostfix(inputString));
				printf("Prefix notation: %s\n", infixToPrefix(inputString));
			}
			else {
				printf("Invalid choice.\n");
			}

			break;
		case PREFIX:
			printf("--------------------------------\n");
			printf("It seems like you entered a prefix string. Options:\n");
			printf("1) Convert to infix string\n");
			printf("2) Convert to postfix string\n");
			printf("3) Convert to both infix and postfix strings\n");
			printf("Enter you choice: ");

			scanf("%d", &choice);

			if ( choice == 1 ) {
				printf("Infix notation: %s\n", prefixToX(inputString, INFIX));
			}
			else if ( choice == 2 ) {
				printf("Prefix notation: %s\n", prefixToX(inputString, POSTFIX));
			}
			else if ( choice == 3) {
				printf("Infix notation: %s\n", prefixToX(inputString, INFIX));
				printf("Prefix notation: %s\n", prefixToX(inputString, POSTFIX));
			}
			else {
				printf("Invalid choice.\n");
			}

			break;
		case POSTFIX:
			printf("--------------------------------\n");
			printf("It seems like you entered a postfix string. Options:\n");
			printf("1) Convert to infix string\n");
			printf("2) Convert to prefix string\n");
			printf("3) Convert to both infix and prefix strings\n");
			printf("Enter your choice: ");

			scanf("%d", &choice);

			if ( choice == 1 ) {
				printf("Infix notation: %s\n", postfixToX(inputString, INFIX));
			}
			else if ( choice == 2 ) {
				printf("Prefix notation: %s\n", postfixToX(inputString, PREFIX));
			}
			else if ( choice == 3) {
				printf("Infix notation: %s\n", postfixToX(inputString, INFIX));
				printf("Prefix notation: %s\n", postfixToX(inputString, PREFIX));
			}
			else {
				printf("Invalid choice.\n");
			}

			break;
	}

	printf("\n");
	return 0;
}

/******** START OF STACK OPERATIONS ********/

/**
 * @brief Pushes a string into the global stack
 * @details Pushes a string into the global stack in a linked list manner
 * 
 * @param s Input string
 */
void stackPush( char* s ) {
	// init node and new string
	node* n;
	char* t;
	// assign mem
	t = (char*)malloc(sizeof(char)*STR_LENGTH);
	n = (node*)malloc(sizeof(node));

	// copy string to buffer
	strcpy(t, s);

	// set node string as the buffer value
	n->string = t;
	// set next node to point to null
	n->next = 0;

	// if the top of the global stack is none
	if ( top == 0 ) {
		// set top as current node
		top = n;
	}
	else {
		// push current top node down and set this node as top of stack
		n->next = top;
		top = n;
	}
}

/**
 * @brief Pop a string from stack
 * @details Pop out a string from stack removing it from stack and
 * 			returning it to the calling function
 * 
 * @return s String value of top node in stack
 */
char* stackPop( void ) {
	// init output string
	char* s;
	s = (char*)malloc(sizeof(char)*STR_LENGTH);
	// init node to be removed
	node *p;

	// if top is null
	if ( top == 0 ) {
		printf("Stack is empty\n");
	}
	// otherwise
	else {
		// copy topmost string into output
		strcpy(s, top->string);
		// top to be removed
		p = top;
		// printf("stackPopped %s\n", s);
		// top takes the next items place
		top = top->next;
		// free removed item
		free(p);
	}

	return s;
}

/**
 * @brief Get stack length
 * @details Get stack length
 * 
 * @return length Length of the stack
 */
int stackLength( void ) {
	// length counter
	int length = 0;
	// node pointer
	node* p;

	// check from top of stack until end
	for( p = top; p != 0; p = p->next ) {
		// increment as you traverse
		length++;
	}

	// yeah
	return length;
}

/**
 * @brief Print out whole stack (used for debugging only)
 * @details Traverse the whole stack and display each string
 */
void stackDisplay( void ) {
	// node pointer
	node* p;

	// printf stack
	printf("\nStack:\n");
	// print each item as you traverse
	for( p = top; p != 0; p = p->next ) {
		printf("%s\n", p->string);
	}
}

/******** END OF STACK OPERATIONS ********/


/**
 * @brief Infix to Postfix string converter
 * @details This method takes in an infix notation string and converts
 * 			it into a postfix string by checking the operators. Stack
 * 			is required and is instantiated in this scope.
 * 	
 * @param s Input string
 * @return out Output string
 */
char* infixToPostfix( char* s ) {
	// initialize counter variables
	int i;
	int j;
	// output string
	char* out = malloc(sizeof(char) * STR_LENGTH);
	// operator stack
	char* opStack = malloc(sizeof(char) * STR_LENGTH);

	// for each character in the infix string
	for( i = 0; i < strlen(s); i++ ) {
		// check the characters
		// printf("Checking: %c\n", s[i]);
		switch(s[i]) {
			// for operators
			case '^':
			case '/':
			case '*':
			case '+':
			case '-':
				// if left parentheses is at the top of stack
				if ( opStack[strlen(opStack) - 1] == '(' ) {
					// stackPush current operator to top of stack
					opStack[strlen(opStack)] = s[i];
				}
				// anything else
				else {
					// do for all the operators in the stack
					for ( j = strlen(opStack) - 1; j >= 0; j-- ) {
						// if left parentheses is found
						// printf("opStack[j] is looking at: %c\n", opStack[j]);
						if ( opStack[j] == '(') {
							// proceed to stackPushing into stack
							break;
						}
						// if a power-of character found and the precedence of top operator in 
						// stack is less than precedence of power-of
						else if ( (s[i] == '^') && (precedence(s[i]) < precedence(opStack[j])) ) {
							// stackPop top operator in stack and append to postfix string
							out[strlen(out)] = opStack[j];
							opStack[j] = '\0';
						}
						// other if an operator has a prencedence value equal to or less than
						// top operator in stack 
						else if ( precedence(s[i]) <= precedence(opStack[j]) ) {
							// stackPop top operator in stack and append to postfix string
							out[strlen(out)] = opStack[j];
							opStack[j] = '\0';
						}
					}
					// stackPush operator into stack
					opStack[strlen(opStack)] = s[i];
				}
				break;

			// for left parentheses
			case '(':
				// stackPush to operator stack
				opStack[strlen(opStack)] = s[i];
				break;

			// for right parentheses
			case ')':
				// for all items in stack
				for ( j = strlen(opStack); j >= 0; j-- ) {
					// if left parentheses found
					if ( opStack[j] == '(') {
						// stackPop from stack and exit
						opStack[j] = '\0';
						break;
					}
					else {
						// stackPop operator out of stack into postfix string
						out[strlen(out)] = opStack[j];
						opStack[j] = '\0';
					}
				}
				break;

			// generic characters
			default: 
				// append to postfix string
				out[strlen(out)] = s[i];
				break;
		}
		// printf("Out: %s\n", out);
		// printf("Op stack (%d): %s\n\n", strlen(opStack), opStack);
	}

	for ( i = strlen(opStack) - 1; i >= 0; i--)
		out[strlen(out)] = opStack[i];

	return out;
}

/**
 * @brief Infix to Prefix string converter
 * @details This method takes in an infix notation string and converts
 * 			it into a prefix string through a reverse-postfix-reverse
 * 			algorithm.
 * 
 * @param s Input string
 * @return out Output string
 */
char* infixToPrefix( char* s ) {
	// output string
	char* out = (char*)malloc(sizeof(char)*STR_LENGTH);

	// reverse string
	out = reverse(s);
	// perform a postfix conversion
	out = infixToPostfix(out);
	// reverse string
	out = reverse(out);

	return out;
}

/**
 * @brief Postfix to Prefix/Infix Converter
 * @details Converts a postfix notation string to prefix or infix
 * 			given an output format. Uses the global stack
 * 
 * @param s Input string
 * @param outputFormat Format to be converted into (PREFIX/INFIX)
 * 
 * @return out Converted string
 */
char* postfixToX( char* s, int outputFormat ) {
	// counter
	int i;
	
	// output string and buffer for characters
	char* out = (char*)malloc(sizeof(char)*STR_LENGTH);
	char* buffer = (char*)malloc(sizeof(char)*STR_LENGTH);
	// two extra strings to be used during popping
	char* stringA = (char*)malloc(sizeof(char)*STR_LENGTH);
	char* stringB = (char*)malloc(sizeof(char)*STR_LENGTH);

	// for each character
	for ( i = 0; i < strlen(s); i++ ) {
		// check character
		switch (s[i]) {
			// for operators
			case '^':
			case '/':
			case '*':
			case '+':
			case '-':
				// empty output string
				strcpy(out,"");
				// copy current char into string buffer
				sprintf(buffer, "%c", s[i]);
				// if there are less than two items in array, somethings wrong
				if ( stackLength() < 2 ) {
					return "Invalid infix string. Check your operators and operands.\n";
				}
				// otherwise
				else {
					if ( outputFormat == INFIX ) {
						// pop out top 2 items
						strcpy(stringA, stackPop());
						strcpy(stringB, stackPop());
						// append left bracket to output string
						strcat(out, "(");
						// arrange output to be <operand2> <operator> <operand1>
						strcat(out,stringB);
						strcat(out,buffer);
						strcat(out,stringA);
						// append right bracket
						strcat(out, ")");
						// push back into stack
						stackPush(out);
					}
					else if ( outputFormat == PREFIX ) {
						// pop out top 2 items
						strcpy(stringA, stackPop());
						strcpy(stringB, stackPop());
						// arrange output to be <operator> <operand2> <operand1>
						strcat(out,buffer);
						strcat(out,stringB);
						strcat(out,stringA);
						// push back into stack
						stackPush(out);
					}
				}
				break;

			// for everything else (operands)
			default:
				sprintf(buffer,"%c",s[i]);
				stackPush(buffer);
				break;
		}
	}

	// if there is anything left after all is done, somethings wrong
	if (stackLength() > 1) {
		return "Invalid infix string. Check your operators and operands.\n";
	}
	// otherwise remove outside brackets and output infix string
	else {
		if ( outputFormat == INFIX )
			return clearBrackets(stackPop());
		else
			return stackPop();
	}
}

/**
 * @brief Prefix to Postfix/Infix Converter
 * @details Converts a prefix notation string to postfix or infix
 * 			given an output format. Uses the global stack
 * 
 * @param s Input string
 * @param outputFormat Format to be converted into (POSTFIX/INFIX)
 * 
 * @return out Converted string
 */
char* prefixToX( char* s, int outputFormat ) {
	// counter
	int i;
	
	// output string and buffer for characters
	char* out = (char*)malloc(sizeof(char)*STR_LENGTH);
	char* buffer = (char*)malloc(sizeof(char)*STR_LENGTH);
	// two extra strings to be used during popping
	char* stringA = (char*)malloc(sizeof(char)*STR_LENGTH);
	char* stringB = (char*)malloc(sizeof(char)*STR_LENGTH);

	// for each character
	for ( i = strlen(s) - 1; i >= 0; i-- ) {
		// check character
		switch (s[i]) {
			// for operators
			case '^':
			case '/':
			case '*':
			case '+':
			case '-':
				// empty output string
				strcpy(out,"");
				// copy current char into string buffer
				sprintf(buffer, "%c", s[i]);
				// if there are less than two items in array, somethings wrong
				if ( stackLength() < 2 ) {
					return "Invalid infix string. Check your operators and operands.\n";
				}
				// otherwise
				else {
					if ( outputFormat == INFIX ) {
						// pop out top 2 items
						strcpy(stringA, stackPop());
						strcpy(stringB, stackPop());
						// append left bracket to output string
						strcat(out, "(");
						// arrange output to be <operand1> <operator> <operand2>
						strcat(out,stringA);
						strcat(out,buffer);
						strcat(out,stringB);
						// append right bracket
						strcat(out, ")");
						// push back into stack
						stackPush(out);
					}
					else if ( outputFormat == POSTFIX ) {
						// pop out top 2 items
						strcpy(stringA, stackPop());
						strcpy(stringB, stackPop());
						// arrange output to be <operand1> <operator> <operand2>
						strcat(out,stringA);
						strcat(out,stringB);
						strcat(out,buffer);
						// push back into stack
						stackPush(out);
					}
				}
				break;

			// for everything else (operands)
			default:
				sprintf(buffer,"%c",s[i]);
				stackPush(buffer);
				break;
		}
	}

	// if there is anything left after all is done, somethings wrong
	if (stackLength() > 1) {
		return "Invalid infix string. Check your operators and operands.\n";
	}
	// otherwise remove outside brackets and output infix string
	else {
		if ( outputFormat == INFIX )
			return clearBrackets(stackPop());
		else 
			return stackPop();
	}
}

/**
 * @brief Remove spaces from input string
 * @details Remove spaces takes in a string and returns the address of
 * 			the new string with the whitespace and newline characters 
 * 			removed
 * 
 * @params s Input string
 * @return out Output string
 */
char* removeSpaces( char* s ) {
	// tokenizer variable
	char* n;
	// output string
	char* out = malloc(sizeof(char) * STR_LENGTH);

	// remove newline at the end of input from getc and sub for EOL
	if (s[strlen(s)-1] == '\n')
		s[strlen(s)-1] = '\0';

	// tokenize input string
	n = strtok(s, " ");
	// initialize new string
	strcpy(out,"");

	do {
		// concat tokenized string
		strcat(out, n);
		// continue tokeninzing
		n = strtok(NULL, " ");
	} while(n != '\0'); // until end

	return out;
}

/**
 * @brief String reverser
 * @details String reverser, parentheses concious
 * 
 * @param s Input string
 * @return r Output reversed string
 */
char* reverse( char* s ) {
	// counter
	int i;
	// output string
	char* r = (char*)malloc(sizeof(char)*STR_LENGTH);
	// for each character starting from the last
	for ( i = strlen(s)-1; i >= 0; i-- ) {
		// flip parentheses if found
		if( s[i] == '(')
			r[strlen(s) - 1 - i] = ')';
		// flip other parentheses if found
		else if( s[i] == ')')
			r[strlen(s) - 1 - i] = '(';
		// otherwise just copy in reverse
		else
			r[strlen(s) - 1 - i] = s[i];
	}
	return r;
}

/**
 * @brief Clear brackets
 * @details Removes parentheses at both ends of a string
 * 
 * @param s Input string
 * @return out Output string with removed parentheses
 */
char* clearBrackets( char* s ) {
	// init output string
	char* out = (char*)malloc(sizeof(char)*STR_LENGTH);
	// if first char is ( and last char is )
	if ( s[0] == '(' && s[strlen(s) - 1] == ')' ) {
		// last char is str terminate
		s[strlen(s) - 1] = '\0';
		// output string points to second item in string
		// (i.e after first open parentheses)
		out = s + 1;
	}

	return out;	
}

/**
 * @brief String notation checker
 * @details Checks the notation of an input string and returns the
 * 
 * 
 * @param s Input string
 * @return integers (INFIX,POSTFIX,PREFIX)
 */
int checkStringNotation( char* s ) {
	if ( s[0] == '+' || s[0] == '-' || s[0] == '*' || s[0] == '/' || s[0] == '^' ) {
		return PREFIX;
	}
	else if ( s[strlen(s)-1] == '+' || s[strlen(s)-1] == '-' || s[strlen(s)-1] == '*' || s[strlen(s)-1] == '/' || s[strlen(s)-1] == '^' ) {
		return POSTFIX;
	}
	else {
		return INFIX;
	}
}

/**
 * @brief Check parentheses balance
 * @details Perform a check on an infix string to check the balance
 * 			of parentheses
 * 
 * @param s Input string
 * @return 1 = True, 0 = False
 */	
int imbalancedParentheses( char* s ) {
	// initialize counters
	int i;
	int lpc = 0;
	int rpc = 0;

	// count left parentheses and right parentheses
	for ( i = 0; i < strlen(s); i++ ) {
		if ( s[i] == '(')
			lpc++;
		if ( s[i] == ')')
			rpc++;
	}

	// if not balanced show message
	if ( rpc > lpc )
		printf("Imbalanced parentheses error. Too many right parentheses.\n");
	else if ( rpc < lpc )
		printf("Imbalanced parentheses error. Too many left parentheses.\n");

	// returns true if not balanced
	return rpc != lpc;
}

/**
 * @brief Precedence returner
 * @details Returns the precedence given an operator
 * 
 * @param op Operator character
 * @return integers
 */
int precedence( char op ) {
	switch(op) {
		case '^': return 3; break;
		case '/':
		case '*': return 2; break;
		default:  return 1; break;
	}
}