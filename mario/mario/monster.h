#pragma once
#include "item.h"
#define MAX_MONSTERS 100
#define MAX_TURTLES 100
#define MAX_BOWSER 1
#define MAX_FIREBALLS 100

struct Monster
{
    int x, y;
    int vx, vy;
    int width, height;
    int leftBound, rightBound;
    bool isFalling;
    bool isAlive;
    bool isDead;
    bool active;
    DWORD deadstart;
};
extern Monster monsters[MAX_MONSTERS];
extern int monsterCount;

//turtle
typedef enum { NORMAL, SHELL, SPINNING } TurtleState;

struct Turtle
{
    int x, y;
    int vx, vy;
    int width, height;
    int direction;      // 0: left  1: right
    bool isFalling;
    bool isAlive;
    bool isDead;
    bool active;
    bool damage;

    TurtleState turtleState;
    int shellTimer;
    int damageTimer;
};
extern Turtle turtles[MAX_TURTLES];
extern int turtleCount;
extern Turtle brownTurtles[MAX_TURTLES];
extern int brownTurtleCount;

//angel turtle
enum { FLYING, HIDE };

struct AngelTurtle 
{
    int x, y;
    int width, height;
    int vy;
    int topY, bottomY;
    bool isFalling;
    bool isDead;
    bool goingUp;
    bool isAlive;
    int state;
    DWORD hideStartTime;
};
extern AngelTurtle angelTurtles[MAX_TURTLES];
extern int angelTurtleCount;

//boss bowser

struct Bowser 
{
    int hp;
    int x, y;
    int vx, vy;
    int width, height;
    bool ignore_tinofire;
    bool ignore_tinobite;
    bool isAlive;
    bool isDead;
    bool isJumping;
    bool isFalling;
    bool isFiring;
    int fireTimer;// �߰�: �� ���� ������ ����
    int frame;

    int jumpTimer;  //�����ϰ� �����ϱ�
    int jumpInterval;

    int startX;            // ���� ��ġ x ��ǥ
    int direction;         // �̵� ���� (-1: ����, 1: ������)
    int moveDistance;      // �̵��� �Ÿ� ����
    int maxDistance;

    int fireInterval;

    int fireDuration; // �� �ִϸ��̼� ���� �ð���
};
//fireball
struct Fireball 
{
    int x, y;
    int vx, vy;
    int width, height;
    int motion;
    bool active;
};
extern Fireball fireballs[MAX_FIREBALLS];
extern Bowser bowser;
extern Fireball bowserFire;

extern Bowser bowser;

void InitMonsters();
void InitMonsters2();
void InitMonsters3();
void InitTurtles();
void InitTurtles2();
void InitTurtles3();
void InitAngelTurtles();
void InitBowser();

void UpdateMonsters();
void CheckMarioMonsterCollision();

void UpdateTurtles();
void CheckMarioTurtleCollision();

void UpdateAngelTurtles();
void CheckMarioAngelTurtleCollision();

void UpdateBowser();
void UpdateFireballs();
void CheckMarioBowserCollision();
void CheckMarioFireballCollision();

void damage_mario();
