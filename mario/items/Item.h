#pragma once

#define _WINSOCKAPI_ // Prevent windows.h from including winsock.h
#include <windows.h>
#include <memory> // For std::unique_ptr

class GameWorld; // Forward declaration

class Item {
public:
    enum class ItemType { Coin, Mushroom, Star, Flower, Tino, UpMushroom, Fireball, PlayerFireball };

    Item(ItemType type, int x, int y);
    virtual ~Item() = default;

    // --- Client-side update ---
    virtual void update();
    virtual void updateAnimation();
    // --------------------------

    virtual void render(HDC hdc, int cameraX) = 0;

    // --- Server-side logic (kept for reference) ---
    virtual void updateServerLogic(GameWorld& world) = 0;
    // ---------------------------------------------

    ItemType getType() const { return type; }
    int getX() const { return x; }
    int getY() const { return y; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    bool isActive() const { return active; }
    bool isMotion() const { return motion; } // Note: This might be the same as active
    void setActive(bool isActive) { active = isActive; }
    void setMotion(bool isMotion) { motion = isMotion; }

    virtual void setX(int newX) { x = newX; }
    virtual void setY(int newY) { y = newY; }
    virtual int getVx() const { return vx; }
    virtual void setVx(int newVx) { vx = newVx; }
    virtual int getVy() const { return vy; }
    virtual void setVy(int newVy) { vy = newVy; }

    virtual void setWidth(int newWidth) { width = newWidth; }
    virtual void setHeight(int newHeight) { height = newHeight; }

    int getAnimationframe() const { return animation_frame; }


protected:
    ItemType type;
    int x, y;
    int vx, vy;
    int width, height;
    bool active;
    bool isFalling;
    int spawn_motion;
    bool motion;
    int animation_frame; // For client-side animation
};
