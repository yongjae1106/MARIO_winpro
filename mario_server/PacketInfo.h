#pragma once
#include <vector>

enum PacketType : unsigned int
{
    PKT_MOVE = 1,
    PKT_ATTACK = 2,
    PKT_HIT = 3,
    PKT_BLOCK_ATTACK = 4
};

struct PacketHeader
{
    unsigned int totalLength;  // 전체 패킷 길이
    unsigned int type;         // PacketType
};

// -------------------------------
// MOVE 패킷 (클라 → 서버)
// -------------------------------
struct Packet_MOVE_C2S
{
    unsigned int x, y;
    unsigned int vx, vy;
    unsigned int state;
};

// -------------------------------
// ATTACK 패킷 (클라 → 서버)
// -------------------------------
struct Packet_ATTACK_C2S
{
    unsigned int targetID;
    unsigned int damage;
};

// -------------------------------
// MOVE 패킷 (서버 → 클라)
// -------------------------------
struct Packet_MOVE_S2C
{
    unsigned int playerID;
    unsigned int x, y;
    unsigned int vx, vy;
    unsigned int state;
};

// -------------------------------
// HIT 패킷 (서버 → 클라)
// -------------------------------
struct Packet_HIT_S2C
{
    unsigned int damage;
};

// -------------------------------
// BLOCK_ATTACK 패킷 (서버 → 클라)
// -------------------------------
struct Packet_BLOCK_S2C
{
    unsigned int blockID;
    unsigned int block_x;
    unsigned int block_y;
};
