#pragma once

enum PacketType : char
{
    PKT_MOVE = 1,
    PKT_ATTACK = 2,
    PKT_HIT = 3,
    PKT_BLOCK_ATTACK = 4
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
};
