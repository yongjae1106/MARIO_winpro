// err_quit와 err_display 함수 다중 정의 방지를 위해 헤더 파일로 분리

#define _CRT_SECURE_NO_WARNINGS // 구형 C 함수 사용 시 경고 끄기
#define _WINSOCK_DEPRECATED_NO_WARNINGS // 구형 소켓 API 사용 시 경고 끄기

#include <winsock2.h> // 윈속2 메인 헤더
#include <ws2tcpip.h> // 윈속2 확장 헤더

#include <tchar.h> // _T(), ...
#include <stdio.h> // printf(), ...
#include <stdlib.h> // exit(), ...
#include <string.h> // strncpy(), ...
#include <windows.h> // FormatMessageA, MessageBoxA 등 WinAPI 함수 사용을 위해 필요

#pragma comment(lib, "ws2_32") // ws2_32.lib 링크

// 소켓 함수 오류 출력 후 종료 (선언만 남김)
void err_quit(const char* msg);

// 소켓 함수 오류 출력 (선언만 남김)
void err_display(const char* msg);
void err_display(int errcode); // 오버로드된 함수도 선언