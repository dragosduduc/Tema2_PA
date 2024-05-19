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

    //se inițializează graful orientat asociat turneului
    Graph* g = createGraph(maxPower);

    //se creează și se populează coada de meciuri
    Queue* matches;
    matches = createQueue();
    moveMatchesFromListToQueue(matches, &mainList, maxPower);

    //se inițializează stiva pentru câștigători lista de pierzători care, în final, va conține toate echipele cu informațiile actualizate
    Team* winnerStackTop = NULL;
    Team* newList = NULL;

    //cât timp au mai rămas meciuri/runde de jucat, se joacă
    int totalRounds = 0;
    while(maxPower > 1){
        totalRounds++;

        winnerStackTop = NULL;

        playMatches(matches, &winnerStackTop, &newList, g);
        
        //se înjumătățește numărul de echipe - rămân doar câștigătorii, se afișează și se pun înapoi în coada de meciuri
        maxPower = maxPower >> 1;
        moveMatchesFromStackToQueue(matches, &winnerStackTop, maxPower);
    }

    //se eliberează spațiul de memorie alocat cozii de meciuri
    freeQueue(matches);

    //echipa câștigătoare, care a rămas în stiva de câștigători, se pune pe poziția corectă în noua listă
    insertAtCorrectPosition(&newList, pop(&winnerStackTop));

    //se scrie matricea de adiacență a grafului turneului în fișierul -graf-
    FILE* outGraph;
    if((outGraph = fopen("graf", "wt")) == NULL){
        printf("Fișierul 'graf' nu a putut fi deschis.\n");
        exit(1);
    }
    printGraph(g, outGraph);
    fclose(outGraph);

    //se calculează scorurile echipelor și se scriu în fișierul -scoruri-
    float q = 0.15;
    calculateScores(newList, q, totalRounds);

    FILE* outScores;
    if((outScores = fopen("scoruri", "wt")) == NULL){
        printf("Fișierul 'scoruri' nu a putut fi deschis.\n");
        exit(1);
    }
    writeList(newList, outScores);
    fclose(outScores);

    //se eliberează spațiul de memorie alocat noii liste
    Team* iter = newList;
    while(iter != NULL){
        freeTeam(iter);
        iter = iter->next;
    }

    //se eliberează spațiul de memorie alocat grafului
    freeGraph(g);

    return 0;
}