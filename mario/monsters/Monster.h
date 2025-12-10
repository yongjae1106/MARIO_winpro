#pragma once

#include "../PacketInfo.h"

class GameWorld;

// Data structure to hold monster state received from the server
//struct MonsterDataPacket {
//    int monsterID;
//    int type; // Should correspond to Monster::MonsterType enum
//    int x, y;
//    bool isAlive;
//    int walk_motion;
//    int direction; // 0 for left, 1 for right
//
//    // Specific states for derived classes (client-side relevant only)
//    int turtleState; // Corresponds to Turtle::TurtleState enum (for Turtles)
//    bool isFirinㅠg;   // For Bowser
//    // Add any other specific state that needs to be communicated
//};

class Monster {
public:
    enum class MonsterType { NormalGoomba, RedGoomba, BlueGoomba, GreenTurtle, BrownTurtle, AngelTurtle, Bowser };

    Monster(MonsterType type, int x, int y, int width, int height);
    virtual ~Monster() = default;

    // --- Client-side methods ---
    virtual void update();
    virtual void updateAnimation();
    virtual void updateStateFromServer(const MonsterDataPacket& packet); // Now virtual
    // --------------------------

    // --- Setters (서버 동기화 및 물리 처리를 위해 추가) ---
    void setX(int newX) { x = newX; }
    void setY(int newY) { y = newY; }
    void setVx(int newVx) { vx = newVx; }
    void setVy(int newVy) { vy = newVy; }
    void setAlive(bool isAlive) { alive = isAlive; }
    // -----------------------------------------------------

    // --- Common getters ---
    MonsterType getType() const;
    int getX() const;
    int getY() const;
    int getVx() const { return vx; } // 필요 시 추가
    int getVy() const { return vy; } // 필요 시 추가
    int getWidth() const;
    int getHeight() const;
    bool isAlive() const;
    int getWalkMotion() const;
    int getDirection() const;

protected:
    MonsterType type;
    int x, y;
    int vx, vy; // [추가] 속도 변수가 선언되어 있어야 Setter/Getter가 작동합니다.
    int width, height;
    bool alive;
    int walk_motion;
    int direction; // 0 for left, 1 for right
};