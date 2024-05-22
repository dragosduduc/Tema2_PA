#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "biblioteca.h"

//se formează lista de echipe cu informațiile din fișierul de intrare
void createInitialList(Team** head, int numberOfTeams, FILE* in){
    int i, pos = 0;
    char buffer[50];
    for(i = 0; i < numberOfTeams; i++){
        Team* team = (Team*)malloc(sizeof(Team));
        if(team == NULL){
            printf("Memory allocation failed.\n");
            exit(1);
        }
        team->next = NULL;
        team->wins = 0;
        team->position = ++pos;
        fscanf(in, "%f", &team->points);
        //se citește separat spațiul dintre numărul de membri și numele echipei
        fgetc(in);
        //se citește numele echipei într-un buffer și se formatează
        fgets(buffer, 50, in);
        buffer[strlen(buffer)-1] = '\0';
        if(buffer[strlen(buffer) - 1] == ' ')
            buffer[strlen(buffer) - 1] = '\0';
        //se alocă memorie la pointer-ul din structură și se copiază string-ul din buffer în memoria de la pointer
        team->name = (char*)malloc((strlen(buffer) + 1) * sizeof(char));
        if(team->name == NULL){
            printf("Memory allocation failed.\n");
            exit(1);
        }
        strcpy(team->name, buffer);
        //se adaugă echipa la începutul listei
        team->next = *head;
        *head = team;
    }
    //în acest moment, echipele sunt în ordine inversă în listă, așa că se inversează folosind o altă listă
    Team* copy = NULL;
    while (*head != NULL) {
        Team* nextNode = (*head)->next;
        (*head)->next = copy;
        copy = *head;
        *head = nextNode;
    }
    *head = copy;
}

//se eliberează spațiul de memorie alocat unei echipe
void freeTeam(Team* team){
    if(team == NULL) return;
    //se eliberează numele echipei
    if(team->name != NULL)
        free(team->name);
    //se eliberează structura în sine
    free(team);
}

//scrierea unei liste într-un fișier
void writeList(Team* head, FILE* out){
    Team* iter = head;
    //cât timp elementul curent NU este NULL, se afișează și se trece la next
    while(iter != NULL){
        fprintf(out, "%s   %d    %.2f    %.2f\n", iter->name, iter->position, iter->points, iter->score);
        iter = iter->next;
    }
}

//se inițializează un graf
Graph* createGraph(int numberOfNodes){
    int i;
    Graph* g = (Graph*)malloc(sizeof(Graph));
    if(g == NULL){
        printf("Memory allocation failed.\n");
        exit(1);
    }
    g->V = numberOfNodes;
    g->E = 0;
    g->a = (int**)malloc((g->V + 1) * sizeof(int*));
    if(g->a == NULL){
        printf("Memory allocation failed.\n");
        exit(1);
    }
    for(i = 1; i <= g->V; i++){
        g->a[i] = (int*)calloc((g->V + 1), sizeof(int));
        if(g->a[i] == NULL){
            printf("Memory allocation failed.\n");
            exit(1);
        }
    }
    return g;
}

//se printează matricea de adiacentă a unui graf într-un fișier
void printGraph(Graph* g, FILE* out){
    int i, j;
    for(i = 1; i <= g->V; i++){
        for(j = 1; j <= g->V; j++)
            fprintf(out, "%d ", g->a[i][j]);
        fprintf(out, "\n");
    }
}

//se eliberează memoria alocată unui graf
void freeGraph(Graph* g){
    int i;
    if(g != NULL){
        for(i = 1; i <= g->V; i++)
            free(g->a[i]);
        free(g->a);
        free(g);
    }
}

//createQueue din curs, adaptat la o coadă de meciuri
queueMatch* createQueueMatch(){
    queueMatch* q;
    q = (queueMatch*)malloc(sizeof(queueMatch));
    if(q == NULL){
        printf("Memory allocation failed.\n");
        exit(1);
    }
    if(q == NULL) return NULL;
    q->front = NULL;
    q->rear = NULL;
    return q;
}

//enQueue din curs, adaptat la o coadă de meciuri
void enQueueMatch(queueMatch* q, Match* game){
    if(q->rear == NULL){
        q->rear = game;
        q->front = game;
    }else{
        (q->rear)->next = game;
        q->rear = game;
    }
    game->next = NULL;
}

//isEmpty pentru cozi de meciuri
int queueMatchIsEmpty(queueMatch* q){
    return (q->front == NULL);
}

//deQueue din curs, adaptat la o coadă de meciuri
Match* deQueueMatch(queueMatch* q){
    Match* aux;
    if(queueMatchIsEmpty(q)){
        aux = (Match*)malloc(sizeof(Match));
        if(aux == NULL){
            printf("Memory allocation failed.\n");
            exit(1);
        }
        aux->team1 = (Team*)malloc(sizeof(Team));
        if(aux->team1 == NULL){
            printf("Memory allocation failed.\n");
            exit(1);
        }
        aux->team2 = (Team*)malloc(sizeof(Team));
        if(aux->team2 == NULL){
            printf("Memory allocation failed.\n");
            exit(1);
        }
        strcpy(aux->team1->name, "Queue is empty.");
        strcpy(aux->team2->name, "Queue is empty.");
        printf("Queue is empty.\n");
        return aux;
    }
    aux = q->front;
    q->front = (q->front)->next;
    aux->next = NULL;
    if(q->front == NULL)
        q->rear = NULL;
    return aux;
}

//eliberarea spațiului alocat unei cozi de meciuri
void freeQueueMatch(queueMatch* q){
    if(q == NULL) return;
    if(q->front != NULL)
        free(q->front);
    if(q->rear != NULL)
        free(q->rear);
    free(q);
}

//createQueue din curs, adaptat la o coadă de echipe
queueTeam* createQueueTeam(){
    queueTeam* q;
    q = (queueTeam*)malloc(sizeof(queueTeam));
    if(q == NULL){
        printf("Memory allocation failed.\n");
        exit(1);
    }
    if(q == NULL) return NULL;
    q->front = NULL;
    q->rear = NULL;
    return q;
}

//enQueue din curs, adaptat la o coadă de echipe
void enQueueTeam(queueTeam* q, Team* team){
    if(q->rear == NULL){
        q->rear = team;
        q->front = team;
    }else{
        (q->rear)->next = team;
        q->rear = team;
    }
    team->next = NULL;
}

//isEmpty pentru cozi de echipe
int queueTeamIsEmpty(queueTeam* q){
    return (q->front == NULL);
}

//deQueue din curs, adaptat la o coadă de echipe
Team* deQueueTeam(queueTeam* q){
    Team* aux;
    if(queueTeamIsEmpty(q)){
        aux = (Team*)malloc(sizeof(Team));
        if(aux == NULL){
            printf("Memory allocation failed.\n");
            exit(1);
        }
        strcpy(aux->name, "Queue is empty.");
        printf("Queue is empty.\n");
        return aux;
    }
    aux = q->front;
    q->front = (q->front)->next;
    aux->next = NULL;
    if(q->front == NULL)
        q->rear = NULL;
    return aux;
}

//eliberarea spațiului alocat unei cozi de echipe
void freeQueueTeam(queueTeam* q){
    if(q == NULL) return;
    if(q->front != NULL)
        free(q->front);
    if(q->rear != NULL)
        free(q->rear);
    free(q);
}

//se mută elementele listei de echipe în coada de meciuri
void moveMatchesFromListToQueue(queueMatch* q, Team** head, int bestTeams){
    int i;
    //din listă se scot câte două echipe alăturate și se pun într-un meci în coadă
    for(i = 0; i < bestTeams / 2; i++){
            Match* game = (Match*)malloc(sizeof(Match));
            if(game == NULL){
                printf("Memory allocation failed.\n");
                exit(1);
            }
            game->team1 = *head;
            *head = (*head)->next;
            game->team2 = *head;
            *head = (*head)->next;
            enQueueMatch(q, game);
    }
}

//push din curs, modificată pentru tipul Team
void push(Team** top, Team* team){
    team->next = *top;
    *top = team;
}

//isEmpty pentru stive
int stackIsEmpty(Team* top){
    return (top == NULL);
}

//pop din curs, modificată pentru tipul Team
Team* pop(Team** top){
    if(stackIsEmpty(*top)){
        printf("Stack is empty.\n");
        return NULL;
    }
    Team* aux = *top;
    *top = (*top)->next;
    aux->next = NULL;
    return aux;
}

//se extrag meciurile din coada de meciuri, se stabilesc câștigătorii și pierzătorii, se umplu cozile respective și se formează graful turneului
void playMatches(queueMatch* cnt, queueTeam* winners, queueTeam* losers, Graph* g){
    while(!queueMatchIsEmpty(cnt)){
        Match* temp = deQueueMatch(cnt);
        if(temp->team1->points > temp->team2->points || (temp->team1->points == temp->team2->points && strcmp(temp->team1->name, temp->team2->name) > 0)){
            temp->team1->wins++;
            enQueueTeam(winners, temp->team1);
            enQueueTeam(losers, temp->team2);
            g->a[temp->team2->position][temp->team1->position] = 1;
        }else if(temp->team1->points < temp->team2->points || (temp->team1->points == temp->team2->points && strcmp(temp->team1->name, temp->team2->name) < 0)){
            temp->team2->wins++;
            enQueueTeam(winners, temp->team2);
            enQueueTeam(losers, temp->team1);
            g->a[temp->team1->position][temp->team2->position] = 1;
        }
    }
}

//se mută elementele din coada de câștigători în coada de meciuri
void moveTeamsFromQueueTeamToQueueMatch(queueMatch* games, queueTeam* winners, int numberOfTeams){
    int i;
    for(i = 0; i < numberOfTeams / 2; i++){
        Match* temp = (Match*)malloc(sizeof(Match));
        temp->team1 = deQueueTeam(winners);
        temp->team2 = deQueueTeam(winners);
        enQueueMatch(games, temp);
    }
}

//se calculează scorul unei echipe pe baza formulei din îndrumar, se afișează și se eliberează spațiul de memorie alocat echipei
void calculateScores(queueTeam* head, float q, int totalMatches, FILE* out){
    while(!queueTeamIsEmpty(head)){
        Team* iter = deQueueTeam(head);
        iter->score = (q * pow(2 - q, iter->wins)) / (pow(2, totalMatches) + pow(2 - q, totalMatches) * (q - 1));
        fprintf(out, "%.4f %s\n", iter->score, iter->name);
        freeTeam(iter);
    }
}