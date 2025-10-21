#include <stdio.h>
#include <string.h>

#define MAX_CITIES 30
#define NAME_LENGTH 50

char cities[MAX_CITIES][NAME_LENGTH];
int cityCount = 0;
int distanceMatrix[MAX_CITIES][MAX_CITIES];

void listCities();
int findCityIndex(char name[]);
void inputOrEditDistance();
void displayDistanceTable();
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
        printf("4. Input/Edit Distance\n");
        printf("5. Display Distance Table\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addCity(); break;
            case 2: renameCity(); break;
            case 3: removeCity(); break;
            case 4: inputOrEditDistance(); break;
            case 5: displayDistanceTable(); break;
            case 6: printf("Exiting City Management...\n"); break;
            default: printf("Invalid choice! Please try again.\n");
        }
    } while (choice != 6);

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
// Input or Edit Distance between two cities
void inputOrEditDistance() {
    if (cityCount < 2) {
        printf("At least two cities are required to set distances.\n");
        return;
    }

    char city1[NAME_LENGTH], city2[NAME_LENGTH];
    printf("Enter first city name: ");
    scanf("%s",&city1);
    city1[strcspn(city1, "\n")] = '\0';

    printf("Enter second city name: ");
    scanf("%s",&city2);
    city2[strcspn(city2, "\n")] = '\0';

    int index1 = findCityIndex(city1);
    int index2 = findCityIndex(city2);

    if (index1 == -1 || index2 == -1) {
        printf("One or both cities not found!\n");
        return;
    }

    if (index1 == index2) {
        printf("Distance from a city to itself must be 0.\n");
        distanceMatrix[index1][index2] = 0;
        return;
    }

    int dist;
    printf("Enter distance between %s and %s (in km): ", city1, city2);
    scanf("%d", &dist);
    getchar();

    if (dist < 0) {
        printf("Distance cannot be negative.\n");
        return;
    }

    distanceMatrix[index1][index2] = dist;
    distanceMatrix[index2][index1] = dist;

    printf("Distance between %s and %s set to %d km.\n", city1, city2, dist);
}

// Display distance table
void displayDistanceTable() {
    if (cityCount == 0) {
        printf("No cities available.\n");
        return;
    }

    printf("\n=== Distance Table (in km) ===\n");

    printf("%15s", " ");
    for (int i = 0; i < cityCount; i++) {
        printf("%15s", cities[i]);
    }
    printf("\n");

    // Print matrix
    for (int i = 0; i < cityCount; i++) {
        printf("%15s", cities[i]);
        for (int j = 0; j < cityCount; j++) {
            printf("%15d", distanceMatrix[i][j]);
        }
        printf("\n");
    }
}
