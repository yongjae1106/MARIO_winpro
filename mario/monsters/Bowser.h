#pragma once

#include "Monster.h"

class Bowser : public Monster {
public:
    Bowser(int x, int y);

    virtual void updateStateFromServer(const MonsterDataPacket& packet) override;

    // Getter for client-side state
    bool isFiring() const { return m_isFiring; }

protected:
    bool m_isFiring; // To be updated from MonsterDataPacket
};