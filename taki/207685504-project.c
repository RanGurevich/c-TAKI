#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define MAX_NAME_SIZE 20
#define CARDS_GIVEN_FIRST_TIME 4
#define MAX_POSSIBLE_CARDS_TYPES 15
#define NUMBER_OF_COLORS 4

typedef struct CardStruct
{
	int cardType;
	char cardColor;
} CARD;

typedef struct UserStruct
{
	char userName[MAX_NAME_SIZE];
	CARD* userCards;
	int sizeOfMaxUserCards;
	int currentUserCards;
} USER;


void setUsers();
int getRandomNumber(int stratPoint, int endPoint);

void main() {
	USER* usersInGame;
	int numberOfPlayers;
	srand(time(NULL));
	setUsers(&usersInGame, &numberOfPlayers);
}

int getRandomNumber(int startPoint, int endPoint) {
	return startPoint + rand() % (endPoint - startPoint);
}

void setUsers(int *numberOfPlayers, USER *usersInGame) {
	int userScanIndex, cardScanIndex;
	printf("************  Welcome to TAKI game !!! ***********\n");
	printf("Please enter the number of players:");
	scanf("%d", numberOfPlayers);
	usersInGame = malloc(sizeof(USER) * *numberOfPlayers);
	for (userScanIndex = 0; userScanIndex < *numberOfPlayers; userScanIndex++)
	{
		printf("Please enter the first name of player #%d:\n", userScanIndex + 1);
		scanf("%s", &usersInGame[userScanIndex].userName);
		usersInGame[userScanIndex].sizeOfMaxUserCards = CARDS_GIVEN_FIRST_TIME;
		usersInGame[userScanIndex].currentUserCards = CARDS_GIVEN_FIRST_TIME;
		usersInGame[userScanIndex].userCards = malloc(sizeof(CARD) * CARDS_GIVEN_FIRST_TIME);

	}
}

void setRandomCard(CARD* cardToSet) {
	int typeOfCard = getRandomNumber(1, MAX_POSSIBLE_CARDS_TYPES);
	int color = getRandomNumber(1, NUMBER_OF_COLORS);
	cardToSet->cardType = typeOfCard;
	cardToSet->cardColor = color;
}