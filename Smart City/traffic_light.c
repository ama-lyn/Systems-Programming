#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define BUFFER_SIZE 128

typedef enum { RED, GREEN, YELLOW } LightState; // States of traffic signal

typedef struct {
    LightState state;
    int duration;
} TrafficSignal;

typedef struct SensorNode {
    int id;
    int vehicle_count;
    time_t last_update;
    struct SensorNode* next; // Pointer for garbage collection simulation
} TrafficSensor;

TrafficSensor* sensor_head = NULL; // Head pointer
int sensor_count = 0;

// Allocate memory for a sensor - Dynamic memory allocation
TrafficSensor* create_sensor(int id) {
    TrafficSensor* sensor = (TrafficSensor*)malloc(sizeof(TrafficSensor));
    if (sensor == NULL) {
        printf("Memory allocation failed for sensor %d\n", id);
        return NULL;
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

// Safe input handling to prevent buffer overflows
int get_int_input(const char* prompt) {
    char buffer[BUFFER_SIZE];
    int value;
    
    while (1) {
        printf("%s", prompt);
        
        if (fgets(buffer, BUFFER_SIZE, stdin) == NULL) {
            printf("Input error. Try again.\n");
            continue;
        }
        
        // Check for buffer overflow
        if (buffer[strlen(buffer) - 1] != '\n') {
            printf("Input too long! Maximum %d characters allowed.\n", BUFFER_SIZE - 1);
            // Clear input buffer
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            continue;
        }
        
        // Remove newline
        buffer[strcspn(buffer, "\n")] = 0;
        
        // Check if input is a valid number
        char* endptr;
        value = strtol(buffer, &endptr, 10);
        
        if (*endptr != '\0') {
            printf("Please enter a valid number.\n");
            continue;
        }
        
        return value;
    }
}

// Deallocate Memory Delete a sensor by ID
void delete_sensor(int id) {
    TrafficSensor* current = sensor_head;
    TrafficSensor* prev = NULL;
    
    while (current != NULL) {
        if (current->id == id) {
            // Found the sensor to delete
            if (prev == NULL) {
                // It's the head node
                sensor_head = current->next;
            } else {
                // It's not the head node
                prev->next = current->next;
            }
            
            printf("Sensor %d removed from memory.\n", id);
            free(current);
            sensor_count--;
            return;
        }
        
        prev = current;
        current = current->next;
    }
    
    printf("Sensor with ID %d not found.\n", id);
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
        signal->duration = 30;
        return;
    }
    
    int median_count = calculate_median();
    
    if (median_count > 10) {
        signal->state = GREEN;
        signal->duration = 45;
    } else if (median_count > 5) {
        signal->state = YELLOW;
        signal->duration = 5;
    } else {
        signal->state = RED;
        signal->duration = 20;
    }
}

// Display traffic light state
void display_signal(TrafficSignal *signal) {
    printf("\n----- TRAFFIC LIGHT STATUS -----\n");
    printf("Current state: ");
    
    switch (signal->state) {
        case RED:    
            printf("RED - Stop (%d seconds)\n", signal->duration); 
            break;
        case GREEN:  
            printf("GREEN - Go (%d seconds)\n", signal->duration); 
            break;
        case YELLOW: 
            printf("YELLOW - Slow Down (%d seconds)\n", signal->duration); 
            break;
    }
    printf("-------------------------------\n");
}

// Display all sensors
void display_sensors() {
    TrafficSensor* current = sensor_head;
    
    if (!current) {
        printf("No sensors available.\n");
        return;
    }
    
    printf("\n----- SENSORS -----\n");
    while (current) {
        printf("Sensor ID: %d\n", current->id);
        printf("  Vehicle Count: %d\n", current->vehicle_count);
        printf("---------------------------\n");
        current = current->next;
    }
}

// Find a sensor by ID
TrafficSensor* find_sensor(int id) {
    TrafficSensor* current = sensor_head;
    
    while (current) {
        if (current->id == id) {
            return current;
        }
        current = current->next;
    }
    
    return NULL;
}

// Update sensor data with buffer overflow protection
void update_sensor(int id) {
    TrafficSensor* sensor = find_sensor(id);
    
    if (!sensor) {
        printf("Sensor with ID %d not found.\n", id);
        return;
    }
    
    int count = get_int_input("Enter vehicle count: ");
    if (count < 0) {
        printf("Invalid vehicle count. Using 0 instead.\n");
        count = 0;
    }
    
    sensor->vehicle_count = count;
    sensor->last_update = time(NULL);
    
    printf("Sensor %d updated successfully.\n", id);
}

// Clean up all resources to prevent memory leaks
void cleanup_resources() {
    TrafficSensor* current = sensor_head;
    int freed = 0;
    
    while (current) {
        TrafficSensor* next = current->next;
        free(current);
        current = next;
        freed++;
    }
    
    sensor_head = NULL;
    sensor_count = 0;
    
    if (freed > 0) {
        printf("Cleanup completed: %d sensors freed from memory.\n", freed);
    }
}

// Reading user input for menu selection
void menu() {
    TrafficSignal signal = { RED, 30 };
    
    while (1) {
        printf("\n----- TRAFFIC MANAGEMENT SYSTEM -----\n");
        printf("1. Add a new sensor\n");
        printf("2. Update sensor data\n");
        printf("3. View all sensors\n");
        printf("4. Update traffic signal\n");
        printf("5. Delete a sensor\n");
        printf("6. Exit\n");
        printf("-------------------------------------\n");
        
        int choice = get_int_input("Enter your choice: ");
        
        switch (choice) {
            case 1: {
                int id = sensor_count + 1;
                create_sensor(id);
                break;
            }
            
            case 2: {
                display_sensors();
                if (sensor_count == 0) {
                    printf("No sensors available to update.\n");
                    break;
                }
                
                int id = get_int_input("Enter sensor ID to update: ");
                update_sensor(id);
                break;
            }
            
            case 3:
                display_sensors();
                break;
                
            case 4:
                update_signal(&signal);
                display_signal(&signal);
                break;
                
            case 5: {
                display_sensors();
                if (sensor_count == 0) {
                    printf("No sensors available to delete.\n");
                    break;
                }
                
                int id = get_int_input("Enter sensor ID to delete: ");
                delete_sensor(id);
                break;
            }
            
            case 6:
                cleanup_resources();
                printf("Exiting the system.\n");
                exit(0);
                
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}

int main() {
    printf("Welcome to the Traffic Light Management System\n");
    printf("--------------------------------------------\n");
    
    // Set up signal handler to ensure cleanup on unexpected termination
    atexit(cleanup_resources);
    
    menu(); // Start input handling loop
    return 0;
}