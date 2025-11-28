#include "Bowser.h"
#include "../Monster.h"
#include "../GameWorld.h"
#include <Gdiplus.h>
using namespace Gdiplus;

Bowser::Bowser(int x, int y)
    : Monster(MonsterType::Bowser, x, y, 120, 120), hp(100), m_isJumping(false), m_isFiring(false), fireTimer(0), jumpTimer(0), jumpInterval(0), fireInterval(0), fireDuration(0), startX(x), moveDistance(0), maxDistance(40 * 10) {
    // Initialize Bowser-specific properties
    setVy(0);
    direction = 1; // Assuming 1 for right, -1 for left
}

void Bowser::monster_logic(GameWorld& world) {
    if (!isAlive()) return;

    // === Movement ===
    int moveSpeed = 2;
    x = getX() + moveSpeed * direction;
    moveDistance += moveSpeed;

    if (moveDistance >= maxDistance)
    {
        // Change direction
        direction *= -1;
        moveDistance = 0;
    }

    // === Jumping ===
    if (!m_isJumping) {
        jumpTimer++;
        if (jumpTimer > jumpInterval) {
            setVy(-18);
            m_isJumping = true;
            jumpTimer = 0;
            jumpInterval = 60 + rand() % 121;
        }
    }

    // === Firing ===
    fireTimer++;
    if (fireTimer > fireInterval) 
    {
        int dir = (world.getPlayer().getX() < getX()) ? -1 : 1;
        //world.spawnFireball(getX() + getWidth() / 2, getY() + getHeight() / 2, 6 * dir);

        fireTimer = 0;
        fireInterval = 120 + rand() % 120;
    }

    // Firing duration
    if (m_isFiring)
    {
        fireDuration--;
        if (fireDuration <= 0)
            m_isFiring = false;
    }

    if (hp <= 0)
    {
        // PlaySoundBuffer(bowserfalls_Sound);
        setFalling(true);
    }
    // Off screen
    if (getY() > 640)
    {
        // PlaySoundBuffer(bowserdead_Sound);
        setAlive(false);
        // Remove koopa blocks
    }
}

void Bowser::takeDamage(GameWorld& world, int damage) {
    if (ignore_tinofire) return; // Bowser is temporarily immune to Tino's fire

    hp -= damage;
    if (hp <= 0) {
        setAlive(false);
    }
}
