#define MAX_TOKEN_LENGTH 250

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <string.h>
#include <ctype.h>

#include "token.h"

bool next_token(FILE *j_file, token *output) {
	// edge case: null file
	if (j_file == NULL) {
		printf("Couldn't open the file\n");
		exit(1);
	}

	int i;
	int c;
	c = fgetc(j_file);

	if (c == EOF) {
		return false;
	}

	// get next char in FILE
	while (c == ';' || isspace(c) || c == '\n') { // parse 
		if (c == ';') {
			while (c != '\n') { // parse through comments
				c = fgetc(j_file);
			}
			c = fgetc(j_file);
		}
		if (isspace(c)) {
			while (isspace(c)) { 
				c = fgetc(j_file);
			}
		}
		if (c == '\n') {
			c = fgetc(j_file);
		}	
	}
	

	for (i = 0; i < MAX_TOKEN_LENGTH; i++) {
		if (isspace(c)) {
			output -> str[i] = '\0';
			break;
		}
		if (!c) {
			output -> str[i] = '\0';
			break;
		} 
		if (c == EOF) {
			output -> str[i] = '\0';
			break;
		} 
		output -> str[i] = c;
		c = fgetc(j_file);
		
	}


	// arithmetic ops
	if ((output -> str[0]) == '+' && (output -> str[1]) == '\0') { // addition
		output -> type = PLUS;
		return true;
	}
	if ((output -> str[0]) == '-' &&  (output -> str[1]) == '\0') { // subtraction
		output -> type = MINUS;
		return true;
	}
	if ((output -> str[0]) == '*' &&  (output -> str[1]) == '\0') { // multiplication
		output -> type = MUL;
		return true;
	}
	if ((output -> str[0]) == '/' &&  (output -> str[1]) == '\0') { // division
		output -> type = DIV;
		return true;
	}
	if ((output -> str[0]) == '%' &&  (output -> str[1]) == '\0') { // mod
		output -> type = MOD;
		return true;
	}

	// literals
	if (isdigit((output -> str[0])) && (output -> str[1]) != 'x') { // positive decimal number
		i = 0;
		while ((output -> str[i]) != '\0') {
			if (!isdigit((output -> str[i]))) {
				output -> type = BAD_TOKEN;
				return false;
			}
			i++;
		}
		output -> type = LITERAL;
		sscanf(output -> str, "%d", &(output -> literal_value));
		return true;
	}
	if ((output -> str[0]) == '-' && isdigit((output -> str[1]))) { // negative decimal number
		i = 1;
		while ((output -> str[i]) != '\0') {
			if (!isdigit((output -> str[i]))) {
				output -> type = BAD_TOKEN;
				return false;
			}
			i++;
		}
		output -> type = LITERAL;
		sscanf(output -> str, "%d", &(output -> literal_value));
		return true;
	}
	if ((output -> str[0]) == '0' && (output -> str[1]) == 'x') { // hexadecimal number
		i = 2;
		while ((output -> str[i]) != '\0') {
			if (!isdigit((output -> str[i])) && toupper((output -> str[i])) != 'A' && toupper((output -> str[i])) != 'B' 
				&& toupper((output -> str[i])) != 'C' && toupper((output -> str[i])) != 'D' 
				&& toupper((output -> str[i])) != 'E' && toupper((output -> str[i])) != 'F') {
				printf("yay");
				output -> type = BAD_TOKEN;
				return false;
			}
			i++;
		}
		output -> type = LITERAL;
		sscanf(output -> str, "%x", &(output -> literal_value));
		return true;
	}

	// comparison ops
	if ((output -> str[0]) == 'l' && (output -> str[1]) == 't' && (output -> str[2]) == '\0') {
		output -> type = LT;
		return true;
	}
	if ((output -> str[0]) == 'l' && (output -> str[1]) == 'e' && (output -> str[2]) == '\0') {
		output -> type = LE;
		return true;
	}
	if ((output -> str[0]) == 'e' && (output -> str[1]) == 'q' && (output -> str[2]) == '\0') {
		output -> type = EQ;
		return true;
	}
	if ((output -> str[0]) == 'g' && (output -> str[1]) == 'e' && (output -> str[2]) == '\0') {
		output -> type = GE;
		return true;
	}
	if ((output -> str[0]) == 'g' && (output -> str[1]) == 't' && (output -> str[2]) == '\0') {
		output -> type = GT;
		return true;
	}

	// logical ops
	if ((output -> str[0]) == 'a' && (output -> str[1]) == 'n' && (output -> str[2]) == 'd' 
		&& (output -> str[3]) == '\0') {
		output -> type = AND;
		return true;
	}
	if ((output -> str[0]) == 'o' && (output -> str[1]) == 'r' && (output -> str[2]) == '\0') {
		output -> type = OR;
		return true;
	}
	if ((output -> str[0]) == 'n' && (output -> str[1]) == 'o' && (output -> str[2]) == 't' 
		&& (output -> str[3]) == '\0') {
		output -> type = NOT;
		return true;
	}
	
	// stack ops
	if ((output -> str[0]) == 'd' && (output -> str[1]) == 'r' && (output -> str[2]) == 'o' 
		&& (output -> str[3]) == 'p' && (output -> str[4]) == '\0') {
		output -> type = DROP;
		return true;
	}
	if ((output -> str[0]) == 'd' && (output -> str[1]) == 'u' && (output -> str[2]) == 'p' 
		&& (output -> str[3]) == '\0') {
		output -> type = DUP;
		return true;
	}
	if ((output -> str[0]) == 's' && (output -> str[1]) == 'w' && (output -> str[2]) == 'a' 
		&& (output -> str[3]) == 'p' && (output -> str[4]) == '\0') {
		output -> type = SWAP;
		return true;
	}
	if ((output -> str[0]) == 'r' && (output -> str[1]) == 'o' && (output -> str[2]) == 't' 
		&& (output -> str[3]) == '\0') {
		output -> type = ROT;
		return true;
	}

	// argN
	if ((output -> str[0]) == 'a' && (output -> str[1]) == 'r' && (output -> str[2]) == 'g' 
		&& isdigit(output -> str[3]) && (output -> str[4]) == '\0') {
		if ((output -> str[3] - 48) > 20) {
			output -> type = BAD_TOKEN;
			return false;
		}
		output -> type = ARG;
		output -> arg_no = output -> str[3] - 48;
		return true;
	}

	// if statements
	if ((output -> str[0]) == 'i' && (output -> str[1]) == 'f' && (output -> str[2]) == '\0') {
		output -> type = IF;
		return true;
	}
	if ((output -> str[0]) == 'e' && (output -> str[1]) == 'l' && (output -> str[2]) == 's' 
		&& (output -> str[3]) == 'e' && (output -> str[4]) == '\0') {
		output -> type = ELSE;
		return true;
	}
	if ((output -> str[0]) == 'e' && (output -> str[1]) == 'n' && (output -> str[2]) == 'd' 
		&& (output -> str[3]) == 'i' && (output -> str[4]) == 'f' && (output -> str[5]) == '\0') {
		output -> type = ENDIF;
		return true;
	}

	// while statements
	if ((output -> str[0]) == 'w' && (output -> str[1]) == 'h' && (output -> str[2]) == 'i' 
		&& (output -> str[3]) == 'l' && (output -> str[4]) == 'e' && (output -> str[5]) == '\0') {
		output -> type = WHILE;
		return true;
	}
	if ((output -> str[0]) == 'e' && (output -> str[1]) == 'n' && (output -> str[2]) == 'd' 
		&& (output -> str[3]) == 'w' && (output -> str[4]) == 'h' && (output -> str[5]) == 'i' 
		&& (output -> str[6]) == 'l' && (output -> str[7]) == 'e' && (output -> str[8]) == '\0') {
		output -> type = ENDWHILE;
		return true;
	}

	// function
	if ((output -> str[0]) == 'd' && (output -> str[1]) == 'e' && (output -> str[2]) == 'f' 
		&& (output -> str[3]) == 'u' && (output -> str[4]) == 'n' && (output -> str[5]) == '\0') {
		output -> type = DEFUN;
		return true;
	}
	if ((output -> str[0]) == 'r' && (output -> str[1]) == 'e' && (output -> str[2]) == 't' 
		&& (output -> str[3]) == 'u' && (output -> str[4]) == 'r' && (output -> str[5]) == 'n' 
		&& (output -> str[6]) == '\0') {
		output -> type = RETURN;
		return true;
	}
	if (isalpha(output -> str[0])) {
		i = 0;
		while ((output -> str[i]) != '\0') {
			if (!isalpha(output -> str[i]) && (output -> str[i]) != '_' && !isdigit(output -> str[i])) {
				output -> type = BAD_TOKEN;
				return false;
			}
			i++;
		}
		output -> type = IDENT;
		return true;
	}

	output -> type = BAD_TOKEN;

	return false;
}
