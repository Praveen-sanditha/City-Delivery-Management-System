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
// Input distance function
void inputDistance() {
    if(city_count < 2) {
        printf("\nNeed at least 2 cities to input distances!\n");
        return;
    }

    displayCities();
    int city1, city2;
    float dist;

    printf("\nEnter first city number: ");
    scanf("%d", &city1);
    printf("Enter second city number: ");
    scanf("%d", &city2);

    if(city1 < 1 || city1 > city_count || city2 < 1 || city2 > city_count) {
        printf("\nInvalid city numbers!\n");
        return;
    }

    if(city1 == city2) {
        printf("\nDistance from a city to itself is always 0!\n");
        return;
    }

    printf("Enter distance between %s and %s (km): ", cities[city1-1], cities[city2-1]);
    scanf("%f", &dist);

    if(dist < 0) {
        printf("\nDistance cannot be negative!\n");
        return;
    }

    distance[city1-1][city2-1] = dist;
    distance[city2-1][city1-1] = dist; // Make symmetrical
    printf("\nDistance updated successfully!\n");
}
// Display Distance Table
void displayDistanceTable() {
    if(city_count == 0) {
        printf("\nNo cities available!\n");
        return;
    }

    printf("\n--- DISTANCE TABLE (km) ---\n");
    printf("%-15s", "");
    for(int i = 0; i < city_count; i++) {
        printf("%-15s", cities[i]);
    }
    printf("\n");

    for(int i = 0; i < city_count; i++) {
        printf("%-15s", cities[i]);
        for(int j = 0; j < city_count; j++) {
            if(distance[i][j] == -1)
                            printf("%-15s", "N/A");
            else
                printf("%-15.1f", distance[i][j]);
        }
        printf("\n");
    }
}
// Vehicle Management
void vehicleManagement() {
    printf("\n--- VEHICLE INFORMATION ---\n");
    printf("%-10s %-12s %-15s %-12s %-15s\n",
           "Type", "Capacity(kg)", "Rate/km(LKR)", "Speed(km/h)", "Efficiency(km/l)");
    printf("----------------------------------------------------------------\n");

    for(int i = 0; i < 3; i++) {
        printf("%-10s %-12d %-15.2f %-12.2f %-15.2f\n",
               vehicles[i].name, vehicles[i].capacity, vehicles[i].rate_per_km,
               vehicles[i].avg_speed, vehicles[i].fuel_efficiency);
    }
}

// Delivery Request
void deliveryRequest() {
    if(city_count < 2) {
        printf("\nNeed at least 2 cities for delivery!\n");
        return;
    }

    if(delivery_count >= MAX_DELIVERIES) {
        printf("\nMaximum delivery records reached!\n");
        return;
    }

    displayCities();
    int source, dest, weight, vehicle_type;

    printf("\n--- NEW DELIVERY REQUEST ---\n");
    printf("Enter source city number: ");
    scanf("%d", &source);
    printf("Enter destination city number: ");
    scanf("%d", &dest);

    if(source < 1 || source > city_count || dest < 1 || dest > city_count) {
        printf("\nInvalid city numbers!\n");
        return;
    }

    if(source == dest) {
        printf("\nSource and destination cannot be the same!\n");
        return;
    }

    printf("Enter weight (kg): ");
    scanf("%d", &weight);

    if(weight <= 0) {
        printf("\nWeight must be positive!\n");
        return;
    }

    vehicleManagement();
    printf("\n1 Van \n2 Truck  \n3 Lorry\n ");
    printf("\nSelect vehicle type enter the vehicle number : ");
    scanf("%d", &vehicle_type);

    if(vehicle_type < 1 || vehicle_type > 3) {
        printf("\nInvalid vehicle type!\n");
        return;
    }

    // Check weight capacity
    if(weight > vehicles[vehicle_type-1].capacity) {
        printf("\nWeight exceeds vehicle capacity! %s can carry only %d kg.\n",
               vehicles[vehicle_type-1].name, vehicles[vehicle_type-1].capacity);
        return;
    }

    // Calculate direct distance
    float direct_distance = distance[source-1][dest-1];

    if(direct_distance == -1) {
        printf("\nNo direct route available between these cities!\n");
        return;
    }

    // Store delivery
    Delivery *delivery = &deliveries[delivery_count];
    delivery->source_city = source-1;
    delivery->dest_city = dest-1;
    delivery->weight = weight;
    delivery->vehicle_type = vehicle_type-1;
    delivery->distance = direct_distance;
    delivery->is_completed = 1;

    calculateDelivery(source-1, dest-1, weight, vehicle_type-1, direct_distance);
    delivery_count++;
}

// Calculate Delivery and Display
void calculateDelivery(int source, int dest, int weight, int vehicle_type, float dist)
{
    Vehicle *vehicle = &vehicles[vehicle_type];
    Delivery *delivery = &deliveries[delivery_count];

    //  Delivery Cost
    delivery->delivery_cost = dist * vehicle->rate_per_km * (1 + weight / 10000.0);
    //  Estimated Delivery Time
    delivery->estimated_time = dist / vehicle->avg_speed;
    //  Fuel Consumption
    delivery->fuel_used = dist / vehicle->fuel_efficiency;
    //  Fuel Cost
    delivery->fuel_cost = delivery->fuel_used * FUEL_PRICE;
    //  Total Operational Cost
    delivery->operational_cost = delivery->delivery_cost + delivery->fuel_cost;
    //  Profit (25% markup on base delivery cost)
    delivery->profit = delivery->delivery_cost * 0.25;
    //  Final Charge to Customer
    delivery->customer_charge = delivery->operational_cost + delivery->profit;

    // Display results
    printf("\n======================================================\n");
    printf("DELIVERY COST ESTIMATION\n");
    printf("------------------------------------------------------\n");
    printf("From: %s\n", cities[source]);
    printf("To: %s\n", cities[dest]);
    printf("Distance: %.2f km\n", dist);
    printf("Vehicle: %s\n", vehicle->name);
    printf("Weight: %d kg\n", weight);
    printf("------------------------------------------------------\n");
    printf("Base Cost: %.2f  × %.2f  × (1 + %d/10000) = %.2f LKR\n",
           dist, vehicle->rate_per_km, weight, delivery->delivery_cost);
    printf("Fuel Used: %.2f L\n", delivery->fuel_used);
    printf("Fuel Cost: %.2f LKR\n", delivery->fuel_cost);
    printf("Operational Cost: %.2f LKR\n", delivery->operational_cost);
    printf("Profit: %.2f LKR\n", delivery->profit);
    printf("Customer Charge: %.2f LKR\n", delivery->customer_charge);
    printf("Estimated Time: %.2f hours\n", delivery->estimated_time);
    printf("======================================================\n");
}

// Delivery Records
void deliveryRecords() {
    if(delivery_count == 0) {
        printf("\nNo delivery records available!\n");
        return;
    }

    printf("\n--- DELIVERY RECORDS ---\n");
    printf("%-3s %-15s %-15s %-8s %-10s %-10s %-15s\n",
           "No", "From", "To", "Weight", "Vehicle", "Distance", "Charge(LKR)");
    printf("-----------------------------------------------------------------------------\n");

    float total_revenue = 0;
    for(int i = 0; i < delivery_count; i++) {
        Delivery *d = &deliveries[i];
        printf("%-3d %-15s %-15s %-8d %-10s %-10.2f %-15.2f\n",
               i+1, cities[d->source_city], cities[d->dest_city],
               d->weight, vehicles[d->vehicle_type].name,
               d->distance, d->customer_charge);
        total_revenue += d->customer_charge;
    }
    printf("\nTotal Revenue: %.2f LKR\n", total_revenue);
}
