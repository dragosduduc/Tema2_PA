#include <stdio.h>
#include <stdlib.h>
#include "biblioteca.h"

int main(int argc, char* argv[]){

    //se deschide fișierul de intrare d.in
    FILE* in;
    if((in = fopen(argv[1], "rt")) == NULL){
        printf("Fisierul de intrare nu a putut fi deschis.\n");
        exit(1);
    }

    //se citește numărul de echipe din fișierul de intrare
    int numberOfTeams;
    fscanf(in, "%d", &numberOfTeams);

    //se formează lista de echipe
    Team* mainList = NULL;
    createInitialList(&mainList, numberOfTeams, in);

    fclose(in);

    //se calculează cea mai mare putere a lui 2 mai mică decât numărul total de echipe
    int maxPower;
    maxPower = teamsRemaining(numberOfTeams);

    int teamsToEliminate = numberOfTeams - maxPower;

    //se elimină echipele cu cele mai mici punctaje, pentru a se ajunge la un număr de echipe - putere a lui 2
    eliminateWorstTeams(&mainList, teamsToEliminate);

    //se deschide fișierul de ieșire
    FILE* out;
    if((out = fopen(argv[2], "wt")) == NULL){
        printf("Fisierul de ieșire nu a putut fi deschis.\n");
        exit(1);
    }
    
    //se scrie lista formată în fișierul de ieșire
    writeListWithPointsAndPosition(mainList, out);

    //se inițializează graful orientat asociat turneului
    Graph* g = createGraph(maxPower);

    //se creează și se populează coada de meciuri
    Queue* q;
    q = createQueue();
    moveMatchesFromListToQueue(q, &mainList, maxPower);

    //se inițializează stivele pentru câștigători și pierzători și lista pentru ultimele opt echipe
    Team* winnerStackTop;
    Team* loserStackTop;

    //cât timp au mai rămas meciuri/runde de jucat, se joacă
    while(maxPower > 1){

        winnerStackTop = loserStackTop = NULL;

        playMatches(q, &winnerStackTop, &loserStackTop, g);

        //se șterg echipele care au pierdut în runda curentă
        deleteStack(&loserStackTop);
        
        //se înjumătățește numărul de echipe - rămân doar câștigătorii, se afișează și se pun înapoi în coada de meciuri
        maxPower = maxPower >> 1;
        moveMatchesFromStackToQueue(q, &winnerStackTop, maxPower, out);
    }

    //se eliberează spațiul de memorie alocat cozii de meciuri
    freeQueue(q);

    //se scrie matricea de adiacență a grafului turneului în fișierul -graf-
    FILE* graf_out;
    if((graf_out = fopen("graf", "wt")) == NULL){
        printf("Fișierul 'graf' nu a putut fi deschis.\n");
        exit(1);
    }
    printGraph(g, graf_out);
    fclose(graf_out);

    fclose(out);

    return 0;
}