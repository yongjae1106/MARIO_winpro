#pragma once

class GameWorld;

// Data structure to hold monster state received from the server
struct MonsterDataPacket {
    int monsterID;
    int type; // Should correspond to Monster::MonsterType enum
    int x, y;
    bool isAlive;
    int walk_motion;
    int direction; // 0 for left, 1 for right

    // Specific states for derived classes (client-side relevant only)
    int turtleState; // Corresponds to Turtle::TurtleState enum (for Turtles)
    bool isFirin¤Ðg;   // For Bowser
    // Add any other specific state that needs to be communicated
};

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

    // --- Common getters ---
    MonsterType getType() const;
    int getX() const;
    int getY() const;
    int getWidth() const;
    int getHeight() const;
    bool isAlive() const;
    int getWalkMotion() const;
    int getDirection() const;

protected:
    MonsterType type;
    int x, y;
    int width, height;
    bool alive;
    int walk_motion;
    int direction; // 0 for left, 1 for right
};