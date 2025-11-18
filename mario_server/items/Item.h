#pragma once

#include <windows.h>
#include <memory> // For std::unique_ptr

class GameWorld; // Forward declaration

class Item {
public:
    enum class ItemType { Coin, Mushroom, Star, Flower, Tino, UpMushroom, Fireball, PlayerFireball };

    Item(ItemType type, int x, int y);
    virtual ~Item() = default;

    virtual void update(GameWorld& world) = 0;
    virtual void render(HDC hdc, int cameraX) = 0;

    ItemType getType() const { return type; }
    int getX() const { return x; }
    int getY() const { return y; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    bool isActive() const { return active; }
    bool isMotion() const { return active; }
    void setActive(bool isActive) { active = isActive; }
    void setMotion(bool isMotion) { active = isMotion; }

    virtual void setX(int newX) { x = newX; }
    virtual void setY(int newY) { y = newY; }
    virtual int getVx() const { return vx; }
    virtual void setVx(int newVx) { vx = newVx; }
    virtual int getVy() const { return vy; }
    virtual void setVy(int newVy) { vy = newVy; }

    virtual void setWidth(int newWidth) { width = newWidth; }
    virtual void setHeight(int newHeight) { height = newHeight; }



protected:
    ItemType type;
    int x, y;
    int vx, vy;
    int width, height;
    bool active;
    bool isFalling;
    int spawn_motion;
    bool motion;
};