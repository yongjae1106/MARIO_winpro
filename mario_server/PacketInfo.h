#pragma once
#include <vector>

// BGM 상태를 정의하는 열거형
enum class BGM_Type {
    NONE,
    GROUND_THEME,
    CASTLE_THEME,
    STAR_POWER,
    VICTORY,
    WORLD_CLEAR
};

// 사운드 효과 등 일회성 이벤트를 정의하는 열거형
enum class GameEvent {
    NO_EVENT,
    PLAYER_SMALL_JUMP,
    PLAYER_BIG_JUMP,
    PLAYER_FIRE,
    PLAYER_TINOFIRE,
    PLAYER_DIE,
    STOMP_ENEMY,
    GET_COIN,
    POWERUP_APPEARS,
    POWERUP,
    PIPE, // 플레이어가 작아질 때
    KICK,
    ONE_UP
};

// 서버가 클라이언트로 보내는 패킷 구조체
struct ServerPacket {
    // TODO: 여기에 게임 상태 데이터 추가 (플레이어 좌표, 몬스터 정보 등)
    BGM_Type currentBGM;
    std::vector<GameEvent> events;
};

enum PacketType : char
{
    PKT_MOVE = 1,
    PKT_ATTACK = 2,
    PKT_HIT = 3,
    PKT_BLOCK_ATTACK = 4
};

// Ŭ�� �� ����
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

// ���� �� Ŭ��
struct PacketInfo_ServerToClient
{
    int playerID;
    char type;

    // MOVE (�ٸ� �÷��̾� ���� ��)
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
