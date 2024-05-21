#ifndef BIBLIOTECA_H_INCLUDED
#define BIBLIOTECA_H_INCLUDED

struct echipa{
    char* name;
    int position;
    int wins;
    float points;
    float score;
    struct echipa *next;
};
typedef struct echipa Team;

struct meci{
    Team* team1;
    Team* team2;
    struct meci *next;
};
typedef struct meci Match;

struct QM{
    Match* front;
    Match* rear;
};
typedef struct QM queueMatch;

struct QT{
    Team* front;
    Team* rear;
};
typedef struct QT queueTeam;

struct graf{
    int V;
    int E;
    int** a;
};
typedef struct graf Graph;

void createInitialList(Team**, int, FILE*);
void freeTeam(Team*);
void writeList(Team*, FILE*);
Graph* createGraph(int);
void printGraph(Graph*, FILE*);
void freeGraph(Graph*);
queueMatch* createQueueMatch();
void enQueueMatch(queueMatch*, Match*);
int queueMatchIsEmpty(queueMatch*);
Match* deQueueMatch(queueMatch*);
void freeQueueMatch(queueMatch*);
queueTeam* createQueueTeam();
void enQueueTeam(queueTeam*, Team*);
int queueTeamIsEmpty(queueTeam*);
Team* deQueueTeam(queueTeam*);
void freeQueueTeam(queueTeam*);
void moveMatchesFromListToQueue(queueMatch*, Team**, int);
void push(Team**, Team*);
int stackIsEmpty(Team*);
Team* pop(Team**);
void calculateScores(queueTeam*, float, int, FILE*);

#endif // BIBLIOTECA_H_INCLUDED