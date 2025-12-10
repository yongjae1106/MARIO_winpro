#pragma once
#include <Windows.h> // For DWORD
#include <vector>

// Forward declare enums to avoid including full headers
enum class PlayerState;
enum class GameState_Trans;

enum PacketType : unsigned int
{
    // C2S (Client to Server)
    PKT_KEY_DOWN = 1, // Key down event
    PKT_KEY_UP = 2,   // Key up event

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

struct Packet_KEY_EVENT_C2S
{
    unsigned int  keyCode; // virtual-key code (WPARAM)
};

struct Packet_PLAYER_STATE_S2C {
    int playerID; // The unique ID of the player
    int x, y;
    int vx, vy;
    int life;
    int coin;
    int width, height;
    int direction;
    int walk_motion;
    bool m_isJumping;
    bool m_isFlying;
    bool m_isWalking;
    bool m_dead;
    bool m_gameOver;
    bool fire_motion;
    bool tino_fire_motion;
    bool tino_attack_motion;
    PlayerState currentState;
    GameState_Trans state_trans;
    DWORD transformStartTime;
    bool _isStarGodModeActive;
    bool _isSuperGodModeActive;
    int tino_cooldown_space;
    int fire_motion_timer;
    int tino_attack_motion_timer;
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
