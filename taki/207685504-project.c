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
#define MAX_USER_CARDS 14

#define START_POINT_OF_SPECIAL_CARDS 10
#define DIRECRTION_RIGHT 1
#define DIRECTION_LEFT -1
#define TAKE_CARD_FROM_DECK 0
#define STOP_TAKI_KEY_STROKE 0

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


void *setUsers(int* numberOfPlayers, int cardSatistic[]);
int getRandomNumber(int stratPoint, int endPoint);
CARD getRandomCard(int cardSatistic[]);
CARD changeColor();
void setStartStackCard(CARD* startCard, int cardSatistic[]);
void printCard(CARD cardToPrint);
void printUserCard(USER *user);
void gameRun(USER* usersPlaying, int numberOfPlayers, int cardSatistic[]);
void gameTurn(USER* userPlaying, int* direction, int cardSatistic[]);
void takeCardFromDeck(USER* userplaying, int cardSatistic[]);
void removeCardFromUserAndSetDeck(USER* user, int indexToRemove, CARD* cardOnStack);
void putCardInGame(USER* userPlaying, int* direction, CARD* cardOnStackint, int cardSatistic[]);
void initCardStatic(int cardSatistic[]);
void printSatistic(int cardSatistic[]);
/*
* +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
* THE TAKI GAME!!!!###@@~~~~
* FINAL COURSE PPROJECT
* 
* Student name: Ran Gurevich
* ID: 207685504
*
* 
* * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-

*/
void main() {
	// the program play the taki game
	USER* usersInGame;
	int cardSatistic[MAX_USER_CARDS];
	int numberOfPlayers;
	CARD stackCard;
	srand(time(NULL));
	initCardStatic(cardSatistic);
	setStartStackCard(&stackCard, cardSatistic);
	usersInGame = setUsers(&numberOfPlayers, cardSatistic);
	// the game run function will manage all the game process
	gameRun(usersInGame, numberOfPlayers, &stackCard, cardSatistic);
}

int getRandomNumber(int startPoint, int endPoint) {
	// the function bring a random number
	return startPoint + rand() % (endPoint - startPoint);
}

void initCardStatic(int cardSatistic[]) {
	// The function init the card satistic array
	int i;
	for (i = 0; i < MAX_USER_CARDS; i++)
	{
		cardSatistic[i] = 0;
	}
}

void printSatistic(int cardSatistic[]) {
	// the function prints the game card satistic
	int i;
	printf("---------------------GAME SATISTIC---------------------\n");
	for (i = 0; i < MAX_USER_CARDS; i++)
	{
		switch (i)
		{
		case STOP_CARD_INDEX:
			printf("STOP: %d\n", cardSatistic[i]);
			break;
		case PLUS_CARD_INDEX:
			printf("PLUS: %d\n", cardSatistic[i]);
			break;
		case TAKI_CARD_INDEX:
			printf("TAKI: %d\n", cardSatistic[i]);
			break;
		case CHANGE_DIRECTION_CARD_INDEX:
			printf("DIRECTION: %d\n", cardSatistic[i]);
			break;
		default:
			printf("CARD NUMBER #%d: %d\n", i + 1, cardSatistic[i]);
			break;
		}
	}
}

void *setUsers(int *numberOfPlayers, int cardSatistic[]) {
	// the function ask the user how many players are, setting their data and give each other 4 cards
	int userScanIndex, cardScanIndex, numberToPlayerToSet;
	USER* usersInGame;
	printf("************  Welcome to TAKI game !!! ***********\n");
	printf("Please enter the number of players:");
	scanf("%d", numberOfPlayers);
	usersInGame = malloc(sizeof(USER) * *numberOfPlayers);
	if (usersInGame == NULL)
	{
		printf("Oh No! :( ERROR: usersInGame throw exception: memory allocation error");
		exit(1);
	}
	for (userScanIndex = 0; userScanIndex < *numberOfPlayers; userScanIndex++)
	{
		printf("Please enter the first name of player #%d:\n", userScanIndex + 1);
		scanf("%s", &usersInGame[userScanIndex].userName);
		usersInGame[userScanIndex].sizeOfMaxUserCards = CARDS_GIVEN_FIRST_TIME;
		usersInGame[userScanIndex].currentUserCards = CARDS_GIVEN_FIRST_TIME;
		usersInGame[userScanIndex].userCards = malloc(sizeof(CARD) * CARDS_GIVEN_FIRST_TIME);
		if (usersInGame[userScanIndex].userCards == NULL) {
			printf("Oh No! :( ERROR: usersInGame[userScanIndex].userCards throw exception: memory allocation error");
			exit(1);
		}
		for (cardScanIndex = 0; cardScanIndex < CARDS_GIVEN_FIRST_TIME; cardScanIndex++)
		{
			usersInGame[userScanIndex].userCards[cardScanIndex] = getRandomCard(cardSatistic);
		}
	}
	return usersInGame;
}

void gameRun(USER usersPlaying[], int numberOfPlayers, CARD *cardOnStack, int cardSatistic[]) {
	// the function is managing the game process, check who is the player, checking if someone won and printing the upper card each turn
	int playingNowIndex = 0;
	bool someoneWon = false;
	int direction = DIRECRTION_RIGHT;
	while (!someoneWon)
	{
		printf("Upper card:\n");
		printCard(*cardOnStack);
		gameTurn(&usersPlaying[playingNowIndex], &direction, cardOnStack, cardSatistic);
		if(usersPlaying[playingNowIndex].currentUserCards == 0) {
			printf("AND THE WINNER IS...\n %s!!!!!!~~\n",usersPlaying[playingNowIndex].userName);
			printSatistic(cardSatistic);
			someoneWon = true;
			return;
		}
		// direction is setting by the speical cards
		playingNowIndex += direction;
		if(playingNowIndex >= numberOfPlayers || playingNowIndex < 0) {
			//doing a circle loop
			playingNowIndex = direction == DIRECRTION_RIGHT ? 0 : (numberOfPlayers - 1);
		}

	}
}
void gameTurn(USER *userPlaying, int* direction, CARD* cardOnStack, int cardSatistic[]) {
	// the function handle the game turn, printing the user cards and ask him to put a card
	int userCardChoice;
	if (*direction > DIRECRTION_RIGHT || *direction < DIRECTION_LEFT) {
		// if the direction jump was more than one acorrding to the STOP card, setting the jump to the normal
		*direction = *direction > 0 ? DIRECRTION_RIGHT : DIRECTION_LEFT;
	}
	printf("%s' turn!\n", userPlaying->userName);
	printUserCard(userPlaying);
	printf("Please enter 0 if you want to take a card from the deck\n");
	printf("or 1 - %d if you want to put one of your cards in the middle : ", userPlaying->currentUserCards);
	putCardInGame(userPlaying, direction, cardOnStack, cardSatistic);

}
void putCardInGame(USER* userPlaying, int* direction, CARD* cardOnStack, int cardSatistic[]) {
	//the function let the user ut a card on the game
	int userCardChoice;
	CARD systemSaveCard;
	scanf("%d", &userCardChoice);
	switch (userCardChoice)
	{
	case TAKE_CARD_FROM_DECK:
		// if the user requested to take a card
		takeCardFromDeck(userPlaying, cardSatistic);
		break;
	default:
		//color change cards
		if (userPlaying->userCards[userCardChoice - 1].cardType == CHANGE_COLOR_CARD_INDEX) {
			systemSaveCard = changeColor();
			removeCardFromUserAndSetDeck(userPlaying, userCardChoice - 1, cardOnStack);
			*cardOnStack = systemSaveCard;
		} 
		else
		//other cards, cheking if the user has the option to use the card he eantted
		if (userPlaying->userCards[userCardChoice - 1].cardColor == cardOnStack->cardColor || userPlaying->userCards[userCardChoice - 1].cardType == cardOnStack->cardType) {
			systemSaveCard = userPlaying->userCards[userCardChoice - 1];
			removeCardFromUserAndSetDeck(userPlaying, userCardChoice - 1, cardOnStack);
			if (systemSaveCard.cardType == PLUS_CARD_INDEX) {
				// the PLUS card, giving the user another turn
				printf("+-+-+-+PLUS PLUS PLUS!!! %s USES THE PLUS CARD!!! PUT ANOTHER CARD!+-+-+-+\n", userPlaying->userName);
				*cardOnStack = systemSaveCard;
				printUserCard(userPlaying);
				putCardInGame(userPlaying, direction, cardOnStack, cardSatistic);
			}
			if (systemSaveCard.cardType == STOP_CARD_INDEX) {
				// the stop card, skipping the next user turn by doubling the direction jump
				printf("+-+-+-+STOP RIGHT NOW!!! %s USES THE PLUS CARD!!! The next player turn skipped+-+-+-+\n", userPlaying->userName);
				*direction *= 2;
			}
			if (systemSaveCard.cardType == CHANGE_DIRECTION_CARD_INDEX) {
				// changing the direction by multiplplying the direction by -1
				printf("+-+-+-+FIFO CHANGED TO LIFO! DIRECTION CHANGED+-+-+-+\n", userPlaying->userName);

				*direction *= -1;
			}
			if (systemSaveCard.cardType == TAKI_CARD_INDEX) {
				// the TAKI card, let the user play any card witth the same color till he press 0
				removeCardFromUserAndSetDeck(userPlaying, userCardChoice - 1, cardOnStack);
				while (userCardChoice != STOP_TAKI_KEY_STROKE)
				{
					printCard(*cardOnStack);
					printUserCard(userPlaying);
					printf("THE GREAT TAKI DOOR IS NOW OPEN! PUT ANY CARD FROM THE SAME COLOR AND TYPE %d AFTER YOU FINISH!\n", STOP_TAKI_KEY_STROKE);
					scanf("%d", &userCardChoice);
					if (userCardChoice != STOP_TAKI_KEY_STROKE) {
						if (userPlaying->userCards[userCardChoice - 1].cardColor == cardOnStack->cardColor) {
							removeCardFromUserAndSetDeck(userPlaying, userCardChoice - 1, cardOnStack);
						}
						else
						{
							printf("BAD CARD NUMBER RECIVED! PLEASE TRY AGAIN!\n");
						}
					}

				}
			}

		}
		else
		{
			// if unvalid card recived telling the user to ttry again
			printf("Invaid Card! please try again!\n");
			printf("Please enter 0 if you want to take a card from the deck\n");
			printf("or 1 - %d if you want to put one of your cards in the middle : ", userPlaying->currentUserCards);
			putCardInGame(userPlaying, direction, cardOnStack, cardSatistic);
		}
		break;
	}
}

CARD changeColor() {
	// the function let the user to change color
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
void takeCardFromDeck(USER* userplaying, int cardSatistic[]) {
	// the function add the user a random card to his hand
	if (userplaying->sizeOfMaxUserCards > userplaying->userCards + 1) {
		userplaying->sizeOfMaxUserCards *= 2;
		userplaying->userCards = realloc(userplaying->userCards, userplaying->sizeOfMaxUserCards *= 2);
		if (userplaying->userCards == NULL) {
			printf("Oh No! :( ERROR: userplaying->userCards throw exception: memory allocation error");
			exit(1);
		}
	}
	userplaying->currentUserCards += 1;
	userplaying->userCards[userplaying->currentUserCards - 1] = getRandomCard(cardSatistic);
}

void printUserCard(USER *user) {
	// the function pring a card
	int i;
	for ( i = 0; i < user->currentUserCards; i++)
	{
		printf("Card #%d:\n",i+1);
		printCard(user->userCards[i]);
	}
}

void setStartStackCard(CARD *startCard, int cardSatistic[]) {
	// the function settting a first card on the stack, if the card was with spiecal type, recreating it
	*startCard = getRandomCard(cardSatistic);
	while (startCard->cardType >= START_POINT_OF_SPECIAL_CARDS)
	{
		*startCard = getRandomCard(cardSatistic);
	}
}

void printCard(CARD cardToPrint) {
	// the function print the card
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
	// the function get a hannd and index of a card and removing it and keeping the card index as the same
	int i;
	cardOnStack->cardColor = user->userCards[indexToRemove].cardColor;
	cardOnStack->cardType = user->userCards[indexToRemove].cardType;
	for (i = indexToRemove; i < user->currentUserCards-1; i++)
	{
		user->userCards[i] = user->userCards[i + 1];
	}
	user->currentUserCards--;
}

CARD getRandomCard(int cardSatistic[]) {
	// the function get a random card and adding his creation to the satistic
	CARD cardToSet;
	int typeOfCard = getRandomNumber(1, MAX_POSSIBLE_CARDS_TYPES);
	int color = getRandomNumber(0, NUMBER_OF_COLORS);
	cardToSet.cardType = typeOfCard;
	cardSatistic[cardToSet.cardType - 1] += 1 ;
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