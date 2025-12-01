#pragma once
#include <vector>

enum PacketType : unsigned int
{
    PKT_MOVE = 1,
    PKT_ATTACK = 2,

    PKT_PLAYER_STATE = 100,
    PKT_MONSTER_STATE,
    PKT_HIT,
    PKT_BLOCK_ATTACK,
    PKT_PLAYER_JOIN,
    PKT_PLAYER_LEAVE
};

struct PacketHeader
{
    unsigned int totalLength;
    unsigned int type;
};

// -------- 데이터 구조 --------

// C2S
struct Packet_MOVE_C2S { unsigned int playerID, x, y, vx, vy, state; };
struct Packet_ATTACK_C2S { unsigned int playerID, targetID, damage; };

// S2C
struct Packet_MOVE_S2C { unsigned int playerID, x, y, vx, vy, state; };
struct Packet_HIT_S2C { unsigned int damage; };
struct Packet_BLOCK_S2C { unsigned int blockID, block_x, block_y; };
