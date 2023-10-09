/* This section of the code was written by: Ken Su */
#include "player.h"
#include <stdio.h>
#include <stdbool.h>

/* Need tp include for rand and srand */
#include <stdlib.h> 
#include <time.h>

/* Need to include this for NULL */
#include <unistd.h> // https://github.com/microsoft/vscode-cpptools/issues/2607

/* Last stored index of book */
 int book_index = 0;

// test code
int add_card(struct player* target, struct card* new_card){

    /* Allocate space for new element */
    struct hand* next_hand;
    next_hand = (struct hand*)malloc(sizeof(struct hand*));
    if (next_hand == NULL) { return -1; }
    
    /* Initialize new element */
    next_hand->top = *new_card;  // next_hand->:payload == (next_hand*).payload
    next_hand->next = target->card_list;
    
    /* Set list to temp, the new front of list*/
    target->card_list = next_hand;

    /* Updated hand_size */
    target->hand_size+=1;

    return 0;
}

// test code
int remove_card(struct player* target, struct card* old_card){
    
    struct hand* iterator = target->card_list;
    struct hand* previous = NULL;
    if (iterator == NULL) { return 0; }         // reach end of the list
    while (iterator->top.rank[0] != old_card->rank[0] && iterator->top.suit != old_card->suit) {              // while the current card is not old_card move iterator
        previous = iterator;
        iterator = iterator->next;
        if (iterator == NULL)                   // if iterator reaches the front
            return 0;
        }
        
        /* Iterator found the last item; break out of loop */
        if (previous != NULL) 
            previous->next = iterator->next;
        else {
            target->card_list = iterator->next;
        }
        
        /* Free memory of item we are removing */
        free(iterator);
        
        /* Updated hand_size */
        target->hand_size-=1;

        /* Return true; we found and removed the item */
        return 1;

};

// test code
char check_add_book(struct player* target){
    if(target->hand_size==0){return 0;}     // check if the player has cards to begin with

    struct hand* cards = target->card_list;
    for(int i=0;i<sizeof(struct hand*);i++){
        struct card tcard = cards[i].top;   // get current card
        int count = 0;                      // count how many of the cards had been matched
        int indexes[4];
        int index = 0;
        for(int j=0;j<target->hand_size;j++){
            struct card scard = cards[j].top;
            if(tcard.rank[0] == scard.rank[0]){
                count++;
                indexes[index] = j;
                index++;
            }
        }
        
        if(book_index>=7){game_over(target);}

        // if there are 4 cards with the same rank
        if(count==4){
            // add rank to book
            target->book[book_index] = tcard.rank[0];
            // remove those cards
            for(int j=0;j<4;j++){
                struct card rcard = cards[j].top; 
                remove_card(target, &rcard);
                book_index++;
            }
            
            return tcard.rank[0];
        }
    }
    return 0;
}

// test code
int search(struct player* target, char rank){
    struct hand* cards = target->card_list;
    bool found = false;
    for(int i=0;i<target->hand_size;i++){
        struct card tcard = cards[i].top;
        if(tcard.rank[0] == rank){
            return 1;
            break;
        }
    }
    if(found==false){return 0;}
}

// test code
int transfer_cards(struct player* src, struct player* dest, char rank){
    
    struct hand* cards = src->card_list;
    bool found = false;
    int cards_transfered = 0;

    // move from src -> dest
    for(int i=0;i<src->hand_size;i++){
        
        struct card tcard = cards[i].top;
        int error = -1;
        
        if(tcard.rank[0] == rank){
            // if card is found set found to true
            found = true;

            // add transfered card(s) to dest
            error = add_card(dest,&tcard);
            if(error!=0){return -1;}

            // remove transfered card(s) from src
            error = remove_card(src,&tcard);
            if(error!=0){return -1;}

            cards_transfered++;
        }
    }
    if(found==false){return 0;}

    // return number of cards transfered.
    return cards_transfered;
}

// test code
int game_over(struct player* target){
    if(sizeof(target->book)>=7){ return 1; }
    return 0;
}

// test code
int reset_player(struct player* target){
    
    struct hand* cards = target->card_list;
    struct card tcard;

    while(target->hand_size>0){
        tcard = cards[0].top;
        remove_card(target,&tcard);
    }
    return 0;

}

// test code question: what happens when (not found)
char computer_play(struct player* target){

    char ranks[13] = {'2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K', 'A'};
    
    // Seed the random number generator
    srand(time(NULL));

    // Generate a random index within the valid range
    int randomIndex = rand()%13;

    char rank = ranks[randomIndex];
    int found = search(target,rank);
    while(found!=true){
        rank = ranks[rand()%13];
        found = search(target,rank);
        if(found == 1){
            return rank;
        } 
    }
    
}

//test code
char user_play(struct player* target){
    printf("Pick a rank user wishes to play: ");
    char rank = getchar();
    int found = search(target,rank);
    if(found == 1){
        return rank;
    } else {
        printf("Error - must have at least one card from rank to play");
        rank = getchar();
    }
}