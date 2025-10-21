#include <stdio.h>
#include <string.h>

#define MAX_CITIES 30
#define NAME_LENGTH 50

char cities[MAX_CITIES][NAME_LENGTH];
int cityCount = 0;
void addCity();
void renameCity();
void removeCity();
int findCityIndex(char name[]);

// ---- MAIN FUNCTION ----
int main() {
    int choice;
    do {
        printf("\n=== City Management Menu ===\n");
        printf("1. Add New City\n");
        printf("2. Rename City\n");
        printf("3. Remove City\n");
        printf("4 Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // consume newline left by scanf

        switch (choice) {
            case 1: addCity(); break;
            case 2: renameCity(); break;
            case 3: removeCity(); break;
            case 4: printf("Exiting City Management...\n"); break;
            default: printf("Invalid choice! Please try again.\n");
        }
    } while (choice != 4);

    return 0;
}


// Add a new city
void addCity() {
    if (cityCount >= MAX_CITIES) {
        printf("City limit reached! Cannot add more cities.\n");
        return;
    }

    char name[NAME_LENGTH];
    printf("Enter new city name: ");
    scanf("%s",&name);
    name[strcspn(name, "\n")] = '\0'; // remove newline

    if (findCityIndex(name) != -1) {
        printf("City '%s' already exists!\n", name);
        return;
    }

    strcpy(cities[cityCount], name);
    cityCount++;
    printf("City '%s' added successfully!\n", name);
}

// Rename a city
void renameCity() {
    char oldName[NAME_LENGTH], newName[NAME_LENGTH];
    printf("Enter the name of the city to rename: ");
    scanf("%s",&oldName);
    oldName[strcspn(oldName, "\n")] = '\0';

    int index = findCityIndex(oldName);
    if (index == -1) {
        printf("City '%s' not found!\n", oldName);
        return;
    }

    printf("Enter the new name: ");
    scanf("%s",&newName);
    newName[strcspn(newName, "\n")] = '\0';

    if (findCityIndex(newName) != -1) {
        printf("A city with the name '%s' already exists!\n", newName);
        return;
    }

    strcpy(cities[index], newName);
    printf("City renamed successfully!\n");
}

// Remove a city
void removeCity() {
    char name[NAME_LENGTH];
    printf("Enter the name of the city to remove: ");
    scanf("%s",&name);
    name[strcspn(name, "\n")] = '\0';

    int index = findCityIndex(name);
    if (index == -1) {
        printf("City '%s' not found!\n", name);
        return;
    }

    for (int i = index; i < cityCount - 1; i++) {
        strcpy(cities[i], cities[i + 1]);
    }
    cityCount--;
    printf("City '%s' removed successfully!\n", name);
}

// Find city index by name
int findCityIndex(char name[]) {
    for (int i = 0; i < cityCount; i++) {
        if (strcmp(cities[i], name) == 0)
            return i;
    }
    return -1;
}
