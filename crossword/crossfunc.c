// Blake Kusky
// Fund Comp
// crossfunc.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "crossfunc.h"
#define BSIZE 15

void sol_board_start(char fboard[BSIZE][BSIZE]) { // initializes solution board
	int i, j;

	for (i = 0; i < BSIZE; i++) {
		for (j = 0; j < BSIZE; j++) {
			fboard[i][j] = '.';
		}
	}

}

void puz_board_start(char fboard[BSIZE][BSIZE]) { // initializes puzzle board
	int i, j;
	
	for (i = 0; i < BSIZE; i++) {
		for (j = 0; j < BSIZE; j++) {
			fboard[i][j] = '#';
		}
	}
}

void disp_board(char fboard[BSIZE][BSIZE]) { // displays board "fboard" with borders
	int i, j;

	for (i = 0; i <= BSIZE + 1; i++) {
		printf("-");
	}
	printf("\n");

	for (i = 0; i < BSIZE; i++) {
		for (j = 0; j < BSIZE; j++) {
			if (j == 0) {
				printf("|");
				printf("%c", fboard[i][j]);
			} else if (j == BSIZE - 1) {
				printf("%c", fboard[i][j]);
				printf("|\n");
			} else {
				printf("%c", fboard[i][j]);
			}
		}
	}

	for (i = 0; i <= BSIZE + 1; i++) {
		printf("-");
	}
	printf("\n");
}


bool is_word(char word[]) { // checks if input word is within parameters
	int i;
	int len = strlen(word);

	if (len < 2 || len > 15) {
		return false;
	}

	for (i = 0; i < len; i++) {
		if ((word[i] < 65 || word[i] > 90) && (word[i] < 97 || word[i] > 122)){
			return false;
		}
	}
}



int get_word(char *words[]) { // gets words from stdin and puts them in char pointer array
	int count = 0;
	char temp[BSIZE];
	
	while(1) {
		words[count] = malloc(16*sizeof(char));
		printf(" - ");
		scanf("%s", words[count]);
		if (words[count][0] == '.' || count == 19) {
			free(words[count]);
			break;
		} else {
			if (is_word(words[count])) {
				count++;
			} else {
				printf("Not a word. Try again -->");
				free(words[count]);
			}
		}
	}
	return count;
}


void word_sort(char *words[], int size) { // sorts the words by length in decreasing order
	int i, j;
	char *temp;
	for (i = 0; i < size - 1; i++) {
		for (j = 0; j < size - i - 1; j++) {
			if (strlen(words[j]) < strlen(words[j+1])) {
				temp = words[j];
				words[j] = words[j+1];
				words[j+1] = temp; 
			}
		}
	}
}

void make_words_upper(char *words[], int size) { // makes all words uppercase
	int i, j;
	for (i = 0; i < size; i++) {
		for (j = 0; j < strlen(words[i]); j++) {
			words[i][j] = toupper(words[i][j]);
		}
	}

}

void insert_first_word(char word[], char fboard[BSIZE][BSIZE], Word_data arr[]) { // puts the first word on board
	int i;
	int length = strlen(word);
	int start_row = BSIZE/2;
	int start_col = (BSIZE - length) / 2;
	
	int count = 0;
	arr[0].word = word;
	arr[0].vert = 0;
	arr[0].row_val = start_row;
	arr[0].col_val = start_col;
	
	for (i = 0; i < length; i++) {
		fboard[start_row][start_col +i] = word[i];
	}
}

bool insert_word(char word[], char fboard[BSIZE][BSIZE], Word_data arr[], int word_num) { // puts the next longest word on board
	int i, j, k, vert;
	bool check;
	int leave = 0;
	int position[2];

	for (i = 0; i < strlen(word); i++) {
		if (leave == 1) {
			break;
		}
		for (j = 0; j < BSIZE; j++) {
			if (leave == 1) {
				break;
			}
			for (k = 0; k < BSIZE; k++) {
				if (leave == 1) {
					break;
				}
				position[0] = j;
				position[1] = k;
				if (word[i] == fboard[j][k]) {


					if (j == 0) { // if intersection is on top border
						if (fboard[j+1][k] == '.') {
							// word on board is horizontal
							vert = 1;
							check = valid_intersection(word, i, fboard, vert, position);

							if (check == true) {
								leave = 1;
							} 
						} else {
							// word on board is vertical
							vert = 0;
							check = valid_intersection(word, i, fboard, vert, position);

							if (check == true) {
								leave = 1;
							} 
						}
					} else if (j == BSIZE - 1) { // if intersection is on bottom border
						if (fboard[j-1][k] == '.') {
							// word on board is horizontal
							vert = 1;
							check = valid_intersection(word, i, fboard, vert, position);

							if (check == true) {
								leave = 1;
							} 
						} else {
							// word on board is vertical
							vert = 0;
							check = valid_intersection(word, i, fboard, vert, position);

							if (check == true) {
								leave = 1;
							} 
						}
					} else { // all other intersection points
						if (fboard[j-1][k] == '.' && fboard[j+1][k] == '.') {
							// word on board is horizontal
							vert = 1;
							check = valid_intersection(word, i, fboard, vert, position);

							if (check == true) {
								leave = 1;
							} 
						} else {
							// word on board is vertical
							vert = 0;
							check = valid_intersection(word, i, fboard, vert, position);

							if (check == true) {
								leave = 1;
							} 

						}
					}
				}
			}
		}
	}

	if (leave == 1) { // if possible word placement is a valid place for word
		put_word_on(word, i, fboard, vert, position, arr, word_num);
		return true;
	} else {
		return false;
	}

}

bool valid_intersection(char word[], int index, char fboard[BSIZE][BSIZE], int vert, int pos[2]) {
	// checks if possible intersection point for a word is a valid place for the word to be placed
	int i, j;
	int length = strlen(word);
	int dist_past_inter = (length - 1) - index;

	int row_inter = pos[0];
	int col_inter = pos[1];
	
	if (vert == 1) { // If word is vertical
		
		if (row_inter - index < 0) {
			return false;
		} else if (row_inter + dist_past_inter > BSIZE - 1) {
			return false;
		}
		int valid_count = 0;


		if (col_inter == 0) { // if word is on left border

			if (row_inter - index == 0) {
				for (i = row_inter - index; i < row_inter; i++) {
					for (j = col_inter + 1; j <= col_inter + 1; j+=2) {
						if (fboard[i][j] == '.') {
							valid_count++;
						}	
					}
				}
				if (valid_count == index) {
					valid_count = 0;
					for (i = row_inter+1; i < row_inter+dist_past_inter+1; i++) {
						for (j = col_inter + 1; j<= col_inter + 1; j += 2) {
							if (fboard[i][j] == '.') {
								valid_count++;
							}
						}
					}
					if (valid_count == dist_past_inter && length < BSIZE) {
						if (fboard[row_inter + dist_past_inter + 1][col_inter] == '.') {
							return true;
						}
					} else if (valid_count == 2*dist_past_inter) {
						return true;
					} else {
						return false;
					}
				} else {
					return false;
				}

			} else if (row_inter + dist_past_inter == BSIZE - 1 && row_inter - index != 0) {

				if (fboard[row_inter - index - 1][col_inter] == '.') {
					valid_count++;
					for (i = row_inter-index; i < row_inter; i++) {
						for (j = col_inter + 1; j <= col_inter + 1; j+=2) {
							if (fboard[i][j] == '.') {
								valid_count++;
							}
						}
					}
					if (valid_count == index + 1) {
						valid_count = 0;
						for (i = row_inter+1; i < row_inter+dist_past_inter+1; i++) {
							for (j = col_inter + 1; j <= col_inter + 1; j += 2) {
								if (fboard[i][j] == '.') {
									valid_count++;
								}
							}
						}
						if (valid_count == dist_past_inter) {
							return true;
						} else {
							return false;
						}
					} else {
						return false;
					}
				} else {
					return false;
				}
			} else {			
				if (fboard[row_inter - index - 1][col_inter] == '.') {

					valid_count++;

					for (i = row_inter - index; i < row_inter; i++) {
						for (j = col_inter + 1; j <= col_inter + 1; j+=2) {

							if (fboard[i][j] == '.') {
								valid_count++;
							}
						}
					}
					if (valid_count == index + 1) {

						valid_count = 0;
						for (i = row_inter+1; i < row_inter + dist_past_inter +1; i++) {
							for (j = col_inter + 1; j <= col_inter + 1; j+=2) {

								if (fboard[i][j] == '.') {
									valid_count++;
								}
							}
						}
						if (valid_count == dist_past_inter) {

							if (fboard[row_inter + dist_past_inter + 1][col_inter] == '.') {
	
								return true;
							} else {
								return false;
							}
						} else {
							return false;
						}
					} else {
						return false;
					}
				} else {
					return false;
				}
			}
		}


		if (col_inter == BSIZE - 1) { // if word is on right border
			if (row_inter - index == 0) {
				for (i = row_inter - index; i < row_inter; i++) {
					for (j = col_inter - 1; j <= col_inter - 1; j+=2) {
						if (fboard[i][j] == '.') {
							valid_count++;
						}
					}
				}
				if (valid_count == index) {
					valid_count = 0;
					for (i = row_inter+1; i < row_inter+dist_past_inter+1; i++) {
						for (j = col_inter - 1; j<= col_inter - 1; j += 2) {
							if (fboard[i][j] == '.') {
								valid_count++;
							}
						}
					}
					if (valid_count == dist_past_inter && length < BSIZE) {
						if (fboard[row_inter + dist_past_inter + 1][col_inter] == '.') {
							return true;
						}
					} else if (valid_count == dist_past_inter) {
						return true;
					} else {
						return false;
					}
				} else {
					return false;
				}
	
			} else if (row_inter + dist_past_inter == BSIZE - 1 && row_inter - index != 0) {
	
				if (fboard[row_inter - index - 1][col_inter] == '.') {
					valid_count++;
					for (i = row_inter-index; i < row_inter; i++) {
						for (j = col_inter - 1; j <= col_inter - 1; j+=2) {
							if (fboard[i][j] == '.') {
								valid_count++;
							}
						}
					}
					if (valid_count == index + 1) {
						valid_count = 0;
						for (i = row_inter+1; i < row_inter+dist_past_inter+1; i++) {
							for (j = col_inter - 1; j <= col_inter - 1; j += 2) {
								if (fboard[i][j] == '.') {
									valid_count++;
								}
							}
						}
						if (valid_count == dist_past_inter) {
							return true;
						} else {
							return false;
						}
					} else {
						return false;
					}
				} else {
					return false;
				}
			} else {			
				if (fboard[row_inter - index - 1][col_inter] == '.') {

					valid_count++;

					for (i = row_inter - index; i < row_inter; i++) {
						for (j = col_inter - 1; j <= col_inter - 1; j+=2) {

							if (fboard[i][j] == '.') {
								valid_count++;
							}
						}
					}
					if (valid_count == index + 1) {
						valid_count = 0;
						for (i = row_inter+1; i < row_inter + dist_past_inter +1; i++) {
							for (j = col_inter - 1; j <= col_inter - 1; j+=2) {

								if (fboard[i][j] == '.') {
									valid_count++;
								}
							}
						}
						if (valid_count == dist_past_inter) {
							if (fboard[row_inter + dist_past_inter + 1][col_inter] == '.') {

								return true;
							} else {
								return false;
							}
						} else {
							return false;
						}
					} else {
						return false;
					}
				} else {
					return false;
				}
			}	
		}





		
	
		if (row_inter - index == 0) { // if word is on top border
			for (i = row_inter - index; i < row_inter; i++) {
				for (j = col_inter - 1; j <= col_inter + 1; j+=2) {
					if (fboard[i][j] == '.') {
						valid_count++;
					}
				}
			}
			if (valid_count == 2*index) {
				valid_count = 0;
				for (i = row_inter+1; i < row_inter+dist_past_inter+1; i++) {
					for (j = col_inter - 1; j<= col_inter + 1; j += 2) {
						if (fboard[i][j] == '.') {
							valid_count++;
						}
					}
				}
				if (valid_count == 2*dist_past_inter && length < BSIZE) {
					if (fboard[row_inter + dist_past_inter + 1][col_inter] == '.') {
						return true;
					}
				} else if (valid_count == 2*dist_past_inter) {
					return true;
				} else {
					return false;
				}
			} else {
				return false;
			}

		} else if (row_inter + dist_past_inter == BSIZE - 1 && row_inter - index != 0) { // if word is on bottom border

			if (fboard[row_inter - index - 1][col_inter] == '.') {
				valid_count++;
				for (i = row_inter-index; i < row_inter; i++) {
					for (j = col_inter - 1; j <= col_inter + 1; j+=2) {
						if (fboard[i][j] == '.') {
							valid_count++;
						}
					}
				}
				if (valid_count == 2*index + 1) {
					valid_count = 0;
					for (i = row_inter+1; i < row_inter+dist_past_inter+1; i++) {
						for (j = col_inter - 1; j <= col_inter + 1; j += 2) {
							if (fboard[i][j] == '.') {
								valid_count++;
							}
						}
					}
					if (valid_count == 2*dist_past_inter) {
						return true;
					} else {
						return false;
					}
				} else {
					return false;
				}
			} else {
				return false;
			}
		} else { // all other locations on board		
			if (fboard[row_inter - index - 1][col_inter] == '.') {

				valid_count++;

				for (i = row_inter - index; i < row_inter; i++) {
					for (j = col_inter - 1; j <= col_inter + 1; j+=2) {

						if (fboard[i][j] == '.') {
							valid_count++;
						}
					}
				}
				if (valid_count == 2*index + 1) {

					valid_count = 0;
					for (i = row_inter+1; i < row_inter + dist_past_inter +1; i++) {
						for (j = col_inter - 1; j <= col_inter + 1; j+=2) {

							if (fboard[i][j] == '.') {
								valid_count++;
							}
						}
					}
					if (valid_count == 2*dist_past_inter) {

						if (fboard[row_inter + dist_past_inter + 1][col_inter] == '.') {

							return true;
						} else {
							return false;
						}
					} else {
						return false;
					}
				} else {
					return false;
				}
			} else {
				return false;
			}
		}
					



						


	} else { // if word is horizontal
		
		if (col_inter - index < 0) {
			return false;
		} else if (col_inter + dist_past_inter > BSIZE - 1) {
			return false;
		}
		
		int valid_count = 0;

		if (row_inter == 0) { // if word is on top border
			if (col_inter - index == 0) {
				for (i = col_inter - index; i < col_inter; i++) {
					for (j = row_inter + 1; j <= row_inter + 1; j+=2) {
						if (fboard[j][i] == '.') {
							valid_count++;
						}
					}
				}
				if (valid_count == index) {
					valid_count = 0;
					for (i = col_inter+1; i < col_inter+dist_past_inter+1; i++) {
						for (j = row_inter + 1; j<= row_inter + 1; j += 2) {
							if (fboard[j][i] == '.') {
								valid_count++;
							}
						}
					}
					if (valid_count == dist_past_inter && length < BSIZE) {
						if (fboard[row_inter][col_inter + dist_past_inter + 1] == '.') {
							return true;
						}
					} else if (valid_count == dist_past_inter) {
						return true;
					} else {
						return false;
					}
				} else {
					return false;
				}

			} else if (col_inter + dist_past_inter == BSIZE - 1 && col_inter - index != 0) {

				if (fboard[row_inter][col_inter - index - 1] == '.') {
					valid_count++;
					for (i = col_inter-index; i < col_inter; i++) {
						for (j = row_inter + 1; j <= row_inter + 1; j+=2) {
							if (fboard[j][i] == '.') {
								valid_count++;
							}
						}
					}
					if (valid_count == index + 1) {
						valid_count = 0;
						for (i = col_inter+1; i < col_inter+dist_past_inter+1; i++) {
							for (j = row_inter + 1; j <= row_inter + 1; j += 2) {
								if (fboard[j][i] == '.') {
									valid_count++;
								}
							}
						}
						if (valid_count == dist_past_inter) {
							return true;
						} else {
							return false;
						}
					} else {
						return false;
					}
				} else {
					return false;
				}
			} else {
			
				if (fboard[row_inter][col_inter - index - 1] == '.') {
					valid_count++;
					for (i = col_inter - index; i < col_inter; i++) {
						for (j = row_inter + 1; j <= row_inter + 1; j+=2) {

							if (fboard[j][i] == '.') {
								valid_count++;
							}
						}
					}
					if (valid_count == index + 1) {
						valid_count = 0;
						for (i = col_inter+1; i < col_inter+dist_past_inter+1; i++) {
							for (j = row_inter + 1; j <= row_inter + 1; j+=2) {

								if (fboard[j][i] == '.') {
									valid_count++;
								}
							}
						}
						if (valid_count == dist_past_inter) {
							if (fboard[row_inter][col_inter + dist_past_inter + 1] == '.') {
								return true;
							} else {
								return false;
							}	 
						} else {
							return false;
						}
					} else {
						return false;
					}
				} else {
					return false;
				}
			}
		}

        if (row_inter == BSIZE - 1) { // if word is on bottom border
			if (col_inter - index == 0) {
				for (i = col_inter - index; i < col_inter; i++) {
					for (j = row_inter - 1; j <= row_inter - 1; j+=2) {
						if (fboard[j][i] == '.') {
							valid_count++;
						}
					}
				}
				if (valid_count == index) {
					valid_count = 0;
					for (i = col_inter+1; i < col_inter+dist_past_inter+1; i++) {
						for (j = row_inter - 1; j<= row_inter - 1; j += 2) {
							if (fboard[j][i] == '.') {
								valid_count++;
							}
						}
					}
					if (valid_count == dist_past_inter && length < BSIZE) {
						if (fboard[row_inter][col_inter + dist_past_inter + 1] == '.') {
							return true;
						}
					} else if (valid_count == dist_past_inter) {
						return true;
					} else {
						return false;
					}
				} else {
					return false;
				}
	
			} else if (col_inter + dist_past_inter == BSIZE - 1 && col_inter - index != 0) {

				if (fboard[row_inter][col_inter - index - 1] == '.') {
					valid_count++;
					for (i = col_inter-index; i < col_inter; i++) {
						for (j = row_inter - 1; j <= row_inter - 1; j+=2) {
							if (fboard[j][i] == '.') {
								valid_count++;
							}
						}
					}
					if (valid_count == index + 1) {
						valid_count = 0;
						for (i = col_inter+1; i < col_inter+dist_past_inter+1; i++) {
							for (j = row_inter - 1; j <= row_inter - 1; j += 2) {
								if (fboard[j][i] == '.') {
									valid_count++;
								}
							}
						}
						if (valid_count == dist_past_inter) {
							return true;
						} else {
							return false;
						}
					} else {
						return false;
					}
				} else {
					return false;
				}
			} else {
				
				if (fboard[row_inter][col_inter - index - 1] == '.') {
					valid_count++;
					for (i = col_inter - index; i < col_inter; i++) {
						for (j = row_inter - 1; j <= row_inter - 1; j+=2) {
	
							if (fboard[j][i] == '.') {
								valid_count++;
							}
						}
					}
					if (valid_count == index + 1) {
						valid_count = 0;
						for (i = col_inter+1; i < col_inter+dist_past_inter+1; i++) {
							for (j = row_inter - 1; j <= row_inter - 1; j+=2) {
	
								if (fboard[j][i] == '.') {
									valid_count++;
								}
							}
						}
						if (valid_count == dist_past_inter) {
							if (fboard[row_inter][col_inter + dist_past_inter + 1] == '.') {
								return true;
							} else {
								return false;
							} 
						} else {
							return false;
						}
					} else {
						return false;
					}
				} else {
					return false;
				}
			}
		}





		if (col_inter - index == 0) { // if word is on left border
			for (i = col_inter - index; i < col_inter; i++) {
				for (j = row_inter - 1; j <= row_inter + 1; j+=2) {
					if (fboard[j][i] == '.') {
						valid_count++;
					}
				}
			}
			if (valid_count == 2*index) {
				valid_count = 0;
				for (i = col_inter+1; i < col_inter+dist_past_inter+1; i++) {
					for (j = row_inter - 1; j<= row_inter + 1; j += 2) {
						if (fboard[j][i] == '.') {
							valid_count++;
						}
					}
				}
				if (valid_count == 2*dist_past_inter && length < BSIZE) {
					if (fboard[row_inter][col_inter + dist_past_inter + 1] == '.') {
						return true;
					}
				} else if (valid_count == 2*dist_past_inter) {
					return true;
				} else {
					return false;
				}
			} else {
				return false;
			}

		} else if (col_inter + dist_past_inter == BSIZE - 1 && col_inter - index != 0) { // if word is on right border

			if (fboard[row_inter][col_inter - index - 1] == '.') {
				valid_count++;
				for (i = col_inter-index; i < col_inter; i++) {
					for (j = row_inter - 1; j <= row_inter + 1; j+=2) {
						if (fboard[j][i] == '.') {
							valid_count++;
						}
					}
				}
				if (valid_count == 2*index + 1) {
					valid_count = 0;
					for (i = col_inter+1; i < col_inter+dist_past_inter+1; i++) {
						for (j = row_inter - 1; j <= row_inter + 1; j += 2) {
							if (fboard[j][i] == '.') {
								valid_count++;
							}
						}
					}
					if (valid_count == 2*dist_past_inter) {
						return true;
					} else {
						return false;
					}
				} else {
					return false;
				}
			} else {
				return false;
			}
		} else { // all other board locations
			
			if (fboard[row_inter][col_inter - index - 1] == '.') {
				valid_count++;
				for (i = col_inter - index; i < col_inter; i++) {
					for (j = row_inter - 1; j <= row_inter + 1; j+=2) {

						if (fboard[j][i] == '.') {
							valid_count++;
						}
					}
				}
				if (valid_count == 2*index + 1) {
					valid_count = 0;
					for (i = col_inter+1; i < col_inter+dist_past_inter+1; i++) {
						for (j = row_inter - 1; j <= row_inter + 1; j+=2) {

							if (fboard[j][i] == '.') {
								valid_count++;
							}
						}
					}
					if (valid_count == 2*dist_past_inter) {
						if (fboard[row_inter][col_inter + dist_past_inter + 1] == '.') {
							return true;
						} else {
							return false;
						} 
					} else {
						return false;
					}
				} else {
					return false;
				}
			} else {
				return false;
			}

		}

	}

}

void put_word_on(char word[], int index, char fboard[BSIZE][BSIZE], int vert, int pos[2], Word_data arr[], int word_num) {
	// puts the word on the board specified by fboard
	// also puts all data regarding word in the main struct
	int i, j;
	int length = strlen(word);
	int dist_past = (length - 1) - index;
	
	int row_inter = pos[0];
	int col_inter = pos[1];
	int count = 0;
	if (vert == 1) {
		for (i = (row_inter - index) +1; i <= row_inter + dist_past+1; i++) {
			fboard[i][col_inter] = word[count];
			count++;
		}
		arr[word_num].row_val = row_inter - index + 1;
		arr[word_num].col_val = col_inter;

	} else {
		for (i = (col_inter - index) + 1; i <= col_inter + dist_past+1; i++) {
			fboard[row_inter][i] = word[count];
			count++;
		}
		arr[word_num].row_val = row_inter;
		arr[word_num].col_val = col_inter - index + 1;
	}
	
	arr[word_num].word = word;
	arr[word_num].vert = vert;

}

void update_puz_board(char fsol_b[BSIZE][BSIZE], char fpuz_b[BSIZE][BSIZE]) { 
	// after solution board is determined, every place on the solution board that
	// is not a period will be made a blank space on the puzzle board
	int i, j;

	for (i = 0; i < BSIZE; i++) {
		for (j = 0; j < BSIZE; j++) {
			if (fsol_b[i][j] != '.') {
				fpuz_b[i][j] = ' ';
			}
		}
	}
}

void display_anagram(Word_data arr[], int count) { // formats clues and calls function that creates anagrams
	int i;
	
	printf("Location | Direction | Anagram\n");
	
	for (i = 0; i < count; i++ ) {
		printf("  %2d, %2d | ", arr[i].col_val, arr[i].row_val);
		
		if (arr[i].vert == 0) {
			printf("   Across | ");
		} else {
			printf("     Down | ");
		}

		create_anagram(arr[i].word);
		printf("\n");
	}
}


void create_anagram(char word[]) { // shuffles word and prints it
	int length = strlen(word);
	int i;
	char *copy = malloc(length*sizeof(char));

	for (i = 0; i < length; i++) {
		copy[i] = word[i];
	}
	
	strfry(copy);

	printf("%s", copy);
}

bool get_file_words(char file_name[], char *words[], int *pcount) { // gets words from file specified in command line
	// puts words in char pointer array
	FILE *fp = fopen(file_name, "r");
	int count = 0;


	if (fp == NULL) { 
		printf("Error. Input file %s does not exist\n", file_name);
		return false;
	}

	while (1) {
		words[count] = malloc(16*sizeof(char));
		fscanf(fp, "%s", words[count]);
		if (feof(fp)) {
			break;
		}
		if (count == 19 || words[count] == ".") {
			free(words[count]);
			break;
		}
		count++;
	}
	*pcount = count - 1;

	return true;
}



void disp_out_board(char fboard[BSIZE][BSIZE], FILE *ffp) { // displays boards with output in file specified by *ffp
	int i, j;

	for (i = 0; i <= BSIZE + 1; i++) {
		fprintf(ffp, "-");
	}
	fprintf(ffp, "\n");

	for (i = 0; i < BSIZE; i++) {
		for (j = 0; j < BSIZE; j++) {
			if (j == 0) {
				fprintf(ffp, "|");
				fprintf(ffp, "%c", fboard[i][j]);
			} else if (j == BSIZE - 1) {
				fprintf(ffp, "%c", fboard[i][j]);
				fprintf(ffp, "|\n");
			} else {
				fprintf(ffp, "%c", fboard[i][j]);
			}
		}
	}

	for (i = 0; i <= BSIZE + 1; i++) {
		fprintf(ffp, "-");
	}
	fprintf(ffp, "\n");
}

void disp_out_ana(Word_data arr[], int count, FILE *ffp) { // displays anagram and clues in file specified by *ffp
	int i;
	
	fprintf(ffp, "Location | Direction | Anagram\n");
	
	for (i = 0; i < count; i++ ) {
		fprintf(ffp, "  %2d, %2d | ", arr[i].col_val, arr[i].row_val);
		
		if (arr[i].vert == 0) {
			fprintf(ffp, "   Across | ");
		} else {
			fprintf(ffp, "     Down | ");
		}

		create_out_anagram(arr[i].word, ffp);
		fprintf(ffp, "\n");
	}
}

void create_out_anagram(char word[], FILE *ffp) { // shuffles word and prints it in file specified by *ffp
	int length = strlen(word);
	int i;
	char *copy = malloc(length*sizeof(char));

	for (i = 0; i < length; i++) {
		copy[i] = word[i];
	}
	
	strfry(copy);

	fprintf(ffp, "%s", copy);
}

	

