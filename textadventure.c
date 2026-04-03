#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ITEMS 20
#define MAX_NAME_LEN 50

typedef struct {
    char name[MAX_NAME_LEN];
    int collected;
} Item;

typedef struct {
    char name[MAX_NAME_LEN];
    char description[200];
    Item items[MAX_ITEMS];
    int itemCount;
    int locked;   
    char requiredKey[MAX_NAME_LEN];
} Room;

typedef struct {
    int health;
    int strength;
    int magic;
} Player;

void showMainMenu();
void showRooms(Room rooms[], int count, Item inventory[], int *inventoryCount, Player *player);
void exploreRoom(Room *room, Item inventory[], int *inventoryCount, Player *player);
void showInventory(Item inventory[], int count);
void showStats(Player *player);
void clearInput();

int main() {
    Item inventory[MAX_ITEMS];
    int inventoryCount = 0;
    Player player = {100, 10, 5}; 

    Room rooms[3];

    strcpy(rooms[0].name, "Ancient Library");
    strcpy(rooms[0].description, "Dusty shelves filled with forgotten books.");
    rooms[0].itemCount = 2;
    strcpy(rooms[0].items[0].name, "Magic Scroll");
    rooms[0].items[0].collected = 0;
    strcpy(rooms[0].items[1].name, "Rusty Key");
    rooms[0].items[1].collected = 0;
    rooms[0].locked = 0;

    strcpy(rooms[1].name, "Hidden Cave");
    strcpy(rooms[1].description, "Dark and damp, echoes of dripping water.");
    rooms[1].itemCount = 2;
    strcpy(rooms[1].items[0].name, "Crystal Shard");
    rooms[1].items[0].collected = 0;
    strcpy(rooms[1].items[1].name, "Torch");
    rooms[1].items[1].collected = 0;
    rooms[1].locked = 1;
    strcpy(rooms[1].requiredKey, "Rusty Key");

    strcpy(rooms[2].name, "Mystic Garden");
    strcpy(rooms[2].description, "Glowing plants and a strange aura.");
    rooms[2].itemCount = 2;
    strcpy(rooms[2].items[0].name, "Golden Apple");
    rooms[2].items[0].collected = 0;
    strcpy(rooms[2].items[1].name, "Silver Feather");
    rooms[2].items[1].collected = 0;
    rooms[2].locked = 0;

    int choice;
    int running = 1;

    while (running) {
        showMainMenu();
        printf("Enter choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input.\n");
            clearInput();
            continue;
        }

        switch (choice) {
            case 1:
                showRooms(rooms, 3, inventory, &inventoryCount, &player);
                break;
            case 2:
                showInventory(inventory, inventoryCount);
                break;
            case 3:
                showStats(&player);
                break;
            case 4:
                printf("\nThanks for playing! Goodbye.\n");
                running = 0;
                break;
            default:
                printf("Invalid choice.\n");
        }
    }

    return 0;
}

void showMainMenu() {
    printf("\n============================\n");
    printf("       Adventure Game \n");
    printf("============================\n");
    printf("1. Explore Rooms\n");
    printf("2. View Inventory\n");
    printf("3. View Player Stats\n");
    printf("4. Exit\n");
    printf("============================\n");
}

void showRooms(Room rooms[], int count, Item inventory[], int *inventoryCount, Player *player) {
    int choice;

    while (1) {
        printf("\nChoose a room:\n");
        for (int i = 0; i < count; i++) {
            printf("%d. %s %s\n", i + 1, rooms[i].name, rooms[i].locked ? "(Locked)" : "");
        }
        printf("0. Back\n");
        printf("Enter choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input.\n");
            clearInput();
            continue;
        }

        if (choice == 0) return;

        if (choice < 1 || choice > count) {
            printf("Invalid room.\n");
            continue;
        }

        Room *room = &rooms[choice - 1];

        if (room->locked) {
            int hasKey = 0;
            for (int i = 0; i < *inventoryCount; i++) {
                if (strcmp(inventory[i].name, room->requiredKey) == 0) {
                    hasKey = 1;
                    break;
                }
            }
            if (!hasKey) {
                printf("The room is locked! You need the %s.\n", room->requiredKey);
                continue;
            } else {
                printf("You unlock the room with the %s!\n", room->requiredKey);
                room->locked = 0;
            }
        }

        exploreRoom(room, inventory, inventoryCount, player);
    }
}

void exploreRoom(Room *room, Item inventory[], int *inventoryCount, Player *player) {
    int choice;

    while (1) {
        printf("\n=== %s ===\n", room->name);
        printf("%s\n", room->description);

        printf("\nItems available:\n");

        int hasItems = 0;
        for (int i = 0; i < room->itemCount; i++) {
            if (!room->items[i].collected) {
                printf("%d. %s\n", i + 1, room->items[i].name);
                hasItems = 1;
            }
        }

        if (!hasItems) {
            printf("No items left in this room.\n");
            return;
        }

        printf("0. Back\n");
        printf("Choose item to collect: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input.\n");
            clearInput();
            continue;
        }

        if (choice == 0) return;

        if (choice < 1 || choice > room->itemCount) {
            printf("Invalid item.\n");
            continue;
        }

        if (room->items[choice - 1].collected) {
            printf("Item already collected.\n");
            continue;
        }

        if (*inventoryCount >= MAX_ITEMS) {
            printf("Inventory full!\n");
            return;
        }

        room->items[choice - 1].collected = 1;
        inventory[*inventoryCount] = room->items[choice - 1];
        (*inventoryCount)++;

        printf("Collected: %s\n", room->items[choice - 1].name);

        
        if (strcmp(room->items[choice - 1].name, "Golden Apple") == 0) {
            player->health += 20;
            printf("The Golden Apple restores your health! (+20)\n");
        } else if (strcmp(room->items[choice - 1].name, "Magic Scroll") == 0) {
            player->magic += 10;
            printf("The Magic Scroll increases your magic! (+10)\n");
        }
    }
}

void showInventory(Item inventory[], int count) {
    printf("\n=== Inventory ===\n");

    if (count == 0) {
        printf("Your inventory is empty.\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        printf("%d. %s\n", i + 1, inventory[i].name);
    }
}

void showStats(Player *player) {
    printf("\n=== Player Stats ===\n");
    printf("Health: %d\n", player->health);
    printf("Strength: %d\n", player->strength);
    printf("Magic: %d\n", player->magic);
}

void clearInput() {
    while (getchar() != '\n');
}