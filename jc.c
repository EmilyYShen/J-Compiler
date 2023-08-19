#define MAX_TOKEN_LENGTH 250

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "token.h"
#include "stack.h"


int main(int argc, char* argv[]) {
	// literal: figure out if need const and hiconst
	// ldr into stack

	FILE* j_file;
	FILE* write_asm;
	int length;
	char* j_file_name;
	int i;
	bool tok;
	int low;
	int high;
	int arg;
	int label1 = 1;
	int label2 = 1;
	int label3 = 1;
	int label4 = 1;
	int label5 = 1;
	int if_label = 1;
	Stack* s = Stack_Allocate();

	struct token_st output;

	// check if contains j file
	if (argc <= 1) {
		printf("must contain a file name\n");
		exit(1);
	}

	j_file = fopen(argv[1], "r");

	// generate asm file
	length = strlen(argv[1]);
	char asm_file[length + 2];
	j_file_name = argv[1];


	for (i = 0; i < length - 1; i++) {
		asm_file[i] = j_file_name[i];
	}

	asm_file[length - 1] = 'a';
	asm_file[length] = 's';
	asm_file[length + 1] = 'm';
	asm_file[length + 2] = '\0';

	write_asm = fopen(asm_file, "w+");

	// start reading tokens from asm
	tok = next_token(j_file, &output);


	while (tok) {
		if (output.type == DEFUN) { // good
			next_token(j_file, &output);
			if (output.type == IDENT) {
				fprintf(write_asm, "%s\n", ".CODE");
				fprintf(write_asm, "%s\n", ".FALIGN");

				fprintf(write_asm, "%s\n", output.str);

				fprintf(write_asm, "%s\n", ";; prologue");
				fprintf(write_asm, "%s\n", "STR R7, R6, #-2");
				fprintf(write_asm, "%s\n", "STR R5, R6, #-3");
				fprintf(write_asm, "%s\n", "ADD R6, R6, #-3");
				fprintf(write_asm, "%s\n", "ADD R5, R6, #0");
			}
			else {
				printf("failed to read a valid IDENT");
			}

		}
		else if (output.type == IDENT) { // good	
			fprintf(write_asm, ";; ident\n");
			fprintf(write_asm, "JSR %s\n", output.str);
			fprintf(write_asm, "%s\n", "ADD R6, R6, #-1");

		}
		else if (output.type == RETURN) { // good
			fprintf(write_asm, "%s\n", ";; epilogue");
			//fprintf(write_asm, "%s\n", "LDR R7, R5, #-1");
			//fprintf(write_asm, "%s\n", "ADD R6, R5, #0");
			//fprintf(write_asm, "%s\n", "ADD R6, R6, #3");
			//fprintf(write_asm, "%s\n", "STR R7, R6, #-1");
			//fprintf(write_asm, "%s\n", "LDR R5, R6, #-3 ");
			//fprintf(write_asm, "%s\n", "LDR R7, R6, #-2");
			//fprintf(write_asm, "%s\n\n", "RET");
			fprintf(write_asm, "%s\n", "LDR R0, R6, #0");
			fprintf(write_asm, "%s\n", "STR R0, R5, #2");
			fprintf(write_asm, "%s\n", "ADD R6, R5, #0");
			fprintf(write_asm, "%s\n", "LDR R5, R6, #0");
			fprintf(write_asm, "%s\n", "LDR R7, R6, #1");
			fprintf(write_asm, "%s\n", "ADD R6, R6, #3");
			fprintf(write_asm, "%s\n\n", "RET");

		}
		else if (output.type == PLUS) { // good
			fprintf(write_asm, "%s\n", ";; plus");
			fprintf(write_asm, "%s\n", "LDR R0, R6, #0");
			fprintf(write_asm, "%s\n", "LDR R1, R6, #1");
			fprintf(write_asm, "%s\n", "ADD R0, R0, R1");
			fprintf(write_asm, "%s\n", "ADD R6, R6, #1");
			fprintf(write_asm, "%s\n", "STR R0, R6, #0");

		}
		else if (output.type == MINUS) { // good
			fprintf(write_asm, "%s\n", ";; minus");
			fprintf(write_asm, "%s\n", "LDR R0, R6, #0");
			fprintf(write_asm, "%s\n", "LDR R1, R6, #1");
			fprintf(write_asm, "%s\n", "SUB R0, R0, R1");
			fprintf(write_asm, "%s\n", "ADD R6, R6, #1");
			fprintf(write_asm, "%s\n", "STR R0, R6, #0");

		}
		else if (output.type == MUL) { // good 
			fprintf(write_asm, "%s\n", ";; mul");
			fprintf(write_asm, "%s\n", "LDR R0, R6, #0");
			fprintf(write_asm, "%s\n", "LDR R1, R6, #1");
			fprintf(write_asm, "%s\n", "MUL R0, R0, R1");
			fprintf(write_asm, "%s\n", "ADD R6, R6, #1");
			fprintf(write_asm, "%s\n", "STR R0, R6, #0");

		}
		else if (output.type == DIV) { // good
			fprintf(write_asm, "%s\n", ";; div");
			fprintf(write_asm, "%s\n", "LDR R0, R6, #0");
			fprintf(write_asm, "%s\n", "LDR R1, R6, #1");
			fprintf(write_asm, "%s\n", "DIV R0, R0, R1");
			fprintf(write_asm, "%s\n", "ADD R6, R6, #1");
			fprintf(write_asm, "%s\n", "STR R0, R6, #0");

		}
		else if (output.type == MOD) { // good
			fprintf(write_asm, "%s\n", ";; mod");
			fprintf(write_asm, "%s\n", "LDR R0, R6, #0");
			fprintf(write_asm, "%s\n", "LDR R1, R6, #1");
			fprintf(write_asm, "%s\n", "MOD R0, R0, R1");
			fprintf(write_asm, "%s\n", "ADD R6, R6, #1");
			fprintf(write_asm, "%s\n", "STR R0, R6, #0");

		}
		else if (output.type == AND) { // good
			fprintf(write_asm, "%s\n", ";; and");
			fprintf(write_asm, "%s\n", "LDR R0, R6, #0");
			fprintf(write_asm, "%s\n", "LDR R1, R6, #1");
			fprintf(write_asm, "%s\n", "AND R0, R0, R1");
			fprintf(write_asm, "%s\n", "ADD R6, R6, #1");
			fprintf(write_asm, "%s\n", "STR R0, R6, #0");

		}
		else if (output.type == OR) { // good
			fprintf(write_asm, "%s\n", ";; or");
			fprintf(write_asm, "%s\n", "LDR R0, R6, #0");
			fprintf(write_asm, "%s\n", "LDR R1, R6, #1");
			fprintf(write_asm, "%s\n", "OR R0, R0, R1");
			fprintf(write_asm, "%s\n", "ADD R6, R6, #1");
			fprintf(write_asm, "%s\n", "STR R0, R6, #0");

		}
		else if (output.type == NOT) { // good
			fprintf(write_asm, "%s\n", ";; not");
			fprintf(write_asm, "%s\n", "LDR R0, R6, #0");
			fprintf(write_asm, "%s\n", "NOT R0, R0");
			fprintf(write_asm, "%s\n", "STR R0, R6, #0");

		}
		else if (output.type == LT) { // good
			fprintf(write_asm, "%s\n", ";; lt");
			fprintf(write_asm, "%s\n", "LDR R0, R6, #0");
			fprintf(write_asm, "%s\n", "LDR R1, R6, #1");
			fprintf(write_asm, "%s\n", "CMP R0, R1");
			fprintf(write_asm, "BRn lt_%d\n", label1); // 1
			label1++; // 2
			fprintf(write_asm, "CONST R0,#0\n"); // false
			fprintf(write_asm, "JMP lt_%d\n", label1); // 2
			label1--; // 1
			fprintf(write_asm, "lt_%d\n", label1); // 1
			fprintf(write_asm, "CONST R0, #1\n"); // true
			label1++; // 2
			fprintf(write_asm, "lt_%d\n", label1); // 2
			label1++; // 3
			fprintf(write_asm, "%s\n", "ADD R6, R6, #1");
			fprintf(write_asm, "%s\n", "STR R0, R6, #0");

		}
		else if (output.type == LE) { // good
			fprintf(write_asm, "%s\n", ";; le");
			fprintf(write_asm, "%s\n", "LDR R0, R6, #0");
			fprintf(write_asm, "%s\n", "LDR R1, R6, #1");
			fprintf(write_asm, "%s\n", "CMP R0, R1");
			fprintf(write_asm, "BRnz le_%d\n", label2); // 1
			label2++; // 2
			fprintf(write_asm, "%s #0\n", "CONST R0,"); // false
			fprintf(write_asm, "JMP le_%d\n", label2); // 2
			label2--; // 1
			fprintf(write_asm, "le_%d\n", label2); // 1
			fprintf(write_asm, "%s #1\n", "CONST R0,"); // true
			label2++; // 2
			fprintf(write_asm, "le_%d\n", label2); // 2
			label2++; // 3
			fprintf(write_asm, "%s\n", "ADD R6, R6, #1");
			fprintf(write_asm, "%s\n", "STR R0, R6, #0");

		}
		else if (output.type == EQ) { // good
			fprintf(write_asm, "%s\n", ";; eq");
			fprintf(write_asm, "%s\n", "LDR R0, R6, #0");
			fprintf(write_asm, "%s\n", "LDR R1, R6, #1");
			fprintf(write_asm, "%s\n", "CMP R0, R1");
			fprintf(write_asm, "BRz eq_%d\n", label3); // 1
			label3++; // 2
			fprintf(write_asm, "CONST R0, #0\n"); // false
			fprintf(write_asm, "JMP eq_%d\n", label3); // 2
			label3--; // 1
			fprintf(write_asm, "eq_%d\n", label3); // 1
			fprintf(write_asm, "CONST R0, #1\n"); // true
			label3++; // 2
			fprintf(write_asm, "eq_%d\n", label3); // 2
			label3++; // 3
			fprintf(write_asm, "%s\n", "ADD R6, R6, #1");
			fprintf(write_asm, "%s\n", "STR R0, R6, #0");

		}
		else if (output.type == GE) { // good
			fprintf(write_asm, "%s\n", ";; ge");
			fprintf(write_asm, "%s\n", "LDR R0, R6, #0");
			fprintf(write_asm, "%s\n", "LDR R1, R6, #1");
			fprintf(write_asm, "%s\n", "CMP R0, R1");
			fprintf(write_asm, "BRzp ge_%d\n", label4); // 1
			label4++; // 2
			fprintf(write_asm, "%s #0\n", "CONST R0,"); // false
			fprintf(write_asm, "JMP ge_%d\n", label4); // 2
			label4--; // 1
			fprintf(write_asm, "ge_%d\n", label4); // 1
			fprintf(write_asm, "%s #1\n", "CONST R0,"); // true
			label4++; // 2
			fprintf(write_asm, "ge_%d\n", label4); // 2
			label4++; // 3
			fprintf(write_asm, "%s\n", "ADD R6, R6, #1");
			fprintf(write_asm, "%s\n", "STR R0, R6, #0");

		}
		else if (output.type == GT) { // good
			fprintf(write_asm, "%s\n", ";; gt");
			fprintf(write_asm, "%s\n", "LDR R0, R6, #0");
			fprintf(write_asm, "%s\n", "LDR R1, R6, #1");
			fprintf(write_asm, "%s\n", "CMP R0, R1");
			fprintf(write_asm, "BRp gt_%d\n", label5); // 1
			label5++; // 2
			fprintf(write_asm, "%s #0\n", "CONST R0,"); // false
			fprintf(write_asm, "JMP gt_%d\n", label5); // 2
			label5--; // 1
			fprintf(write_asm, "gt_%d\n", label5); // 1
			fprintf(write_asm, "%s #1\n", "CONST R0,"); // true
			label5++; // 2
			fprintf(write_asm, "gt_%d\n", label5); // 2
			label5++; // 3
			fprintf(write_asm, "%s\n", "ADD R6, R6, #1");
			fprintf(write_asm, "%s\n", "STR R0, R6, #0");

		}
		else if (output.type == IF) { 
			// push value
			push(s, if_label, false);
			fprintf(write_asm, "%s\n", ";; if");
			fprintf(write_asm, "if_%d\n", peek(s));
			fprintf(write_asm, "%s\n", "LDR R0, R6, #0");
			fprintf(write_asm, "%s\n", "ADD R6, R6, #1");
			fprintf(write_asm, "%s\n", "CMPI R0, #0");
			fprintf(write_asm, "BRz else_%d\n", peek(s));
			if_label++;

		}
		else if (output.type == ELSE) {
			fprintf(write_asm, "%s\n", ";; else");
			fprintf(write_asm, "JMP endif_%d\n", peek(s));
			fprintf(write_asm, "else_%d\n", peek(s));
			check_change(s);

		}
		else if (output.type == ENDIF) {
			fprintf(write_asm, "%s\n", ";; endif");
			if (s->back->check == false) {
				fprintf(write_asm, "else_%d\n", peek(s));
			}
			fprintf(write_asm, "endif_%d\n", peek(s));
			// pop stack
			pop(s, peek(s));
		}
		else if (output.type == WHILE) {
			// push value
			push(s, if_label, false);
			fprintf(write_asm, "%s\n", ";; while");
			fprintf(write_asm, "while_%d\n", peek(s));
			fprintf(write_asm, "%s\n", "LDR R0, R6, #0");
			fprintf(write_asm, "%s\n", "ADD R6, R6, #1");
			fprintf(write_asm, "%s\n", "CMPI R0, #0");
			fprintf(write_asm, "BRz endwhile_%d\n", peek(s));
			if_label++;

		}
		else if (output.type == ENDWHILE) {
			fprintf(write_asm, "%s\n", ";; endwhile");
			fprintf(write_asm, "JMP while_%d\n", peek(s));
			//fprintf(write_asm, "BRnp while_%d\n", peek(s));
			fprintf(write_asm, "endwhile_%d\n", peek(s));
			pop(s, peek(s));

		}
		else if (output.type == DROP) { // good
			fprintf(write_asm, "%s\n", ";; drop");
			fprintf(write_asm, "%s\n", "ADD R6, R6, #1");
		}
		else if (output.type == DUP) { // good
			fprintf(write_asm, "%s\n", ";; dup");
			fprintf(write_asm, "%s\n", "LDR R0, R6, #0");
			fprintf(write_asm, "%s\n", "ADD R6, R6, #-1");
			fprintf(write_asm, "%s\n", "STR R0, R6, #0");

		}
		else if (output.type == SWAP) { // good
			fprintf(write_asm, "%s\n", ";; swap");
			fprintf(write_asm, "%s\n", "LDR R0, R6, #0");
			fprintf(write_asm, "%s\n", "LDR R1, R6, #1");
			fprintf(write_asm, "%s\n", "STR R1, R6, #0");
			fprintf(write_asm, "%s\n", "STR R0, R6, #1");

		}
		else if (output.type == ROT) { // good
			fprintf(write_asm, "%s\n", ";; rot");
			fprintf(write_asm, "%s\n", "LDR R0, R6, #0");
			fprintf(write_asm, "%s\n", "LDR R1, R6, #1");
			fprintf(write_asm, "%s\n", "LDR R2, R6, #2");
			fprintf(write_asm, "%s\n", "STR R2, R6, #0");
			fprintf(write_asm, "%s\n", "STR R0, R6, #1");
			fprintf(write_asm, "%s\n", "STR R1, R6, #2");

		}
		else if (output.type == ARG) { // good
			fprintf(write_asm, "%s\n", ";; arg");
			arg = output.arg_no + 2;
			fprintf(write_asm, "LDR R0, R5, #%d\n", arg);
			fprintf(write_asm, "%s\n", "ADD R6, R6, #-1");
			fprintf(write_asm, "%s\n", "STR R0, R6, #0");

		}
		else if (output.type == LITERAL) { // good
			fprintf(write_asm, "%s\n", ";; literal");
			fprintf(write_asm, "%s\n", "ADD R6, R6, #-1");
			if (output.literal_value > 255 || output.literal_value < -256) {
				low = output.literal_value & 0x00FF;
				high = (output.literal_value & 0xFF00) >> 8;
				fprintf(write_asm, "%s #%d\n", "CONST R0,", low);
				fprintf(write_asm, "%s #%d\n", "HICONST R0,", high);

			}
			else {
				fprintf(write_asm, "%s #%d\n", "CONST R0,", output.literal_value);
			}
			fprintf(write_asm, "%s\n", "STR R0, R6, #0");
		}
		else if (output.type == BAD_TOKEN) {
			printf("why");
		}

		tok = next_token(j_file, &output);
	}
	

	Stack_Free(s);
	fclose(j_file);
	fclose(write_asm);

}