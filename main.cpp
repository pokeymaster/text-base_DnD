#include <iostream>
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

    void takeDamage(int damage) {
        health -= damage;
        if (health < 0) {
            health = 0;
        }
    }

    int performAttack() const {
        return attack + (rand() % 5); // Add a random factor to attack
    }
};

class Enemy : public Character {
public:
    Enemy(std::string n, int h, int a) : Character(n, h, a) {}
};

void printCharacterStatus(const Character& character) {
    std::cout << character.name << "'s Status - Health: " << character.health << " Attack: " << character.attack << std::endl;
}

int main() {
    srand(time(0)); // Seed for random number generation

    Character player("Adventurer", 100, 15);
    Enemy dragon("Dragon", 50, 10);

    std::cout << "Welcome to the Text-Based D&D Adventure!" << std::endl;

    while (player.isAlive() && dragon.isAlive()) {
        printCharacterStatus(player);
        printCharacterStatus(dragon);

        // Player's turn
        int playerAttack = player.performAttack();
        dragon.takeDamage(playerAttack);
        std::cout << "You attacked the Dragon and dealt " << playerAttack << " damage!" << std::endl;

        // Check if the dragon is defeated
        if (!dragon.isAlive()) {
            std::cout << "Congratulations! You defeated the Dragon!" << std::endl;
            break;
        }

        // Dragon's turn
        int dragonAttack = dragon.performAttack();
        player.takeDamage(dragonAttack);
        std::cout << "The Dragon attacked you and dealt " << dragonAttack << " damage!" << std::endl;

        // Check if the player is defeated
        if (!player.isAlive()) {
            std::cout << "Game over! The Dragon defeated you." << std::endl;
            break;
        }

        // Pause for readability
        std::cout << "\n--- Next Turn ---\n" << std::endl;
    }

    return 0;
}
