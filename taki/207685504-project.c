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

#define YELLOW_INDEX 0
#define RED_INDEX 1
#define BLUE_INDEX 2
#define GREEN_INDEX 3
#define NO_COLOR_INDEX '#'

#define YELLOW_CHAR 'Y'
#define RED_CHAR 'R'
#define GREEN_CHAR 'G'
#define BLUE_CHAR 'B'
#define NO_COLOR_CHAR ' '

#define STOP_CARD_INDEX 10
#define PLUS_CARD_INDEX 11
#define CHANGE_DIRECTION_CARD_INDEX 12
#define TAKI_CARD_INDEX 13
#define CHANGE_COLOR_CARD_INDEX 14

#define START_POINT_OF_SPECIAL_CARDS 10
#define DIRECRTION_RIGHT 1
#define DIRECTION_LEFT -1
#define TAKE_CARD_FROM_DECK 0

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


void *setUsers(int* numberOfPlayers);
int getRandomNumber(int stratPoint, int endPoint);
CARD getRandomCard();
void setStartStackCard(CARD* startCard);
void printCard(CARD cardToPrint);
void printUserCard(USER user);
void gameRun(USER* usersPlaying, int numberOfPlayers);
void gameTurn(USER* userPlaying, int* direction);
void takeCardFromDeck(USER* userplaying);

void main() {
	USER* usersInGame;
	int numberOfPlayers;
	CARD stackCard;
	srand(time(NULL));
	setStartStackCard(&stackCard);
	usersInGame = setUsers(&numberOfPlayers);
	gameRun(usersInGame, numberOfPlayers, stackCard);
}

int getRandomNumber(int startPoint, int endPoint) {
	return startPoint + rand() % (endPoint - startPoint);
}



void *setUsers(int *numberOfPlayers) {
	int userScanIndex, cardScanIndex, numberToPlayerToSet;
	USER* usersInGame;
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
		for (cardScanIndex = 0; cardScanIndex < CARDS_GIVEN_FIRST_TIME; cardScanIndex++)
		{
			usersInGame[userScanIndex].userCards[cardScanIndex] = getRandomCard();
		}
	}
	return usersInGame;
}

void gameRun(USER usersPlaying[], int numberOfPlayers, CARD cardOnStack) {
	int playingNowIndex = 0;
	bool someoneWon = false;
	int direction = DIRECRTION_RIGHT;
	while (!someoneWon)
	{
		printf("Upper card:\n");
		printCard(cardOnStack);
		gameTurn(&usersPlaying[playingNowIndex], &direction);
		if(usersPlaying[playingNowIndex].currentUserCards == 0) {
			printf("AND THE WINNER IS...\n %s!!!!!!~~",usersPlaying[playingNowIndex].userName);
			someoneWon = true;
		}
		playingNowIndex += direction;
		if(playingNowIndex == numberOfPlayers || playingNowIndex == -1) {
			playingNowIndex = direction == DIRECRTION_RIGHT ? 0 : (numberOfPlayers - 1);
		}

	}
}
void gameTurn(USER *userPlaying, int* direction) {
	int userCardChoice;
	printf("%s' turn!\n", userPlaying->userName);
	printUserCard(*userPlaying);
	printf("or 1 - %d if you want to put one of your cards in the middle : ", userPlaying)
	printf("Please enter 0 if you want to take a card from the deck\n");
	scanf("%d", &userCardChoice);
	switch (userCardChoice)
	{
	case TAKE_CARD_FROM_DECK:
		takeCardFromDeck(userPlaying);
		break;
	default:
		break;
	}
}
void takeCardFromDeck(USER* userplaying) {
	if (userplaying->sizeOfMaxUserCards > userplaying->userCards + 1) {
		userplaying->sizeOfMaxUserCards *= 2;
		userplaying->userCards = realloc(userplaying->userCards, userplaying->sizeOfMaxUserCards *= 2);
		if (userplaying->userCards == NULL) {
			printf("MEMORY ERROR!");
			return;
		}
	}
	userplaying->currentUserCards += 1;
	userplaying->userCards[userplaying->currentUserCards - 1] = getRandomCard();
}
void printUserCard(USER user) {
	int i;
	for ( i = 0; i < user.currentUserCards; i++)
	{
		printf("Card #%d:\n",i+1);
		printCard(user.userCards[i]);
	}
}
void setStartStackCard(CARD *startCard) {
	*startCard = getRandomCard();
	while (startCard->cardType >= START_POINT_OF_SPECIAL_CARDS)
	{
		*startCard = getRandomCard();
	}
}
void printCard(CARD cardToPrint) {
	printf("*********\n");
	printf("*       *\n");
	switch (cardToPrint.cardType)
	{
		case STOP_CARD_INDEX:
			printf("* STOP  *\n");
		break;
		case PLUS_CARD_INDEX:
			printf("* PLUS  *\n");
		break;
		case CHANGE_COLOR_CARD_INDEX:
			printf("* COLOR *\n");
		break;
		case TAKI_CARD_INDEX:
			printf("* TAKI  *\n");
		break;
		case CHANGE_DIRECTION_CARD_INDEX:
			printf("*  <->  *\n");
		break;
	default:
		printf("*   %d   *\n", cardToPrint.cardType);

		break;
	}
	printf("*   %c   *\n", cardToPrint.cardColor);
	printf("*       *\n");
	printf("*********\n");
	printf("\n");
}
CARD getRandomCard() {
	CARD cardToSet;
	int typeOfCard = getRandomNumber(1, MAX_POSSIBLE_CARDS_TYPES);
	int color = getRandomNumber(0, NUMBER_OF_COLORS);
	cardToSet.cardType = typeOfCard;
	if (cardToSet.cardType == CHANGE_COLOR_CARD_INDEX)
	{
		cardToSet.cardColor = NO_COLOR_CHAR;
	}
	else
	{
		switch (color)
		{
		case YELLOW_INDEX:
			cardToSet.cardColor = YELLOW_CHAR;
		break;
		case RED_INDEX:
			cardToSet.cardColor = RED_CHAR;
		break;
		case BLUE_INDEX:
			cardToSet.cardColor = BLUE_CHAR;
		break;
		case GREEN_INDEX:
			cardToSet.cardColor = GREEN_CHAR;
		break;
		
		default:
			break;
		}
	}
	return cardToSet;
}