#include <stdio.h>
#include <stdlib.h>
#include "biblioteca.h"

int main(int argc, char* argv[]){

    //fiecare turneu(fișier de intrare) conține 32 de echipe
    int numberOfTeams = 32;

    //se deschide fișierul de intrare
    FILE* in;
    if((in = fopen(argv[1], "rt")) == NULL){
        printf("Fisierul de intrare nu a putut fi deschis.\n");
        exit(1);
    }

    //se formează lista de echipe
    Team* mainList = NULL;
    createInitialList(&mainList, numberOfTeams, in);

    fclose(in);

    //se inițializează graful orientat asociat turneului
    Graph* g = createGraph(numberOfTeams);

    //se inițializează cozile pentru câștigători și pierzători (loserQueue va conține, în final, toate echipele în ordinea în care au fost eliminate din turneu)
    queueTeam* winnerQueue = createQueueTeam();
    queueTeam* loserQueue = createQueueTeam();

    //se creează și se populează coada de meciuri
    queueMatch* matches = createQueueMatch();
    moveMatchesFromListToQueue(matches, &mainList, numberOfTeams);

    //cât timp se mai pot juca meciuri(adica mai sunt cel puțin două echipe în turneu), se joacă
    int COPYnumberOfTeams = numberOfTeams;
    int numberOfGamesPlayed = 0;

    while(COPYnumberOfTeams > 1){
        numberOfGamesPlayed++;

        //se extrag meciurile din coada de meciuri, se stabilesc câștigătorii și pierzătorii, se umplu cozile respective și se formează graful turneului
        playMatches(matches, winnerQueue, loserQueue, g);

        //se înjumătățește numărul de echipe rămase în turneu(rămân doar câștigătorii)
        COPYnumberOfTeams = COPYnumberOfTeams >> 1;

        //câștigătorii se scot din coadă și se pun înapoi în coada de meciuri, pentru a se relua ciclul pentru o nouă rundă
        moveTeamsFromQueueTeamToQueueMatch(matches, winnerQueue, COPYnumberOfTeams);
    }

    //echipa câștigătoare, care a rămas în coada de câștigători(nu s-a mai putut forma un meci), se mută în coada de pierzători care, acum, conține toate echipele în ordinea în care au fost eliminate din turneu
    enQueueTeam(loserQueue, deQueueTeam(winnerQueue));

    //se eliberează spațiul de memorie alocat cozilor de câștigători și de meciuri
    freeQueueTeam(winnerQueue);
    freeQueueMatch(matches);

    //se scrie matricea de adiacență a grafului turneului în fișierul -graf-
    FILE* outGraph;
    if((outGraph = fopen(argv[2], "wt")) == NULL){
        printf("Fișierul 'graf' nu a putut fi deschis.\n");
        exit(1);
    }
    printGraph(g, outGraph);
    fclose(outGraph);

    //se calculează scorurile echipelor și se scriu în fișierul -scoruri-
    FILE* outScores;
    if((outScores = fopen(argv[3], "wt")) == NULL){
        printf("Fișierul 'scoruri' nu a putut fi deschis.\n");
        exit(1);
    }
    float q = 0.15;
    calculateScores(loserQueue, q, numberOfGamesPlayed + 1, outScores);
    fclose(outScores);

    //se eliberează spațiul de memorie alocat cozii de pierzători și grafului turneului
    freeQueueTeam(loserQueue);
    freeGraph(g);

    return 0;
}