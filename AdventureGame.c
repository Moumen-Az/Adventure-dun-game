#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INVENTORY 5
#define MAX_ROOMS 10
#define MAX_DESC 256
#define MAX_COMMAND 100

typedef struct Item {
    char name[MAX_DESC];
} Item;

typedef struct Creature {
    char name[MAX_DESC];
    int health;
} Creature;

typedef struct Room {
    char description[MAX_DESC];
    struct Room *up, *down, *left, *right;
    Item *item;
    Creature *creature;
} Room;

typedef struct Player {
    int health;
    int strength;
    int inventory_count;
    Item *inventory[MAX_INVENTORY];
    Room *current_room;
} Player;

void print_welcome() {
    printf("Welcome to the Dungeon Adventure Game!\n");
    printf("Commands: move <direction>, look, inventory, pickup <item>, attack, save <filepath>, load <filepath>, exit\n");
}

void look(Room *room) {
    printf("%s\n", room->description);
    if (room->item) {
        printf("You see a %s here.\n", room->item->name);
    }
    if (room->creature) {
        printf("A %s is here with %d health.\n", room->creature->name, room->creature->health);
    }
}

void move(Player *player, const char *direction) {
    Room *new_room = NULL;
    if (strcmp(direction, "up") == 0) new_room = player->current_room->up;
    else if (strcmp(direction, "down") == 0) new_room = player->current_room->down;
    else if (strcmp(direction, "left") == 0) new_room = player->current_room->left;
    else if (strcmp(direction, "right") == 0) new_room = player->current_room->right;

    if (new_room) {
        player->current_room = new_room;
        printf("You moved %s.\n", direction);
    } else {
        printf("You can't move %s.\n", direction);
    }
}

void inventory(Player *player) {
    printf("Your inventory: \n");
    for (int i = 0; i < player->inventory_count; i++) {
        printf("- %s\n", player->inventory[i]->name);
    }
    if (player->inventory_count == 0) {
        printf("(empty)\n");
    }
}

void pickup(Player *player) {
    if (player->inventory_count >= MAX_INVENTORY) {
        printf("Your inventory is full!\n");
        return;
    }
    Item *item = player->current_room->item;
    if (item) {
        player->inventory[player->inventory_count++] = item;
        player->current_room->item = NULL;
        printf("You picked up %s.\n", item->name);
    } else {
        printf("There's nothing to pick up here.\n");
    }
}

void attack(Player *player) {
    Creature *creature = player->current_room->creature;
    if (!creature) {
        printf("There's nothing to attack here.\n");
        return;
    }

    printf("You attack the %s!\n", creature->name);
    creature->health -= player->strength;
    if (creature->health <= 0) {
        printf("You defeated the %s!\n", creature->name);
        free(creature);
        player->current_room->creature = NULL;
    } else {
        printf("The %s attacks you back!\n", creature->name);
        player->health -= 10;
        if (player->health <= 0) {
            printf("You have been defeated. Game over.\n");
            exit(0);
        }
    }
}

void save_game(Player *player, const char *filepath) {
    FILE *file = fopen(filepath, "w");
    if (!file) {
        printf("Failed to save the game.\n");
        return;
    }

    fprintf(file, "%d %d\n", player->health, player->strength);
    fprintf(file, "%s\n", player->current_room->description);
    fclose(file);
    printf("Game saved to %s.\n", filepath);
}

void load_game(Player *player, Room *rooms, const char *filepath) {
    FILE *file = fopen(filepath, "r");
    if (!file) {
        printf("Failed to load the game.\n");
        return;
    }

    char room_desc[MAX_DESC];
    fscanf(file, "%d %d\n", &player->health, &player->strength);
    fgets(room_desc, MAX_DESC, file);
    for (int i = 0; i < MAX_ROOMS; i++) {
        if (strcmp(rooms[i].description, room_desc) == 0) {
            player->current_room = &rooms[i];
            break;
        }
    }
    fclose(file);
    printf("Game loaded from %s.\n", filepath);
}

void initialize_rooms(Room *rooms) {
    
    strcpy(rooms[0].description, "You are in a dark room with a single torch on the wall.");
    strcpy(rooms[1].description, "You are in a bright hallway with paintings on the walls.");
    strcpy(rooms[2].description, "You are in a damp cave with water dripping from the ceiling.");
    strcpy(rooms[3].description, "You are in a large hall with a high ceiling and ancient carvings.");

    
    rooms[0].right = &rooms[1];
    rooms[1].left = &rooms[0];
    rooms[1].down = &rooms[2];
    rooms[2].up = &rooms[1];
    rooms[2].right = &rooms[3];
    rooms[3].left = &rooms[2];

    
    rooms[0].item = malloc(sizeof(Item));
    strcpy(rooms[0].item->name, "Rusty Sword");

    rooms[2].creature = malloc(sizeof(Creature));
    strcpy(rooms[2].creature->name, "Goblin");
    rooms[2].creature->health = 30;
}

int main() {
    Room rooms[MAX_ROOMS] = {0};
    Player player = {100, 20, 0, {NULL}, &rooms[0]};

    initialize_rooms(rooms);

    print_welcome();
    char command[MAX_COMMAND];

    while (1) {
        printf("> ");
        fgets(command, MAX_COMMAND, stdin);
        command[strcspn(command, "\n")] = 0;

        if (strncmp(command, "move ", 5) == 0) {
            move(&player, command + 5);
        } else if (strcmp(command, "look") == 0) {
            look(player.current_room);
        } else if (strcmp(command, "inventory") == 0) {
            inventory(&player);
        } else if (strncmp(command, "pickup", 6) == 0) {
            pickup(&player);
        } else if (strcmp(command, "attack") == 0) {
            attack(&player);
        } else if (strncmp(command, "save ", 5) == 0) {
            save_game(&player, command + 5);
        } else if (strncmp(command, "load ", 5) == 0) {
            load_game(&player, rooms, command + 5);
        } else if (strcmp(command, "exit") == 0) {
            printf("Exiting the game. Goodbye!\n");
            break;
        } else {
            printf("Unknown command.\n");
        }
    }

    return 0;
}