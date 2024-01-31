#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>

class Character {
public:
    std::string name;
    int health;
    int attack;

    Character(std::string n, int h, int a) : name(n), health(h), attack(a) {}

    bool isAlive() const {
        return health > 0;
    }

    int rollDice() const {
        return rand() % 20 + 1; // Rolling a 20-sided die
    }

    void takeDamage(int damage) {
        health -= damage;
        if (health < 0) {
            health = 0;
        }
    }

    int performAttack() const {
        int diceRoll = rollDice();
        int totalDamage = attack + diceRoll;

        std::cout << name << " rolled a " << diceRoll << ". ";
        return totalDamage;
    }
};

class Enemy : public Character {
public:
    Enemy(std::string n, int h, int a) : Character(n, h, a) {}
};

class Player : public Character {
public:
    std::vector<std::string> inventory;

    Player(std::string n, int h, int a) : Character(n, h, a) {}

    void addItemToInventory(const std::string& item) {
        inventory.push_back(item);
    }

    void printInventory() const {
        std::cout << "Inventory: ";
        if (inventory.empty()) {
            std::cout << "Empty";
        } else {
            for (const auto& item : inventory) {
                std::cout << item << " ";
            }
        }
        std::cout << std::endl;
    }
};

class Maze {
public:
    enum TileType {
        Empty,
        Wall,
        Treasure,
        Enemy
    };

    struct Tile {
        TileType type;
        Character* character; // To store enemy or player occupying the tile
    };

private:
    std::vector<std::vector<Tile>> map;
    int rows;
    int cols;

public:
    Maze(int r, int c) : rows(r), cols(c) {
        map.resize(rows, std::vector<Tile>(cols, {Empty, nullptr}));
    }

    void generateRandomMaze() {
        // Randomly place walls, treasures, and enemies
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                int randomValue = rand() % 100; // Adjust the percentage based on your preference

                if (randomValue < 20) {
                    map[i][j].type = Wall;
                } else if (randomValue < 40) {
                    map[i][j].type = Treasure;
                } else if (randomValue < 60) {
                    map[i][j].type = Enemy;
                    map[i][j].character = new Enemy("Monster", 20, 10); // Adjust enemy attributes as needed
                }
            }
        }
    }

    void printMaze(const Player& player) const {
        // Display the maze with the player's position
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                if (i == playerPosition.first && j == playerPosition.second) {
                    std::cout << "P ";
                } else {
                    switch (map[i][j].type) {
                        case Empty:
                            std::cout << ". ";
                            break;
                        case Wall:
                            std::cout << "# ";
                            break;
                        case Treasure:
                            std::cout << "T ";
                            break;
                        case Enemy:
                            std::cout << "E ";
                            break;
                    }
                }
            }
            std::cout << std::endl;
        }
    }

    std::pair<int, int> getPlayerPosition() const {
        return playerPosition;
    }

    void setPlayerPosition(int x, int y) {
        playerPosition = {x, y};
    }

private:
    std::pair<int, int> playerPosition;
};

void printCharacterStatus(const Character& character) {
    std::cout << character.name << "'s Status - Health: " << character.health << " Attack: " << character.attack << std::endl;
}

void movePlayer(Maze& maze, Player& player, char direction) {
    std::pair<int, int> currentPosition = maze.getPlayerPosition();
    int newRow = currentPosition.first;
    int newCol = currentPosition.second;

    switch (direction) {
        case 'W':
            newRow--;
            break;
        case 'S':
            newRow++;
            break;
        case 'A':
            newCol--;
            break;
        case 'D':
            newCol++;
            break;
        default:
            std::cout << "Invalid move. Try again." << std::endl;
            return;
    }

    if (newRow >= 0 && newRow < maze.getRows() && newCol >= 0 && newCol < maze.getCols()) {
        // Check if the new position is a valid tile (not a wall)
        if (maze.getTileType(newRow, newCol) != Maze::Wall) {
            maze.setPlayerPosition(newRow, newCol);
            processTileEvent(maze, player);
        } else {
            std::cout << "You cannot move through walls. Try again." << std::endl;
        }
    } else {
        std::cout << "Invalid move. Try again." << std::endl;
    }
}

void processTileEvent(Maze& maze, Player& player) {
    int row = maze.getPlayerPosition().first;
    int col = maze.getPlayerPosition().second;

    switch (maze.getTileType(row, col)) {
        case Maze::Empty:
            std::cout << "You move to an empty space." << std::endl;
            break;
        case Maze::Wall:
            std::cout << "You hit a wall. Ouch!" << std::endl;
            break;
        case Maze::Treasure:
            std::cout << "You found a treasure! You gain 10 gold." << std::endl;
            player.addItemToInventory("Gold");
            break;
        case Maze::Enemy:
            std::cout << "You encounter an enemy!" << std::endl;
            startCombat(player, maze.getEnemy(row, col));
            break;
    }
}

void startCombat(Player& player, Enemy& enemy) {
    std::cout << "Combat begins with " << enemy.name << "!" << std::endl;

    while (player.isAlive() && enemy.isAlive()) {
        printCharacterStatus(player);
        printCharacterStatus(enemy);

        std::cout << "\nChoose your action:\n";
        std::cout << "1. Attack\n2. Use Item\n";
        int playerChoice;
        std::cin >> playerChoice;

        switch (playerChoice) {
            case 1:
                // Player's turn to attack
                int playerAttack = player.performAttack();
                enemy.takeDamage(playerAttack);
                std::cout << "You attacked the " << enemy.name << " and dealt " << playerAttack << " damage!" << std::endl;
                break;

            case 2:
                // Player uses an item
                if (!player.inventory.empty()) {
                    std::cout << "Choose an item to use:\n";
                    for (int i = 0; i < player.inventory.size(); ++i) {
                        std::cout << i + 1 << ". " << player.inventory[i] << std::endl;
                    }
                    int itemChoice;
                    std::cin >> itemChoice;

                    if (itemChoice > 0 && itemChoice <= player.inventory.size()) {
                        std::cout << "You used " << player.inventory[itemChoice - 1] << "." << std::endl;
                        player.inventory.erase(player.inventory.begin() + itemChoice - 1);
                        // Implement item effects as needed
                    } else {
                        std::cout << "Invalid item choice." << std::endl;
                    }
                } else {
                    std::cout << "Your inventory is empty. Choose another action." << std::endl;
                }
                break;

            default:
                std::cout << "Invalid choice. Try again." << std::endl;
                break;
        }

        // Check if the enemy is defeated
        if (!enemy.isAlive()) {
            std::cout << "You defeated the " << enemy.name << "!" << std::endl;
            // Add loot or rewards here if desired
            player.addItemToInventory("Gold");
            player.printInventory();
            break;
        }

        // Enemy's turn to attack
        int enemyAttack = enemy.performAttack();
        player.takeDamage(enemyAttack);
        std::cout << "The " << enemy.name << " attacked you and dealt " << enemyAttack << " damage!" << std::endl;

        // Check if the player is defeated
        if (!player.isAlive()) {
            std::cout << "Game over! You were defeated by the " << enemy.name << "." << std::endl;
            break;
        }

        // Pause for readability
        std::cout << "\n--- Next Turn ---\n" << std::endl;
    }
}

int main() {
    srand(time(0)); // Seed for random number generation

    Maze maze(5, 5); // Adjust the maze dimensions based on your preference
    maze.generateRandomMaze();

    Player player("Adventurer", 100, 15);
    maze.setPlayerPosition(0, 0);

    std::cout << "Welcome to the Enhanced Text-Based D&D Adventure!" << std::endl;

    while (player.isAlive()) {
        maze.printMaze(player);

        std::cout << "Choose your move (W/A/S/D): ";
        char move;
        std::cin >> move;

        movePlayer(maze, player, move);

        // Check if player reached the end of the maze
        if (maze.getPlayerPosition() == std::make_pair(maze.getRows() - 1, maze.getCols() - 1)) {
            std::cout << "Congratulations! You completed the maze!" << std::endl;
            break;
        }
    }

    return 0;
}
