#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

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

void printCharacterStatus(const Character& character) {
    std::cout << character.name << "'s Status - Health: " << character.health << " Attack: " << character.attack << std::endl;
}

int main() {
    srand(time(0)); // Seed for random number generation

    Player player("Adventurer", 100, 15);
    std::vector<Enemy> enemies = {{"Dragon", 50, 10}, {"Orc", 30, 8}, {"Goblin", 20, 5}};

    std::cout << "Welcome to the Enhanced Text-Based D&D Adventure!" << std::endl;

    for (const auto& enemy : enemies) {
        std::cout << "You encounter a " << enemy.name << "!" << std::endl;

        while (player.isAlive() && enemy.isAlive()) {
            printCharacterStatus(player);
            printCharacterStatus(enemy);
            player.printInventory();

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

    std::cout << "Congratulations! You completed the D&D Adventure!" << std::endl;

    return 0;
}
