// Blake Kusky
// Fund Comp
// crossfunc.h

#define BSIZE 15
#include <string.h>

typedef struct {
	char *word;
	int row_val;
	int col_val;
	int vert;
} Word_data ;

void sol_board_start(char fboard[BSIZE][BSIZE]);
void puz_board_start(char fboard[BSIZE][BSIZE]);
void disp_board(char fboard[BSIZE][BSIZE]);
int get_word(char *words[]);
bool is_word(char word[]);
void word_sort(char *words[], int size);
void make_words_upper(char *words[], int size);
void insert_first_word(char word[], char fboard[BSIZE][BSIZE], Word_data arr[]);
bool insert_word(char word[], char fboard[BSIZE][BSIZE], Word_data arr[], int word_num);
bool valid_intersection(char word[], int index, char fboard[BSIZE][BSIZE], int vert, int pos[2]);
void put_word_on(char word[], int index, char fboard[BSIZE][BSIZE], int vert, int pos[2], Word_data arr[], int word_num);
void update_puz_board(char fsol_b[BSIZE][BSIZE], char fpuz_b[BSIZE][BSIZE]);
void display_anagram(Word_data arr[], int count);
void create_anagram(char word[]);
bool get_file_words(char file_name[], char *words[], int *pcount);
bool open_out_file(char file_name[]);
void disp_out_board(char fboard[BSIZE][BSIZE], FILE *ffp);
void disp_out_ana(Word_data arr[], int count, FILE *ffp);
void create_out_anagram(char word[], FILE *ffp);
