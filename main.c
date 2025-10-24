#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_CITIES 30
#define MAX_DELIVERIES 50
#define MAX_ROUTE_LENGTH 4
#define FUEL_PRICE 310.0

// Vehicle structure
typedef struct {
    char name[20];
    int capacity;
    float rate_per_km;
    float avg_speed;
    float fuel_efficiency;
} Vehicle;

// Delivery structure
typedef struct {
    int source_city;
    int dest_city;
    int weight;
    int vehicle_type;
    float distance;
    float delivery_cost;
    float fuel_used;
    float fuel_cost;
    float operational_cost;
    float profit;
    float customer_charge;
    float estimated_time;
    int is_completed;
} Delivery;

// Global arrays
char cities[MAX_CITIES][50];
float distance[MAX_CITIES][MAX_CITIES];
Delivery deliveries[MAX_DELIVERIES];
Vehicle vehicles[3] = {
    {"Van", 1000, 30.0, 60.0, 12.0},
    {"Truck", 5000, 40.0, 50.0, 6.0},
    {"Lorry", 10000, 80.0, 45.0, 4.0}
};

int city_count = 0;
int delivery_count = 0;

// Function prototypes
void initializeSystem();
void mainMenu();
void cityManagement();
void addCity();
void renameCity();
void removeCity();
void displayCities();
void distanceManagement();
void inputDistance();
void editDistance();
void displayDistanceTable();
void vehicleManagement();
void deliveryRequest();
void calculateDelivery(int source, int dest, int weight, int vehicle_type, float dist);
void deliveryRecords();
void findLeastCostRoute();
void generatePermutations(int cities[], int n, int current[], int used[], int level,
                         float *min_distance, int *best_route, int start, int end);
float calculateRouteDistance(int route[], int n);
void performanceReports();
int findCityIndex(char* city_name);
int isValidCityName(char* name);

int main() {
    initializeSystem();
    mainMenu();
    return 0;
}

void initializeSystem() {
    // Initialize distance matrix
    for(int i = 0; i < MAX_CITIES; i++) {
        for(int j = 0; j < MAX_CITIES; j++) {
            if(i == j)
                distance[i][j] = 0;
            else
                distance[i][j] = -1; // -1 means no connection
        }
    }

    // Add some sample cities
    strcpy(cities[city_count++], "Colombo");
    strcpy(cities[city_count++], "Kandy");
    strcpy(cities[city_count++], "Galle");
    strcpy(cities[city_count++], "Jaffna");

    // Add sample distances
    distance[0][1] = distance[1][0] = 120; // Colombo-Kandy
    distance[0][2] = distance[2][0] = 115; // Colombo-Galle
    distance[0][3] = distance[3][0] = 400; // Colombo-Jaffna
    distance[1][2] = distance[2][1] = 200; // Kandy-Galle
    distance[1][3] = distance[3][1] = 350; // Kandy-Jaffna
    distance[2][3] = distance[3][2] = 450; // Galle-Jaffna
}

void mainMenu() {
    int choice;

    do {
        printf("\n================================================");
        printf("\n          DELIVERY MANAGEMENT SYSTEM");
        printf("\n================================================");
        printf("\n1. City Management");
        printf("\n2. Distance Management");
        printf("\n3. Vehicle Information");
        printf("\n4. New Delivery Request");
        printf("\n5. Delivery Records");
        printf("\n6. Find Least Cost Route");
        printf("\n7. Performance Reports");
        printf("\n8. Exit");
        printf("\n------------------------------------------------");
        printf("\nEnter your choice (1-8): ");
        scanf("%d", &choice);

        switch(choice) {
            case 1: cityManagement(); break;
            case 2: distanceManagement(); break;
            case 3: vehicleManagement(); break;
            case 4: deliveryRequest(); break;
            case 5: deliveryRecords(); break;
            case 6: findLeastCostRoute(); break;
            case 7: performanceReports(); break;
            case 8: printf("\nThank you for using the system!\n"); break;
            default: printf("\nInvalid choice! Please try again.\n");
        }
    } while(choice != 8);
}

void cityManagement() {
    int choice;

    do {
        printf("\n--- CITY MANAGEMENT ---");
        printf("\n1. Add New City");
        printf("\n2. Rename City");
        printf("\n3. Remove City");
        printf("\n4. Display All Cities");
        printf("\n5. Back to Main Menu");
        printf("\nEnter your choice (1-5): ");
        scanf("%d", &choice);

        switch(choice) {
            case 1: addCity(); break;
            case 2: renameCity(); break;
            case 3: removeCity(); break;
            case 4: displayCities(); break;
            case 5: break;
            default: printf("\nInvalid choice!\n");
        }
    } while(choice != 5);
}

void addCity() {
    if(city_count >= MAX_CITIES) {
        printf("\nMaximum number of cities reached (%d)!\n", MAX_CITIES);
        return;
    }

    char name[50];
    printf("\nEnter city name: ");
    scanf(" %[^\n]", name);    // %[^\n] - because some names enter with space

    if(!isValidCityName(name)) {
        printf("\nInvalid city name! Name cannot be empty.\n");
        return;
    }

    // Check if city already exists
    for(int i = 0; i < city_count; i++) {
        if(strcasecmp(cities[i], name) == 0) {
            printf("\nCity '%s' already exists!\n", name);
            return;
        }
    }

    strcpy(cities[city_count], name);
    city_count++;
    printf("\nCity '%s' added successfully!\n", name);
}

int isValidCityName(char* name) {
    return strlen(name) > 0 && strlen(name) < 50;
}

void renameCity() {
    if(city_count == 0) {
        printf("\nNo cities available!\n");
        return;
    }

    displayCities();
    int index;
    char new_name[50];

    printf("\nEnter city number to rename: ");
    scanf("%d", &index);

    if(index < 1 || index > city_count) {
        printf("\nInvalid city number!\n");
        return;
    }

    printf("Enter new name: ");
    scanf(" %[^\n]", new_name);

    if(!isValidCityName(new_name)) {
        printf("\nInvalid city name!\n");
        return;
    }

    // Check if new name already exists
    for(int i = 0; i < city_count; i++) {
        if(i != index-1 && strcasecmp(cities[i], new_name) == 0) {
            printf("\nCity '%s' already exists!\n", new_name);
            return;
        }
    }

    printf("\nCity '%s' renamed to '%s'\n", cities[index-1], new_name);
    strcpy(cities[index-1], new_name);
}

void removeCity() {
    if(city_count == 0) {
        printf("\nNo cities available!\n");
        return;
    }

    displayCities();
    int index;

    printf("\nEnter city number to remove: ");
    scanf("%d", &index);

    if(index < 1 || index > city_count) {
        printf("\nInvalid city number!\n");
        return;
    }

    printf("\nAre you sure you want to remove '%s'? (1=Yes, 0=No): ", cities[index-1]);
    int confirm;
    scanf("%d", &confirm);

    if(confirm) {
        // Shift cities array
        for(int i = index-1; i < city_count-1; i++) {
            strcpy(cities[i], cities[i+1]);
        }

        // Shift distance matrix
        for(int i = index-1; i < city_count-1; i++) {
            for(int j = 0; j < city_count; j++) {
                distance[i][j] = distance[i+1][j];
            }
        }
        for(int j = index-1; j < city_count-1; j++) {
            for(int i = 0; i < city_count; i++) {
                distance[i][j] = distance[i][j+1];
            }
        }

        city_count--;
        printf("\nCity removed successfully!\n");
    }
}

void displayCities() {
    if(city_count == 0) {
        printf("\nNo cities available!\n");
        return;
    }

    printf("\n--- AVAILABLE CITIES ---\n");
    for(int i = 0; i < city_count; i++) {
        printf("%d. %s\n", i+1, cities[i]);
    }
}

void distanceManagement() {
    int choice;

    do {
        printf("\n--- DISTANCE MANAGEMENT ---");
        printf("\n1. Input/Edit Distance");
        printf("\n2. Display Distance Table");
        printf("\n3. Back to Main Menu");
        printf("\nEnter your choice (1-3): ");
        scanf("%d", &choice);

        switch(choice) {
            case 1: inputDistance(); break;
            case 2: displayDistanceTable(); break;
            case 3: break;
            default: printf("\nInvalid choice!\n");
        }
    } while(choice != 3);
}

