#include "Item.h"
#include "../GameWorld.h"
#include <Gdiplus.h>

using namespace Gdiplus;

Item::Item(ItemType type, int x, int y)
    : type(type), x(x), y(y), vx(0), vy(0), width(40), height(40), 
      active(true), isFalling(false), spawn_motion(0), motion(true), animation_frame(0) {
}

void Item::update()
{
    // This will be called for client-side presentation updates.
    updateAnimation();
}

void Item::updateAnimation()
{
    // Generic animation for items like flashing, etc.
    // Can be overridden by derived classes for specific behavior.
    if (GameWorld::getInstance().getGlobalAnimationFrameCounter() % 10 == 0) { // Adjust speed as needed
        animation_frame = (animation_frame + 1) % 4; // Assuming 4 frames for items like coins/stars
    }
}
