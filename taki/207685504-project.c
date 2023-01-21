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
CARD changeColor();
void setStartStackCard(CARD* startCard);
void printCard(CARD cardToPrint);
void printUserCard(USER *user);
void gameRun(USER* usersPlaying, int numberOfPlayers);
void gameTurn(USER* userPlaying, int* direction);
void takeCardFromDeck(USER* userplaying);
void removeCardFromUserAndSetDeck(USER* user, int indexToRemove, CARD* cardOnStack);
void putCardInGame(USER* userPlaying, int* direction, CARD* cardOnStack);

void main() {
	USER* usersInGame;
	int numberOfPlayers;
	CARD stackCard;
	srand(time(NULL));
	setStartStackCard(&stackCard);
	usersInGame = setUsers(&numberOfPlayers);
	gameRun(usersInGame, numberOfPlayers, &stackCard);
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

void gameRun(USER usersPlaying[], int numberOfPlayers, CARD *cardOnStack) {
	int playingNowIndex = 0;
	bool someoneWon = false;
	int direction = DIRECRTION_RIGHT;
	while (!someoneWon)
	{

		printf("Upper card:\n");
		printCard(*cardOnStack);
		gameTurn(&usersPlaying[playingNowIndex], &direction, cardOnStack);
		if(usersPlaying[playingNowIndex].currentUserCards == 0) {
			printf("AND THE WINNER IS...\n %s!!!!!!~~",usersPlaying[playingNowIndex].userName);
			someoneWon = true;
		}
		playingNowIndex += direction;
		if(playingNowIndex >= numberOfPlayers || playingNowIndex < 0) {
			playingNowIndex = direction == DIRECRTION_RIGHT ? 0 : (numberOfPlayers - 1);
		}

	}
}
void gameTurn(USER *userPlaying, int* direction, CARD* cardOnStack) {
	int userCardChoice;
	printf("%s' turn!\n", userPlaying->userName);
	printUserCard(userPlaying);
	printf("Please enter 0 if you want to take a card from the deck\n");
	printf("or 1 - %d if you want to put one of your cards in the middle : ", userPlaying->currentUserCards);
	putCardInGame(userPlaying, direction, cardOnStack);

}
void putCardInGame(USER* userPlaying, int* direction, CARD* cardOnStack) {
	int userCardChoice;
	CARD systemSaveCard;
	scanf("%d", &userCardChoice);
	switch (userCardChoice)
	{
	case TAKE_CARD_FROM_DECK:
		takeCardFromDeck(userPlaying);
		break;
	default:
		//color
		if (userPlaying->userCards[userCardChoice - 1].cardType == CHANGE_COLOR_CARD_INDEX) {
			systemSaveCard = changeColor();
			removeCardFromUserAndSetDeck(userPlaying, userCardChoice - 1, cardOnStack);
			*cardOnStack = systemSaveCard;
		} 
		else
		//normal card
		if (userPlaying->userCards[userCardChoice - 1].cardColor == cardOnStack->cardColor || userPlaying->userCards[userCardChoice - 1].cardType == cardOnStack->cardType) {
			systemSaveCard = userPlaying->userCards[userCardChoice - 1];
			removeCardFromUserAndSetDeck(userPlaying, userCardChoice - 1, cardOnStack);
			if (systemSaveCard.cardType == PLUS_CARD_INDEX) {
				printf("+-+-+-+PLUS PLUS PLUS!!! %s USES THE PLUS CARD!!! PUT ANOTHER CARD!+-+-+-+", userPlaying->userName);
				*cardOnStack = systemSaveCard;
				printUserCard(userPlaying);
				putCardInGame(userPlaying, direction, cardOnStack);
			}
			if (systemSaveCard.cardType == CHANGE_DIRECTION_CARD_INDEX) {
				*direction *= -1;
			}
		}
		else
		{
			printf("Invaid Card! please try again!\n");
			printf("Please enter 0 if you want to take a card from the deck\n");
			printf("or 1 - %d if you want to put one of your cards in the middle : ", userPlaying->currentUserCards);
			putCardInGame(userPlaying, direction, cardOnStack);
		}
		break;
	}
}

CARD changeColor() {
	int userChoice;
	CARD newColorCard;
	newColorCard.cardType = CHANGE_COLOR_CARD_INDEX;
	printf("Please enter your color choice:\n1 - Yellow\n2 - Red\n3 - Blue\n4 - Green\n");
	scanf("%d", &userChoice);
	switch (userChoice)
	{
	case 1:
		newColorCard.cardColor = YELLOW_CHAR;
		return newColorCard;
	case 2:
		newColorCard.cardColor = RED_CHAR;
		return newColorCard;
	case 3:
		newColorCard.cardColor = BLUE_CHAR;
		return newColorCard;
	case 4:
		newColorCard.cardColor = GREEN_CHAR;
		return newColorCard;
	default:
		printf("Invalid color! Try again.\n");
		return changeColor();
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
void printUserCard(USER *user) {
	int i;
	for ( i = 0; i < user->currentUserCards; i++)
	{
		printf("Card #%d:\n",i+1);
		printCard(user->userCards[i]);
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
			printf("*   +   *\n");
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

void removeCardFromUserAndSetDeck(USER* user, int indexToRemove, CARD* cardOnStack) {
	int i;
	cardOnStack->cardColor = user->userCards[indexToRemove].cardColor;
	cardOnStack->cardType = user->userCards[indexToRemove].cardType;
	for (i = indexToRemove; i < user->currentUserCards-1; i++)
	{
		user->userCards[i] = user->userCards[i + 1];
	}
	user->currentUserCards--;
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