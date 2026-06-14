

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

/* ---- Constants ---- */
#define MAX_NAME_LEN 50
#define MAX_SCORES   5

/* ---- Difficulty Settings ---- */
typedef struct {
    const char *name;
    int         max_number;
    int         max_attempts;
} Difficulty;

static const Difficulty LEVELS[] = {
    { "Easy",   50,  10 },
    { "Medium", 100,  7 },
    { "Hard",   200,  5 }
};

/* ---- Score Record ---- */
typedef struct {
    char name[MAX_NAME_LEN];
    int  attempts_used;
    int  level;        /* 0=Easy, 1=Medium, 2=Hard */
} ScoreRecord;

/* ---- Globals ---- */
ScoreRecord leaderboard[MAX_SCORES];
int         score_count = 0;

/* ---- Function Prototypes ---- */
void        clear_screen(void);
void        print_banner(void);
int         choose_difficulty(void);
void        give_hint(int guess, int secret, int max_num);
int         play_round(const char *player, int level);
void        add_score(const char *player, int attempts, int level);
void        show_leaderboard(void);
void        play_again_menu(void);

/* ============================================================ */

void clear_screen(void) {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void print_banner(void) {
    printf("\n");
    printf("  ╔══════════════════════════════════════╗\n");
    printf("  ║       NUMBER  GUESSING  GAME         ║\n");
    printf("  ║           Written in C               ║\n");
    printf("  ╚══════════════════════════════════════╝\n");
    printf("\n");
}

int choose_difficulty(void) {
    int choice;
    printf("  Select Difficulty:\n");
    printf("  [1] Easy   - Guess 1-%d  (%d attempts)\n",
           LEVELS[0].max_number, LEVELS[0].max_attempts);
    printf("  [2] Medium - Guess 1-%d (%d attempts)\n",
           LEVELS[1].max_number, LEVELS[1].max_attempts);
    printf("  [3] Hard   - Guess 1-%d (%d attempts)\n",
           LEVELS[2].max_number, LEVELS[2].max_attempts);
    printf("\n  Your choice: ");

    if (scanf("%d", &choice) != 1 || choice < 1 || choice > 3) {
        printf("  Invalid choice. Defaulting to Medium.\n");
        choice = 2;
    }
    return choice - 1;   /* 0-indexed */
}

void give_hint(int guess, int secret, int max_num) {
    int diff = secret - guess;
    if (diff < 0) diff = -diff;   /* abs value */

    /* Distance hint */
    if (diff > max_num / 2)
        printf("  Hint: You are FAR away!\n");
    else if (diff > max_num / 5)
        printf("  Hint: Getting warmer...\n");
    else
        printf("  Hint: Very CLOSE! Almost there!\n");

    /* Direction hint */
    if (guess < secret)
        printf("  Direction: Go HIGHER ↑\n");
    else
        printf("  Direction: Go LOWER  ↓\n");
}

int play_round(const char *player, int level) {
    int secret    = (rand() % LEVELS[level].max_number) + 1;
    int max_tries = LEVELS[level].max_attempts;
    int guess, attempt;

    printf("\n  I have picked a number between 1 and %d.\n",
           LEVELS[level].max_number);
    printf("  You have %d attempts. Good luck, %s!\n\n",
           max_tries, player);

    for (attempt = 1; attempt <= max_tries; attempt++) {
        printf("  [Attempt %d/%d] Enter your guess: ", attempt, max_tries);

        if (scanf("%d", &guess) != 1) {
            printf("  Please enter a valid number.\n");
            /* flush bad input */
            int c; while ((c = getchar()) != '\n' && c != EOF);
            attempt--;
            continue;
        }

        if (guess < 1 || guess > LEVELS[level].max_number) {
            printf("  Out of range! Guess between 1 and %d.\n",
                   LEVELS[level].max_number);
            attempt--;
            continue;
        }

        if (guess == secret) {
            printf("\n  *** Correct! You got it in %d attempt(s)! ***\n\n",
                   attempt);
            add_score(player, attempt, level);
            return attempt;
        }

        give_hint(guess, secret, LEVELS[level].max_number);

        if (attempt == max_tries)
            printf("\n  Sorry %s, you ran out of attempts.\n"
                   "  The number was: %d\n\n", player, secret);
        else
            printf("  Attempts remaining: %d\n\n", max_tries - attempt);
    }
    return -1;   /* player lost */
}

void add_score(const char *player, int attempts, int level) {
    if (score_count >= MAX_SCORES) {
        /* Replace worst (highest attempts) score */
        int worst = 0;
        for (int i = 1; i < MAX_SCORES; i++)
            if (leaderboard[i].attempts_used > leaderboard[worst].attempts_used)
                worst = i;
        if (attempts < leaderboard[worst].attempts_used) {
            strncpy(leaderboard[worst].name, player, MAX_NAME_LEN - 1);
            leaderboard[worst].attempts_used = attempts;
            leaderboard[worst].level         = level;
        }
    } else {
        strncpy(leaderboard[score_count].name, player, MAX_NAME_LEN - 1);
        leaderboard[score_count].attempts_used = attempts;
        leaderboard[score_count].level         = level;
        score_count++;
    }
}

void show_leaderboard(void) {
    if (score_count == 0) {
        printf("  No scores yet. Play a round first!\n");
        return;
    }

    printf("\n  ╔═══════════════════════════════════════╗\n");
    printf("  ║           LEADERBOARD (Top Wins)      ║\n");
    printf("  ╠═══════════════════════════════════════╣\n");
    printf("  ║  %-15s %-10s %-10s  ║\n", "Player", "Attempts", "Level");
    printf("  ╠═══════════════════════════════════════╣\n");

    /* Simple sort: bubble sort by attempts ascending */
    ScoreRecord temp;
    for (int i = 0; i < score_count - 1; i++) {
        for (int j = 0; j < score_count - i - 1; j++) {
            if (leaderboard[j].attempts_used > leaderboard[j+1].attempts_used) {
                temp              = leaderboard[j];
                leaderboard[j]   = leaderboard[j+1];
                leaderboard[j+1] = temp;
            }
        }
    }

    for (int i = 0; i < score_count; i++) {
        printf("  ║  %-15s %-10d %-10s  ║\n",
               leaderboard[i].name,
               leaderboard[i].attempts_used,
               LEVELS[leaderboard[i].level].name);
    }
    printf("  ╚═══════════════════════════════════════╝\n\n");
}

/* ============================================================ */

int main(void) {
    srand((unsigned int)time(NULL));

    char player[MAX_NAME_LEN];
    int  choice;

    clear_screen();
    print_banner();

    printf("  Enter your name: ");
    if (scanf("%49s", player) != 1)
        strcpy(player, "Player");

    while (1) {
        clear_screen();
        print_banner();
        printf("  Welcome, %s!\n\n", player);
        printf("  MAIN MENU\n");
        printf("  ─────────────────────────\n");
        printf("  [1] Play Game\n");
        printf("  [2] View Leaderboard\n");
        printf("  [3] Change Player Name\n");
        printf("  [4] Quit\n");
        printf("\n  Choice: ");

        if (scanf("%d", &choice) != 1) {
            int c; while ((c = getchar()) != '\n' && c != EOF);
            continue;
        }

        switch (choice) {
            case 1: {
                int level = choose_difficulty();
                play_round(player, level);
                printf("  Press ENTER to return to menu...");
                int c; while ((c = getchar()) != '\n' && c != EOF);
                getchar();
                break;
            }
            case 2:
                show_leaderboard();
                printf("  Press ENTER to continue...");
                int c; while ((c = getchar()) != '\n' && c != EOF);
                getchar();
                break;
            case 3:
                printf("  Enter new name: ");
                scanf("%49s", player);
                break;
            case 4:
                printf("\n  Thanks for playing, %s! Goodbye!\n\n", player);
                return 0;
            default:
                printf("  Invalid option. Try again.\n");
                break;
        }
    }

    return 0;
}
