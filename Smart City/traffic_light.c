#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <time.h>
#include <string.h>
#include <limits.h>

typedef enum { RED, GREEN, YELLOW } LightState; // States of traffic signal

typedef struct {
    LightState state;
    int vehicle_count; // Simulated vehicle density
} TrafficSignal;

typedef struct SensorNode {
    int id;
    int vehicle_count;
    time_t last_update; 
    struct SensorNode* next; // Pointer for garbage collection simulation
} TrafficSensor;

TrafficSensor* sensor_head = NULL; // Head pointer
int sensor_count = 0;

// Allocate memory for a sensor
TrafficSensor* create_sensor(int id) {
    TrafficSensor* sensor = (TrafficSensor*)malloc(sizeof(TrafficSensor));
    if (sensor == NULL) {
        printf("Memory allocation failed for sensor %d\n", id);
        exit(1);
    }
    sensor->id = id;
    sensor->vehicle_count = 0;
    sensor->last_update = time(NULL); // Properly timestamp new sensors
    sensor->next = sensor_head; 
    sensor_head = sensor;
    sensor_count++;
    printf("Sensor %d added successfully.\n", id);
    return sensor;
}

// Deallocate memory for a sensor
void delete_sensor(TrafficSensor* sensor) {
    if (sensor) {
        printf("Sensor %d removed from memory.\n", sensor->id);
        free(sensor);
        sensor_count--;
    }
}

// Calculate the median vehicle count, ignoring faulty sensors
int calculate_median() {
    if (sensor_count == 0) return 0;
    
    int counts[sensor_count];
    int valid_count = 0;
    TrafficSensor* current = sensor_head;
    
    while (current) {
        counts[valid_count++] = current->vehicle_count;
        current = current->next;
    }
    
    for (int j = 0; j < valid_count - 1; j++) {
        for (int k = j + 1; k < valid_count; k++) {
            if (counts[j] > counts[k]) {
                int temp = counts[j];
                counts[j] = counts[k];
                counts[k] = temp;
            }
        }
    }
    
    if (valid_count % 2 == 0)
        return (counts[valid_count/2 - 1] + counts[valid_count/2]) / 2;
    else
        return counts[valid_count/2];
}

// Update traffic signal state based on median vehicle count
void update_signal(TrafficSignal *signal) {
    if (sensor_count == 0) {
        signal->state = RED;
        return;
    }
    
    int median_count = calculate_median();
    
    if (median_count > 10) {
        signal->state = GREEN;
    } else if (median_count > 5) {
        signal->state = YELLOW;
    } else {
        signal->state = RED;
    }
}

// Display traffic light state
void display_signal(TrafficSignal *signal) {
    switch (signal->state) {
        case RED:    printf("RED - Stop\n"); break;
        case GREEN:  printf("GREEN - Go\n"); break;
        case YELLOW: printf("YELLOW - Slow Down\n"); break;
    }
}

// Reading user input for menu selection
void menu() {
    TrafficSignal signal = { RED, 0 };
    
    while (1) {
        printf("\nOptions:\n");
        printf("1. Enter vehicle counts\n");
        printf("2. Add a sensor\n");
        printf("3. Remove the oldest sensor\n");
        printf("4. View active sensors\n");
        printf("5. Exit\n");
        printf("Enter choice: ");
        
        int choice;
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n'); // Clear input buffer
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        
        getchar(); // Consume newline character
        
        if (choice == 5) {
            printf("Exiting...\n");
            exit(0);
        } else if (choice == 2) {
            create_sensor(sensor_count + 1);
        } else if (choice == 3) {
            if (sensor_head) {
                TrafficSensor* to_remove = sensor_head;
                sensor_head = sensor_head->next;
                delete_sensor(to_remove);
            } else {
                printf("No sensors to remove.\n");
            }
        } else if (choice == 4) {
            TrafficSensor* current = sensor_head;
            printf("Active Sensors:\n");
            while (current) {
                printf("Sensor ID: %d, Vehicle Count: %d\n", current->id, current->vehicle_count);
                current = current->next;
            }
            if (!sensor_head) {
                printf("No active sensors available.\n");
            }
        } else if (choice == 1) {
            printf("Enter %d vehicle counts (space-separated): ", sensor_count);
            char buffer[256]; 
            if (fgets(buffer, sizeof(buffer), stdin)) {
                buffer[strcspn(buffer, "\n")] = 0; // Remove newline character
                
                int entered_count = 0;
                TrafficSensor* current = sensor_head;
                char* token = strtok(buffer, " ");
                while (current && token) {
                    int value = atoi(token);
                    if (value < 0) {
                        printf("Invalid vehicle count. Only positive integers allowed.\n");
                        break;
                    }
                    current->vehicle_count = value;
                    current->last_update = time(NULL);
                    current = current->next;
                    token = strtok(NULL, " ");
                    entered_count++;
                }
                
                if (entered_count != sensor_count) {
                    printf("Incorrect number of values entered. Expected %d.\n", sensor_count);
                    continue;
                }
            }
            update_signal(&signal);
            display_signal(&signal);
        } else {
            printf("Invalid choice. Please select a valid option.\n");
        }
    }
}

int main() {
    int num_sensors;
    printf("Enter number of initial sensors: ");
    if (scanf("%d", &num_sensors) != 1 || num_sensors <= 0) {
        printf("Invalid number of sensors. Exiting.\n");
        return 1;
    }
    getchar(); // Consume newline left by scanf
    
    for (int i = 1; i <= num_sensors; i++) {
        create_sensor(i);
    }
    
    menu(); // Start input handling loop
    return 0;
}

