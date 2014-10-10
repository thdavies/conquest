typedef int boolean;

typedef struct ssector {
        int enemy, star, tf;
} sector;

typedef struct ssector tboard[bdsize+1][bdsize+1];

typedef int tteam;
#define ENEMY   0
#define player  1
#define none    2

typedef int attribute[2];

typedef struct sttf {
                int x,y,xf,yf,
                s,t,c,b,
                dest,eta,origeta;
                int blasting, withdrew;
} ttf;

typedef struct ststar {
        int x, y;
        struct stplanet *first_planet;
        int visit[2];
} tstar;

typedef struct stplanet {
                int number,capacity,psee_capacity;
                int team;
                int inhabitants,iu,mb,amb;
                int conquered,under_attack;
                int esee_team;  /*the   team when the enemy last saw it*/
                int esee_def; /*the     mbs when enemy last saw it*/
                int pstar;
                struct stplanet *next;
} tplanet;

typedef struct sttf tfray[2][27];

typedef struct ststar starray[nstars+1];

typedef char tline[81];

typedef char alfa[6];

typedef float starlist[nstars+1];

typedef int toption;
#define right   0
#define left    1
#define both    2

typedef int termtype;
#define hardcopy        0
#define adm3    1
#define vt52    2
#define concept 3
#define vi      4
#define vis400  5


