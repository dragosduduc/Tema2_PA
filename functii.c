#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "biblioteca.h"

void createInitialList(Team** head, int numberOfTeams, FILE* in){
    int i,j;
    char buffer1[50], buffer2[50];
    for(i = 0; i < numberOfTeams; i++){
        Team* team = (Team*)malloc(sizeof(Team));
        if(team == NULL){
            printf("Memory allocation failed.\n");
            exit(1);
        }
        team->next = NULL;
        team->points = 0;
        team->wins = 0;
        fscanf(in, "%d", &team->numberOfPlayers);
        //se citește separat spațiul dintre numărul de membri și numele echipei
        fgetc(in);
        //se citește numele echipei într-un buffer și se formatează
        fgets(buffer1, 50, in);
        buffer1[strlen(buffer1)-2] = '\0';
        if(buffer1[strlen(buffer1) - 1] == ' ')
            buffer1[strlen(buffer1) - 1] = '\0';
        //se alocă memorie la pointer-ul din structură și se copiază string-ul din buffer în memoria de la pointer
        team->name = (char*)malloc((strlen(buffer1) + 1) * sizeof(char));
        if(team->name == NULL){
            printf("Memory allocation failed.\n");
            exit(1);
        }
        strcpy(team->name, buffer1);
        //se citesc, în mod asemănător, informațiile legate de jucători
        team->members = (Player*)malloc(team->numberOfPlayers * sizeof(Player));
        if(team->members == NULL){
            printf("Memory allocation failed.\n");
            exit(1);
        }
        for(j = 0; j < team->numberOfPlayers; j++){
            fscanf(in, "%s %s %d", buffer1, buffer2, &team->members[j].points);
            if(buffer1[strlen(buffer1) - 1] == ' ')
                buffer1[strlen(buffer1) - 1] = '\0';
            if(buffer2[strlen(buffer2) - 1] == ' ')
                buffer2[strlen(buffer2) - 1] = '\0';
            team->members[j].firstName = (char*)malloc((strlen(buffer1) + 1) * sizeof(char));
            if(team->members[j].firstName == NULL){
                printf("Memory allocation failed.\n");
                exit(1);
            }
            team->members[j].secondName = (char*)malloc((strlen(buffer2) + 1) * sizeof(char));
            if(team->members[j].secondName == NULL){
                printf("Memory allocation failed.\n");
                exit(1);
            }
            strcpy(team->members[j].firstName, buffer1);
            strcpy(team->members[j].secondName, buffer2);
            team->points += team->members[j].points;
        }
        //se calculează media aritmetică specifică fiecărei echipe
        team->points = team->points / (team->numberOfPlayers * 1.0);
        //se adaugă echipa la începutul listei
        team->next = *head;
        *head = team;
    }
}

int teamsRemaining(int numberOfTeams){
    int bestTeams = 1;
    //se folosesc operațiile pe biți pentru a obține cea mai mare putere a lui 2 mai mică decât numărul total de echipe
    while(bestTeams < numberOfTeams)
        bestTeams = bestTeams << 1;
    if(bestTeams > numberOfTeams)
        bestTeams = bestTeams >> 1;
    return bestTeams;
}

//se eliberează spațiul de memorie alocat unei echipe
void freeTeam(Team* team){
    if(team == NULL) return;
    //se eliberează numele echipei
    if(team->name != NULL)
        free(team->name);
    int i;
    //se eliberează toate numele coechipierilor
    for(i = 0; i < team->numberOfPlayers; i++){
        if(team->members[i].firstName != NULL)
            free(team->members[i].firstName);
        if(team->members[i].secondName != NULL)
            free(team->members[i].secondName);
    }
    //se eliberează tabloul de coechipieri
    if(team->members != NULL)
        free(team->members);
    //se eliberează structura în sine
    free(team);
}

void eliminateWorstTeams(Team** head, int worstTeams){
    int i;
    Team* iter;
    //se repetă ciclul căutare minim + ștergere minim pentru câte elemente trebuie șterse
    for(i = 0; i < worstTeams; i++){
        float minTeamPoints = INT_MAX;
        char minTeamPointsName[50] = "";
        iter = *head;
        //căutare minim
        while(iter != NULL){
            if(iter->points < minTeamPoints){
                minTeamPoints = iter->points;
                strcpy(minTeamPointsName, iter->name);
            }
            iter = iter->next;
        }
        //ștergere minim, dacă minimul este primul element din listă
        if(strcmp((*head)->name, minTeamPointsName) == 0){
            Team* headcopy = *head;
            *head = (*head)->next;
            freeTeam(headcopy);
        }else{//ștergere minim, dacă minimul NU este primul element din listă
            //elementul precedent elementului curent
            Team* iter_prev = *head;
            iter = *head;
            int deleted = 0;
            //nu se mai continuă parcurgerea listei dacă am șters deja elementul
            while(iter != NULL && deleted == 0)
                if(strcmp(iter->name, minTeamPointsName) != 0){
                    iter_prev = iter;
                    iter = iter->next;
                }else{
                    //ștergerea efectivă
                    iter_prev->next = iter->next;
                    freeTeam(iter);
                    deleted = 1;
                }
        }
    }
    //se parcurge lista pentru a se stabili pozițiile în turneu
    int pos = 0;
    iter = *head;
    while(iter != NULL){
        iter->position = ++pos;
        iter = iter->next;
    }
}

void writeList(Team* head, FILE* out){
    Team* iter = head;
    //cât timp elementul curent NU este NULL, i se afișează numele și se trece la next
    while(iter != NULL){
        fprintf(out, "%s\n", iter->name);
        iter = iter->next;
    }
}

//aceeași funcție ca și writeList, dar se afișează și poziția echipelor în turneu echipelor
void writeListWithPointsAndPosition(Team* head, FILE* out){
    Team* iter = head;
    //cât timp elementul curent NU este NULL, i se afișează numele și se trece la next
    while(iter != NULL){
        fprintf(out, "%s   %d    %.2f\n", iter->name, iter->position, iter->points);
        iter = iter->next;
    }
}

//aceeași funcție ca și writeList, dar se afișează și punctajul echipelor
void writeListWithPoints(Team* head, FILE* out){
    Team* iter = head;
    while(iter != NULL){
        fprintf(out, "%-33s -  %.2f\n", iter->name, iter->points);
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

//createQueue din curs
Queue* createQueue(){
    Queue* q;
    q = (Queue*)malloc(sizeof(Queue));
    if(q == NULL){
        printf("Memory allocation failed.\n");
        exit(1);
    }
    if(q == NULL) return NULL;
    q->front = NULL;
    q->rear = NULL;
    return q;
}

//enQueue din curs, modificată pentru elemente de tip Match
void enQueue(Queue* q, Match* game){
    if(q->rear == NULL){
        q->rear = game;
        q->front = game;
    }else{
        (q->rear)->next = game;
        q->rear = game;
    }
    game->next = NULL;
}

//isEmpty pentru cozi
int queueIsEmpty(Queue* q){
    return (q->front == NULL);
}

//deQueue din curs, modificată pentru elemente de tip Match
Match* deQueue(Queue* q){
    Match* aux;
    if(queueIsEmpty(q)){
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

void freeQueue(Queue* q){
    if(q == NULL) return;
    if(q->front != NULL)
        free(q->front);
    if(q->rear != NULL)
        free(q->rear);
    free(q);
}

void moveMatchesFromListToQueue(Queue* q, Team** head, int bestTeams){
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
            enQueue(q, game);
    }
}

//scriere(cu format) din coadă
void writeQueue(Queue* q, FILE* out){
    while(!queueIsEmpty(q)){
        Match* cnt = (Match*)malloc(sizeof(Match));
        if(cnt == NULL){
            printf("Memory allocation failed.\n");
            exit(1);
        }
        cnt = deQueue(q);
        fprintf(out, "%-32s - %32s\n", cnt->team1->name, cnt->team2->name);
        free(cnt);
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

//pop din curs, modificată pentru tipul Team(returnează chiar elementul - același bloc de memorie)
Team* pop(Team** top){
    Team* aux;
    if(stackIsEmpty(*top)){
        aux = (Team*)malloc(sizeof(Team));
        if(aux == NULL){
            printf("Memory allocation failed.\n");
            exit(1);
        }
        strcpy(aux->name, "Stack is empty.");
        printf("Stack is empty.\n");
        return aux;
    }
    aux = *top;
    *top = (*top)->next;
    aux->next = NULL;
    return aux;
}

//deleteStack din curs
void deleteStack(Team** top){
    Team* temp;
    while(!stackIsEmpty(*top)){
        temp = *top;
        *top = (*top)->next;
        free(temp);
    }
}

void moveMatchesFromStackToQueue(Queue* q, Team** stackTop, int bestTeams, FILE* out){
    int i;
    //din stivă se scot câte două echipe alăturate și se pun într-un meci în coadă
    for(i = 0; i < bestTeams / 2; i++){
        Match* game = (Match*)malloc(sizeof(Match));
        if(game == NULL){
            printf("Memory allocation failed.\n");
            exit(1);
        }
        game->team1 = pop(stackTop);
        game->team2 = pop(stackTop);
        enQueue(q, game);
    }
}

void matchResult(Team* firstTeam, Team* secondTeam, Team** winners, Team** losers, Graph* g){
    int i;
    //pe baza punctajului pe echipă, se stabilesc câștigătorii și pierzătorii, se pun în stivele corespunzătoare și se actualizează punctajele
    if(firstTeam->points > secondTeam->points){
        push(winners, firstTeam);
        push(losers, secondTeam);
        firstTeam->points++;
        for(i = 0; i < firstTeam->numberOfPlayers; i++)
            firstTeam->members[i].points++;
        g->a[secondTeam->position][firstTeam->position] = 1;
        firstTeam->wins++;
    }else{
        push(winners, secondTeam);
        push(losers, firstTeam);
        secondTeam->points++;
        for(i = 0; i < secondTeam->numberOfPlayers; i++)
            secondTeam->members[i].points++;
        g->a[firstTeam->position][secondTeam->position] = 1;
        secondTeam->wins++;
    }
}

//se COPIAZĂ(dintr-un bloc de memorie în altul) echipele dintr-o stivă într-o listă
void copyTeamsFromStackToList(Team* stack, Team** head ){
    while(stack != NULL){
        Team* temp = stack;
        stack = stack->next;
        Team* newElement = (Team*)malloc(sizeof(Team));
        if(newElement == NULL){
            printf("Memory allocation failed.\n");
            exit(1);
        }
        *newElement = *temp;
        newElement->next = *head;
        *head = newElement;
    }
}

//se scot meciurile din coadă și se stabilește rezultatul lor folosind funcția matchResult
void playMatches(Queue* q, Team** win, Team** lose, Graph* g){
    while(!queueIsEmpty(q)){
                Match* cntMatch = deQueue(q);
                matchResult(cntMatch->team1, cntMatch->team2, win, lose, g);
            }
}

void printGraph(Graph* g, FILE* out){
    int i, j;
    for(i = 1; i <= g->V; i++){
        for(j = 1; j <= g->V; j++)
            fprintf(out, "%d ", g->a[i][j]);
        fprintf(out, "\n");
    }
}