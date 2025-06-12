#include "item.h"
#include "data.h"
#include "func.h"
#include "image.h"
#include "sound.h"
#include "monster.h"

Item up_mushroom[MAX_ITEMS];
Item mushroom[MAX_ITEMS];
Item star[MAX_ITEMS];
Item flower[MAX_ITEMS];
Item tino[MAX_ITEMS];
Shot fireball[MAX_SHOT];
Shot tinofire[MAX_SHOT];
Shot_Effect tinofire_effect[MAX_SHOT];

// 마리오와 버섯 충돌 처리 함수
void CheckCollision_mushroom()
{
    for (int i = 0; i < MAX_ITEMS; i++)
    {
        if (!mushroom[i].active) continue;

        int mushroomXWorld = mushroom[i].x - cameraX; // 화면 출력할 때 cameraX를 뺏으니, 실제 월드 좌표는 더해줘야 함

        if (IsColliding_item(mario.x, mario.y, mario.width, mario.height,
            mushroomXWorld, mushroom[i].y, mushroom[i].width, mushroom[i].height))
        {
            PlaySoundBuffer(powerup_Sound);
            mushroom[i].active = false;  // 버섯 먹기
            if (!mario.isBig && !mario.flower && !mario.tino && gameState == GAME_RUNNING) 
            {
                mario.y -= TILE_SIZE + 1;       // 위치 맞추기
                gameState = GAME_TRANSFORMING;
                transformStartTime = GetTickCount();
            }
            // TODO: 마리오의 상태 변화 (예: 성장, 점수 증가 등)
            // 예) score += 1000;
        }
    }
}
void CheckCollision_up_mushroom()
{
    for (int i = 0; i < MAX_ITEMS; i++)
    {
        if (!up_mushroom[i].active) continue;

        int mushroomXWorld = up_mushroom[i].x - cameraX; // 화면 출력할 때 cameraX를 뺏으니, 실제 월드 좌표는 더해줘야 함

        if (IsColliding_item(mario.x, mario.y, mario.width, mario.height,
            mushroomXWorld, up_mushroom[i].y, up_mushroom[i].width, up_mushroom[i].height))
        {
            PlaySoundBuffer(up_Sound);
            up_mushroom[i].active = false;  // 버섯 먹기
            mario.life++;
        }
    }
}
void CheckCollision_flower()
{
    for (int i = 0; i < MAX_ITEMS; i++)
    {
        if (!flower[i].active) continue;

        int flowerXWorld = flower[i].x - cameraX; // 화면 출력할 때 cameraX를 뺏으니, 실제 월드 좌표는 더해줘야 함

        if (IsColliding_item(mario.x, mario.y, mario.width, mario.height,
            flowerXWorld, flower[i].y, flower[i].width, flower[i].height))
        {
            PlaySoundBuffer(powerup_Sound);
            flower[i].active = false;  // 먹기
            if (!mario.flower && gameState == GAME_RUNNING)
            {
                if(!mario.tino && !mario.isBig)
                {
                    mario.y -= TILE_SIZE + 1;       // 위치 맞추기
                }
                gameState = GAME_FLOWER_TRANS;
                transformStartTime = GetTickCount();
            }
            // TODO: 마리오의 상태 변화 (예: 성장, 점수 증가 등)
            // 예) score += 1000;
        }
    }
}
void CheckCollision_tino()
{
    for (int i = 0; i < MAX_ITEMS; i++)
    {
        if (!tino[i].active) continue;

        int tinoXWorld = tino[i].x - cameraX; // 화면 출력할 때 cameraX를 뺏으니, 실제 월드 좌표는 더해줘야 함

        if (IsColliding_item(mario.x, mario.y, mario.width, mario.height,
            tinoXWorld, tino[i].y, tino[i].width, tino[i].height))
        {
            PlaySoundBuffer(powerup_Sound);
            tino[i].active = false;  // 먹기
            if (!mario.tino && gameState == GAME_RUNNING)
            {
                if (!mario.flower && !mario.isBig)
                {
                    mario.y -= TILE_SIZE + 1;       // 위치 맞추기
                }
                gameState = GAME_TINO_TRANS;
                transformStartTime = GetTickCount();
            }
            // TODO: 마리오의 상태 변화 (예: 성장, 점수 증가 등)
            // 예) score += 1000;
        }
    }
}
void CheckCollision_star()
{
    for (int i = 0; i < MAX_ITEMS; i++)
    {
        if (!star[i].active) continue;

        int starXWorld = star[i].x - cameraX; // 화면 출력할 때 cameraX를 뺏으니, 실제 월드 좌표는 더해줘야 함

        if (IsColliding_item(mario.x, mario.y, mario.width, mario.height,
            starXWorld, star[i].y, star[i].width, star[i].height))
        {
            star[i].active = false;  // 먹기
            if (!mario.star)
            {
                PlayBGM("resource\\sound\\bgm\\InvincibilityTheam.wav");
                mario.star = true;
                starStartTime = GetTickCount();
            }
            // TODO: 마리오의 상태 변화 (예: 성장, 점수 증가 등)
            // 예) score += 1000;
        }
    }
}
void CheckCollision_fireball()
{
    for (int i = 0; i < MAX_SHOT; i++)
    {
        if (!fireball[i].active) continue;

        int fireballXWorld = fireball[i].x - cameraX;

        for (int j = 0; j < MAX_MONSTERS; j++)
        {
            int monsterXWorld = monsters[j].x - cameraX;
            if (!monsters[j].isDead && monsters[j].isAlive && fireball[i].active) // 여러 조건 같이 확인
            {
                if (IsColliding_item(monsterXWorld, monsters[j].y, monsters[j].width, monsters[j].height,
                    fireballXWorld, fireball[i].y, fireball[i].width, fireball[i].height))
                {
                    PlaySoundBuffer(kick_Sound);
                    monsters[j].vy = -15;
                    monsters[j].isDead = true;
                    monsters[j].isFalling = true;
                    fireball[i].active = false;
                    break; // 이미 충돌했으면 더 검사 X
                }
            }
        }

        for (int j = 0; j < MAX_TURTLES; j++)
        {
            int turtleXWorld = turtles[j].x - cameraX;
            if (!turtles[j].isDead && turtles[j].isAlive && fireball[i].active)
            {
                if (IsColliding_item(turtleXWorld, turtles[j].y, turtles[j].width, turtles[j].height,
                    fireballXWorld, fireball[i].y, fireball[i].width, fireball[i].height))
                {
                    PlaySoundBuffer(kick_Sound);
                    turtles[j].vy = -15;
                    turtles[j].isDead = true;
                    turtles[j].isFalling = true;
                    fireball[i].active = false;
                    break;
                }
            }
        }
        for (int j = 0; j < MAX_TURTLES; j++)
        {
            int turtleXWorld = angelTurtles[j].x - cameraX;
            if (!angelTurtles[j].isDead && angelTurtles[j].isAlive && fireball[i].active)
            {
                if (IsColliding_item(turtleXWorld, angelTurtles[j].y, angelTurtles[j].width, angelTurtles[j].height,
                    fireballXWorld, fireball[i].y, fireball[i].width, fireball[i].height))
                {
                    PlaySoundBuffer(kick_Sound);
                    angelTurtles[j].vy = -15;
                    angelTurtles[j].isDead = true;
                    angelTurtles[j].isFalling = true;
                    fireball[i].active = false;
                    break;
                }
            }
        }

        int bowserXWorld = bowser.x - cameraX;
        if (!bowser.isDead && bowser.isAlive && fireball[i].active)
        {
            if (IsColliding_item(bowserXWorld, bowser.y, bowser.width, bowser.height,
                fireballXWorld, fireball[i].y, fireball[i].width, fireball[i].height))
            {
                PlaySoundBuffer(bump_Sound);
                bowser.hp--;
                fireball[i].active = false;
            }
        }
    }
}
void CheckCollision_tinofire()
{
    Graphics graphics(g_memDC);
    graphics.SetInterpolationMode(InterpolationModeNearestNeighbor);
    for (int i = 0; i < MAX_SHOT; i++)
    {
        if (!tinofire[i].active) continue;

        int tinofireXWorld = tinofire[i].x - cameraX;

        for (int j = 0; j < MAX_MONSTERS; j++)
        {
            int monsterXWorld = monsters[j].x - cameraX;
            if (!monsters[j].isDead && monsters[j].isAlive && tinofire[i].active) // 여러 조건 같이 확인
            {
                if (IsColliding_item(monsterXWorld, monsters[j].y, monsters[j].width, monsters[j].height,
                    tinofireXWorld, tinofire[i].y, tinofire[i].width, tinofire[i].height))
                {
                    PlaySoundBuffer(kick_Sound);
                    monsters[j].vy = -15;
                    monsters[j].isDead = true;
                    monsters[j].isFalling = true;
                    OnMonsterHit(monsters[j].x, monsters[j].y);
                    break; // 이미 충돌했으면 더 검사 X
                }
            }
        }

        for (int j = 0; j < MAX_TURTLES; j++)
        {
            int turtleXWorld = turtles[j].x - cameraX;
            if (!turtles[j].isDead && turtles[j].isAlive && tinofire[i].active)
            {
                if (IsColliding_item(turtleXWorld, turtles[j].y, turtles[j].width, turtles[j].height,
                    tinofireXWorld, tinofire[i].y, tinofire[i].width, tinofire[i].height))
                {
                    PlaySoundBuffer(kick_Sound);
                    turtles[j].vy = -15;
                    turtles[j].isDead = true;
                    turtles[j].isFalling = true;
                    OnMonsterHit(turtles[j].x, turtles[j].y);
                    break;
                }
            }
        }
        for (int j = 0; j < MAX_TURTLES; j++)
        {
            int angelturtleXWorld = angelTurtles[j].x - cameraX;
            if (!angelTurtles[j].isDead && angelTurtles[j].isAlive && tinofire[i].active)
            {
                if (IsColliding_item(angelturtleXWorld, angelTurtles[j].y + 20, angelTurtles[j].width, angelTurtles[j].height,
                    tinofireXWorld, tinofire[i].y, tinofire[i].width, tinofire[i].height))
                {
                    PlaySoundBuffer(kick_Sound);
                    angelTurtles[j].vy = -15;
                    angelTurtles[j].isDead = true;
                    angelTurtles[j].isFalling = true;
                    OnMonsterHit(angelTurtles[j].x, angelTurtles[j].y);
                    break;
                }
            }
        }

        int bowserXWorld = bowser.x - cameraX;
        if (!bowser.isDead && bowser.isAlive && tinofire[i].active)
        {
            if (IsColliding_item(bowserXWorld, bowser.y, bowser.width, bowser.height,
                tinofireXWorld, tinofire[i].y, tinofire[i].width, tinofire[i].height) && !bowser.ignore_tinofire)
            {
                PlaySoundBuffer(kick_Sound);
                OnMonsterHit(bowser.x, bowser.y);
                bowser.hp -= 5;
                bowser.ignore_tinofire = true;
            }
        }
    }
}
void OnMonsterHit(int x, int y) 
{
    static int tinofire_effect_count = 0;

    if (tinofire_effect_count >= MAX_SHOT)
    {
        tinofire_effect_count = 0;
    }
    tinofire_effect[tinofire_effect_count].x = x;
    tinofire_effect[tinofire_effect_count].y = y;
    tinofire_effect[tinofire_effect_count].timer = 0;
    tinofire_effect[tinofire_effect_count].active = true;
    tinofire_effect_count++;
}
void tino_attack()
{
    for (int j = 0; j < MAX_MONSTERS; j++)
    {
        int monsterXWorld = monsters[j].x - cameraX;
        if (!monsters[j].isDead && monsters[j].isAlive) // 여러 조건 같이 확인
        {
            if (IsColliding_item(monsterXWorld, monsters[j].y, monsters[j].width, monsters[j].height,
                (mario.direction == 0) ? mario.x - 50 :mario.x, mario.y - 15, 50, 100))
            {
                PlaySoundBuffer(kick_Sound);
                monsters[j].vy = -15;
                monsters[j].isDead = true;
                monsters[j].isFalling = true;
            }
        }
    }

    for (int j = 0; j < MAX_TURTLES; j++)
    {
        int turtleXWorld = turtles[j].x - cameraX;
        if (!turtles[j].isDead && turtles[j].isAlive)
        {
            if (IsColliding_item(turtleXWorld, turtles[j].y, turtles[j].width, turtles[j].height,
                (mario.direction == 0) ? mario.x - 50 : mario.x, mario.y - 15, 50, 100))
            {
                PlaySoundBuffer(kick_Sound);
                turtles[j].vy = -15;
                turtles[j].isDead = true;
                turtles[j].isFalling = true;
            }
        }
    }
    for (int j = 0; j < MAX_TURTLES; j++)
    {
        int turtleXWorld = angelTurtles[j].x - cameraX;
        if (!angelTurtles[j].isDead && angelTurtles[j].isAlive)
        {
            if (IsColliding_item(turtleXWorld, angelTurtles[j].y, angelTurtles[j].width, angelTurtles[j].height,
                (mario.direction == 0) ? mario.x - 50 : mario.x, mario.y - 15, 50, 100))
            {
                PlaySoundBuffer(kick_Sound);
                angelTurtles[j].vy = -15;
                angelTurtles[j].isDead = true;
                angelTurtles[j].isFalling = true;
                break;
            }
        }
    }

    int bowserXWorld = bowser.x - cameraX;
    if (!bowser.isDead && bowser.isAlive)
    {
        if (IsColliding_item(bowserXWorld, bowser.y, bowser.width, bowser.height,
            (mario.direction == 0) ? mario.x - 50 : mario.x, mario.y - 15, 50, 100) && !bowser.ignore_tinobite)
        {
            PlaySoundBuffer(kick_Sound);
            bowser.hp -= 10;
            bowser.ignore_tinobite = true;
        }
    }

}

void transform_to_flower()
{
    mario.isBig = false;
    mario.tino = false;
    mario.flower = true;
    mario.height = 80;          // 높이 맞추기
}
void transform_to_tino()
{
    mario.isBig = false;
    mario.flower = false;
    mario.tino = true;
    mario.height = 80;          // 높이 맞추기
}
void transform_bigmario()
{
    mario.tino = false;
    mario.flower = false;
    mario.isBig = true;
    mario.height = 80;          // 높이 맞추기
}
void transform_smallmario()
{
    mario.isBig = false;
    mario.height = 40;          // 높이 맞추기
}

// 버섯 움직임
void UpdateItems()
{
    for (int i = 0; i < MAX_ITEMS; i++)
    {
        // 모션
        if (mushroom[i].motion)
        {
            mushroom[i].y -= 3;
            mushroom[i].spawn_motion++;
            if (mushroom[i].spawn_motion == 18)
            {
                mushroom[i].motion = false;
                mushroom[i].active = true;
                mushroom[i].spawn_motion = 0;
            }
        }
        if (!mushroom[i].active) continue;

        // 1️⃣ 수평 이동
        mushroom[i].x += mushroom[i].vx;

        // 충돌 계산
        int left = mushroom[i].x / TILE_SIZE;
        int right = (mushroom[i].x + TILE_SIZE - 1) / TILE_SIZE;
        int top = mushroom[i].y / TILE_SIZE;
        int bottom = (mushroom[i].y + TILE_SIZE - 1) / TILE_SIZE;

        // 2️⃣ 수평 충돌 처리
        if (mushroom[i].vx < 0 && (currentMap[top][left] || currentMap[bottom][left]) && (currentMap[bottom][left] != 2 && currentMap[bottom][right] != 2)) 
        {
            mushroom[i].x = (left + 1) * TILE_SIZE; // 경계 맞춤
            mushroom[i].vx = -mushroom[i].vx;      // 방향 반전
        }
        else if (mushroom[i].vx > 0 && (currentMap[top][right] || currentMap[bottom][right]) && (currentMap[bottom][left] != 2 && currentMap[bottom][right] != 2)) 
        {
            mushroom[i].x = right * TILE_SIZE - mushroom[i].width; // 경계 맞춤
            mushroom[i].vx = -mushroom[i].vx;                       // 방향 반전
        }

        // 3️⃣ 수직 낙하 (중력)
        mushroom[i].vy += 1;
        if (mushroom[i].vy > 10) mushroom[i].vy = 10;
        mushroom[i].y += mushroom[i].vy;

        left = mushroom[i].x / TILE_SIZE;
        right = (mushroom[i].x + TILE_SIZE - 1) / TILE_SIZE;
        top = mushroom[i].y / TILE_SIZE;
        bottom = (mushroom[i].y + TILE_SIZE - 1) / TILE_SIZE;

        // 아래 충돌 처리
        if (mushroom[i].vy > 0 && (currentMap[bottom][left] || currentMap[bottom][right]) && (currentMap[bottom][left] != 2 && currentMap[bottom][right] != 2))
        {
            mushroom[i].y = bottom * TILE_SIZE - TILE_SIZE; // 위치 보정 (딱 맞게)
            mushroom[i].vy = 0;

            // 땅에 닿았으면 떨림 방지용으로 소폭 위치 보정 (필요시)
            // mushroom[i].x = round(mushroom[i].x / (float)TILE_SIZE) * TILE_SIZE;
        }
    }
}
void UpdateItems_up_mushroom()
{
    for (int i = 0; i < MAX_ITEMS; i++)
    {
        // 모션
        if (up_mushroom[i].motion)
        {
            up_mushroom[i].y -= 3;
            up_mushroom[i].spawn_motion++;
            if (up_mushroom[i].spawn_motion == 18)
            {
                up_mushroom[i].motion = false;
                up_mushroom[i].active = true;
                up_mushroom[i].spawn_motion = 0;
            }
        }
        if (!up_mushroom[i].active) continue;

        // 1️⃣ 수평 이동
        up_mushroom[i].x += up_mushroom[i].vx;

        // 충돌 계산
        int left = up_mushroom[i].x / TILE_SIZE;
        int right = (up_mushroom[i].x + TILE_SIZE - 1) / TILE_SIZE;
        int top = up_mushroom[i].y / TILE_SIZE;
        int bottom = (up_mushroom[i].y + TILE_SIZE - 1) / TILE_SIZE;

        // 2️⃣ 수평 충돌 처리
        if (up_mushroom[i].vx < 0 && (currentMap[top][left] || currentMap[bottom][left]) && (currentMap[bottom][left] != 2 && currentMap[bottom][right] != 2))
        {
            up_mushroom[i].x = (left + 1) * TILE_SIZE; // 경계 맞춤
            up_mushroom[i].vx = -up_mushroom[i].vx;      // 방향 반전
        }
        else if (up_mushroom[i].vx > 0 && (currentMap[top][right] || currentMap[bottom][right]) && (currentMap[bottom][left] != 2 && currentMap[bottom][right] != 2))
        {
            up_mushroom[i].x = right * TILE_SIZE - up_mushroom[i].width; // 경계 맞춤
            up_mushroom[i].vx = -up_mushroom[i].vx;                       // 방향 반전
        }

        // 3️⃣ 수직 낙하 (중력)
        up_mushroom[i].vy += 1;
        if (up_mushroom[i].vy > 10) up_mushroom[i].vy = 10;
        up_mushroom[i].y += up_mushroom[i].vy;

        left = up_mushroom[i].x / TILE_SIZE;
        right = (up_mushroom[i].x + TILE_SIZE - 1) / TILE_SIZE;
        top = up_mushroom[i].y / TILE_SIZE;
        bottom = (up_mushroom[i].y + TILE_SIZE - 1) / TILE_SIZE;

        // 아래 충돌 처리
        if (up_mushroom[i].vy > 0 && (currentMap[bottom][left] || currentMap[bottom][right]) && (currentMap[bottom][left] != 2 && currentMap[bottom][right] != 2))
        {
            up_mushroom[i].y = bottom * TILE_SIZE - TILE_SIZE; // 위치 보정 (딱 맞게)
            up_mushroom[i].vy = 0;

        }
    }
}
// 스타 움직임
void UpdateItems_star()
{
    for (int i = 0; i < MAX_ITEMS; i++)
    {
        // 모션
        if (star[i].motion)
        {
            star[i].y -= 3;
            star[i].spawn_motion++;
            if (star[i].spawn_motion == 18)
            {
                star[i].motion = false;
                star[i].active = true;
                star[i].spawn_motion = 0;
            }
        }
        if (!star[i].active) continue;

        // 1️⃣ 수평 이동
        star[i].x += star[i].vx;

        // 충돌 계산
        int left = star[i].x / TILE_SIZE;
        int right = (star[i].x + TILE_SIZE - 1) / TILE_SIZE;
        int top = star[i].y / TILE_SIZE;
        int bottom = (star[i].y + TILE_SIZE - 1) / TILE_SIZE;
        int middle = (mario.y + mario.height / 2 - 1) / TILE_SIZE;
        
        // 2️⃣ 수평 충돌 처리
        if (star[i].vx < 0 && 
            (currentMap[top][left] || currentMap[bottom][left] || currentMap[middle][left]) &&
            !(currentMap[top][left] == 2 || currentMap[bottom][left] == 2 || currentMap[middle][left] == 2) &&
            !(currentMap[top][left] == 7 || currentMap[bottom][left] == 7 || currentMap[middle][left] == 7) &&
            !(currentMap[top][left] == 8 || currentMap[bottom][left] == 8 || currentMap[middle][left] == 8))
        {
            star[i].x = (left + 1) * TILE_SIZE; // 경계 맞춤
            star[i].vx = -star[i].vx;      // 방향 반전
        }
        else if (star[i].vx > 0 && 
            (currentMap[top][right] || currentMap[bottom][right] || currentMap[middle][right]) &&
            !(currentMap[top][right] == 2 || currentMap[bottom][right] == 2 || currentMap[middle][right] == 2) &&
            !(currentMap[top][right] == 7 || currentMap[bottom][right] == 7 || currentMap[middle][right] == 7) &&
            !(currentMap[top][right] == 8 || currentMap[bottom][right] == 8 || currentMap[middle][right] == 8))
        {
            star[i].x = right * TILE_SIZE - star[i].width; // 경계 맞춤
            star[i].vx = -star[i].vx;                       // 방향 반전
        }

        // 3️⃣ 수직 낙하 (중력)
        star[i].vy += 1;
        if (star[i].vy > 10) star[i].vy = 10;
        star[i].y += star[i].vy;

        left = star[i].x / TILE_SIZE;
        right = (star[i].x + TILE_SIZE - 1) / TILE_SIZE;
        top = star[i].y / TILE_SIZE;
        bottom = (star[i].y + TILE_SIZE - 1) / TILE_SIZE;

        // 위 충돌
        if (star[i].vy < 0 && (currentMap[top][left] || currentMap[top][right])
            && !(currentMap[top][left] == 2 || currentMap[top][right] == 2)
            && !(currentMap[top][left] == 7 || currentMap[top][right] == 7)
            && !(currentMap[top][left] == 8 || currentMap[top][right] == 8))
        {
            // 마리오가 정통으로 친 블럭 찾기
            int centerX = star[i].x + TILE_SIZE / 2 + cameraX;
            int blockX = centerX / TILE_SIZE;

            star[i].y = (top + 1) * TILE_SIZE;
            star[i].vy = 0;
        }

        // 아래 충돌 처리
        if (star[i].vy > 0 && (currentMap[bottom][left] || currentMap[bottom][right])
            && !(currentMap[bottom][left] == 2 || currentMap[bottom][right] == 2)
            && !(currentMap[bottom][left] == 7 || currentMap[bottom][right] == 7)
            && !(currentMap[bottom][left] == 8 || currentMap[bottom][right] == 8))
        {
            star[i].y = bottom * TILE_SIZE - TILE_SIZE; // 위치 보정 (딱 맞게)
            star[i].vy = -15;

            // 땅에 닿았으면 떨림 방지용으로 소폭 위치 보정 (필요시)
            // star[i].x = round(star[i].x / (float)TILE_SIZE) * TILE_SIZE;
        }
    }
}
void UpdateItems_flower()
{

    for (int i = 0; i < MAX_ITEMS; i++)
    {
        // 모션
        if (flower[i].motion)
        {
            flower[i].y -= 2;
            flower[i].spawn_motion++;
            if (flower[i].spawn_motion == 18)
            {
                flower[i].motion = false;
                flower[i].active = true;
                flower[i].spawn_motion = 0;
            }
        }
        if (!flower[i].active) continue;
    }
}
void UpdateItems_tino()
{
    for (int i = 0; i < MAX_ITEMS; i++)
    {
        // 모션
        if (tino[i].motion)
        {
            tino[i].y -= 2;
            tino[i].spawn_motion++;
            if (tino[i].spawn_motion == 18)
            {
                tino[i].motion = false;
                tino[i].active = true;
                tino[i].spawn_motion = 0;
            }
        }
        if (!tino[i].active) continue;
    }
}
void UpdateShot_fireball()
{
    for (int i = 0; i < MAX_SHOT; i++)
    {
        if (!fireball[i].active) continue;
        fireball[i].motion++;
        if (fireball[i].motion > 3)
        {
            fireball[i].motion = 0;
        }

        fireball[i].x += fireball[i].vx;    // 수평이동

        // 충돌 계산
        int left = fireball[i].x / TILE_SIZE;
        int right = (fireball[i].x + fireball[i].width - 1) / TILE_SIZE;
        int top = fireball[i].y / TILE_SIZE;
        int bottom = (fireball[i].y + fireball[i].height - 1) / TILE_SIZE;
        int middle = (fireball[i].y + 30 / 2 - 1) / TILE_SIZE;

        // 2️⃣ 수평 충돌 처리
        if (fireball[i].vx < 0 &&
            (currentMap[top][left] || currentMap[bottom][left] || currentMap[middle][left]) &&
            !(currentMap[top][left] == 2 || currentMap[bottom][left] == 2 || currentMap[middle][left] == 2) &&
            !(currentMap[top][left] == 7 || currentMap[bottom][left] == 7 || currentMap[middle][left] == 7) &&
            !(currentMap[top][left] == 8 || currentMap[bottom][left] == 8 || currentMap[middle][left] == 8))
        {
            fireball[i].active = false;
            fireball[i].fade = true;
        }
        else if (fireball[i].vx > 0 &&
            (currentMap[top][right] || currentMap[bottom][right] || currentMap[middle][right]) &&
            !(currentMap[top][right] == 2 || currentMap[bottom][right] == 2 || currentMap[middle][right] == 2) &&
            !(currentMap[top][right] == 7 || currentMap[bottom][right] == 7 || currentMap[middle][right] == 7) &&
            !(currentMap[top][right] == 8 || currentMap[bottom][right] == 8 || currentMap[middle][right] == 8))
        {
            fireball[i].active = false;
            fireball[i].fade = true;
        }

        // 3️⃣ 수직 낙하 (중력)
        fireball[i].vy += 1;
        if (fireball[i].vy > 10) fireball[i].vy = 5;
        fireball[i].y += fireball[i].vy;

        left = fireball[i].x / TILE_SIZE;
        right = (fireball[i].x + TILE_SIZE - 1) / TILE_SIZE;
        top = fireball[i].y / TILE_SIZE;
        bottom = (fireball[i].y + TILE_SIZE - 1) / TILE_SIZE;

        // 아래 충돌 처리
        if (fireball[i].vy > 0 && (currentMap[bottom][left] || currentMap[bottom][right])
            && !(currentMap[bottom][left] == 2 || currentMap[bottom][right] == 2)
            && !(currentMap[bottom][left] == 7 || currentMap[bottom][right] == 7)
            && !(currentMap[bottom][left] == 8 || currentMap[bottom][right] == 8))
        {
            fireball[i].y = bottom * TILE_SIZE - fireball[i].height - 10;
            fireball[i].vy = -9;

            // 땅에 닿았으면 떨림 방지용으로 소폭 위치 보정 (필요시)
            // fireball[i].x = round(fireball[i].x / (float)TILE_SIZE) * TILE_SIZE;
        }
    }
}
void UpdateShot_tinofire()
{
    for (int i = 0; i < MAX_SHOT; i++)
    {
        if (!tinofire[i].active) continue;
        tinofire[i].motion++;
        tinofire[i].duration++;
        if (tinofire[i].motion > 6)
        {
            tinofire[i].motion = 0;
        }
        if (tinofire[i].duration == 100)
        {
            tinofire[i].active = false;
            continue;
        }

        tinofire[i].x += tinofire[i].vx;    // 수평이동
    }
}
void UpdateShot_tinofire_effect()
{
    for (int i = 0; i < MAX_SHOT; i++)
    {
        if (tinofire_effect[i].active)
        {
            tinofire_effect[i].timer++;
            if (tinofire_effect[i].timer >= 15)
            {
                tinofire_effect[i].timer = 0;
                tinofire_effect[i].active = false;
            }
        }
    }
}

void SpawnItem(int x, int y)
{
    for (int i = 0; i < MAX_ITEMS; i++)
    {
        if (!mushroom[i].active)
        {
            mushroom[i].x = x;
            mushroom[i].y = y;
            mushroom[i].vx = 1; // 기본 속도
            mushroom[i].motion = true;
            break;
        }
    }
}
void SpawnItem_up_mushroom(int x, int y)
{
    for (int i = 0; i < MAX_ITEMS; i++)
    {
        if (!up_mushroom[i].active)
        {
            up_mushroom[i].x = x;
            up_mushroom[i].y = y;
            up_mushroom[i].vx = 1; // 기본 속도
            up_mushroom[i].motion = true;
            break;
        }
    }
}
void SpawnItem_star(int x, int y)
{
    for (int i = 0; i < MAX_ITEMS; i++)
    {
        if (!star[i].active)
        {
            star[i].x = x;
            star[i].y = y;
            star[i].vx = 2; // 기본 속도
            star[i].vy = -15;
            star[i].motion = true;
            break;
        }
    }
}
void SpawnItem_flower(int x, int y)
{
    for (int i = 0; i < MAX_ITEMS; i++)
    {
        if (!flower[i].active)
        {
            flower[i].x = x;
            flower[i].y = y;
            flower[i].motion = true;
            break;
        }
    }
}
void SpawnItem_tino(int x, int y)
{
    for (int i = 0; i < MAX_ITEMS; i++)
    {
        if (!tino[i].active)
        {
            tino[i].x = x;
            tino[i].y = y;
            tino[i].motion = true;
            break;
        }
    }
}

void fireball_spawn(int x, int y)
{
    static int fireball_count = 0;

    if (fireball_count >= MAX_SHOT)
    {
        fireball_count = 0;
    }
    fireball[fireball_count].x = x;
    fireball[fireball_count].y = y;
    fireball[fireball_count].vx = (mario.direction == 0) ? -7 : 7;
    fireball[fireball_count].vy = 0;
    fireball[fireball_count].width = 10;
    fireball[fireball_count].height = 10;
    fireball[fireball_count].active = true;
    fireball_count++;
}
void tinofire_spawn(int x, int y)
{
    static int tinofire_count = 0;

    if (tinofire_count >= MAX_SHOT)
    {
        tinofire_count = 0;
    }
    tinofire[tinofire_count].x = x;
    tinofire[tinofire_count].y = y;
    tinofire[tinofire_count].vx = (mario.direction == 0) ? -7 : 7;
    tinofire[tinofire_count].vy = 0;
    tinofire[tinofire_count].motion = 0;
    tinofire[tinofire_count].motion_fade = 0;
    tinofire[tinofire_count].duration = 0;
    tinofire[tinofire_count].direction = mario.direction;
    tinofire[tinofire_count].width = TILE_SIZE * 2;
    tinofire[tinofire_count].height = TILE_SIZE * 2;
    tinofire[tinofire_count].active = true;
    tinofire[tinofire_count].fade = false;
    tinofire_count++;
}

void god_mario(DWORD _godstart)
{
    DWORD now = GetTickCount();
    if (now - _godstart >= 1000)
    {
        mario.god = false;
    }
}
void star_mario(DWORD _starstart)
{
    DWORD now = GetTickCount();
    if (now - _starstart >= 10000)
    {
        SetStage_BGM();
        mario.star = false;
    }
}
