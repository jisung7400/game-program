#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct Character
{
    char name[32];
    int hp;
    int maxHp;
    int atk;
    int def;
    int potion;   // 포션 개수
    int guarding; // 이번 턴 방어중인지 (1이면 방어)
} Character;

int ClampMin(int value, int min)
{
    if (value < min) return min;
    return value;
}

int CalcDamage(int atk, int def, int targetGuarding)
{
    // 방어중이면 방어력 추가 효과
    int effectiveDef = def;
    if (targetGuarding) effectiveDef += 3; // 방어 보너스 (원하면 조절)

    int dmg = atk - effectiveDef;
    dmg = ClampMin(dmg, 1); // 최소 1은 들어가게
    return dmg;
}

void PrintStatus(Character* p, Character* m)
{
    printf("\n=============================\n");
    printf(" [플레이어] %s  HP: %d/%d  포션:%d\n", p->name, p->hp, p->maxHp, p->potion);
    printf(" [몬스터 ] %s  HP: %d/%d\n", m->name, m->hp, m->maxHp);
    printf("=============================\n");
}

int PlayerTurn(Character* player, Character* monster)
{
    // 턴 시작 시 방어 상태 초기화
    player->guarding = 0;

    printf("\n플레이어의 턴!\n");
    printf("1) 공격\n");
    printf("2) 방어(이번 턴 피해 감소)\n");
    printf("3) 포션 사용(+10 회복)\n");
    printf("4) 도망(50%% 확률)\n");
    printf("선택: ");

    int choice;
    scanf("%d", &choice);

    if (choice == 1)
    {
        int dmg = CalcDamage(player->atk, monster->def, monster->guarding);
        monster->hp -= dmg;
        if (monster->hp < 0) monster->hp = 0;

        printf("%s의 공격! %s에게 %d 피해!\n", player->name, monster->name, dmg);
    }
    else if (choice == 2)
    {
        player->guarding = 1;
        printf("%s은(는) 방어 자세를 취했다!\n", player->name);
    }
    else if (choice == 3)
    {
        if (player->potion <= 0)
        {
            printf("포션이 없다! 턴을 낭비했다...\n");
        }
        else
        {
            player->potion--;
            player->hp += 10;
            if (player->hp > player->maxHp) player->hp = player->maxHp;
            printf("포션 사용! HP가 회복됐다. (현재 HP: %d/%d)\n", player->hp, player->maxHp);
        }
    }
    else if (choice == 4)
    {
        int r = rand() % 100; // 0~99
        if (r < 50)
        {
            printf("도망 성공!\n");
            return 1; // 전투 종료(도망)
        }
        else
        {
            printf("도망 실패!\n");
        }
    }
    else
    {
        printf("잘못된 선택! 턴을 낭비했다...\n");
    }

    return 0; // 계속 전투
}

int MonsterTurn(Character* monster, Character* player)
{
    // 몬스터도 방어 상태 초기화
    monster->guarding = 0;

    printf("\n몬스터의 턴!\n");

    // 간단 AI: 80% 공격, 20% 방어
    int ai = rand() % 100;

    if (ai < 80)
    {
        int dmg = CalcDamage(monster->atk, player->def, player->guarding);
        player->hp -= dmg;
        if (player->hp < 0) player->hp = 0;

        printf("%s의 공격! %s에게 %d 피해!\n", monster->name, player->name, dmg);
    }
    else
    {
        monster->guarding = 1;
        printf("%s은(는) 방어 자세를 취했다!\n", monster->name);
    }

    return 0;
}

int main()
{
    srand((unsigned int)time(NULL));

    Character player = { "용사", 30, 30, 8, 2, 2, 0 };
    Character monster = { "슬라임", 20, 20, 6, 1, 0, 0 };

    printf("턴제 전투 시작!\n");

    while (1)
    {
        PrintStatus(&player, &monster);

        // 플레이어 턴
        int escaped = PlayerTurn(&player, &monster);
        if (escaped) break;

        if (monster.hp <= 0)
        {
            printf("\n%s을(를) 처치했다! 승리!\n", monster.name);
            break;
        }

        // 몬스터 턴
        MonsterTurn(&monster, &player);

        if (player.hp <= 0)
        {
            printf("\n%s이(가) 쓰러졌다... 패배!\n", player.name);
            break;
        }
    }
    
    printf("\n전투 종료.\n");
    return 0;
}
