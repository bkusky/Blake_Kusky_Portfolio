// Blake Kusky
// Fund Comp
// crossword.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "crossfunc.h"
#define BSIZE 15

int main(int argc, char *argv[])
{
	char sol_board[BSIZE][BSIZE];
	char puz_board[BSIZE][BSIZE];
	const int word_len = 15;
	const int max_words = 20;
	int count;
	char *words[max_words];

	if (argc == 1) { // if just executable
		sol_board_start(sol_board);
		puz_board_start(puz_board);


		printf("Enter a list of words:\n");
		count = get_word(words);
		Word_data word_arr[count + 1];

		word_sort(words, count);
		make_words_upper(words, count);

		insert_first_word(words[0], sol_board, word_arr);
		

		int i;
		bool check;
		for (i = 1; i < count; i++) {
			check = insert_word(words[i], sol_board, word_arr, i);
			if (check == false) {
				printf("Error placing %s\n", words[i]);
				count = i;
				break;
			}
		}
		update_puz_board(sol_board, puz_board);
		puts("SOLUTION:");
		disp_board(sol_board);
		printf("\n");
		puts("PUZZLE:");
		disp_board(puz_board);
		printf("CLUES:\n");
		display_anagram(word_arr, count);



	} else if (argc == 2) { // if executable + input file
		bool file_check;
		int fcount;

		file_check = get_file_words(argv[1], words, &fcount);
		if (file_check == false) {
			return 1;
		}

		sol_board_start(sol_board);
		puz_board_start(puz_board);
		
		Word_data word_arr[fcount + 1];
		word_sort(words, fcount);
		make_words_upper(words, fcount);

		insert_first_word(words[0], sol_board, word_arr);
		

		int i;
		bool check;
		for (i = 1; i < fcount; i++) {
			check = insert_word(words[i], sol_board, word_arr, i);
			if (check == false) {
				printf("Error placing %s\n", words[i]);
				fcount = i;
				break;
			}
		}
		update_puz_board(sol_board, puz_board);
		puts("SOLUTION:");
		disp_board(sol_board);
		printf("\n");
		puts("PUZZLE:");
		disp_board(puz_board);
		printf("CLUES:\n");
		display_anagram(word_arr, fcount);




	} else if (argc == 3) { // if executable + input file + output file
		bool in_file_check;
		bool out_file_check;
		int fcount;

		in_file_check = get_file_words(argv[1], words, &fcount);
		if (in_file_check == false) {
			return 1;
		}
		FILE *fp = fopen(argv[2], "w");
		if (fp == NULL) {
			printf("Output file %s does not exist", argv[2]);
			return 1;
		}

		sol_board_start(sol_board);
		puz_board_start(puz_board);
		
		Word_data word_arr[fcount + 1];
		word_sort(words, fcount);
		make_words_upper(words, fcount);

		insert_first_word(words[0], sol_board, word_arr);
		

		int i;
		bool check;
		for (i = 1; i < fcount; i++) {
			check = insert_word(words[i], sol_board, word_arr, i);
			if (check == false) {
				fprintf(fp, "Cant place word: %s \n", words[i]);
				fcount = i;
				break;
			}
		}
		update_puz_board(sol_board, puz_board);
		
		
		
		fprintf(fp, "SOLUTION:\n");
		disp_out_board(sol_board, fp);
		fprintf(fp, "\n");
		fprintf(fp, "PUZZLE:\n");
		disp_out_board(puz_board, fp);
		fprintf(fp, "CLUES:\n");

		disp_out_ana(word_arr, fcount, fp);
	
		


	} else {
		puts("Invalid number of command line arguments");
		return 2;
	} 


	return 0;
}


				
