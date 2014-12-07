#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define STR_LENGTH 100
#define PREFIX 1
#define INFIX 2
#define POSTFIX 3

char* removeSpaces( char* );
char* reverse( char* );
int precedence( char );
int checkStringNotation( char* );
char* infixToPostfix( char* );
char* infixToPrefix( char* );

int main(void) {
	char* inputString;
	int choice;
	int type;
	inputString = (char*)malloc(sizeof(char) * STR_LENGTH);

	printf("Enter an expression (use letters for variables): ");
	fgets(inputString, STR_LENGTH, stdin);

	// remove spaces to normalize string
	inputString = removeSpaces(inputString);

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
			printf("You entered a prefix string. Options:\n");
			printf("1) Convert to infix string\n");
			printf("2) Convert to postfix string\n");
			printf("3) Convert to both infix and postfix strings\n");
			printf("Enter you choice: ");

			scanf("%d", &choice);

			break;
		case POSTFIX:
			printf("You entered a postfix string. Options:\n");
			printf("1) Convert to infix string\n");
			printf("2) Convert to prefix string\n");
			printf("3) Convert to both infix and prefix strings\n");
			printf("Enter you choice: ");

			scanf("%d", &choice);

			break;
	}

	return 0;
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
					// push current operator to top of stack
					opStack[strlen(opStack)] = s[i];
				}
				// anything else
				else {
					// do for all the operators in the stack
					for ( j = strlen(opStack) - 1; j >= 0; j-- ) {
						// if left parentheses is found
						// printf("opStack[j] is looking at: %c\n", opStack[j]);
						if ( opStack[j] == '(') {
							// proceed to pushing into stack
							break;
						}
						// if a power-of character found and the precedence of top operator in 
						// stack is less than precedence of power-of
						else if ( (s[i] == '^') && (precedence(s[i]) < precedence(opStack[j])) ) {
							// pop top operator in stack and append to postfix string
							out[strlen(out)] = opStack[j];
							opStack[j] = '\0';
						}
						// other if an operator has a prencedence value equal to or less than
						// top operator in stack 
						else if ( precedence(s[i]) <= precedence(opStack[j]) ) {
							// pop top operator in stack and append to postfix string
							out[strlen(out)] = opStack[j];
							opStack[j] = '\0';
						}
					}
					// push operator into stack
					opStack[strlen(opStack)] = s[i];
				}
				break;

			// for left parentheses
			case '(':
				// push to operator stack
				opStack[strlen(opStack)] = s[i];
				break;

			// for right parentheses
			case ')':
				// for all items in stack
				for ( j = strlen(opStack); j >= 0; j-- ) {
					// if left parentheses found
					if ( opStack[j] == '(') {
						// pop from stack and exit
						opStack[j] = '\0';
						break;
					}
					else {
						// pop operator out of stack into postfix string
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