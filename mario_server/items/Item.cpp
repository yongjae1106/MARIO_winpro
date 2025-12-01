#include "Item.h"
#include <Gdiplus.h>

using namespace Gdiplus;

Item::Item(ItemType type, int x, int y)
    : type(type), x(x), y(y), vx(0), vy(0), width(40), height(40), active(true), isFalling(false), spawn_motion(0), motion(true) {
}