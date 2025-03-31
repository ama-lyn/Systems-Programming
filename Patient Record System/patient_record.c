#include <stdio.h>    
#include <stdlib.h>  
#include <string.h>   
#include <stdbool.h>   
#include <ctype.h>     //charcater manipulation

#define INITIAL_MED_HISTORY_SIZE 1000   
#define MAX_NAME_LENGTH 100             
#define MAX_DIAGNOSIS_LENGTH 200        
#define MAX_INPUT_LENGTH 500            

typedef struct {
    char* details;
    size_t capacity;
    size_t length;
} MedicalHistory;  //medical_history_structure

typedef struct {
    int id;
    char name[MAX_NAME_LENGTH];
    int age;
    char diagnosis[MAX_DIAGNOSIS_LENGTH];
    MedicalHistory medicalHistory;
} Patient;  //patientstructure

typedef struct {
    Patient* patients;
    size_t count;
    size_t capacity;
} PatientDatabase;  //patientdatabase

//functions
void initMedicalHistory(MedicalHistory* history);  //prepares memory to store medical history for a new patient
bool expandMedicalHistory(MedicalHistory* history, size_t requiredLength);  //expand memory allocation when history becomes too large
bool addToMedicalHistory(MedicalHistory* history, const char* entry);  //appends new history to a patients history
void freeMedicalHistory(MedicalHistory* history);   //release memory used by medical history(clean up space when a patient is removed)
void initPatientDatabase(PatientDatabase* db, size_t initialCapacity); //initializes an empty patient database(prepares memory to store patients)
bool addPatient(PatientDatabase* db); 
Patient* findPatient(PatientDatabase* db, int id); 
bool updateMedicalHistory(PatientDatabase* db);  
bool removePatient(PatientDatabase* db, int id); 
void displayAllPatients(const PatientDatabase* db);
void displayPatient(const Patient* p);  
void freePatientDatabase(PatientDatabase* db); //prevent memory leaks
void flushInputBuffer();
int getIntInput(const char* prompt);
void getStringInput(const char* prompt, char* buffer, size_t maxLength);

// Main menu
void displayMainMenu() {
    printf("\n=== HOSPITAL PATIENT RECORD SYSTEM ===\n");
    printf("1. Add New Patient\n");
    printf("2. Update Patient Medical History\n");
    printf("3. Remove Discharged Patient\n");
    printf("4. View All Patients\n");
    printf("5. View Specific Patient\n");
    printf("6. Exit\n");
    printf("====================================\n");
}

int main() {
    PatientDatabase db;
    initPatientDatabase(&db, 5);

    int choice;
    do {
        displayMainMenu();
        choice = getIntInput("Enter your choice (1-6): ");

        switch (choice) {
            case 1:
                addPatient(&db);
                break;
            case 2:
                updateMedicalHistory(&db);
                break;
            case 3:{
                int id = getIntInput("Enter patient ID to remove: ");
                removePatient(&db, id);
                break;
            }  
            case 4:
                displayAllPatients(&db);
                break;
            case 5: {
                int id = getIntInput("Enter patient ID to view: ");
                Patient* p = findPatient(&db, id);
                if (p) {
                    displayPatient(p);
                } else {
                    printf("Patient with ID %d not found.\n", id);
                }
                break;
            }
            case 6:
                printf("Exiting system...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 6);

    freePatientDatabase(&db);
    return 0;
}

// Helper function to clear input buffer
void flushInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}     //this clears the input buffer after the fgets call to prevent reading invalid data.

// Get integer input with validation
int getIntInput(const char* prompt) {
    int value;
    char buffer[MAX_INPUT_LENGTH];
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            if (sscanf(buffer, "%d", &value) == 1) {  //convert the string to an interger
                return value;
            }
        }
        printf("Invalid input. Please enter a number.\n");
    }
} 

// Get string input with validation
void getStringInput(const char* prompt, char* buffer, size_t maxLength) {
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, maxLength, stdin) != NULL) {
            // Remove trailing newline if present
            size_t len = strlen(buffer);
            if (len > 0 && buffer[len-1] == '\n') {
                buffer[len-1] = '\0';
            } else {
                // Input was too long, clear the buffer
                flushInputBuffer();
                printf("Input too long. Maximum %zu characters allowed.\n", maxLength-1);
                continue;
            }
            
            if (strlen(buffer) > 0) {
                return;
            }
        }
        printf("Invalid input. Please try again.\n");
    }
}

// Initialize a new medical history
void initMedicalHistory(MedicalHistory* history) {
    history->details = (char*)malloc(INITIAL_MED_HISTORY_SIZE);
    if (history->details == NULL) {
        fprintf(stderr, "Failed to allocate memory for medical history\n");
        exit(EXIT_FAILURE);
    }
    history->details[0] = '\0';
    history->capacity = INITIAL_MED_HISTORY_SIZE;
    history->length = 0;
} //dynamically allocates memory for a new patient and initializes the length to 0.

// Expand medical history 
bool expandMedicalHistory(MedicalHistory* history, size_t requiredLength) {
    if (history->length + requiredLength + 1 <= history->capacity) {
        return true; // No need to expand
    }

    size_t newCapacity = history->capacity * 2;
    while (newCapacity < history->length + requiredLength + 1) {
        newCapacity *= 2;
    }

    char* newDetails = (char*)realloc(history->details, newCapacity);
    if (newDetails == NULL) {
        fprintf(stderr, "Failed to expand medical history\n");
        return false;
    }

    history->details = newDetails;
    history->capacity = newCapacity;
    return true;
}

// Add to medical history
bool addToMedicalHistory(MedicalHistory* history, const char* entry) {
    size_t entryLength = strlen(entry);
    if (!expandMedicalHistory(history, entryLength)) {
        return false;
    }

    if (history->length > 0) {
        strcat(history->details, "\n"); // Add newline between entries
        history->length++;
    }
    
    strcat(history->details, entry);
    history->length += entryLength;
    return true;
}

// Free medical history memory
void freeMedicalHistory(MedicalHistory* history) {
    free(history->details);
    history->details = NULL;
    history->capacity = 0;
    history->length = 0;
}

// Initialize patient database
void initPatientDatabase(PatientDatabase* db, size_t initialCapacity) {
    db->patients = (Patient*)malloc(initialCapacity * sizeof(Patient));
    if (db->patients == NULL) {
        fprintf(stderr, "Failed to allocate memory for patient database\n");
        exit(EXIT_FAILURE);
    }
    db->count = 0;
    db->capacity = initialCapacity;
}

// Add a new patient 
bool addPatient(PatientDatabase* db) {
    printf("\n*** Add New Patient ***\n");
    
    // Get patient details from user
    int id;
    while (1) {
        id = getIntInput("Enter patient ID: ");
        if (findPatient(db, id) == NULL) {
            break;
        }
        printf("Patient ID %d already exists. Please enter a different ID.\n", id);
    }
    
    char name[MAX_NAME_LENGTH];
    getStringInput("Enter patient name: ", name, sizeof(name));
    
    int age;
    while (1) {
        age = getIntInput("Enter patient age: ");
        if (age > 0 && age < 1000) {
            break;
        }
        printf("Invalid age. Please enter a value between 1 and 999.\n");
    }
    
    char diagnosis[MAX_DIAGNOSIS_LENGTH];
    getStringInput("Enter primary diagnosis: ", diagnosis, sizeof(diagnosis));
    
    // Resize database 
    if (db->count >= db->capacity) {
        size_t newCapacity = db->capacity * 2;
        Patient* newPatients = (Patient*)realloc(db->patients, newCapacity * sizeof(Patient));
        if (newPatients == NULL) {
            fprintf(stderr, "Failed to expand patient database\n");
            return false;
        }
        db->patients = newPatients;
        db->capacity = newCapacity;
    }

    // Add the new patient
    Patient* p = &db->patients[db->count];
    p->id = id;
    strncpy(p->name, name, MAX_NAME_LENGTH - 1);
    p->name[MAX_NAME_LENGTH - 1] = '\0';
    p->age = age;
    strncpy(p->diagnosis, diagnosis, MAX_DIAGNOSIS_LENGTH - 1);
    p->diagnosis[MAX_DIAGNOSIS_LENGTH - 1] = '\0';
    initMedicalHistory(&p->medicalHistory);
    
    // Get initial medical history
    char historyEntry[MAX_INPUT_LENGTH];
    getStringInput("Enter initial medical history notes: ", historyEntry, sizeof(historyEntry));
    addToMedicalHistory(&p->medicalHistory, historyEntry);
    
    db->count++;
    printf("Patient added successfully.\n");
    return true;
}

// Find a patient by ID
Patient* findPatient(PatientDatabase* db, int id) {
    for (size_t i = 0; i < db->count; i++) {
        if (db->patients[i].id == id) {
            return &db->patients[i];
        }
    }
    return NULL;
}

// Update patient medical history
bool updateMedicalHistory(PatientDatabase* db) {
    printf("\n*** Update Medical History ***\n");
    int id = getIntInput("Enter patient ID to update: ");
    
    Patient* p = findPatient(db, id);
    if (p == NULL) {
        printf("Patient with ID %d not found.\n", id);
        return false;
    }
    
    printf("\nCurrent medical history for %s (ID: %d):\n", p->name, p->id);
    printf("%s\n", p->medicalHistory.details);
    
    char newEntry[MAX_INPUT_LENGTH];
    getStringInput("Enter new medical history entry: ", newEntry, sizeof(newEntry));
    
    if (addToMedicalHistory(&p->medicalHistory, newEntry)) {
        printf("Medical history updated successfully.\n");
        return true;
    } else {
        printf("Failed to update medical history.\n");
        return false;
    }
}

// Remove a patient from the database 
bool removePatient(PatientDatabase* db, int id) {
    printf("\n*** Remove Discharged Patient ***\n");
    int id = getIntInput("Enter patient ID to remove: ");

    for (size_t i = 0; i < db->count; i++) {
        if (db->patients[i].id == id) {
            printf("Removing patient %s (ID: %d)...\n", db->patients[i].name, id);
            
            // Free medical history memory
            freeMedicalHistory(&db->patients[i].medicalHistory);

            // Shift remaining patients left
            for (size_t j = i; j < db->count - 1; j++) {
                db->patients[j] = db->patients[j + 1];
            }
            db->count--;
            printf("Patient with ID %d removed successfully.\n", id);
            return true;
        }
    }
    printf("Patient with ID %d not found.\n", id);
    return false;
}

// Display all patients
void displayAllPatients(const PatientDatabase* db) {
    printf("\n=== Patient Database (%zu/%zu) ===\n", db->count, db->capacity);
    if (db->count == 0) {
        printf("No patients in the database.\n");
    } else {
        for (size_t i = 0; i < db->count; i++) {
            displayPatient(&db->patients[i]);
            printf("----------------------------\n");
        }
    }
    printf("*** End of Database ***\n");
}

// Display a single patient
void displayPatient(const Patient* p) {
    printf("\nPatient ID: %d\n", p->id);
    printf("Name: %s\n", p->name);
    printf("Age: %d\n", p->age);
    printf("Diagnosis: %s\n", p->diagnosis);
    printf("Medical History:\n%s\n", p->medicalHistory.details);
}

// Free all database memory
void freePatientDatabase(PatientDatabase* db) {
    for (size_t i = 0; i < db->count; i++) {
        freeMedicalHistory(&db->patients[i].medicalHistory);
    }
    free(db->patients);
    db->patients = NULL;
    db->count = 0;
    db->capacity = 0;
}
