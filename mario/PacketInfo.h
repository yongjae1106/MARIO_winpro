#pragma once
#include <vector>

enum PacketType : char
{
    PKT_MOVE = 1,
    PKT_ATTACK = 2,
    PKT_HIT = 3,
    PKT_BLOCK_ATTACK = 4
};

enum class GameEvent
{
    NO_EVENT = 0,
    PLAYER_JUMP,
    STOMP_ENEMY,
    GET_COIN,
    GET_GROW,
    GET_SHRINK,
    PLAYER_DIE
};

// 클라 → 서버
struct PacketInfo_ClientToServer
{
    int playerID;
    char type;

    // MOVE
    int x, y;
    int vx, vy;
    char state;

    // ATTACK
    int targetID;
    int damage;
};

// 서버 → 클라
struct PacketInfo_ServerToClient
{
    int playerID;
    char type;

    // MOVE (다른 플레이어 정보 등)
    int x, y;
    int vx, vy;
    char state;

    // HIT
    int damage;

    // BLOCK_ATTACK
    int blockID;
    int block_x;
    int block_y;

    std::vector<GameEvent> events;
};
