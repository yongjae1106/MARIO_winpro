#pragma once

#include "../Monster.h"

class Bowser : public Monster {
public:
    Bowser(int x, int y);

    void monster_logic(GameWorld& world) override;
    void takeDamage(GameWorld& world, int damage) override;

    int getHp() const { return hp; }

private:
    int hp;
    bool ignore_tinofire;
    bool m_isJumping;
    bool m_isFiring;
    int fireTimer;
    int direction;
    float startX;
    float moveDistance;
    float maxDistance;
    int jumpTimer;
    int jumpInterval;
    int fireInterval;
    int fireDuration;
};
