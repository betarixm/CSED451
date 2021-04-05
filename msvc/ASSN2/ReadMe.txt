Game.h/cpp
- 게임 적/플레이어 생성 및 게임 승패 체크등 슈팅게임 class 정의
Shape.h/cpp
- Triangle/Square 기본 modelframe class 정의
Ship.h/cpp
- Player/Enemy class 정의
Node.h/cpp, Tree.h/cpp
- Scene Graph 구현에 필요한 Node들 정의
Stellar.h/cpp
- 항성계 정의
main.cpp
- main 함수와 timer, keyboard, display등 openGL callback 함수 정의

keyboard 조작법
1. 상하좌우 입력키 : 상하좌우 입력키를 이용하여 player 우주선 상하좌우로 이동 가능
(단, 윈도우 밖으로 나갈 수 없음)
2. 스페이스바 : 스페이스바를 눌러 총알 발사 가능
* 멀티키 입력을 지원하여 움직임과 동시에 총알 발사 가능.
