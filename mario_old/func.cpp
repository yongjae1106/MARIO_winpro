#include <tchar.h>
#include "data.h"
#include "func.h"
#include "image.h"
#include "item.h"
#include "sound.h"
#include "monster.h"
#include "map.h"
#include <mmsystem.h>  // PlaySound 함수 포함
#pragma comment(lib, "winmm.lib")  // winmm.lib 링크

using namespace Gdiplus;
// 0: 구멍 1: 땅 2: 코인 3:굼바 4:파이프 5:계단 6:미스테리박스 7:깃발 8: 깃발꼭짓점 9:성 10: 벽돌 
// 90. 버섯머리1 91. 버섯머리2 92. 버섯머리3 11. 버섯줄기 12. 버섯줄기2 13.구름 14: 불기둥스위치 15: 회색벽돌
// 16: 사용된블럭 17:용암head 18:용암body 19: 쿠파블럭 20.스위치에서 나오는 불꽃 60:스타박스 61:꽃박스 62: 티노박스 63: 생명버섯박스 64: 코인박스

//void InitMap() //맵 1 구조
//{
//    // 0: 구멍 1: 땅 2: 코인 3:굼바 4:파이프 5:계단 6:미스테리박스 7:깃발 8: 깃발꼭짓점 9:성 10: 벽돌 
//    // 90. 버섯머리1 91. 버섯머리2 92. 버섯머리3 11. 버섯줄기 12. 버섯줄기2 13.구름 14: 불기둥스위치 15: 회색벽돌
//    // 16: 사용된블럭 60:스타박스 61:꽃박스 62: 티노박스 63: 생명버섯박스 64: 코인박스
//
//    // 1. 땅
//    for (int j = 0; j < MAP_WIDTH; j++)
//    {
//        map1[MAP_HEIGHT - 2][j] = 1;
//        map1[MAP_HEIGHT - 1][j] = 1;
//    }
//
//    // 1. 땅
//    for (int j = 0; j < MAP_WIDTH; j++)
//    {
//        map1[MAP_HEIGHT - 2][j] = 1;
//        map1[MAP_HEIGHT - 1][j] = 1;
//    }
//
//    //벽돌
//    map1[6][16] = 10;
//    map1[6][18] = 10;
//    map1[10][15] = 10;
//    map1[10][16] = 10;
//    map1[10][18] = 10;
//    map1[10][19] = 10;
//    map1[10][30] = 10;
//    map1[10][32] = 10;
//    map1[10][75] = 10;
//    map1[10][76] = 10;
//    map1[10][77] = 10;
//    map1[7][77] = 10;
//    map1[7][45] = 10;
//    map1[7][44] = 10;
//    map1[7][46] = 10;
//    map1[7][43] = 10;
//    map1[7][47] = 10;
//    map1[9][49] = 10;
//    map1[9][50] = 10;
//
//    // 0. 구멍
//    map1[14][21] = 0;
//    map1[13][21] = 0;
//    map1[14][22] = 0;
//    map1[13][22] = 0;
//    map1[14][23] = 0;
//    map1[13][23] = 0;
//    map1[14][45] = 0;
//    map1[13][45] = 0;
//    map1[14][68] = 0;
//    map1[13][68] = 0;
//    map1[14][69] = 0;
//    map1[13][69] = 0;
//    map1[14][70] = 0;
//    map1[13][70] = 0;
//    map1[14][85] = 0;
//    map1[13][85] = 0;
//    map1[14][86] = 0;
//    map1[13][86] = 0;
//    map1[14][87] = 0;
//    map1[13][87] = 0;
//
//    // 2. 코인
//    map1[12][14] = 2;
//    map1[5][17] = 2;
//    map1[9][15] = 2;
//    map1[9][18] = 2;
//    map1[9][76] = 2;
//    map1[9][31] = 2;
//    map1[10][60] = 2;
//    map1[6][43] = 2;
//    map1[6][44] = 2;
//    map1[6][45] = 2;
//    map1[6][78] = 2;
//
//    //4. 파이프
//    map1[10][55] = 43; // 좌상단
//    map1[10][56] = 42; // 우상단
//    map1[11][55] = 41; // 좌하단
//    map1[11][56] = 40; // 우하단
//    map1[12][55] = 41;
//    map1[12][56] = 40;
//
//
//    map1[11][38] = 43; // 좌하단
//    map1[11][39] = 42; // 우하단
//    map1[12][38] = 41;
//    map1[12][39] = 40;
//    map1[13][38] = 41;
//    map1[13][39] = 40;
//    map1[14][38] = 41;
//    map1[14][39] = 40;
//
//
//
//    // 5. 계단 
//    map1[14][80] = 5;
//    map1[13][81] = 5; map1[14][81] = 5;
//    map1[12][82] = 5; map1[13][82] = 5; map1[14][82] = 5;
//    map1[11][83] = 5; map1[12][83] = 5; map1[13][83] = 5; map1[14][83] = 5;
//    map1[10][84] = 5; map1[11][84] = 5; map1[12][84] = 5; map1[13][84] = 5; map1[14][84] = 5;
//
//    map1[14][88] = 5;
//    map1[13][88] = 5;
//    map1[12][88] = 5;
//    map1[11][88] = 5;
//    map1[10][88] = 5;
//    map1[14][89] = 5;
//    map1[13][89] = 5;
//    map1[12][89] = 5;
//    map1[11][89] = 5;
//    map1[14][90] = 5;
//    map1[13][90] = 5;
//    map1[12][90] = 5;
//    map1[14][91] = 5;
//    map1[13][91] = 5;
//    map1[14][92] = 5;
//
//    // 6. 미스테리 박스
//    map1[9][10] = 6;
//    map1[6][17] = 64;
//    map1[10][17] = 64;
//    map1[10][31] = 64;
//    map1[10][60] = 64;
//    map1[20][76] = 64;
//    map1[10][62] = 64;
//    map1[10][64] = 64;
//    map1[6][62] = 6;
//    map1[7][78] = 64;
//    map1[7][73] = 64;
//
//    // 60. star박스
//    map1[10][2] = 60;
//
//    // 61. flower박스
//    map1[10][4] = 61;
//
//    // 62. tino박스
//    map1[10][6] = 62;
//
//    // 63. 생명버섯 박스
//    map1[7][2] = 63;
//
//    // 64. 생명버섯 박스
//    map1[7][4] = 64;
//
//    // 깃발
//    map1[MAP_HEIGHT - 3][108] = 5;
//    // 7. 깃발
//    for (int y = 3; y <= 11; y++)
//    {
//        map1[y][108] = 7;
//    }
//    // 8. 깃발 꼭짓점
//    map1[2][108] = 8;
//
//    // 9. 성
//    map1[12][115] = 9;
//
//    //for (int y = 10; y <= 12; y++)
//    //{
//    //    for (int x = 112; x <= 116; x++)
//    //    {
//    //        map1[y][x] = 9;
//    //    }
//    //}
//}

void InitMap() //맵 1 구조
{
    // 0: 구멍 1: 땅 2: 코인 3:굼바 4:파이프 5:계단 6:미스테리박스 7:깃발 8: 깃발꼭짓점 9:성 10: 벽돌 
    // 90. 버섯머리1 91. 버섯머리2 92. 버섯머리3 11. 버섯줄기 12. 버섯줄기2 13.구름 14: 불기둥스위치 15: 회색벽돌
    // 16: 사용된블럭 17:용암head 18:용암body 19: 쿠파블럭 20.스위치에서 나오는 불꽃 60:스타박스 61:꽃박스 62: 티노박스 63: 생명버섯박스 64: 코인박스 65: 투명스타블럭

    // 1. 땅
    for (int j = 0; j < MAP_WIDTH; j++)
    {
        map1[MAP_HEIGHT - 2][j] = 1;
        map1[MAP_HEIGHT - 1][j] = 1;
    }
    map1[9][1] = 65;    // 보여주기식 스타박스
    //벽돌
    map1[6][16] = 10;
    map1[6][18] = 10;
    map1[10][15] = 10;
    map1[10][16] = 10;
    map1[10][18] = 10;
    map1[10][19] = 10;
    map1[10][30] = 10;
    map1[10][32] = 10;
    map1[10][75] = 10;
    map1[10][76] = 10;
    map1[10][77] = 10;
    map1[7][77] = 10;
    map1[7][45] = 10;
    map1[7][44] = 10;
    map1[7][46] = 10;
    map1[7][43] = 10;
    map1[7][47] = 10;
    map1[9][49] = 10;
    map1[9][50] = 10;

    // 0. 구멍
    map1[14][21] = 0;
    map1[13][21] = 0;
    map1[14][22] = 0;
    map1[13][22] = 0;
    map1[14][23] = 0;
    map1[13][23] = 0;
    map1[14][45] = 0;
    map1[13][45] = 0;
    map1[14][68] = 0;
    map1[13][68] = 0;
    map1[14][69] = 0;
    map1[13][69] = 0;
    map1[14][70] = 0;
    map1[13][70] = 0;
    map1[14][85] = 0;
    map1[13][85] = 0;
    map1[14][86] = 0;
    map1[13][86] = 0;
    map1[14][87] = 0;
    map1[13][87] = 0;

    // 2. 코인
    map1[12][14] = 2;
    map1[5][17] = 2;
    map1[9][15] = 2;
    map1[9][18] = 2;
    map1[9][76] = 2;
    map1[9][31] = 2;
    map1[10][60] = 2;
    map1[6][43] = 2;
    map1[6][44] = 2;
    map1[6][45] = 2;
    map1[6][78] = 2;

    //4. 파이프
    map1[10][55] = 43; // 좌상단
    map1[10][56] = 42; // 우상단
    map1[11][55] = 41; // 좌하단
    map1[11][56] = 40; // 우하단
    map1[12][55] = 41;
    map1[12][56] = 40;


    map1[11][38] = 43; // 좌하단
    map1[11][39] = 42; // 우하단
    map1[12][38] = 41;
    map1[12][39] = 40;
    map1[13][38] = 41;
    map1[13][39] = 40;
    map1[14][38] = 41;
    map1[14][39] = 40;



    // 5. 계단 
    map1[14][80] = 5;
    map1[13][81] = 5; map1[14][81] = 5;
    map1[12][82] = 5; map1[13][82] = 5; map1[14][82] = 5;
    map1[11][83] = 5; map1[12][83] = 5; map1[13][83] = 5; map1[14][83] = 5;
    map1[10][84] = 5; map1[11][84] = 5; map1[12][84] = 5; map1[13][84] = 5; map1[14][84] = 5;

    map1[14][88] = 5;
    map1[13][88] = 5;
    map1[12][88] = 5;
    map1[11][88] = 5;
    map1[10][88] = 5;
    map1[14][89] = 5;
    map1[13][89] = 5;
    map1[12][89] = 5;
    map1[11][89] = 5;
    map1[14][90] = 5;
    map1[13][90] = 5;
    map1[12][90] = 5;
    map1[14][91] = 5;
    map1[13][91] = 5;
    map1[14][92] = 5;

    // 6. 미스테리 박스
    map1[9][10] = 6;
    map1[6][17] = 64;
    map1[10][17] = 64;
    map1[10][31] = 64;
    map1[10][60] = 64;
    map1[20][76] = 64;
    map1[10][62] = 64;
    map1[10][64] = 64;
    map1[6][62] = 62;
    map1[7][78] = 64;
    map1[7][73] = 64;

    // 깃발
    map1[MAP_HEIGHT - 3][108] = 5;
    // 7. 깃발
    for (int y = 3; y <= 11; y++)
    {
        map1[y][108] = 7;
    }
    // 8. 깃발 꼭짓점
    map1[2][108] = 8;

    // 9. 성
    map1[12][115] = 9;


}

// 맵 2 구조
void InitMap2() 
{
    // 0: 구멍 1: 땅 2: 코인 3:굼바 4:파이프 5:계단 6:미스테리박스 7:깃발 8: 깃발꼭짓점 9:성 10: 벽돌 
    // 90. 버섯머리1 91. 버섯머리2 92. 버섯머리3 11. 버섯줄기 12. 버섯줄기2 13.구름 14: 불기둥스위치 15: 회색벽돌
    // 16: 사용된블럭 17:용암head 18:용암body 60:스타박스 61:꽃박스 62: 티노박스 63: 생명버섯박스 64: 코인박스 999: 피치공주

    currentMap = map2;

    memset(currentMap, 0, sizeof(map2));  //맵 초기화


    // 1. 시작 벽돌(=땅)


    for (int y = 13; y <= 14; y++)
    {
        for (int x = 0; x <= 3; x++)
        {
            map2[y][x] = 1;
        }
    }

    for (int y = 13; y <= 14; y++)
    {
        for (int x = 111; x < 140; x++)
        {
            map2[y][x] = 1;
        }
    }

    // 6. 미스테리 박스
    map2[2][69] = 64;
    map2[2][71] = 6;
    map2[6][75] = 64;
    map2[8][102] = 61;

    //13. 구름
    map2[8][30] = 13; map2[8][31] = 13; map2[8][32] = 13; map2[8][33] = 13;
    map2[6][34] = 13; map2[6][35] = 13; map2[6][36] = 13;
    map2[6][44] = 13; map2[6][45] = 13; map2[6][46] = 13;
    map2[7][50] = 13; map2[7][51] = 13; map2[7][52] = 13; map2[7][53] = 13;
    map2[6][55] = 13; map2[6][56] = 13;
    map2[13][48] = 13; map2[13][49] = 13; map2[13][50] = 13; map2[13][51] = 13; map2[13][52] = 13; map2[13][53] = 13; map2[13][54] = 13; map2[13][55] = 13; map2[13][56] = 13; map2[13][57] = 13;
    map2[8][95] = 13; map2[8][96] = 13; map2[8][97] = 13;
    map2[12][97] = 13; map2[12][98] = 13; map2[12][101] = 13; map2[12][101] = 13; map2[12][102] = 13; map2[12][103] = 13;
    map2[12][104] = 13;
    map2[7][63] = 13; map2[7][64] = 13; map2[7][65] = 13;
    map2[5][68] = 13; map2[5][69] = 13; map2[5][70] = 13; map2[5][71] = 13; map2[5][72] = 13;
    map2[3][77] = 13; map2[3][78] = 13; map2[3][79] = 13;
    map2[12][66] = 13; map2[12][67] = 13; map2[12][68] = 13; map2[12][69] = 13; map2[12][70] = 13; map2[12][71] = 13; map2[12][72] = 13; map2[12][73] = 13; map2[12][74] = 13; map2[12][75] = 13; map2[12][76] = 13;
    map2[5][101] = 13; map2[5][100] = 13; map2[5][99] = 13;
    map2[5][102] = 13;
    map2[5][105] = 13; map2[5][106] = 13; map2[5][107] = 13;
    map2[4][22] = 13; map2[4][23] = 13; map2[4][24] = 13;




    // 9. 버섯 머리
    map2[11][6] = 90; map2[11][7] = 91; map2[11][8] = 92;
    map2[10][8] = 90; map2[10][9] = 91; map2[10][10] = 92;
    map2[6][13] = 90; map2[6][14] = 91; map2[6][15] = 91; map2[6][16] = 91; map2[6][17] = 92;
    map2[11][17] = 90; map2[11][18] = 91; map2[11][19] = 91; map2[11][20] = 92;
    map2[8][24] = 90; map2[8][25] = 91; map2[8][26] = 92;
    map2[4][38] = 90; map2[4][39] = 91; map2[4][40] = 91; map2[4][41] = 91; map2[4][42] = 92;
    map2[10][42] = 90; map2[10][43] = 91; map2[10][44] = 92;
    map2[10][42] = 90; map2[10][43] = 91; map2[10][44] = 92;
    map2[3][58] = 90; map2[3][59] = 91; map2[3][60] = 91; map2[3][61] = 91;  map2[3][62] = 92;
    map2[6][84] = 90; map2[6][85] = 91; map2[6][86] = 92;
    map2[4][90] = 90; map2[4][91] = 91; map2[4][92] = 92;
    map2[9][79] = 90; map2[9][80] = 91; map2[9][81] = 91; map2[9][82] = 91; map2[9][83] = 92;
    map2[10][106] = 90; map2[10][107] = 91; map2[10][108] = 92;

    // 10. 버섯 줄기
    for (int y = 12; y <= 14; y++) map2[y][7] = 11;
    for (int y = 11; y <= 14; y++) map2[y][9] = 11;
    for (int y = 7; y <= 14; y++) map2[y][15] = 11;
    for (int y = 12; y <= 14; y++) map2[y][18] = 11;
    for (int y = 9; y <= 14; y++) map2[y][25] = 11;
    for (int y = 5; y <= 14; y++) map2[y][40] = 11;
    for (int y = 11; y <= 14; y++) map2[y][43] = 11;
    for (int y = 4; y <= 14; y++) map2[y][60] = 11;
    for (int y = 7; y <= 14; y++) map2[y][85] = 11;
    for (int y = 5; y <= 14; y++) map2[y][91] = 11;
    for (int y = 10; y <= 14; y++) map2[y][81] = 11;
    for (int y = 11; y <= 14; y++) map2[y][107] = 11;


    // 2. 코인
    map2[5][13] = 2;
    map2[5][14] = 2;
    map2[5][15] = 2;
    map2[5][16] = 2;
    map2[5][17] = 2;
    map2[6][51] = 2;
    map2[11][68] = 2;
    map2[11][69] = 2;
    map2[11][70] = 2;
    map2[12][48] = 2;
    map2[12][49] = 2;
    map2[12][52] = 2;
    map2[12][53] = 2;
    map2[12][54] = 2;
    map2[11][101] = 2;
    map2[11][102] = 2;
    map2[11][103] = 2;

    map2[9][53] = 63;

    // 깃발
    map2[MAP_HEIGHT - 3][115] = 5;
    // 7. 깃발
    for (int y = 3; y <= 11; y++)
    {
        map2[y][115] = 7;
    }
    // 8. 깃발 꼭짓점
    map2[2][115] = 8;
    
    // 9. 성
    map2[12][121] = 9;

}
//맵 3 구조
void InitMap3()
{
    // 0: 구멍 1: 땅 2: 코인 3:굼바 4:파이프 5:계단 6:미스테리박스 7:깃발 8: 깃발꼭짓점 9:성 10: 벽돌 
    // 90. 버섯머리1 91. 버섯머리2 92. 버섯머리3 11. 버섯줄기 12. 버섯줄기2 13.구름 14: 불기둥스위치 15: 회색벽돌
    // 16: 사용된블럭 17:용암head 18:용암body 19: 쿠파블럭 60:스타박스 61:꽃박스 62: 티노박스 63: 생명버섯박스 64: 코인박스
    currentMap = map3;


    memset(map3, 0, sizeof(map3));  // 초기화


    // 바닥 (전체 회색 벽돌)
    for (int x = 0; x < MAP_WIDTH; x++)
    {
        for (int y = 10; y <= 14; y++)
        {
            map3[y][x] = 15;
        }
    }

    //윗부분
    for (int x = 0; x < MAP_WIDTH; x++)
    {
        for (int y = 0; y <= 0; y++)
        {
            map3[y][x] = 15;
        }
    }

    for (int x = 0; x < 30; x++)
    {
        for (int y = 1; y <= 4; y++)
        {
            map3[y][x] = 15;
        }
    }

    for (int x = 50; x < 56; x++)
    {
        for (int y = 1; y <= 2; y++)
        {
            map3[y][x] = 15;
        }
    }

    for (int x = 76; x < 82; x++)
    {
        for (int y = 1; y <= 2; y++)
        {
            map3[y][x] = 15;
        }
    }


    for (int x = 95; x < 103; x++)
    {
        for (int y = 1; y <= 3; y++)
        {
            map3[y][x] = 15;
        }
    }

    for (int x = 110; x < 115; x++)
    {
        for (int y = 1; y <= 2; y++)
        {
            map3[y][x] = 15;
        }
    }

    for (int y = 0; y <= 14; y++)
    {
        for (int x = 137; x <= 139; x++)
        {
            map3[y][x] = 15;
        }
    }

    map3[7][120] = 61; // 보스전아이템 꽃
    map3[7][121] = 62; // 보스전아이템 티노

    //map3[7][3] = 60;    // 시작지점 테스트 스타
    //map3[7][6] = 62;    // 시작지점 테스트 티노

    //구멍
    for (int y = 10; y <= 14; y++)
    {
        for (int x = 30; x <= 31; x++)
        {
            map3[y][x] = 0;
        }
    }

    for (int y = 10; y <= 14; y++)
    {
        for (int x = 37; x <= 39; x++)
        {
            map3[y][x] = 0;
        }
    }

    for (int y = 10; y <= 14; y++)
    {
        for (int x = 42; x <= 44; x++)
        {
            map3[y][x] = 0;
        }
    }

    for (int y = 10; y <= 12; y++)
    {
        for (int x = 103; x <= 109; x++)
        {
            map3[y][x] = 0;
        }
    }

    for (int y = 5; y <= 7; y++)
    {
        for (int x = 137; x <= 139; x++)
        {
            map3[y][x] = 0;
        }
    }

    for (int x = 116; x <= 136; x++)
    {
        map3[10][x] = 0;
        map3[11][x] = 19;
    }
    for (int y = 12; y <= 14; y++)
    {
        for (int x = 116; x <= 136; x++)
        {
            map3[y][x] = 0;
        }
    }

    map3[6][105] = 15;
    map3[6][106] = 15;
    map3[6][107] = 15;

    // 2. 코인
    map3[3][104] = 2;
    map3[3][106] = 62;   // 난이도 높은 티노
    map3[3][108] = 2;
    map3[8][104] = 2;
    map3[8][106] = 2;
    map3[8][108] = 2;
    map3[6][12] = 2;
    map3[6][13] = 2;
    map3[6][17] = 2;
    map3[6][18] = 2;
    map3[6][21] = 2;
    map3[6][22] = 2;
    map3[6][23] = 2;
    map3[6][26] = 2;
    map3[6][30] = 2;
    map3[6][31] = 2;
    map3[6][37] = 2;
    map3[6][38] = 2;
    map3[6][39] = 2;
    map3[6][42] = 2;
    map3[6][43] = 2;
    map3[6][44] = 2;
    map3[6][56] = 2;
    map3[6][57] = 2;
    map3[6][58] = 2;

    map3[6][62] = 2;
    map3[7][63] = 2;
    map3[6][64] = 2;
    map3[7][65] = 2;
    map3[6][66] = 2;
    map3[7][67] = 2;
    map3[6][68] = 2;

    map3[6][71] = 2;
    map3[7][72] = 2;
    map3[6][73] = 2;
    map3[7][74] = 2;
    map3[6][75] = 2;

    //13. 불구덩이
    for (int i = 12; i <= 14; i++)
    {
        map3[i][30] = 18;
        map3[i][31] = 18;

        map3[i][37] = 18;
        map3[i][38] = 18;
        map3[i][39] = 18;

        map3[i][42] = 18;
        map3[i][43] = 18;
        map3[i][44] = 18;

    }

    map3[11][30] = 17;
    map3[11][31] = 17;

    map3[11][37] = 17;
    map3[11][38] = 17;
    map3[11][39] = 17;

    map3[11][42] = 17;
    map3[11][43] = 17;
    map3[11][44] = 17;


    for (int y = 13; y <= 14; y++)
    {
        for (int x = 116; x <= 136; x++)
        {
            map3[y][x] = 18;
        }
    }

    for (int x = 116; x <= 136; x++)
    {
        map3[12][x] = 17;
    }


    //14 불 나오는 스위치
    map3[5][10] = 14;
    map3[5][20] = 14;
    map3[5][28] = 14;
    map3[9][15] = 14;
    map3[9][24] = 14;
    map3[9][50] = 14;
    map3[3][52] = 14;
    map3[9][55] = 14;
    map3[9][60] = 14;
    map3[9][70] = 14;
    map3[3][78] = 14;
    map3[3][79] = 14;
    map3[9][76] = 14;
    map3[9][81] = 14;
    map3[0][105] = 14;
    map3[0][107] = 14;


    /*스위치에서 나오는 불꽃
    for (int i = 6; i <= 9; i++) {
        map3[i][10] = 20;
    }

    for (int i = 5; i <= 8; i++) {
        map3[i][15] = 20;
    }

    for (int i = 6; i <= 9; i++) {
        map3[i][28] = 20;
    }


    for (int i = 6; i <= 9; i++) {
        map3[i][28] = 20;
    }
    */

    //18. 쿠파블럭 벽
    map3[5][138] = 19;
    map3[6][138] = 19;
    map3[7][138] = 19;

    for (int y = 0; y < MAP_HEIGHT; y++)
    {
        map3[y][155] = 15;
        map3[y][156] = 15;
        map3[y][157] = 15;
        map3[y][158] = 15;
        map3[y][159] = 15;
        map3[y][160] = 15;
        map3[y][161] = 15;
        map3[y][162] = 15;
        map3[y][163] = 15;
    }
    map3[8][147] = 999;
}

// 오브젝트 그리기
void Draw()
{
    RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
    if (stage == 1 || stage == 2) FillRect(g_memDC, &rect, sky_brush);
    else if (stage == 3) FillRect(g_memDC, &rect, black_brush);
    Draw_background();
    Graphics graphics(g_memDC);
    graphics.SetInterpolationMode(InterpolationModeNearestNeighbor);

    Draw_spawn_item();
    Draw_map();
    Draw_item();
    Draw_Monsters();
    Draw_Turtles();
    Draw_Angel_Turtles();
    Draw_Bowser();
    Draw_Fireballs();
    Draw_fireball();
    DrawFireTraps();

    // mario
    if (gameState == GAME_TRANSFORMING && !mario.isBig && !mario.flower && !mario.tino)
    {
        if ((GetTickCount() / 100) % 2 == 0)
        {
            graphics.DrawImage(big_mario_change, mario.x, mario.y, mario.width, TILE_SIZE * 2);
        }
        else
        {
            graphics.DrawImage(mario_stop, mario.x, mario.y + TILE_SIZE, mario.width, mario.height);
        }
    }
    // flower
    else if (gameState == GAME_TRANSFORMING && mario.isBig && !mario.flower && !mario.tino)
    {
        if ((GetTickCount() / 100) % 2 == 0)
        {
            graphics.DrawImage(big_mario_change, mario.x, mario.y - TILE_SIZE, mario.width, mario.height);
        }
        else
        {
            graphics.DrawImage(mario_stop, mario.x, mario.y, mario.width, TILE_SIZE);
        }
    }
    else if (gameState == GAME_FLOWER_TRANS && !mario.isBig && !mario.flower && !mario.tino)    // small mario >> flower
    {
        if ((GetTickCount() / 100) % 2 == 0)
        {
            graphics.DrawImage(flower_mario_change, mario.x, mario.y, mario.width, TILE_SIZE * 2);
        }
        else
        {
            graphics.DrawImage(mario_stop, mario.x, mario.y + TILE_SIZE, mario.width, mario.height);
        }
    }
    else if (gameState == GAME_FLOWER_TRANS && mario.isBig && !mario.flower && !mario.tino)     // big mario >> flower
    {
        if ((GetTickCount() / 100) % 2 == 0)
        {
            graphics.DrawImage(flower_mario_stop, mario.x, mario.y, mario.width, mario.height);
        }
        else
        {
            graphics.DrawImage(big_mario_stop, mario.x, mario.y, mario.width, mario.height);
        }
    }
    else if (gameState == GAME_FLOWER_TRANS && !mario.isBig && !mario.flower && mario.tino)     // tino >> flower
    {
        if ((GetTickCount() / 100) % 2 == 0)
        {
            graphics.DrawImage(tino_mario_stop, mario.x, mario.y, mario.width, mario.height);
        }
        else
        {
            graphics.DrawImage(flower_mario_stop, mario.x, mario.y, mario.width, mario.height);
        }
    }
    else if (gameState == GAME_FLOWER_TRANS && mario.flower)     // flower >> big mario
    {
        if ((GetTickCount() / 100) % 2 == 0)
        {
            graphics.DrawImage(flower_mario_stop, mario.x, mario.y, mario.width, mario.height);
        }
        else
        {
            graphics.DrawImage(big_mario_stop, mario.x, mario.y, mario.width, mario.height);
        }
    }
    // tino
    else if (gameState == GAME_TINO_TRANS && !mario.isBig && !mario.flower && !mario.tino)    // small mario >> tino
    {
        if ((GetTickCount() / 100) % 2 == 0)
        {
            graphics.DrawImage(tino_mario_stop, mario.x, mario.y, mario.width, TILE_SIZE * 2);
        }
        else
        {
            graphics.DrawImage(mario_stop, mario.x, mario.y + TILE_SIZE, mario.width, mario.height);
        }
    }
    else if (gameState == GAME_TINO_TRANS && mario.isBig && !mario.flower && !mario.tino)     // big mario >> tino
    {
        if ((GetTickCount() / 100) % 2 == 0)
        {
            graphics.DrawImage(tino_mario_stop, mario.x, mario.y, mario.width, mario.height);
        }
        else
        {
            graphics.DrawImage(big_mario_stop, mario.x, mario.y, mario.width, mario.height);
        }
    }
    else if (gameState == GAME_TINO_TRANS && !mario.isBig && mario.flower && !mario.tino)     // flower >> tino
    {
        if ((GetTickCount() / 100) % 2 == 0)
        {
            graphics.DrawImage(tino_mario_stop, mario.x, mario.y, mario.width, mario.height);
        }
        else
        {
            graphics.DrawImage(flower_mario_stop, mario.x, mario.y, mario.width, mario.height);
        }
    }
    else if (gameState == GAME_TINO_TRANS && mario.tino)     // tino >> big mario
    {
        if ((GetTickCount() / 100) % 2 == 0)
        {
            graphics.DrawImage(tino_mario_stop, mario.x, mario.y, mario.width, mario.height);
        }
        else
        {
            graphics.DrawImage(big_mario_stop, mario.x, mario.y, mario.width, mario.height);
        }
    }
    // gameover
    else if (gameState == GAME_OVER && !mario.isDead)
    {
        graphics.DrawImage(title_dead, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
        Draw_information();
        TCHAR life_print_dead[32], world_dead[32];
        _stprintf_s(world_dead, _T("WORLD %d"), stage);
        _stprintf_s(life_print_dead, _T("%d"), mario.life);

        TextOut(g_memDC, 300, 207, world_dead, lstrlen(world_dead));
        TextOut(g_memDC, 430, 295, life_print_dead, lstrlen(life_print_dead));
        return;
    }
    else
    {
        if(!mario.isDead)Draw_mario();
    }

    if (mario.isDead)
    {
        graphics.DrawImage(mario_dead, mario.x, mario.y, mario.width, TILE_SIZE);
    }

    Draw_castle_blank();
}
void Draw_map()
{
    // 0: 구멍 1: 땅 2: 코인 3:굼바 4:파이프 5:계단 6:미스테리박스 7:깃발 8: 깃발꼭짓점 9:성 10: 벽돌 
    // 90. 버섯머리1 91. 버섯머리2 92. 버섯머리3 11. 버섯줄기 12. 버섯줄기2 13.구름 16: 사용된블럭 60:스타박스 61:꽃박스
    Graphics graphics(g_memDC);
    graphics.SetInterpolationMode(InterpolationModeNearestNeighbor);

    // 맵 타일 그리기
    for (int i = 0; i < MAP_HEIGHT; i++)
    {
        for (int j = 0; j < MAP_WIDTH; j++)
        {
            int screenX = j * TILE_SIZE - cameraX;
            int screenY = i * TILE_SIZE;
            if (currentMap[i][j] == 1)         // 장애물
            {
                if (screenX + TILE_SIZE >= 0 && screenX < SCREEN_WIDTH)
                {
                    graphics.DrawImage(stage_1_dirt, screenX, screenY, TILE_SIZE + 1, TILE_SIZE + 1);
                }
            }
            else if (currentMap[i][j] == 10)         // 벽돌
            {
                if (screenX + TILE_SIZE >= 0 && screenX < SCREEN_WIDTH)
                {
                    graphics.DrawImage(stage_1_brick, screenX, screenY, TILE_SIZE + 1, TILE_SIZE + 1);
                }
            }
            else if (currentMap[i][j] == 2)         // 코인
            {
                if (screenX + TILE_SIZE >= 0 && screenX < SCREEN_WIDTH)
                {
                    if (frame_motion == 0 || (frame_motion >= 4 && frame_motion < 7))
                    {
                        graphics.DrawImage(coin_1, screenX, screenY, TILE_SIZE + 1, TILE_SIZE);
                    }
                    else if (frame_motion == 1 || frame_motion == 3)
                    {
                        graphics.DrawImage(coin_2, screenX, screenY, TILE_SIZE + 1, TILE_SIZE);
                    }
                    else if (frame_motion == 2)
                    {
                        graphics.DrawImage(coin_3, screenX, screenY, TILE_SIZE + 1, TILE_SIZE);
                    }
                }
            }
            else if (currentMap[i][j] == 5)    // 계단
            {
                if (screenX + TILE_SIZE >= 0 && screenX < SCREEN_WIDTH)
                {
                    graphics.DrawImage(unbreakable_block, screenX, screenY, TILE_SIZE + 1, TILE_SIZE + 1);
                }
            }
            else if (currentMap[i][j] == 6 || currentMap[i][j] == 60 || currentMap[i][j] == 61 || currentMap[i][j] == 62 || currentMap[i][j] == 63 || currentMap[i][j] == 64)    // 아이템 블럭
            {
                if (screenX + TILE_SIZE >= 0 && screenX < SCREEN_WIDTH)
                {
                    if (frame_motion == 0 || (frame_motion >= 4 && frame_motion < 7))
                    {
                        graphics.DrawImage(item_block_1, screenX, screenY, TILE_SIZE + 1, TILE_SIZE + 1);
                    }
                    else if (frame_motion == 1 || frame_motion == 3)
                    {
                        graphics.DrawImage(item_block_2, screenX, screenY, TILE_SIZE + 1, TILE_SIZE + 1);
                    }
                    else if (frame_motion == 2)
                    {
                        graphics.DrawImage(item_block_3, screenX, screenY, TILE_SIZE + 1, TILE_SIZE + 1);
                    }
                }
            }
            // 깃발 봉
            else if (currentMap[i][j] == 7)
            {
                if (screenX + TILE_SIZE >= 0 && screenX < SCREEN_WIDTH)
                {
                    graphics.DrawImage(flag_stick, screenX, screenY, TILE_SIZE + 1, TILE_SIZE + 1);
                }
            }
            // 깃발 꼭대기
            else if (currentMap[i][j] == 8)
            {
                if (screenX + TILE_SIZE >= 0 && screenX < SCREEN_WIDTH)
                {
                    graphics.DrawImage(flag_marble, screenX, screenY, TILE_SIZE + 1, TILE_SIZE + 1);
                }
            }
            // 성
            else if (currentMap[i][j] == 9)
            {
                if (screenX + TILE_SIZE >= 0 && screenX < SCREEN_WIDTH)
                {
                    graphics.DrawImage(castle_1, screenX - TILE_SIZE * 3, screenY - TILE_SIZE * 4, (TILE_SIZE + 1) * 5, (TILE_SIZE + 1) * 5);
                }
            }
            // 버섯머리1
            else if (currentMap[i][j] == 90)
            {
                if (screenX + TILE_SIZE >= 0 && screenX < SCREEN_WIDTH)
                {
                    graphics.DrawImage(mushroom_head_1, screenX, screenY, TILE_SIZE + 1, TILE_SIZE + 1);
                }
            }
            else if (currentMap[i][j] == 91)
            {
                if (screenX + TILE_SIZE >= 0 && screenX < SCREEN_WIDTH)
                {
                    graphics.DrawImage(mushroom_head_2, screenX, screenY, TILE_SIZE + 1, TILE_SIZE + 1);
                }
            }
            else if (currentMap[i][j] == 92)
            {
                if (screenX + TILE_SIZE >= 0 && screenX < SCREEN_WIDTH)
                {
                    graphics.DrawImage(mushroom_head_3, screenX, screenY, TILE_SIZE + 1, TILE_SIZE + 1);
                }
            }
            // 버섯줄기
            else if (currentMap[i][j] == 11)
            {
                if (screenX + TILE_SIZE >= 0 && screenX < SCREEN_WIDTH)
                {
                    graphics.DrawImage(mushroom_trunk_1, screenX, screenY, TILE_SIZE + 1, TILE_SIZE + 1);
                }
            }
            else if (currentMap[i][j] == 12)
            {
                if (screenX + TILE_SIZE >= 0 && screenX < SCREEN_WIDTH)
                {
                    graphics.DrawImage(mushroom_trunk_2, screenX, screenY, TILE_SIZE + 1, TILE_SIZE + 1);
                }
            }
            else if (currentMap[i][j] == 13)
            {
                if (screenX + TILE_SIZE >= 0 && screenX < SCREEN_WIDTH)
                {
                    graphics.DrawImage(cloud_block, screenX, screenY, TILE_SIZE + 1, TILE_SIZE + 1);
                }
            }
            else if (currentMap[i][j] == 14)
            {
                if (screenX + TILE_SIZE >= 0 && screenX < SCREEN_WIDTH)
                {
                    graphics.DrawImage(fire_switch_tile, screenX, screenY, TILE_SIZE + 1, TILE_SIZE + 1);
                }
                }
            //스테이지 3 블럭
            else if (currentMap[i][j] == 15)
            {
                if (screenX + TILE_SIZE >= 0 && screenX < SCREEN_WIDTH)
                {
                    graphics.DrawImage(stone_tile, screenX, screenY, TILE_SIZE + 1, TILE_SIZE + 1);
                }
            }
            // 사용된 아이템블럭
            else if (currentMap[i][j] == 16)
            {
                if (screenX + TILE_SIZE >= 0 && screenX < SCREEN_WIDTH)
                {
                    graphics.DrawImage(item_block_used, screenX, screenY, TILE_SIZE + 1, TILE_SIZE + 1);
                }
             }
            //불 블럭
            else if (currentMap[i][j] == 17)
            {
                if (screenX + TILE_SIZE >= 0 && screenX < SCREEN_WIDTH)
                {
                    graphics.DrawImage(fire_head, screenX, screenY, TILE_SIZE + 1, TILE_SIZE + 1);
                }
            }
            else if (currentMap[i][j] == 18)
            {
                if (screenX + TILE_SIZE >= 0 && screenX < SCREEN_WIDTH)
                {
                    graphics.DrawImage(fire_body, screenX, screenY, TILE_SIZE + 1, TILE_SIZE + 1);
                }
            }
            else if (currentMap[i][j] == 19)
            {
                if (screenX + TILE_SIZE >= 0 && screenX < SCREEN_WIDTH)
                {
                    graphics.DrawImage(koopa_block, screenX, screenY, TILE_SIZE + 1, TILE_SIZE + 1);
                }
             }
            // 파이프
            else if (currentMap[i][j] == 40) // 우하
            {
                if (screenX + TILE_SIZE >= 0 && screenX < SCREEN_WIDTH)
                {
                    graphics.DrawImage(pipe_1, screenX, screenY, TILE_SIZE + 1, TILE_SIZE + 1);
                }
            }
            else if (currentMap[i][j] == 41) // 좌하
            {
                if (screenX + TILE_SIZE >= 0 && screenX < SCREEN_WIDTH)
                {
                    graphics.DrawImage(pipe_2, screenX, screenY, TILE_SIZE + 1, TILE_SIZE + 1);
                }
            }
            else if (currentMap[i][j] == 42) // 좌상
            {
                if (screenX + TILE_SIZE >= 0 && screenX < SCREEN_WIDTH)
                {
                    graphics.DrawImage(pipe_3, screenX, screenY, TILE_SIZE + 1, TILE_SIZE + 1);
                }
            }
            else if (currentMap[i][j] == 43) // 좌상
            {
                if (screenX + TILE_SIZE >= 0 && screenX < SCREEN_WIDTH)
                {
                    graphics.DrawImage(pipe_4, screenX, screenY, TILE_SIZE + 1, TILE_SIZE + 1);
                }
            }
            else if (currentMap[i][j] == 999)
            {
                if (screenX + TILE_SIZE >= 0 && screenX < SCREEN_WIDTH)
                {
                    graphics.DrawImage(peach, screenX, screenY, TILE_SIZE + 1, TILE_SIZE * 2);
                }
           }
            
        }
    }

}
void Draw_castle_blank()
{
    Graphics graphics(g_memDC);
    graphics.SetInterpolationMode(InterpolationModeNearestNeighbor);

    for (int i = 0; i < MAP_HEIGHT; i++)
    {
        for (int j = 0; j < MAP_WIDTH; j++)
        {
            int screenX = j * TILE_SIZE - cameraX;
            int screenY = i * TILE_SIZE;
            if (currentMap[i][j] == 9)
            {
                if (screenX + TILE_SIZE >= 0 && screenX < SCREEN_WIDTH)
                {
                    graphics.DrawImage(castle_blank, screenX - TILE_SIZE, screenY - TILE_SIZE, TILE_SIZE + 1, (TILE_SIZE + 1) * 2);
                }
            }

        }
    }
}
void Draw_mario()
{
    Graphics graphics(g_memDC);
    graphics.SetInterpolationMode(InterpolationModeNearestNeighbor);

    if (!mario.isBig && !mario.flower && !mario.tino && !mario.star) // small mario
    {
        if (mario.isJumping)
        {
            graphics.DrawImage(mario_jump, mario.x, mario.y, mario.width, mario.height);
        }
        else if (mario.isWalking)
        {
            if (mario.walk_motion == 0)
            {
                graphics.DrawImage(mario_walk_motion_1, mario.x, mario.y, mario.width, mario.height);
            }
            else if (mario.walk_motion == 1)
            {
                graphics.DrawImage(mario_walk_motion_2, mario.x, mario.y, mario.width, mario.height);
            }
            else if (mario.walk_motion == 2)
            {
                graphics.DrawImage(mario_walk_motion_3, mario.x, mario.y, mario.width, mario.height);
            }
        }
        else
        {
            graphics.DrawImage(mario_stop, mario.x, mario.y, mario.width, mario.height);
        }
    }
    else if (mario.isBig && !mario.flower && !mario.tino && !mario.star) // big mario
    {
        if (mario.isJumping)
        {
            graphics.DrawImage(big_mario_jump, mario.x, mario.y, mario.width, mario.height);
        }
        else if (mario.isWalking)
        {
            if (mario.walk_motion == 0)
            {
                graphics.DrawImage(big_mario_walk_motion_1, mario.x, mario.y, mario.width, mario.height);
            }
            else if (mario.walk_motion == 1)
            {
                graphics.DrawImage(big_mario_walk_motion_2, mario.x, mario.y, mario.width, mario.height);
            }
            else if (mario.walk_motion == 2)
            {
                graphics.DrawImage(big_mario_walk_motion_3, mario.x, mario.y, mario.width, mario.height);
            }
        }
        else
        {
            graphics.DrawImage(big_mario_stop, mario.x, mario.y, mario.width, mario.height);
        }
    }

    else if (!mario.isBig && !mario.flower && !mario.tino && mario.star) // star small mario
    {
        if (frame_motion_star == 0)
        {
            if (mario.isJumping)
            {
                graphics.DrawImage(star_mario_jump_1, mario.x, mario.y, mario.width, mario.height);
            }
            else if (mario.isWalking)
            {
                if (mario.walk_motion == 0)
                {
                    graphics.DrawImage(star_mario_walk_motion_1_1, mario.x, mario.y, mario.width, mario.height);
                }
                else if (mario.walk_motion == 1)
                {
                    graphics.DrawImage(star_mario_walk_motion_2_1, mario.x, mario.y, mario.width, mario.height);
                }
                else if (mario.walk_motion == 2)
                {
                    graphics.DrawImage(star_mario_walk_motion_3_1, mario.x, mario.y, mario.width, mario.height);
                }
            }
            else
            {
                graphics.DrawImage(star_mario_stop_1, mario.x, mario.y, mario.width, mario.height);
            }
        }
        else if (frame_motion_star == 1)
        {
            if (mario.isJumping)
            {
                graphics.DrawImage(star_mario_jump_2, mario.x, mario.y, mario.width, mario.height);
            }
            else if (mario.isWalking)
            {
                if (mario.walk_motion == 0)
                {
                    graphics.DrawImage(star_mario_walk_motion_1_2, mario.x, mario.y, mario.width, mario.height);
                }
                else if (mario.walk_motion == 1)
                {
                    graphics.DrawImage(star_mario_walk_motion_2_2, mario.x, mario.y, mario.width, mario.height);
                }
                else if (mario.walk_motion == 2)
                {
                    graphics.DrawImage(star_mario_walk_motion_3_2, mario.x, mario.y, mario.width, mario.height);
                }
            }
            else
            {
                graphics.DrawImage(star_mario_stop_2, mario.x, mario.y, mario.width, mario.height);
            }
        }
        else if (frame_motion_star == 2)
        {
            if (mario.isJumping)
            {
                graphics.DrawImage(star_mario_jump_3, mario.x, mario.y, mario.width, mario.height);
            }
            else if (mario.isWalking)
            {
                if (mario.walk_motion == 0)
                {
                    graphics.DrawImage(star_mario_walk_motion_1_3, mario.x, mario.y, mario.width, mario.height);
                }
                else if (mario.walk_motion == 1)
                {
                    graphics.DrawImage(star_mario_walk_motion_2_3, mario.x, mario.y, mario.width, mario.height);
                }
                else if (mario.walk_motion == 2)
                {
                    graphics.DrawImage(star_mario_walk_motion_3_3, mario.x, mario.y, mario.width, mario.height);
                }
            }
            else
            {
                graphics.DrawImage(star_mario_stop_3, mario.x, mario.y, mario.width, mario.height);
            }
        }
    }
    else if ((mario.isBig || mario.flower || mario.tino) && mario.star) // star big mario
    {
        if(frame_motion_star == 0)
        {
            if (mario.isJumping)
            {
                graphics.DrawImage(star_big_mario_jump_1, mario.x, mario.y, mario.width, mario.height);
            }
            else if (mario.isWalking)
            {
                if (mario.walk_motion == 0)
                {
                    graphics.DrawImage(star_big_mario_walk_motion_1_1, mario.x, mario.y, mario.width, mario.height);
                }
                else if (mario.walk_motion == 1)
                {
                    graphics.DrawImage(star_big_mario_walk_motion_2_1, mario.x, mario.y, mario.width, mario.height);
                }
                else if (mario.walk_motion == 2)
                {
                    graphics.DrawImage(star_big_mario_walk_motion_3_1, mario.x, mario.y, mario.width, mario.height);
                }
            }
            else
            {
                graphics.DrawImage(star_big_mario_stop_1, mario.x, mario.y, mario.width, mario.height);
            }
        }
        else if (frame_motion_star == 1)
        {
            if (mario.isJumping)
            {
                graphics.DrawImage(star_big_mario_jump_2, mario.x, mario.y, mario.width, mario.height);
            }
            else if (mario.isWalking)
            {
                if (mario.walk_motion == 0)
                {
                    graphics.DrawImage(star_big_mario_walk_motion_1_2, mario.x, mario.y, mario.width, mario.height);
                }
                else if (mario.walk_motion == 1)
                {
                    graphics.DrawImage(star_big_mario_walk_motion_2_2, mario.x, mario.y, mario.width, mario.height);
                }
                else if (mario.walk_motion == 2)
                {
                    graphics.DrawImage(star_big_mario_walk_motion_3_2, mario.x, mario.y, mario.width, mario.height);
                }
            }
            else
            {
                graphics.DrawImage(star_big_mario_stop_2, mario.x, mario.y, mario.width, mario.height);
            }
        }
        else if (frame_motion_star == 2)
        {
            if (mario.isJumping)
            {
                graphics.DrawImage(star_big_mario_jump_3, mario.x, mario.y, mario.width, mario.height);
            }
            else if (mario.isWalking)
            {
                if (mario.walk_motion == 0)
                {
                    graphics.DrawImage(star_big_mario_walk_motion_1_3, mario.x, mario.y, mario.width, mario.height);
                }
                else if (mario.walk_motion == 1)
                {
                    graphics.DrawImage(star_big_mario_walk_motion_2_3, mario.x, mario.y, mario.width, mario.height);
                }
                else if (mario.walk_motion == 2)
                {
                    graphics.DrawImage(star_big_mario_walk_motion_3_3, mario.x, mario.y, mario.width, mario.height);
                }
            }
            else
            {
                graphics.DrawImage(star_big_mario_stop_3, mario.x, mario.y, mario.width, mario.height);
            }
        }
    }

    else if (mario.flower) // flower mario
    {
        if (mario.fire_motion)
        {
            graphics.DrawImage(flower_mario_fire, mario.x, mario.y, mario.width, mario.height);
        }
        else if (mario.isJumping)
        {
            graphics.DrawImage(flower_mario_jump, mario.x, mario.y, mario.width, mario.height);
        }
        else if (mario.isWalking)
        {
            if (mario.walk_motion == 0)
            {
                graphics.DrawImage(flower_mario_walk_motion_1, mario.x, mario.y, mario.width, mario.height);
            }
            else if (mario.walk_motion == 1)
            {
                graphics.DrawImage(flower_mario_walk_motion_2, mario.x, mario.y, mario.width, mario.height);
            }
            else if (mario.walk_motion == 2)
            {
                graphics.DrawImage(flower_mario_walk_motion_3, mario.x, mario.y, mario.width, mario.height);
            }
        }
        else
        {
            graphics.DrawImage(flower_mario_stop, mario.x, mario.y, mario.width, mario.height);
        }
    }
    else if (mario.tino) // tino mario
    {
        if (mario.tino_motion)
        {
            if (mario.motion_timer >= 0 && mario.motion_timer <= 5)
            {
                graphics.DrawImage(tino_mario_attack_6, mario.x - 25, mario.y - 15, 100, 100);
            }
            else if (mario.motion_timer > 5 && mario.motion_timer <= 10)
            {
                graphics.DrawImage(tino_mario_attack_5, mario.x - 25, mario.y - 15, 100, 100);
            }
            else if (mario.motion_timer > 10 && mario.motion_timer <= 15)
            {
                graphics.DrawImage(tino_mario_attack_4, mario.x - 25, mario.y - 15, 100, 100);
            }
            else if (mario.motion_timer > 15 && mario.motion_timer <= 20)
            {
                graphics.DrawImage(tino_mario_attack_3, mario.x - 25, mario.y - 15, 100, 100);
            }
            else if (mario.motion_timer > 20 && mario.motion_timer <= 25)
            {
                graphics.DrawImage(tino_mario_attack_2, mario.x - 25, mario.y - 15, 100, 100);
            }
            else if (mario.motion_timer > 25 && mario.motion_timer <= 30)
            {
                graphics.DrawImage(tino_mario_attack_1, mario.x - 25, mario.y - 15, 100, 100);
            }
            tino_attack();
        }
        else if (mario.tino_fire_motion)
        {
            graphics.DrawImage(tino_mario_attack_2, mario.x - 25, mario.y - 15, 100, 100);
        }
        else if (mario.isJumping)
        {
            graphics.DrawImage(tino_mario_jump, mario.x, mario.y, mario.width, mario.height);
        }
        else if (mario.isWalking)
        {
            if (mario.walk_motion == 0)
            {
                graphics.DrawImage(tino_mario_walk_motion_1, mario.x, mario.y, mario.width, mario.height);
            }
            else if (mario.walk_motion == 1)
            {
                graphics.DrawImage(tino_mario_walk_motion_2, mario.x, mario.y, mario.width, mario.height);
            }
            else if (mario.walk_motion == 2)
            {
                graphics.DrawImage(tino_mario_walk_motion_3, mario.x, mario.y, mario.width, mario.height);
            }
        }
        else
        {
            graphics.DrawImage(tino_mario_stop, mario.x, mario.y, mario.width, mario.height);
        }
    }

}
void Draw_spawn_item()
{
    Graphics graphics(g_memDC);
    graphics.SetInterpolationMode(InterpolationModeNearestNeighbor);

    // 스폰모션그리기
    for (int i = 0; i < MAX_ITEMS; i++)
    {
        // 버섯
        if (mushroom[i].motion)
        {
            graphics.DrawImage(item_mushroom, mushroom[i].x - cameraX, mushroom[i].y, 40, 40);
        }
        // 생명 버섯
        if (up_mushroom[i].motion)
        {
            graphics.DrawImage(item_up_mushroom, up_mushroom[i].x - cameraX, up_mushroom[i].y, 40, 40);
        }
        // 스타
        if (star[i].motion)
        {
            if (frame_motion == 0 || frame_motion == 4 || frame_motion == 5 || frame_motion == 6)
            {
                graphics.DrawImage(item_star_1, star[i].x - cameraX, star[i].y, 40, 40);
            }
            else if (frame_motion == 1)
            {
                graphics.DrawImage(item_star_2, star[i].x - cameraX, star[i].y, 40, 40);
            }
            else if (frame_motion == 2)
            {
                graphics.DrawImage(item_star_3, star[i].x - cameraX, star[i].y, 40, 40);
            }
            else if (frame_motion == 3)
            {
                graphics.DrawImage(item_star_4, star[i].x - cameraX, star[i].y, 40, 40);
            }
        }
        // 꽃
        if (flower[i].motion) // 꽃
        {
            if (frame_motion == 0 || frame_motion == 4 || frame_motion == 5 || frame_motion == 6)
            {
                graphics.DrawImage(item_flower_1, flower[i].x - cameraX, flower[i].y, 40, 40);
            }
            else if (frame_motion == 1)
            {
                graphics.DrawImage(item_flower_2, flower[i].x - cameraX, flower[i].y, 40, 40);
            }
            else if (frame_motion == 2)
            {
                graphics.DrawImage(item_flower_3, flower[i].x - cameraX, flower[i].y, 40, 40);
            }
            else if (frame_motion == 3)
            {
                graphics.DrawImage(item_flower_4, flower[i].x - cameraX, flower[i].y, 40, 40);
            }
        }
        // 티노
        if (tino[i].motion)
        {
            graphics.DrawImage(item_tino, tino[i].x - cameraX, tino[i].y, 40, 40);
        }
    }

}
void Draw_item()
{
    Graphics graphics(g_memDC);
    graphics.SetInterpolationMode(InterpolationModeNearestNeighbor);

    for (int i = 0; i < MAX_ITEMS; i++)
    {
        // 버섯
        if (mushroom[i].active) // 버섯
        {
            graphics.DrawImage(item_mushroom, mushroom[i].x - cameraX, mushroom[i].y, 40, 40);
        }
        if (up_mushroom[i].active) // 생명버섯
        {
            graphics.DrawImage(item_up_mushroom, up_mushroom[i].x - cameraX, up_mushroom[i].y, 40, 40);
        }
        // 스타
        if (star[i].active) // 스타
        {
            if(frame_motion == 0 || frame_motion == 4 || frame_motion == 5 || frame_motion == 6)
            {
                graphics.DrawImage(item_star_1, star[i].x - cameraX, star[i].y, 40, 40);
            }
            else if (frame_motion == 1)
            {
                graphics.DrawImage(item_star_2, star[i].x - cameraX, star[i].y, 40, 40);
            }
            else if (frame_motion == 2)
            {
                graphics.DrawImage(item_star_3, star[i].x - cameraX, star[i].y, 40, 40);
            }
            else if (frame_motion == 3)
            {
                graphics.DrawImage(item_star_4, star[i].x - cameraX, star[i].y, 40, 40);
            }
        }
        // 꽃
        if (flower[i].active) // 꽃
        {
            if (frame_motion == 0 || frame_motion == 4 || frame_motion == 5 || frame_motion == 6)
            {
                graphics.DrawImage(item_flower_1, flower[i].x - cameraX, flower[i].y, 40, 40);
            }
            else if (frame_motion == 1)
            {
                graphics.DrawImage(item_flower_2, flower[i].x - cameraX, flower[i].y, 40, 40);
            }
            else if (frame_motion == 2)
            {
                graphics.DrawImage(item_flower_3, flower[i].x - cameraX, flower[i].y, 40, 40);
            }
            else if (frame_motion == 3)
            {
                graphics.DrawImage(item_flower_4, flower[i].x - cameraX, flower[i].y, 40, 40);
            }
        }
        // 티노
        if (tino[i].active)
        {
            graphics.DrawImage(item_tino, tino[i].x - cameraX, tino[i].y, 40, 40);
        }

    }

}
void Draw_fireball()
{
    Graphics graphics(g_memDC);
    graphics.SetInterpolationMode(InterpolationModeNearestNeighbor);

    for (int i = 0; i < MAX_SHOT; i++)
    {
        // flower fire
        if (!fireball[i].active) continue;
        int screenX = fireball[i].x - cameraX;
        int screenY = fireball[i].y;
        if (fireball[i].motion == 0)
        {
            graphics.DrawImage(shot_fireball_1, screenX, screenY, 20, 20);
        }
        else if (fireball[i].motion == 1)
        {
            graphics.DrawImage(shot_fireball_2, screenX, screenY, 20, 20);
        }
        else if (fireball[i].motion == 2)
        {
            graphics.DrawImage(shot_fireball_3, screenX, screenY, 20, 20);
        }
        else if (fireball[i].motion == 3)
        {
            graphics.DrawImage(shot_fireball_4, screenX, screenY, 20, 20);
        }

    }
    for (int i = 0; i < MAX_SHOT; i++)
    {
        // tino fire
        if (tinofire_effect[i].active)
        {
            int screenX = tinofire_effect[i].x - cameraX;
            int screenY = tinofire_effect[i].y;
            if(tinofire_effect[i].timer < 5)
            {
                graphics.DrawImage(tino_mario_fire_fade_1, screenX, screenY - 40, TILE_SIZE * 2, TILE_SIZE * 2);
            }
            else if (tinofire_effect[i].timer < 10)
            {
                graphics.DrawImage(tino_mario_fire_fade_2, screenX, screenY - 40, TILE_SIZE * 2, TILE_SIZE * 2);
            }
            else if (tinofire_effect[i].timer < 15)
            {
                graphics.DrawImage(tino_mario_fire_fade_3, screenX, screenY - 40, TILE_SIZE * 2, TILE_SIZE * 2);
            }
        }
        if (!tinofire[i].active) continue;
        int screenX = tinofire[i].x - cameraX;
        int screenY = tinofire[i].y;
        if (tinofire[i].direction == 0)
        {
            if (tinofire[i].motion < 3)
            {
                graphics.DrawImage(tino_mario_fire_1, screenX, screenY, TILE_SIZE * 2, TILE_SIZE * 2);
            }
            else 
            {
                graphics.DrawImage(tino_mario_fire_2, screenX, screenY, TILE_SIZE * 2, TILE_SIZE * 2);
            }
        }
        else if (tinofire[i].direction == 1)
        {
            if (tinofire[i].motion < 3)
            {
                graphics.DrawImage(tino_mario_fire_R_1, screenX, screenY, TILE_SIZE * 2, TILE_SIZE * 2);
            }
            else
            {
                graphics.DrawImage(tino_mario_fire_R_2, screenX, screenY, TILE_SIZE * 2, TILE_SIZE * 2);
            }
        }
    }
}
void Draw_information()
{
    Graphics graphics(g_memDC);
    graphics.SetInterpolationMode(InterpolationModeNearestNeighbor);
    TCHAR cooldown_z[32], cooldown_space[32];
    TCHAR clear_text_1[32];
    TCHAR clear_text_2[32];

    // 타이틀 스크린
    _stprintf_s(life_print, _T("MARIO"));
    _stprintf_s(life_print2, _T("%05d"), mario.life);
    _stprintf_s(time_print, _T("TIME"));
    _stprintf_s(time_print2, _T("%03d"), stage_time);
    _stprintf_s(coin_print, _T("%02d"), mario.coin);
    _stprintf_s(time_print2, _T("%03d"), stage_time);
    _stprintf_s(stage_print, _T("WORLD"));
    _stprintf_s(stage_print2, _T("%d"), stage);
    _stprintf_s(cooldown_z, _T("Z (fire): %d"), mario.tino_cooldown_z);
    _stprintf_s(cooldown_space, _T("SPACE (bite): %d"), mario.tino_cooldown_space);
    _stprintf_s(clear_text_1, _T("THANK YOU MARIO!"));
    _stprintf_s(clear_text_2, _T("YOUR QUEST IS OVER.!"));


    TextOut(g_memDC, 80, 20, life_print, lstrlen(life_print));         // 생명
    TextOut(g_memDC, 80, 40, life_print2, lstrlen(life_print2));         // 생명

    TextOut(g_memDC, 620, 20, time_print, lstrlen(time_print));         // 시간
    TextOut(g_memDC, 640, 40, time_print2, lstrlen(time_print2));         // 시간


    // 코인
    if (frame_motion < 4)
    {
        graphics.DrawImage(screen_coin_1, 260, 40, 25, 25);
    }
    else if (frame_motion == 4 || frame_motion == 6)
    {
        graphics.DrawImage(screen_coin_2, 260, 40, 25, 25);
    }
    else if (frame_motion == 5)
    {
        graphics.DrawImage(screen_coin_3, 260, 40, 25, 25);
    }
    graphics.DrawImage(screen_coin_x, 290, 40, 25, 25);
    TextOut(g_memDC, 320, 40, coin_print, lstrlen(coin_print));         // 코인

    TextOut(g_memDC, 440, 20, stage_print, lstrlen(stage_print));         // 월드
    TextOut(g_memDC, 480, 40, stage_print2, lstrlen(stage_print2));

    if(mario.tino)
    {
        TextOut(g_memDC, 80, 80, cooldown_z, lstrlen(cooldown_z));         // 쿨타임
        TextOut(g_memDC, 80, 100, cooldown_space, lstrlen(cooldown_space));         // 쿨타임
    }
    if (gameclear_text)
    {
        TextOut(g_memDC, 240, 160, clear_text_1, lstrlen(clear_text_1));         
        TextOut(g_memDC, 200, 240, clear_text_2, lstrlen(clear_text_2));        
    }

}

void Draw_background()
{
    Graphics graphics(g_memDC);
    graphics.SetInterpolationMode(InterpolationModeNearestNeighbor);

    if (stage == 1)
    {
        graphics.DrawImage(stage_1_background, -cameraX, -36, 4600, SCREEN_HEIGHT);
    }
    else if (stage == 2)
    {
        graphics.DrawImage(stage_2_background, -cameraX, -36, 4600, SCREEN_HEIGHT);
    }
}
void Draw_Monsters()
{
    Graphics graphics(g_memDC);
    graphics.SetInterpolationMode(InterpolationModeNearestNeighbor);

    for (int i = 0; i < monsterCount; i++)
    {
        if (!monsters[i].active)
            continue;

        int screenX = monsters[i].x - cameraX;
        int screenY = monsters[i].y;

        if (!monsters[i].isAlive)
        {
            if (stage == 1)
                graphics.DrawImage(monster1_dead, screenX, screenY, TILE_SIZE, TILE_SIZE);
            else if (stage == 2)
                graphics.DrawImage(monster2_dead, screenX, screenY, TILE_SIZE, TILE_SIZE);
            else if (stage == 3)
                graphics.DrawImage(monster3_dead, screenX, screenY, TILE_SIZE, TILE_SIZE);

            DWORD now = GetTickCount();
            if (monsters[i].deadstart == 0)
                monsters[i].deadstart = now;
            if (now - monsters[i].deadstart >= 300)
                monsters[i].active = false;

            continue;
        }

        if (screenX + TILE_SIZE < 0 || screenX >= SCREEN_WIDTH) continue;

        if (stage == 1)
            graphics.DrawImage((frame_motion % 2 == 0) ? monster1_motion1 : monster1_motion2, screenX, screenY, TILE_SIZE, TILE_SIZE);
        else if (stage == 2)
            graphics.DrawImage((frame_motion % 2 == 0) ? monster2_motion1 : monster2_motion2, screenX, screenY, TILE_SIZE, TILE_SIZE);
        else if (stage == 3)
            graphics.DrawImage((frame_motion % 2 == 0) ? monster3_motion1 : monster3_motion2, screenX, screenY, TILE_SIZE, TILE_SIZE);
    }
}
void Draw_Turtles()
{
    Graphics graphics(g_memDC);
    graphics.SetInterpolationMode(InterpolationModeNearestNeighbor);
    for (int i = 0; i < turtleCount; i++)
    {
        if (!turtles[i].isAlive) continue;

        int drawX = turtles[i].x - cameraX;
        int drawY = turtles[i].y;

        switch (turtles[i].turtleState)
        {
        case NORMAL:
        {
            if(turtles[i].direction == 0)
            {
                graphics.DrawImage((stage == 3)
                    ? ((frame_motion % 2 == 0) ? brown_turtle_1 : brown_turtle_2)
                    : ((frame_motion % 2 == 0) ? turtle_1 : turtle_2), drawX, drawY - TILE_SIZE, TILE_SIZE, TILE_SIZE * 2);
            }
            else if (turtles[i].direction == 1)
            {
                graphics.DrawImage((stage == 3) 
                    ? ((frame_motion % 2 == 0) ? brown_turtle_R_1 : brown_turtle_R_2) 
                    : ((frame_motion % 2 == 0) ? turtle_R_1 : turtle_R_2), drawX, drawY - TILE_SIZE, TILE_SIZE, TILE_SIZE * 2);
            }
            break;
        }
        case SHELL:
        case SPINNING:
        {
            graphics.DrawImage(stage==3 ? brown_turtle_hide : turtle_hide, drawX, drawY, 40, 40);
            break;
        }
        }
    }
}
void Draw_Angel_Turtles()
{
    Graphics graphics(g_memDC);
    graphics.SetInterpolationMode(InterpolationModeNearestNeighbor);
    for (int i = 0; i < angelTurtleCount; i++)
    {
        if (!angelTurtles[i].isAlive) continue;

        int drawX = angelTurtles[i].x - cameraX;
        int drawY = angelTurtles[i].y;

        switch (angelTurtles[i].state)
        {
        case FLYING:
        {
            graphics.DrawImage((frame_motion % 2 == 0) ? angel_turtle_1 : angel_turtle_2, drawX, drawY - TILE_SIZE, TILE_SIZE, TILE_SIZE + 20);
            break;
        }
        case HIDE:
        {
            graphics.DrawImage(turtle_hide, drawX, drawY, 40, 40);
            break;
        }
        }
    }
}
// bowser
void Draw_Bowser()
{
    if (!bowser.isAlive) return;

    Graphics graphics(g_memDC);
    graphics.SetInterpolationMode(InterpolationModeNearestNeighbor);

    int screenX = bowser.x - cameraX;
    int screenY = bowser.y;

    // 걷기 또는 불뿜기 애니메이션 프레임 처리
    static int walkFrame = 0;
    static DWORD lastFrameTime = GetTickCount();

    if (GetTickCount() - lastFrameTime > 200) { // 0.2초마다 프레임 전환
        walkFrame = (walkFrame + 1) % 2;
        lastFrameTime = GetTickCount();
    }

    // 불을 뿜는 중이면 fire 이미지 사용
    if (bowser.isFiring)
    {
        if (walkFrame == 0)
            graphics.DrawImage(bowser_fire_walk_1, screenX, screenY, bowser.width, bowser.height);
        else
            graphics.DrawImage(bowser_fire_walk_2, screenX, screenY, bowser.width, bowser.height);
    }
    else {
        if (walkFrame == 0)
            graphics.DrawImage(bowser_walk_1, screenX, screenY, bowser.width, bowser.height);
        else
            graphics.DrawImage(bowser_walk_2, screenX, screenY, bowser.width, bowser.height);
    }
}
void Draw_Fireballs() 
{
    Graphics graphics(g_memDC);
    graphics.SetInterpolationMode(InterpolationModeNearestNeighbor);

    for (int i = 0; i < MAX_FIREBALLS; i++) 
    {
        if (!fireballs[i].active) continue;

        int sx = fireballs[i].x - cameraX;
        int sy = fireballs[i].y;

        if(fireball[i].motion < 3)
        {
            graphics.DrawImage(bowser_fireball_1, sx, sy, TILE_SIZE + 30, 30);
        }
        else
        {
            graphics.DrawImage(bowser_fireball_2, sx, sy, TILE_SIZE + 30, 30);
        }
    }
}
void DrawFireTraps()
{
    Graphics graphics(g_memDC);
    graphics.SetInterpolationMode(InterpolationModeNearestNeighbor);

    for (int i = 0; i < fireTrapCount; i++) 
    {
        if (!fireTraps[i].active) continue;

        for (int j = 0; j < fireTraps[i].tileCount; j++)
        {
            int drawX = fireTraps[i].x - cameraX - 20;
            int drawY = fireTraps[i].y + j * TILE_SIZE;

            graphics.DrawImage(firetrap, drawX, drawY, TILE_SIZE, TILE_SIZE);
        }
    }
}
// 방향키 함수 
void movePlayer()
{
    if (keyState[VK_LEFT] && !keyState[VK_RIGHT])
    {
        if (mario.direction == 1)
        {
            mario_stop->RotateFlip(RotateNoneFlipX); // 좌우반전
            mario_walk_motion_1->RotateFlip(RotateNoneFlipX); // 좌우반전
            mario_walk_motion_2->RotateFlip(RotateNoneFlipX); // 좌우반전
            mario_walk_motion_3->RotateFlip(RotateNoneFlipX); // 좌우반전
            mario_jump->RotateFlip(RotateNoneFlipX); // 좌우반전
            big_mario_stop->RotateFlip(RotateNoneFlipX); // 좌우반전
            big_mario_walk_motion_1->RotateFlip(RotateNoneFlipX); // 좌우반전
            big_mario_walk_motion_2->RotateFlip(RotateNoneFlipX); // 좌우반전
            big_mario_walk_motion_3->RotateFlip(RotateNoneFlipX); // 좌우반전
            big_mario_jump->RotateFlip(RotateNoneFlipX); // 좌우반전

            flower_mario_change->RotateFlip(RotateNoneFlipX); // 좌우반전
            flower_mario_stop->RotateFlip(RotateNoneFlipX); // 좌우반전
            flower_mario_walk_motion_1->RotateFlip(RotateNoneFlipX); // 좌우반전
            flower_mario_walk_motion_2->RotateFlip(RotateNoneFlipX); // 좌우반전
            flower_mario_walk_motion_3->RotateFlip(RotateNoneFlipX); // 좌우반전
            flower_mario_jump->RotateFlip(RotateNoneFlipX); // 좌우반전
            flower_mario_fire->RotateFlip(RotateNoneFlipX); // 좌우반전

            tino_mario_stop->RotateFlip(RotateNoneFlipX); // 좌우반전
            tino_mario_walk_motion_1->RotateFlip(RotateNoneFlipX); // 좌우반전
            tino_mario_walk_motion_2->RotateFlip(RotateNoneFlipX); // 좌우반전
            tino_mario_walk_motion_3->RotateFlip(RotateNoneFlipX); // 좌우반전
            tino_mario_jump->RotateFlip(RotateNoneFlipX); // 좌우반전
            tino_mario_attack_1->RotateFlip(RotateNoneFlipX); // 좌우반전
            tino_mario_attack_2->RotateFlip(RotateNoneFlipX); // 좌우반전
            tino_mario_attack_3->RotateFlip(RotateNoneFlipX); // 좌우반전
            tino_mario_attack_4->RotateFlip(RotateNoneFlipX); // 좌우반전
            tino_mario_attack_5->RotateFlip(RotateNoneFlipX); // 좌우반전
            tino_mario_attack_6->RotateFlip(RotateNoneFlipX); // 좌우반전

            star_mario_stop_1->RotateFlip(RotateNoneFlipX); // 좌우반전
            star_mario_walk_motion_1_1->RotateFlip(RotateNoneFlipX); // 좌우반전
            star_mario_walk_motion_2_1->RotateFlip(RotateNoneFlipX); // 좌우반전
            star_mario_walk_motion_3_1->RotateFlip(RotateNoneFlipX); // 좌우반전
            star_mario_jump_1->RotateFlip(RotateNoneFlipX); // 좌우반전
            star_big_mario_stop_1->RotateFlip(RotateNoneFlipX); // 좌우반전
            star_big_mario_walk_motion_1_1->RotateFlip(RotateNoneFlipX); // 좌우반전
            star_big_mario_walk_motion_2_1->RotateFlip(RotateNoneFlipX); // 좌우반전
            star_big_mario_walk_motion_3_1->RotateFlip(RotateNoneFlipX); // 좌우반전
            star_big_mario_jump_1->RotateFlip(RotateNoneFlipX); // 좌우반전

            star_mario_stop_2->RotateFlip(RotateNoneFlipX); // 좌우반전
            star_mario_walk_motion_1_2->RotateFlip(RotateNoneFlipX); // 좌우반전
            star_mario_walk_motion_2_2->RotateFlip(RotateNoneFlipX); // 좌우반전
            star_mario_walk_motion_3_2->RotateFlip(RotateNoneFlipX); // 좌우반전
            star_mario_jump_2->RotateFlip(RotateNoneFlipX); // 좌우반전
            star_big_mario_stop_2->RotateFlip(RotateNoneFlipX); // 좌우반전
            star_big_mario_walk_motion_1_2->RotateFlip(RotateNoneFlipX); // 좌우반전
            star_big_mario_walk_motion_2_2->RotateFlip(RotateNoneFlipX); // 좌우반전
            star_big_mario_walk_motion_3_2->RotateFlip(RotateNoneFlipX); // 좌우반전
            star_big_mario_jump_2->RotateFlip(RotateNoneFlipX); // 좌우반전

            star_mario_stop_3->RotateFlip(RotateNoneFlipX); // 좌우반전
            star_mario_walk_motion_1_3->RotateFlip(RotateNoneFlipX); // 좌우반전
            star_mario_walk_motion_2_3->RotateFlip(RotateNoneFlipX); // 좌우반전
            star_mario_walk_motion_3_3->RotateFlip(RotateNoneFlipX); // 좌우반전
            star_mario_jump_3->RotateFlip(RotateNoneFlipX); // 좌우반전
            star_big_mario_stop_3->RotateFlip(RotateNoneFlipX); // 좌우반전
            star_big_mario_walk_motion_1_3->RotateFlip(RotateNoneFlipX); // 좌우반전
            star_big_mario_walk_motion_2_3->RotateFlip(RotateNoneFlipX); // 좌우반전
            star_big_mario_walk_motion_3_3->RotateFlip(RotateNoneFlipX); // 좌우반전
            star_big_mario_jump_3->RotateFlip(RotateNoneFlipX); // 좌우반전
        }
        mario.vx = -5;
        mario.direction = 0;
    }
    else if (keyState[VK_RIGHT] && !keyState[VK_LEFT]) 
    {
        if (mario.direction == 0)
        {
            mario_stop->RotateFlip(RotateNoneFlipX); // 좌우반전
            mario_walk_motion_1->RotateFlip(RotateNoneFlipX); // 좌우반전
            mario_walk_motion_2->RotateFlip(RotateNoneFlipX); // 좌우반전
            mario_walk_motion_3->RotateFlip(RotateNoneFlipX); // 좌우반전
            mario_jump->RotateFlip(RotateNoneFlipX); // 좌우반전
            big_mario_stop->RotateFlip(RotateNoneFlipX); // 좌우반전
            big_mario_walk_motion_1->RotateFlip(RotateNoneFlipX); // 좌우반전
            big_mario_walk_motion_2->RotateFlip(RotateNoneFlipX); // 좌우반전
            big_mario_walk_motion_3->RotateFlip(RotateNoneFlipX); // 좌우반전
            big_mario_jump->RotateFlip(RotateNoneFlipX); // 좌우반전
            big_mario_change->RotateFlip(RotateNoneFlipX); // 좌우반전

            flower_mario_change->RotateFlip(RotateNoneFlipX); // 좌우반전
            flower_mario_stop->RotateFlip(RotateNoneFlipX); // 좌우반전
            flower_mario_walk_motion_1->RotateFlip(RotateNoneFlipX); // 좌우반전
            flower_mario_walk_motion_2->RotateFlip(RotateNoneFlipX); // 좌우반전
            flower_mario_walk_motion_3->RotateFlip(RotateNoneFlipX); // 좌우반전
            flower_mario_jump->RotateFlip(RotateNoneFlipX); // 좌우반전
            flower_mario_fire->RotateFlip(RotateNoneFlipX); // 좌우반전

            tino_mario_stop->RotateFlip(RotateNoneFlipX); // 좌우반전
            tino_mario_walk_motion_1->RotateFlip(RotateNoneFlipX); // 좌우반전
            tino_mario_walk_motion_2->RotateFlip(RotateNoneFlipX); // 좌우반전
            tino_mario_walk_motion_3->RotateFlip(RotateNoneFlipX); // 좌우반전
            tino_mario_jump->RotateFlip(RotateNoneFlipX); // 좌우반전
            tino_mario_attack_1->RotateFlip(RotateNoneFlipX); // 좌우반전
            tino_mario_attack_2->RotateFlip(RotateNoneFlipX); // 좌우반전
            tino_mario_attack_3->RotateFlip(RotateNoneFlipX); // 좌우반전
            tino_mario_attack_4->RotateFlip(RotateNoneFlipX); // 좌우반전
            tino_mario_attack_5->RotateFlip(RotateNoneFlipX); // 좌우반전
            tino_mario_attack_6->RotateFlip(RotateNoneFlipX); // 좌우반전

            star_mario_stop_1->RotateFlip(RotateNoneFlipX); // 좌우반전
            star_mario_walk_motion_1_1->RotateFlip(RotateNoneFlipX); // 좌우반전
            star_mario_walk_motion_2_1->RotateFlip(RotateNoneFlipX); // 좌우반전
            star_mario_walk_motion_3_1->RotateFlip(RotateNoneFlipX); // 좌우반전
            star_mario_jump_1->RotateFlip(RotateNoneFlipX); // 좌우반전
            star_big_mario_stop_1->RotateFlip(RotateNoneFlipX); // 좌우반전
            star_big_mario_walk_motion_1_1->RotateFlip(RotateNoneFlipX); // 좌우반전
            star_big_mario_walk_motion_2_1->RotateFlip(RotateNoneFlipX); // 좌우반전
            star_big_mario_walk_motion_3_1->RotateFlip(RotateNoneFlipX); // 좌우반전
            star_big_mario_jump_1->RotateFlip(RotateNoneFlipX); // 좌우반전

            star_mario_stop_2->RotateFlip(RotateNoneFlipX); // 좌우반전
            star_mario_walk_motion_1_2->RotateFlip(RotateNoneFlipX); // 좌우반전
            star_mario_walk_motion_2_2->RotateFlip(RotateNoneFlipX); // 좌우반전
            star_mario_walk_motion_3_2->RotateFlip(RotateNoneFlipX); // 좌우반전
            star_mario_jump_2->RotateFlip(RotateNoneFlipX); // 좌우반전
            star_big_mario_stop_2->RotateFlip(RotateNoneFlipX); // 좌우반전
            star_big_mario_walk_motion_1_2->RotateFlip(RotateNoneFlipX); // 좌우반전
            star_big_mario_walk_motion_2_2->RotateFlip(RotateNoneFlipX); // 좌우반전
            star_big_mario_walk_motion_3_2->RotateFlip(RotateNoneFlipX); // 좌우반전
            star_big_mario_jump_2->RotateFlip(RotateNoneFlipX); // 좌우반전

            star_mario_stop_3->RotateFlip(RotateNoneFlipX); // 좌우반전
            star_mario_walk_motion_1_3->RotateFlip(RotateNoneFlipX); // 좌우반전
            star_mario_walk_motion_2_3->RotateFlip(RotateNoneFlipX); // 좌우반전
            star_mario_walk_motion_3_3->RotateFlip(RotateNoneFlipX); // 좌우반전
            star_mario_jump_3->RotateFlip(RotateNoneFlipX); // 좌우반전
            star_big_mario_stop_3->RotateFlip(RotateNoneFlipX); // 좌우반전
            star_big_mario_walk_motion_1_3->RotateFlip(RotateNoneFlipX); // 좌우반전
            star_big_mario_walk_motion_2_3->RotateFlip(RotateNoneFlipX); // 좌우반전
            star_big_mario_walk_motion_3_3->RotateFlip(RotateNoneFlipX); // 좌우반전
            star_big_mario_jump_3->RotateFlip(RotateNoneFlipX); // 좌우반전
        }
        mario.vx = 5;
        mario.direction = 1;
    }
    else if (keyState[VK_LEFT] && keyState[VK_RIGHT])
    {
        // 둘 다 눌린 경우 마지막 방향 유지
        mario.vx = (mario.direction == 0 ? -5 : 5);
    }
    else 
    {
        mario.isWalking = false;
        mario.vx = 0;
        mario.walk_motion = 0;
    }
}

// 물리엔진
void UpdateGame()
{
    // mushroom 변신 모션
    if (gameState == GAME_TRANSFORMING) 
    {
        mario.god = true;
        DWORD now = GetTickCount();
        if (now - transformStartTime >= 700) 
        {
            if (!mario.isBig)
            {
                transform_bigmario();
            }
            else if (mario.god)
            {
                transform_smallmario();
                godstart = GetTickCount();
            }
            gameState = GAME_RUNNING;  // 다시 정상 진행
        }

        return; // 게임 상태 업데이트 생략해서 "멈춘 듯한" 연출
    }
    // flower 변신 모션
    else if (gameState == GAME_FLOWER_TRANS)
    {
        mario.god = true;
        DWORD now = GetTickCount();
        if (now - transformStartTime >= 700)
        {
            if (!mario.flower)
            {
                transform_to_flower();
            }
            else if (mario.god)
            {
                transform_bigmario();
                godstart = GetTickCount();
            }
            gameState = GAME_RUNNING;  // 다시 정상 진행
        }

        return; // 게임 상태 업데이트 생략해서 "멈춘 듯한" 연출
    }
    // tino 변신 모션
    else if (gameState == GAME_TINO_TRANS)
    {
        mario.god = true;
        DWORD now = GetTickCount();
        if (now - transformStartTime >= 700)
        {
            if (!mario.tino)
            {
                transform_to_tino();
            }
            else if (mario.god)
            {
                transform_bigmario();
                godstart = GetTickCount();
            }
            gameState = GAME_RUNNING;  // 다시 정상 진행
        }

        return; // 게임 상태 업데이트 생략해서 "멈춘 듯한" 연출
    }
    // 승리 모션
    else if (gameState == GAME_VICTORY)
    {
        DWORD now = GetTickCount();
        if (now - victoryStart >= 0)
        {
            mario.vx = 1;
            mario.x += mario.vx;
        }
        if (now - victoryStart >= 5000)
        {
            stage++;
            stage_load();
            gameState = GAME_RUNNING;
        }
    }
    // 최종 승리 모션
    else if (gameState == GAME_CLEAR)
    {
        if (GetTickCount() - clearStart >= 0 && GetTickCount() - clearStart <= 5000)
        {
            mario.vx = 1;
            mario.x += mario.vx;
        }
        else
        {
            gameclear_text = true;
            mario.isWalking = false;
            mario.vx = 0;
        }
        if (GetTickCount() - clearStart >= 10000)
        {
            stage = 1;
            gameclear_text = false;
            gamestart = false;
            mario = { 100, 300, 0, 0, 5, 0, 40, 40, 1, 0, 0, 0, 0, false, false, false, false, false, false, false, false, false, false, false, false, false, false };
            gameState = GAME_RUNNING;
        }
    }
    // 시망모션
    else if (gameState == GAME_OVER)
    {
        DWORD now = GetTickCount();
        if (mario.isDead)
        {
            static bool motion1;
            if (motion1) UpdateDeadMotion();
            if (now - deadStartTime >= 500 && !motion1)
            {
                mario.vy -= 14;
                motion1 = true;
            }
            if (now - deadStartTime >= 2000)
            {
                if (mario.life <= 0)
                {
                    stage = 1;
                    gameclear_text = false;
                    gamestart = false;
                    mario = { 100, 300, 0, 0, 5, 0, 40, 40, 1, 0, 0, 0, 0, false, false, false, false, false, false, false, false, false, false, false, false, false, false };
                    gameState = GAME_RUNNING;
                    return;
                }
                mario.isDead = false;
                motion1 = false;
            }
            return;
        }
        if (now - deadStartTime >= 3000)
        {
            gameState = GAME_RUNNING;  // 다시 정상 진행
            resurrection();
        }

        return;
    }

    // 무적처리
    if (mario.god)
    {
        god_mario(godstart);
    }
    // 스타 무적처리
    else if (mario.star)
    {
        star_mario(starStartTime);
    }

    if (mario.coin > 99)
    {
        PlaySoundBuffer(up_Sound);
        mario.life++;
        mario.coin = 0;
    }

    UpdateMario_motion();
    UpdatePlayer();
    UpdateItems();
    UpdateItems_up_mushroom();
    UpdateItems_star();
    UpdateItems_flower();
    UpdateItems_tino();
    UpdateMonsters();
    UpdateTurtles();
    UpdateAngelTurtles();
    UpdateBowser();
    UpdateFireballs();
    UpdateFireTraps();
    UpdateShot_fireball();
    UpdateShot_tinofire();
    UpdateShot_tinofire_effect();

    CheckCollision_mushroom();
    CheckCollision_up_mushroom();
    CheckCollision_star();
    CheckCollision_flower();
    CheckCollision_tino();

    checkcollision_coin();
    CheckMarioMonsterCollision();
    CheckMarioTurtleCollision();
    CheckMarioAngelTurtleCollision();

    CheckMarioHazardCollision();

    CheckMarioBowserCollision();
    CheckMarioFireballCollision();
    CheckCollision_fireball();
    CheckCollision_tinofire();
    checkcollision_flag();
    if(stage == 3) checkcollision_clear();
}
void UpdateMario_motion()
{
    if (mario.fire_motion)
    {
        if (mario.motion_timer > 0)
        {
            mario.motion_timer--;
        }
        else
        {
            mario.fire_motion = false;
        }
    }
    if (mario.tino_motion)
    {
        if (mario.motion_timer > 0)
        {
            mario.motion_timer--;
        }
        else
        {
            mario.tino_motion = false;
        }
    }
    if (mario.tino_fire_motion)
    {
        if (mario.motion_timer > 0)
        {
            mario.motion_timer--;
        }
        else
        {
            mario.tino_fire_motion = false;
        }
    }
}
void UpdatePlayer()
{
    if(gameState != GAME_VICTORY && gameState != GAME_CLEAR)
    {
        movePlayer();
    }
    // 수평 이동
    mario.x += mario.vx;

    // 좌우 충돌 처리
    int left = (mario.x + cameraX) / TILE_SIZE;
    int right = (mario.x + mario.width - 1 + cameraX) / TILE_SIZE;
    int top = mario.y / TILE_SIZE;
    int bottom = (mario.y + mario.height - 1) / TILE_SIZE;
    int middle = (mario.y + mario.height / 2 - 1) / TILE_SIZE;

    // 블럭 왼쪽 벽 충돌
    if (!(mario.y < 0) && mario.vx < 0 &&
        (currentMap[top][left] || currentMap[bottom][left] || currentMap[middle][left]) &&
        !(currentMap[top][left] == 2 || currentMap[bottom][left] == 2 || currentMap[middle][left] == 2) &&
        !(currentMap[top][left] == 7 || currentMap[bottom][left] == 7 || currentMap[middle][left] == 7) &&
        !(currentMap[top][left] == 8 || currentMap[bottom][left] == 8 || currentMap[middle][left] == 8))
    {
        mario.x = left * TILE_SIZE - cameraX + TILE_SIZE;
    }

    // 블럭 오른쪽 벽 충돌
    if (!(mario.y < 0) && mario.vx > 0 &&
        (currentMap[top][right] || currentMap[bottom][right] || currentMap[middle][right]) &&
        !(currentMap[top][right] == 2 || currentMap[bottom][right] == 2 || currentMap[middle][right] == 2) &&
        !(currentMap[top][right] == 7 || currentMap[bottom][right] == 7 || currentMap[middle][right] == 7) &&
        !(currentMap[top][right] == 8 || currentMap[bottom][right] == 8 || currentMap[middle][right] == 8))
    {
        mario.x = right * TILE_SIZE - cameraX - mario.width;
    }

    // 중력 적용
    mario.isflying = true;
    mario.vy += 1;
    if (mario.vy > 15) mario.vy = 15;
    mario.y += mario.vy;

    // y축 충돌 다시 계산
    left = (mario.x + cameraX) / TILE_SIZE;
    right = (mario.x + mario.width - 1 + cameraX) / TILE_SIZE;
    top = mario.y / TILE_SIZE;
    bottom = (mario.y + mario.height - 1) / TILE_SIZE;

    // 아래 충돌
    if (!(mario.y < 0) && mario.vy > 0 && (currentMap[bottom][left] || currentMap[bottom][right])
        && !(currentMap[bottom][left] == 2 || currentMap[bottom][right] == 2)
        && !(currentMap[bottom][left] == 7 || currentMap[bottom][right] == 7)
        && !(currentMap[bottom][left] == 8 || currentMap[bottom][right] == 8))
    {
        if ((currentMap[bottom][left] == 17 || currentMap[bottom][left] == 17) || (currentMap[bottom][left] == 18 || currentMap[bottom][left] == 18))
        {
            dead();
        }
        int centerX = mario.x + TILE_SIZE / 2 + cameraX;
        int blockX = centerX / TILE_SIZE;
        
        mario.y = bottom * TILE_SIZE - mario.height;
        mario.vy = 0;
        mario.isJumping = false;
        mario.isflying = false;
    }
    // 위 충돌
    else if (!(mario.y < 0) && mario.vy < 0 && (currentMap[top][left] || currentMap[top][right])
        && !(currentMap[top][left] == 2 || currentMap[top][right] == 2)
        && !(currentMap[top][left] == 7 || currentMap[top][right] == 7)
        && !(currentMap[top][left] == 8 || currentMap[top][right] == 8))
    {
        // 마리오가 정통으로 친 블럭 찾기
        int centerX = mario.x + TILE_SIZE / 2 + cameraX;
        int blockX = centerX / TILE_SIZE;
        
        mario.y = (top + 1) * TILE_SIZE;
        mario.vy = 0;

        // 아이템 블럭 머리로 치기
        if (currentMap[top][blockX] == 6)
        {
            PlaySoundBuffer(powerup_appears_Sound);
            if (currentMap[top - 1][blockX] == 2)
            {
                PlaySoundBuffer(coin_Sound);
                currentMap[top - 1][blockX] = 0;
                mario.coin++;
            }
            int itemX = blockX * TILE_SIZE;
            int itemY = top * TILE_SIZE;
            currentMap[top][blockX] = 16;
            SpawnItem(itemX, itemY);
        }
        else if (currentMap[top][blockX] == 60)
        {
            PlaySoundBuffer(powerup_appears_Sound);
            if (currentMap[top - 1][blockX] == 2)
            {
                PlaySoundBuffer(coin_Sound);
                currentMap[top - 1][blockX] = 0;
                mario.coin++;
            }
            int itemX = blockX * TILE_SIZE;
            int itemY = top * TILE_SIZE;
            currentMap[top][blockX] = 16;
            SpawnItem_star(itemX, itemY);
        }
        else if (currentMap[top][blockX] == 61)
        {
            PlaySoundBuffer(powerup_appears_Sound);
            if (currentMap[top - 1][blockX] == 2)
            {
                PlaySoundBuffer(coin_Sound);
                currentMap[top - 1][blockX] = 0;
                mario.coin++;
            }
            int itemX = blockX * TILE_SIZE;
            int itemY = top * TILE_SIZE;
            currentMap[top][blockX] = 16;
            SpawnItem_flower(itemX, itemY);
        }
        else if (currentMap[top][blockX] == 62)
        {
            PlaySoundBuffer(powerup_appears_Sound);
            if (currentMap[top - 1][blockX] == 2)
            {
                PlaySoundBuffer(coin_Sound);
                currentMap[top - 1][blockX] = 0;
                mario.coin++;
            }
            int itemX = blockX * TILE_SIZE;
            int itemY = top * TILE_SIZE;
            currentMap[top][blockX] = 16;
            SpawnItem_tino(itemX, itemY);
        }
        else if (currentMap[top][blockX] == 63)
        {
            PlaySoundBuffer(powerup_appears_Sound);
            if (currentMap[top - 1][blockX] == 2)
            {
                PlaySoundBuffer(coin_Sound);
                currentMap[top - 1][blockX] = 0;
                mario.coin++;
            }
            int itemX = blockX * TILE_SIZE;
            int itemY = top * TILE_SIZE;
            currentMap[top][blockX] = 16;
            SpawnItem_up_mushroom(itemX, itemY);
        }
        else if (currentMap[top][blockX] == 64)
        {
            PlaySoundBuffer(coin_Sound);
            if (currentMap[top - 1][blockX] == 2)
            {
                PlaySoundBuffer(coin_Sound);
                currentMap[top - 1][blockX] = 0;
                mario.coin++;
            }
            int itemX = blockX * TILE_SIZE;
            int itemY = top * TILE_SIZE;
            currentMap[top][blockX] = 16;
            mario.coin++;
        }
        else if (currentMap[top][blockX] == 65)
        {
            PlaySoundBuffer(powerup_appears_Sound);
            if (currentMap[top - 1][blockX] == 2)
            {
                PlaySoundBuffer(coin_Sound);
                currentMap[top - 1][blockX] = 0;
                mario.coin++;
            }
            int itemX = blockX * TILE_SIZE;
            int itemY = top * TILE_SIZE;
            currentMap[top][blockX] = 16;
            SpawnItem_star(itemX, itemY);
        }

    }


    // 마리오 왼쪽 벽 충돌
    if (mario.x < 0)
    {
        mario.x = 0;
    }
    // 카메라 이동
    if (mario.x > SCREEN_WIDTH / 2)
    {
        cameraX += mario.vx;
        mario.x = SCREEN_WIDTH / 2;
    }
    if (mario.x < SCREEN_WIDTH / 2 && cameraX > 0)
    {
        cameraX += mario.vx;
        mario.x = SCREEN_WIDTH / 2;
    }

    if (cameraX < 0) cameraX = 0;
    if (cameraX > MAP_WIDTH * TILE_SIZE - SCREEN_WIDTH)
        cameraX = MAP_WIDTH * TILE_SIZE - SCREEN_WIDTH;

    if (mario.y > 800)
    {
        dead();
    }
}
void UpdateDeadMotion()
{
    mario.y += mario.vy;
    mario.vy += 1;
    if (mario.vy > 15) mario.vy = 15;
}

void timegoes()
{
    stage_time--;
    if (stage_time == 0)
    {
        dead();
    }
}

// 깃발 함수
void checkcollision_flag()
{
    int left = (mario.x + cameraX) / TILE_SIZE;
    int right = (mario.x + mario.width - 1 + cameraX) / TILE_SIZE;
    int top = mario.y / TILE_SIZE;
    int bottom = (mario.y + mario.height - 1) / TILE_SIZE;
    int middle = (mario.y + mario.height / 2 - 1) / TILE_SIZE;
    for (int i = 0; i < MAP_HEIGHT; i++)
    {
        for (int j = 0; j < MAP_WIDTH; j++)
        {
            int screenX = j * TILE_SIZE - cameraX;
            int screenY = i * TILE_SIZE;
            if ((currentMap[i][j] == 7 || currentMap[i][j] == 8) && IsColliding_item(mario.x, mario.y, mario.width, mario.height, screenX, screenY, 10, 30))
            {
                g_pBGMBuffer->Stop();
                PlaySoundBuffer(stage_clear_Sound);
                gameState = GAME_VICTORY;
                victoryStart = GetTickCount();
            }

        }
    }
}
// 최종클리어 함수
void checkcollision_clear()
{
    for (int i = 0; i < MAP_HEIGHT; i++)
    {
        for (int j = 0; j < MAP_WIDTH; j++)
        {
            int screenX = j * TILE_SIZE - cameraX;
            int screenY = i * TILE_SIZE;
            if (j == 139 && IsColliding_item(mario.x, mario.y, mario.width, mario.height, screenX, screenY, 40, 40))
            {
                g_pBGMBuffer->Stop();
                PlaySoundBuffer(world_clear_Sound);
                gameState = GAME_CLEAR;
                clearStart = GetTickCount();
            }

        }
    }
}

// 코인먹기 함수
void checkcollision_coin()
{
    for (int i = 0; i < MAP_HEIGHT; i++)
    {
        for (int j = 0; j < MAP_WIDTH; j++)
        {
            int screenX = j * TILE_SIZE - cameraX;
            int screenY = i * TILE_SIZE;
            if (currentMap[i][j] == 2 && IsColliding_item(mario.x, mario.y, mario.width, mario.height, screenX, screenY, 30, 30))
            {
                PlaySoundBuffer(coin_Sound);
                currentMap[i][j] = 0;
                mario.coin++;
            }
        }
    }
}

// 충돌 확인 함수
bool IsColliding(int ax, int ay, int aw, int ah, int bx, int by, int bw, int bh) 
{
    if (mario.god)
    {
        return false;
    }
    return (ax < bx + bw &&
        ax + aw > bx &&
        ay < by + bh &&
        ay + ah > by);
}
bool IsColliding_item(int ax, int ay, int aw, int ah, int bx, int by, int bw, int bh)
{
    return (ax < bx + bw &&
        ax + aw > bx &&
        ay < by + bh &&
        ay + ah > by);
}

void dead()
{
    g_pBGMBuffer->Stop();
    PlaySoundBuffer(die_Sound);
    gameState = GAME_OVER;
    mario.isDead = true;
    mario.isBig = false;
    mario.flower = false;
    mario.tino = false;
    deadStartTime = GetTickCount();

    mario.life--;
    stage_time = 400;

}
void resurrection()
{
    if (mario.direction == 0)
    {
        mario_stop->RotateFlip(RotateNoneFlipX); // 좌우반전
        mario_walk_motion_1->RotateFlip(RotateNoneFlipX); // 좌우반전
        mario_walk_motion_2->RotateFlip(RotateNoneFlipX); // 좌우반전
        mario_walk_motion_3->RotateFlip(RotateNoneFlipX); // 좌우반전
        mario_jump->RotateFlip(RotateNoneFlipX); // 좌우반전

        big_mario_stop->RotateFlip(RotateNoneFlipX); // 좌우반전
        big_mario_walk_motion_1->RotateFlip(RotateNoneFlipX); // 좌우반전
        big_mario_walk_motion_2->RotateFlip(RotateNoneFlipX); // 좌우반전
        big_mario_walk_motion_3->RotateFlip(RotateNoneFlipX); // 좌우반전
        big_mario_jump->RotateFlip(RotateNoneFlipX); // 좌우반전
    }
    item_reset();
    monster_reset();
    stage_load();
    cameraX = 0;
    gameState = GAME_RUNNING;
    mario.god = false;
    mario.star = false;
    mario.x = 100;
    mario.y = 300;
    mario = { 100, 300, 0, 0, mario.life, mario.coin, 40, 40, 1, 0, 0, 0, 0, false, false, false, false, false, false, false, false, false, false, false, false, false, false };    // x, y, vx, xy, life, coin, width, height, direction, walk_motion, motion_timer, cooldown_z, cooldown_space, isJumping, isflying, isWalking, isDead, gameover, isBig, god, star, flower, flower_motion tino, tino_motion, tino_fire_motion
    // x, y, vx, xy, life, coin, width, height, direction, walk_motion, motion_timer, cooldown_z, cooldown_space, isJumping, isflying, isWalking, isDead, gameover, 
    // isBig, god, star, flower, flower_motion, tino, tino_motion, tino_fire_motion, supergod
}

void monster_reset()
{
    for (int i = 0; i < MAX_MONSTERS; i++)
    {
        monsters[i].active = false;
    }
    for (int i = 0; i < MAX_TURTLES; i++)
    {
        turtles[i].active = false;
        angelTurtles[i].isAlive = false;
    }
}
void item_reset()
{
    for (int i = 0; i < MAX_ITEMS; i++) // 아이템 초기화
    {
        mushroom[i].active = false;
        mushroom[i].motion = false;

        up_mushroom[i].active = false;
        up_mushroom[i].motion = false;

        flower[i].active = false;
        flower[i].motion = false;

        tino[i].active = false;
        tino[i].motion = false;

        star[i].active = false;
        star[i].motion = false;
    }
    for (int i = 0; i < MAX_SHOT; i++)
    {
        fireball[i].active = false;
        fireball[i].fade = false;
        tinofire[i].active = false;
        tinofire_effect[i].active = false;
        tinofire[i].fade = false;
    }
}
void stage_load()
{
    SetStage_BGM();
    item_reset();
    trap_reset();
    mario.god = false;
    mario.star = false;

    if (stage == 1)
    {
        cameraX = 0;
        InitMap();           // 맵 초기화
        currentMap = map1;

        InitMonsters();     //몬스터 초기화
        InitTurtles();

        mario.x = 100;
        mario.y = 300;
        stage_time = 400;

    }
    else if (stage == 2)
    {
        monster_reset();
        cameraX = 0;
        InitMap2();  //맵 초기화
        currentMap = map2;

        InitMonsters2();     //몬스터 초기화
        InitTurtles2();
        InitAngelTurtles();

        mario.x = 100;
        mario.y = 300;
        stage_time = 400;
    }
    else if (stage == 3)
    {
        monster_reset();
        cameraX = 0;
        InitMap3();           // 맵 초기화
        currentMap = map3;

        InitFireTraps();
        InitMonsters3();     //몬스터 초기화
        InitTurtles3();
        InitBowser();

        mario.x = 100;
        mario.y = 300;
        stage_time = 400;
    }
}