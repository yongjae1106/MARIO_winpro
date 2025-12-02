#pragma once
#include <vector>

enum PacketType : unsigned int
{
    // C2S (Client to Server)
    PKT_INPUT = 1,

    // S2C (Server to Client)
    PKT_PLAYER_STATE = 100,
    PKT_MONSTER_STATE = 101,
    PKT_HIT = 102,
    PKT_BLOCK_ATTACK = 103,
    PKT_PLAYER_JOIN = 104,
    PKT_PLAYER_LEAVE = 105,
};

struct PacketHeader
{
    unsigned int totalLength;
    unsigned int type;
};

// -------------------------------
// 클라이언트 입력 통합 패킷
// -------------------------------
enum class InputType : unsigned int
{
    NONE = 0,
    MOVE = 1,
    ATTACK = 2,
};

struct Packet_INPUT_C2S
{
    InputType inputType;

    // move
    unsigned int x, y;
    unsigned int vx, vy;
    unsigned int state;

    // attack
    unsigned int targetID;
    unsigned int damage;
};

// -------------------------------
// 서버 → 클라 공용 패킷 형식
// -------------------------------
struct Packet_MOVE_S2C
{
    unsigned int playerID;
    unsigned int x, y;
    unsigned int vx, vy;
    unsigned int state;
};

struct Packet_HIT_S2C
{
    unsigned int damage;
};

struct Packet_BLOCK_S2C
{
    unsigned int blockID;
    unsigned int block_x;
    unsigned int block_y;
};