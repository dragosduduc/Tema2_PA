#ifndef BIBLIOTECA_H_INCLUDED
#define BIBLIOTECA_H_INCLUDED

struct jucator{
    char* firstName;
    char* secondName;
    int points;
};
typedef struct jucator Player;

struct echipa{
    int numberOfPlayers;
    char* name;
    int position;
    int wins;
    Player* members;
    float points;
    float score;
    struct echipa *next;
};
typedef struct echipa Team;

struct graf{
    int V;
    int E;
    int** a;
};
typedef struct graf Graph;

struct meci{
    Team* team1;
    Team* team2;
    struct meci *next;
};
typedef struct meci Match;

struct Q{
    Match* front;
    Match* rear;
};
typedef struct Q Queue;

void createInitialList(Team**, int, FILE*);
int teamsRemaining(int);
void freeTeam(Team*);
void eliminateWorstTeams(Team**, int);
void writeList(Team*, FILE*);
void writeListWithPointsAndPosition(Team*, FILE*);
void writeListWithPoints(Team*, FILE*);
Graph* createGraph(int);
Queue* createQueue();
void enQueue(Queue*, Match*);
int queueIsEmpty(Queue*);
Match* deQueue(Queue*);
void freeQueue(Queue*);
void moveMatchesFromListToQueue(Queue*, Team**, int);
void writeQueue(Queue*, FILE*);
void push(Team**, Team*);
int stackIsEmpty(Team*);
Team* pop(Team**);
void deleteStack(Team**);
void moveMatchesFromStackToQueue(Queue*, Team**, int, FILE*);
void matchResult(Team*, Team*, Team**, Team**, Graph*);
void copyTeamsFromStackToList(Team*, Team**);
void playMatches(Queue*, Team**, Team**, Graph*);
void printGraph(Graph*, FILE*);

#endif // BIBLIOTECA_H_INCLUDED