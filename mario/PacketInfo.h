#pragma once
#include <vector>

enum PacketType : unsigned int
{
    // C2S (Client to Server)
    PKT_MOVE = 1,
    PKT_ATTACK = 2,

    // S2C (Server to Client)
    PKT_PLAYER_STATE = 100, // 개별 플레이어의 전체 상태 정보
    PKT_MONSTER_STATE = 101, // 몬스터의 상태 정보
    PKT_HIT = 102,
    PKT_BLOCK_ATTACK = 103,
    PKT_PLAYER_JOIN = 104, // 새로운 플레이어 접속
    PKT_PLAYER_LEAVE = 105, // 플레이어 접속 종료
};

struct PacketHeader
{
    unsigned int totalLength;
    unsigned int type;         // PacketType
};


struct Packet_MOVE_C2S
{
    unsigned int x, y;
    unsigned int vx, vy;
    unsigned int state;
};


struct Packet_ATTACK_C2S
{
    unsigned int targetID;
    unsigned int damage;
};


struct Packet_MOVE_S2C
{
    unsigned int playerID;
    unsigned int x, y;
    unsigned int vx, vy;
    unsigned int state;
};

// -------------------------------
// HIT ��Ŷ (���� �� Ŭ��)
// -------------------------------
struct Packet_HIT_S2C
{
    unsigned int damage;
};

// -------------------------------
// BLOCK_ATTACK ��Ŷ (���� �� Ŭ��)
// -------------------------------
struct Packet_BLOCK_S2C
{
    unsigned int blockID;
    unsigned int block_x;
    unsigned int block_y;
};
