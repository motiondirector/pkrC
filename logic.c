#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>

#define SUITS 4
#define FACES 13
#define CARDS 52
#define HAND_SIZE 5

struct Card {
    const char *suit;
    const char *face;
};

//Prototypes
void shuffle(unsigned int wDeck[][FACES]); //shuffling modifies wDeck
void dealHand(unsigned int wDeck[][FACES], const char *wFace[], 
    const char *wSuit[], struct Card wHand[], int *deckTracker);
void printHand(struct Card wHand[]);
bool hasPair(struct Card wHand[]); 
bool hasTwoPairs(struct Card wHand[]); 
bool hasThreeOfaKind(struct Card wHand[]); 
bool hasFourOfaKind(struct Card wHand[]); 
bool isFulsh(struct Card wHand[]); 
bool isStraight(const char *wFace[], struct Card wHand[]);
void bubbleSort(int a[], int size); 
bool isConsecutive(int a[], int size);
void printArray(int a[], int size);
int isBetterHand(struct Card handOne[], struct Card handTwo[], const char *wFace[]);
void printHandResults(const char *wFace[], struct Card wHand[]);

int main(void)
{
    //initialize deck array
    unsigned int deck[SUITS][FACES] = {0};
    int deckTracker = 0; //to keep track of what is in the deck after dealing a hand

    srand(time(NULL)); // seed random-number generator
    shuffle(deck); // shuffle the deck

    //initialize suit array
    const char *suit[SUITS]= {"Hearts", "Diamonds", "Clubs", "Spades"};

    //initialize face array
    const char *face[FACES] = 
        {"Ace", "Deuce", "Three", "Four",
         "Five", "Six", "Seven", "Eight",
         "Nine", "Ten", "Jack", "Queen", "King"};

    //initialize card hand array
    struct Card hand[HAND_SIZE];

    //initialize card hand array
    struct Card handTwo[HAND_SIZE];
     
    dealHand(deck, face, suit, hand, &deckTracker); //deal a Hand
    dealHand(deck, face, suit, handTwo, &deckTracker); //deal a Hand
    
    printHand(hand);//print dealed hand
    printHandResults(face, hand);
    
    printHand(handTwo);//print dealed hand
    printHandResults(face, handTwo);

    unsigned winner = isBetterHand(hand, handTwo, face);
    if(winner == 3) {
        printf("%s\n\n", "Hands are even");
    }
    else if(winner == 2) {
        printf("%s\n\n", "Hand TWO is better");
    }
    else {
        printf("%s\n\n", "Hand ONE is better");
    }

}

//shuffle cards in deck
void shuffle(unsigned int wDeck[][FACES])
{
    //for each of the cards. choose slot of deck randomly
    for (size_t card = 1; card <= CARDS; ++card) {
        size_t row; // row number
        size_t column; // column number

        // chose new random location until unoccupied slot found
        do {
            row = rand() % SUITS;
            column = rand() % FACES;
        } while(wDeck[row][column] != 0);

        //place card number in chosen slot of deck
        wDeck[row][column] = card;
    }
}

//deal cards in deck
void dealHand(unsigned int wDeck[][FACES], const char *wFace[], 
    const char *wSuit[], struct Card wHand[], int *deckTracker)
{
    unsigned int c = 0;
    //deal  each of the cards
    for (size_t card = 1 + *deckTracker; card <= HAND_SIZE + *deckTracker; ++card) {
        //loop through rows of wDeck
        for (size_t row = 0; row < SUITS; ++row) {
            //loop throught columns of wDeck for current row
            for (size_t column = 0; column < FACES; ++column) {
                //if slot contains current card, display card
                if (wDeck[row][column] == card){
                    struct Card aCard;
                    aCard.face = wFace[column];
                    aCard.suit = wSuit[row];
                    wHand[c] = aCard;
                    c++;
                }
            }
        }
    }
    *deckTracker = *deckTracker + HAND_SIZE;
}

//To print a hand
void printHand(struct Card wHand[]) {
    printf("\n");
    for (unsigned i = 0; i < HAND_SIZE; ++i) {
        printf("%5s of %-8s\n", wHand[i].face, wHand[i].suit); 
    }
}

//To print Game
void printHandResults(const char *wFace[], struct Card wHand[]) {
    printf("\n%s%19s\n", "Has pair: ", hasPair(wHand) ? "true" : "false");
    printf("%s%14s\n", "Has two pairs: ", hasTwoPairs(wHand) ? "true" : "false");
    printf("%s%9s\n", "Has three of a kind: ", hasThreeOfaKind(wHand) ? "true" : "false");
    printf("%s%10s\n", "Has four of a kind: ", hasFourOfaKind(wHand) ? "true" : "false");
    printf("%s%18s\n", "Is a flush: ", isFulsh(wHand) ? "true" : "false");
    printf("%s%15s\n\n", "Is a straight: ", isStraight(wFace, wHand) ? "true" : "false");
}

//Determine whether the hand contains a pair.
bool hasPair(struct Card wHand[]) {
    for (unsigned i = 0; i < HAND_SIZE; ++i) {
        for (unsigned j = i + 1; j < HAND_SIZE; ++j) {
            if( i != j && wHand[i].face == wHand[j].face) {
                return true;
            }
        }
    }
    return false;
}

//Determine whether the hand contains two pairs. 
bool hasTwoPairs(struct Card wHand[]) {
    unsigned int count = 0;
    for (unsigned i = 0; i < HAND_SIZE; ++i) {
        for (unsigned j = i + 1; j < HAND_SIZE; ++j) {
            if( i != j && wHand[i].face == wHand[j].face) {
                count++;
            }
        }
    }
    if (count > 1) { return true; }
    return false;
}

//Determine whether the hand contains three of a kind(e.g, three jacks)
bool hasThreeOfaKind(struct Card wHand[]) {
    const char *candidate;
    for (unsigned i = 0; i < HAND_SIZE; ++i) {
        for (unsigned j = i + 1; j < HAND_SIZE; ++j) {
            if( i != j && wHand[i].face == wHand[j].face) {
                if (candidate == wHand[i].face) {
                    return true;
                }
                else {
                    candidate = wHand[i].face;
                }
            }
        }
    }
    return false;
}

//Determine whether the hand contains four of a kind (e.g, four aces)
bool hasFourOfaKind(struct Card wHand[]) {
    const char *candidate;
    unsigned int count;
    for (unsigned i = 0; i < HAND_SIZE; ++i) {
        for (unsigned j = i + 1; j < HAND_SIZE; ++j) {
            if( i != j && wHand[i].face == wHand[j].face) {
                if (candidate == wHand[i].face && count > 3) {
                    return true;
                }
                else {
                    candidate = wHand[i].face;
                    count++;
                }
            }
        }
    }
    return false;
}

//Determine whether the hand contains a flush (i.e, all five cards of the same suit)
bool isFulsh(struct Card wHand[]) {
    for (unsigned i = 0; i < HAND_SIZE; ++i) {
        for (unsigned j = i + 1; j < HAND_SIZE; ++j) {
            if( i != j && wHand[i].suit != wHand[j].suit) {
                return false;
            }
        }
    }
    return true;
}

//Determine whether the hand contains a straight (i.e., five cards of consecutive face values)
bool isStraight(const char *wFace[], struct Card wHand[]) {
    int array[HAND_SIZE] = {0}; //to store face values
    unsigned c = 0;
    for (unsigned i = 0; i < HAND_SIZE; ++i) {
        for(unsigned k = 0; k < FACES; ++k) {
            if(wHand[i].face == wFace[k]) {
                array[c] = k; //assign face value to each card
                c++;
            }
        }
    }
    bubbleSort(array, HAND_SIZE); //sort the cards
    // printArray(array, HAND_SIZE);
    if ( isConsecutive(array, HAND_SIZE)) { //check if consecutive
        return true;
    }
    return false;
}

//Helper function to sort values in array
void bubbleSort(int a[], int size) {
   //loop control
   for(unsigned int pass = 1; pass < size; ++pass) { 
      //loop control for number of compare
      for (unsigned int j = 0; j < size -1; ++j) {
         if(a[j] > a[j + 1]) { //swap elmnts if out of order
            unsigned int hold = a[j];
            a[j] = a[j + 1];
            a[j + 1] = hold;
         }
      }
   }
}

//helper function to check if consecutive
bool isConsecutive(int a[], int size) {
   for(unsigned int i = 1; i < size - 1; ++i) { 
      if (a[i + 1] - a[i] != 1) {
        return false;
      }
   }
   return true;
}

//To pint all values in int array 
void printArray(int a[], int size) {
    for (unsigned i = 0; i < size; ++i) {
      printf("%7u%13d\n", i, a[i]);
   }
}

//Determines winner hand and return winner or even
int isBetterHand(struct Card handOne[], struct Card handTwo[], const char *wFace[]) {
    unsigned int oneWins = 1;
    unsigned int twoWins = 2;
    unsigned int even = 3;

    if (isStraight(wFace, handOne) == true && isStraight(wFace, handTwo) == true) {
        return even;
    }
    else if (isStraight(wFace, handOne) == true) {
        return oneWins;
    }
    else if (isStraight(wFace, handTwo) == true) {
        return twoWins;
    }
    else if (isFulsh(handOne) == true && isFulsh(handTwo) == true) {
        return even;
    }
    else if (isFulsh(handOne) == true) {
        return oneWins;
    }
    else if (isFulsh(handTwo) == true) {
        return twoWins;
    }
    else if (hasFourOfaKind(handOne) == true && hasFourOfaKind(handTwo) == true) {
        return even;
    }
    else if (hasFourOfaKind(handOne) == true) {
        return oneWins;
    }
    else if (hasFourOfaKind(handTwo) == true) {
        return twoWins;
    }
    else if (hasThreeOfaKind(handOne) == true && hasThreeOfaKind(handTwo) == true) {
        return even;
    }
    else if (hasThreeOfaKind(handOne) == true) {
        return oneWins;
    }
    else if (hasThreeOfaKind(handTwo) == true) {
        return twoWins;
    }
    else if (hasTwoPairs(handOne) == true && hasTwoPairs(handTwo) == true) {
        return even;
    }
    else if (hasTwoPairs(handOne) == true) {
        return oneWins;
    }
    else if (hasTwoPairs(handTwo) == true) {
        return twoWins;
    }
    else if (hasPair(handOne) == true && hasPair(handTwo) == true) {
        return even;
    }
    else if (hasPair(handOne) == true) {
        return oneWins;
    }
    else if (hasPair(handTwo) == true) {
        return twoWins;
    }
    else {
        return even;
    }
}












