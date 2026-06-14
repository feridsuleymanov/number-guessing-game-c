# 🎯 Number Guessing Game in C

A terminal-based number guessing game written in C, featuring multiple difficulty levels, a smart hint system, and an in-session leaderboard.

## Features

- **3 Difficulty Levels** — Easy (1–50), Medium (1–100), Hard (1–200)
- **Smart Hint System** — tells you how far away you are and which direction to go
- **Leaderboard** — tracks the top 5 wins per session, sorted by fewest attempts
- **Input Validation** — handles invalid input gracefully without crashing
- **Clean UI** — menu-driven interface with ASCII borders

## How to Compile & Run

### Using GCC directly
```bash
gcc -Wall -Wextra -std=c99 -o guessing_game main.c
./guessing_game
```

### Using Make
```bash
make        # compile
make run    # compile and run
make clean  # remove binary
```

## How to Play

1. Enter your name
2. Choose **Play Game** from the main menu
3. Select a difficulty level
4. Start guessing — the game gives you hints after each wrong guess
5. Win by guessing the correct number within the allowed attempts
6. Check the **Leaderboard** to compare your score

## Project Structure

```
number_guessing_game/
├── main.c      # Full source code
├── Makefile    # Build configuration
└── README.md   # This file
```

## Requirements

- GCC (or any C99-compatible compiler)
- Linux / macOS / Windows (with MinGW)

## License

This project is open source and available under the [MIT License](https://opensource.org/licenses/MIT).
