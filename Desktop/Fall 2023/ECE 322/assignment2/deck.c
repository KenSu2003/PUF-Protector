/* This section of the code was written by: John Works */
#include "deck.h"
#include <stdio.h>


//shuffle
// Assuming that the deck has already been initalized
int shuffle(){
    printf("Shuffling deck....");
    char suits[] = {'C', 'D', 'H', 'S'};   // valid suits
    char ranks[] = {'2', '3', '4', '5', '6', '7', '8', '9',
    'T', 'J', 'Q', 'K', 'A'};  // 10 = T, convert back when displaying

    deck_instance.top_card = 52;   // initialize top card to 52

    // fill the deck with cards
    for (int i = 0; i < 52; i++){
        deck_instance.list[i].suit = suits[i/13];
        deck_instance.list[i].rank[0] = ranks[i%13];
        deck_instance.list[i].rank[1] = '\0';  // terminate the string
    }

    // Go through each card and switch it with a random card from the rest of the cards
    for (int i = deck_instance.top_card; i > 0; i--){   // 52, 51, 50, ..., 1
        int j = rand() % (i+1);   // random index from 0 to i

        // swap the cards
        struct card temp = deck_instance.list[i];        // set temp = ith card
        deck_instance.list[i] = deck_instance.list[j];   // ith card = jth card
        deck_instance.list[j] = temp;                    // ith card = ith card
    }
return 0;
}