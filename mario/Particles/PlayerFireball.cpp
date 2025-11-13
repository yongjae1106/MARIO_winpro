#include "PlayerFireball.h"
#include "../GameWorld.h"

PlayerFireball::PlayerFireball(int x, int y, int vx)
    : Particle(ParticleType::PlayerFireball, x, y), motion(0), fade(false), fade_timer(0) {
    setVx(vx);
    setWidth(20);
    setHeight(20);
    setActive(true); // Make it active on creation
}

void PlayerFireball::update(GameWorld& world) {
    if (!isActive()) return;

    if (fade) {
        fade_timer++;
        // Update motion for fade-out animation (3 frames: 0, 1, 2)
        // Cycle through fade-out frames
        if (fade_timer % 3 == 1) { // Change frame every 3 updates, starting from 1
            motion = (motion + 1) % 3;
        }
        // Ensure motion doesn't go beyond the last fade-out frame if fade_timer is very large
        if (motion > 2) motion = 2;


        if (fade_timer >= 10) { // Play fade-out animation for 10 frames
            setActive(false);
        }
        return; // Stop all other updates if fading
    }

    // Animation motion for active fireball (4 frames: 0, 1, 2, 3)
    motion++;
    if (motion > 3) {
        motion = 0;
    }

    // Horizontal movement
    setX(getX() + getVx());

    // Get map and camera info from world
    const int(*currentMap)[MAP_WIDTH] = world.getCurrentMap();
    double cameraX = world.getCameraX();

    // Horizontal collision
    int leftTile = static_cast<int>((getX()) / TILE_SIZE);
    int rightTile = static_cast<int>((getX() + getWidth() - 1) / TILE_SIZE);
    int topTile = static_cast<int>(getY() / TILE_SIZE);
    int bottomTile = static_cast<int>((getY() + getHeight() - 1) / TILE_SIZE);

    if (getVx() < 0) { // Moving left
        if (leftTile < 0 || (topTile >=0 && topTile < MAP_HEIGHT && world.isSolidTile(currentMap[topTile][leftTile])) || (bottomTile >=0 && bottomTile < MAP_HEIGHT && world.isSolidTile(currentMap[bottomTile][leftTile]))) {
            fade = true;
        }
    } else if (getVx() > 0) { // Moving right
        if (rightTile >= MAP_WIDTH || (topTile >= 0 && topTile < MAP_HEIGHT && world.isSolidTile(currentMap[topTile][rightTile])) || (bottomTile >= 0 && bottomTile < MAP_HEIGHT && world.isSolidTile(currentMap[bottomTile][rightTile]))) {
            fade = true;
        }
    }

    // Apply gravity
    setVy(getVy() + 1);
    if (getVy() > 10) setVy(10); // Terminal velocity

    // Vertical movement
    setY(getY() + getVy());

    // Update tile coordinates after vertical movement
    leftTile = static_cast<int>((getX()) / TILE_SIZE);
    rightTile = static_cast<int>((getX() + getWidth() - 1) / TILE_SIZE);
    bottomTile = static_cast<int>((getY() + getHeight() - 1) / TILE_SIZE);

    // Vertical collision (ground)
    if (getVy() > 0 && bottomTile < MAP_HEIGHT && bottomTile >= 0 &&
        ( (leftTile >= 0 && world.isSolidTile(currentMap[bottomTile][leftTile])) || (rightTile < MAP_WIDTH && world.isSolidTile(currentMap[bottomTile][rightTile])) ) )
    {
        setY(bottomTile * TILE_SIZE - getHeight());
        setVy(-9); // Bounce
    }

    // Check if it fell off the screen
    if (getY() > SCREEN_HEIGHT) {
        setActive(false);
    }
}

void PlayerFireball::render(HDC hdc, int cameraX) {
    // Rendering is handled by GameRender
}
