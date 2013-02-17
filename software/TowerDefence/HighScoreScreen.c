/*
 * HighScoreScreen.c
 *
 *  Created on: 2013-02-08
 *      Author: Adam
 */

#include "HighScoreScreen.h"
#include "string.h"

int swap;
alt_up_pixel_buffer_dma_dev* pixel_buffer;


struct NameInput {
	unsigned char name[NAME_LENGTH];
	int index;
};

struct HighScore {
	char name[NAME_LENGTH];
	char score[SCORE_LENGTH];
	int score_;
};

struct HighScore* initHighScore(char string[])
{
	struct HighScore* highScore = (struct HighScore*)malloc(sizeof(struct HighScore));
	int currentIndex = 0;
	int scoreIndex = 0;
	char temp_string[ SCORE_LENGTH ];
	sprintf(highScore->name, "%.3s", string );

	currentIndex = NAME_LENGTH;
	while( currentIndex < NAME_LENGTH + SCORE_LENGTH )
	{
		temp_string[ scoreIndex ] = string[ currentIndex ];
		scoreIndex++;
		currentIndex++;
	}

	sprintf( highScore->score, "%.5s", temp_string );
	highScore->score_ = atoi(highScore->score);
	return highScore;
}

void high_score_to_string( struct HighScore* highScore, char buffer[] )
{
	sprintf( buffer, "%.3s %.5s\n", highScore->name, highScore->score );
	return;
}

void add_high_score( struct HighScore *highScores[], int numHighScores, unsigned char name[NAME_LENGTH], int score )
{
	char temp_string[CHARS_PER_LINE];
	sprintf( temp_string, "%.3s %.5d", name, score );
	if(numHighScores > MAX_NUM_HIGH_SCORES) {
		if(score > highScores[MAX_NUM_HIGH_SCORES-1]->score_)
			highScores[MAX_NUM_HIGH_SCORES-1] = initHighScore( temp_string);
		return;
	}
	highScores[ numHighScores ] = initHighScore( temp_string );
}

void sort_high_scores( struct HighScore *highScores[MAX_NUM_HIGH_SCORES], int numHighScores )
{
    int i, j;
    //char temp_score[SCORE_LENGTH];
    //char temp_name[NAME_LENGTH];
    struct HighScore* temp;
    do{
        j = 0;
        for ( i = 0; i < numHighScores - 1; i++ )
        {
			if( highScores[i]->score_  < highScores[i+1]->score_ )
			{
				j = 1;/*
				sprintf( temp_score, "%.5s", highScores[i]->score );
				sprintf( temp_name, "%.3s", highScores[i]->name );
				sprintf( highScores[i]->score, "%.5s", highScores[i+1]->score );
				sprintf(highScores[i]->name, "%.3s", highScores[i+1]->name );
				sprintf( highScores[i+1]->name, "%.3s", temp_name );
				sprintf( highScores[i+1]->score, "%.5s", temp_score );*/
				temp = highScores[i];
				highScores[i] = highScores[i+1];
				highScores[i+1] = temp;
			}
		}
    } while (j == 1);
}

bool clear_high_scores(int numHighScores)
{
	int i, j;
	int file_pointer = alt_up_sd_card_fopen(FILE_NAME, false);
	if(file_pointer < 0) {
		alt_up_sd_card_fclose(file_pointer); //close the file
		return false;
	}

	alt_up_sd_card_write(file_pointer, 0);

	/**
	 * Clear out the high scores
	 */
	for( i = 0; i < numHighScores; i++ )
	{
		for( j = 0; j < 10; j++ )
			alt_up_sd_card_write(file_pointer, ' ');
	}

	alt_up_sd_card_fclose(file_pointer);
	return true;
}

int read_high_scores(struct HighScore *highScores[])
{
	char read_buffer[15];
	int i, j, numHighScores;
	int file_pointer = alt_up_sd_card_fopen(FILE_NAME, false);
	if(file_pointer < 0) {
		alt_up_sd_card_fclose(file_pointer); //close the file
		return -1;
	}

	// Grab the number of high scores
/*	for( i = 0; i < NUM_HIGH_SCORES_LENGTH; i++ )
	{
		if((read_buffer[ i ] = alt_up_sd_card_read(file_pointer))<0) {
			alt_up_sd_card_fclose(file_pointer); //close the file
			return -1;
		} printf("first 2 %c\n", read_buffer[i]);
	}*/
	i = 0;
	do {
		if((read_buffer[ i ] = alt_up_sd_card_read(file_pointer))<0) {
			alt_up_sd_card_fclose(file_pointer); //close the file
			return -1;
		}
		i++;
	}while(read_buffer[i-1] != ' ');
	read_buffer[i] = '\0';
	numHighScores = atoi(read_buffer);
	if( numHighScores > MAX_NUM_HIGH_SCORES )
	{
		numHighScores = MAX_NUM_HIGH_SCORES;
	}
	int count = 0;
	for( i = 0; i < numHighScores; i++ )
	{j = 0; count = 0;
		while(count < 2) {
			if((read_buffer[ j ] = alt_up_sd_card_read(file_pointer))<0) {
				alt_up_sd_card_fclose(file_pointer); //close the file
				return -1;
			}
			if(read_buffer[j] == ' ')
				count++;
			j++;
		}
		read_buffer[j-1] = '\0';
		highScores[ i ] = initHighScore(read_buffer);
	}
	while(i < MAX_NUM_HIGH_SCORES) {
		highScores[i] = NULL;
		i++;
	}
	alt_up_sd_card_fclose(file_pointer);
	return numHighScores;
}

bool write_high_scores(struct HighScore *highScores[], int numHighScores)
{
	int i, j;
	char write_buffer[ CHARS_PER_LINE ];
	int file_pointer = alt_up_sd_card_fopen(FILE_NAME, false);
	// ASCII for a space
	char space = 32;
	if(file_pointer < 0) {
		alt_up_sd_card_fclose(file_pointer); //close the file
		return false;
	}

	if( numHighScores > MAX_NUM_HIGH_SCORES )
	{
		numHighScores = MAX_NUM_HIGH_SCORES;
	}

	sprintf( write_buffer, "%d", numHighScores );

	int size = strlen(write_buffer);
	for( i = 0; i < size; i++ )
		alt_up_sd_card_write(file_pointer, write_buffer[ i ] );

	alt_up_sd_card_write(file_pointer, space);

	// Only write the max number of high scores

	for( i = 0; i < numHighScores; i++ )
	{
		high_score_to_string( highScores[ i ], write_buffer );

		for( j = 0; j < CHARS_PER_LINE; j++ )
		{
			alt_up_sd_card_write(file_pointer, write_buffer[j]);
		}
		alt_up_sd_card_write(file_pointer, space);
	}

	alt_up_sd_card_fclose(file_pointer);
	return true;
}

void show_high_scores(struct HighScore *highScores[], int numHighScores)
{
	int i, size;
	char high_score_string[50];

	alt_up_char_buffer_string(char_buffer, "NAME       SCORE", 33, 9);

	if(numHighScores > MAX_NUM_HIGH_SCORES)
		size = MAX_NUM_HIGH_SCORES;
	else
		size = numHighScores;
	for( i = 0; i < size; i++ )
	{
		sprintf( high_score_string, "%.3s        %.5s", highScores[ i ]->name, highScores[i]->score );
		if(i > 9) {
			alt_up_char_buffer_string(char_buffer, high_score_string, 33, 12 + i);
		} else
			alt_up_char_buffer_string(char_buffer, high_score_string, 33, 11 + i);
	}
}

void update_input_name( struct KeyController* keyController, struct NameInput* nameInput )
{
	if( wasKeyJustPressed( keyController, 0 ) )
		nameInput->index++;
	else if( wasKeyJustPressed( keyController, 1 ) )
	{
		if( nameInput->index > 0 )
			nameInput->index--;
	}
	if( wasKeyJustPressed( keyController, 2 ) )
	{
		if( nameInput->name[ nameInput->index ] < 255 )
			nameInput->name[ nameInput->index ]++;
		else
			nameInput->name[ nameInput->index ] = 0;
	}
	if( wasKeyJustPressed( keyController, 3 ) )
	{
		if( nameInput->name[ nameInput->index ] > 0 )
			nameInput->name[ nameInput->index ]--;
		else
			nameInput->name[ nameInput->index ] = 255;
	}
}

struct NameInput* get_name_input( struct KeyController* keyController )
{
	int count = 0;
	int startX = 69;
	int startY = 15;
	int lastIndex = 0;
	struct NameInput* nameInput = (struct NameInput*)malloc(sizeof(struct NameInput));
	nameInput->index = 0;
	int i;
	for(i = 0; i < 3; i++) {
		nameInput->name[i] = 'A';
	}

	alt_up_char_buffer_string(char_buffer, nameInput->name, startX, startY);
	addKeyListener( keyController, (void *)update_input_name, nameInput, 0xF );

	while( nameInput->index < NAME_LENGTH - 1 )
	{
		if( count < 100000 )
			alt_up_char_buffer_draw(char_buffer, nameInput->name[nameInput->index], startX+nameInput->index, startY);
		else if( count < 200000 )
			alt_up_char_buffer_string(char_buffer, " ", startX+nameInput->index, startY);
		else
			count = 0;
		count++;

		// Check to make sure the last modified char is displayed
		if( lastIndex != nameInput->index )
		{
			if( lastIndex < NAME_LENGTH - 1 )
				alt_up_char_buffer_draw(char_buffer, nameInput->name[lastIndex], startX+lastIndex, startY);
			lastIndex = nameInput->index;
		}
	}
	removeKeyListener( keyController );
	return nameInput;
}

void start_high_score_screen( unsigned int score, struct KeyController* keyController )
{
	alt_up_char_buffer_clear(char_buffer);
	alt_up_char_buffer_string(char_buffer, "Game Over", 65, 10);
	alt_up_char_buffer_string(char_buffer, "PLEASE", 67, 12);
	alt_up_char_buffer_string(char_buffer, "ENTER YOUR", 65, 13);
	alt_up_char_buffer_string(char_buffer, "INITIALS", 66, 14);
	alt_up_char_buffer_string(char_buffer, "Credits", 65, 30);
	alt_up_char_buffer_string(char_buffer, "Adam Berg", 63, 32);
	alt_up_char_buffer_string(char_buffer, "Diana Sutandie", 63, 33);
	alt_up_char_buffer_string(char_buffer, "Perry Yan", 63, 34);
	alt_up_char_buffer_string(char_buffer, "Fahad Almisbahi", 63, 35);
	alt_up_char_buffer_string(char_buffer, "Danny Hsieh", 63, 36);

	struct HighScore *highScores[MAX_NUM_HIGH_SCORES];

	int numHighScores;
	while((numHighScores= read_high_scores(highScores)) == -1) {
		printf("reading high score fail\n");
	}

	struct NameInput* nameInput = get_name_input(keyController);

	add_high_score( highScores, numHighScores, nameInput->name, score );
	numHighScores++;

	alt_up_pixel_buffer_dma_clear_screen(pixel_buffer, 0);

	sort_high_scores(highScores, numHighScores);
	write_high_scores(highScores, numHighScores);
	while(read_high_scores(highScores)==-1) {
		printf("reading high score fail\n");
	}
	show_high_scores(highScores, numHighScores);

	// Wait for 0 key to be pressed
	//while(isKeyUp(3));

	//clear prompt for user to enter the name
	alt_up_char_buffer_string(char_buffer, "              ", 67, 12);
	alt_up_char_buffer_string(char_buffer, "              ", 65, 13);
	alt_up_char_buffer_string(char_buffer, "              ", 66, 14);
	free(nameInput);
	nameInput = NULL;
	int i;
	for(i = 0; i < MAX_NUM_HIGH_SCORES; i++) {
		safefree(&highScores[i]);
	}
}
