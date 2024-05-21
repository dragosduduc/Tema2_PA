#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "biblioteca.h"

int main(int argc, char* argv[]){

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

    //se creează și se populează coada de meciuri
    queueMatch* matches = createQueueMatch();
    moveMatchesFromListToQueue(matches, &mainList, numberOfTeams);

    //se inițializează cozile pentru câștigători și pierzători
    queueTeam* winnerQueue = createQueueTeam();
    queueTeam* loserQueue = createQueueTeam();

    //cât timp au mai rămas meciuri/runde de jucat, se joacă
    int COPYnumberOfTeam = numberOfTeams;
    int numberOfGames = 0;
    while(COPYnumberOfTeam > 1){
        numberOfGames++;
        while(!queueMatchIsEmpty(matches)){
            Match* cnt = deQueueMatch(matches);
            if(cnt->team1->points > cnt->team2->points || (cnt->team1->points == cnt->team2->points && strcmp(cnt->team1->name, cnt->team2->name) > 0)){
                cnt->team1->wins++;
                enQueueTeam(winnerQueue, cnt->team1);
                enQueueTeam(loserQueue, cnt->team2);
                g->a[cnt->team2->position][cnt->team1->position] = 1;
            }else if(cnt->team1->points < cnt->team2->points || (cnt->team1->points == cnt->team2->points && strcmp(cnt->team1->name, cnt->team2->name) < 0)){
                cnt->team2->wins++;
                enQueueTeam(winnerQueue, cnt->team2);
                enQueueTeam(loserQueue, cnt->team1);
                g->a[cnt->team1->position][cnt->team2->position] = 1;
            }
        }
        COPYnumberOfTeam = COPYnumberOfTeam >> 1;
        for(int i = 0; i < COPYnumberOfTeam / 2; i++){
            Match* temp = (Match*)malloc(sizeof(Match));
            temp->team1 = deQueueTeam(winnerQueue);
            temp->team2 = deQueueTeam(winnerQueue);
            enQueueMatch(matches, temp);
        }

    }
    enQueueTeam(loserQueue, deQueueTeam(winnerQueue));

    //se eliberează spațiul de memorie alocat cozii de meciuri
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

    FILE* outScores;
    if((outScores = fopen(argv[3], "wt")) == NULL){
        printf("Fișierul 'scoruri' nu a putut fi deschis.\n");
        exit(1);
    }

    //se calculează scorurile echipelor și se scriu în fișierul -scoruri-
    float q = 0.15;
    calculateScores(loserQueue, q, numberOfGames + 1, outScores);

    fclose(outScores);

    //se eliberează spațiul de memorie alocat grafului
    freeGraph(g);

    return 0;
}