#include "Particle.h"

Particle::Particle(ParticleType type, int x, int y)
    : type(type), x(x), y(y), vx(0), vy(0), width(0), height(0), active(true) {
}
