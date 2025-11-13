#pragma once

#include <windows.h>
#include <memory>
#include <tchar.h>

class GameWorld; // Forward declaration

class Particle {
public:
    enum class ParticleType {
        PlayerFireball,
        EnemyFireball,
        TinoFireball,
        TinoFireballEffect,
        Sparkle
    };

    Particle(ParticleType type, int x, int y);
    virtual ~Particle() {
        TCHAR debugMessage[256];
        _stprintf_s(debugMessage, _T("Particle: Destructor called for type %d.\n"), (int)type);
        OutputDebugString(debugMessage);
    }

    virtual void update(GameWorld& world) = 0;
    virtual void render(HDC hdc, int cameraX) = 0;

    ParticleType getType() const { return type; }
    int getX() const { return x; }
    int getY() const { return y; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    bool isActive() const { return active; }
    void setActive(bool isActive) { active = isActive; }

    virtual void setX(int newX) { x = newX; }
    virtual void setY(int newY) { y = newY; }
    virtual int getVx() const { return vx; }
    virtual void setVx(int newVx) { vx = newVx; }
    virtual int getVy() const { return vy; }
    virtual void setVy(int newVy) { vy = newVy; }

    virtual void setWidth(int newWidth) { width = newWidth; }
    virtual void setHeight(int newHeight) { height = newHeight; }

protected:
    ParticleType type;
    int x, y;
    int vx, vy;
    int width, height;
    bool active;
};
