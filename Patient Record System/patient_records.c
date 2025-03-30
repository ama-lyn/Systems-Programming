#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct Patient {
    int id;                     // Unique patient ID
    char *name;                 // Full name
    char *medical_history;       // Past medical conditions
    char *contact_info;          
    char *dob;                   // Date of birth 
    int doctor_id;               
    char *insurance_info;        
    char *emergency_contact;     // Name & phone of next of kin
    char *date;
    struct Patient *next;        // Pointer for linked list structure
} Patient;

FILE *fp;
void admitPatient();
void patientList();
void dischargePatient();
void updatePatient();

int main(){
    


    int choice;

    while(1){
        system("cls");
        printf("<== Patient Record System ==>\n");
        printf("1. Admit Patient\n");
        printf("2. Patient List\n");
        printf("3. Discharge Patient\n");
        printf("4. Update Patient Details\n");         
        printf("0. Exit\n\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch(choice){
        case 0:
            exit(0);

        case 1:
            admitPatient();
            break;
        
       case 2:
            patientList();
            break;

        case 3:
            dischargePatient();
            break;

        case 4:
            updatePatient();
            break;

        default:
            printf("Invalid Choice...\n\n");

        }
        printf("\nPress Enter to continue...");
        while (getchar() != '\n'); // Wait for user input
        getchar();
    }

    return 0;
}

void admitPatient() {
    char myDate[20];  // Declare before usage
    char temp[100];   // Temporary buffer for user input

    // Allocate memory for new patient
    Patient *newPatient = (Patient *)malloc(sizeof(Patient));
    if (!newPatient) {
        printf("Memory allocation failed for patient!\n");
        return;
    }

    // Initialize fields to avoid undefined behavior
    newPatient->id = 0;
    newPatient->name = NULL;
    newPatient->medical_history = NULL;
    newPatient->contact_info = NULL;
    newPatient->dob = NULL;
    newPatient->insurance_info = NULL;
    newPatient->emergency_contact = NULL;
    newPatient->date = NULL;

   

    // Allocate memory for date
    newPatient->date = strdup(myDate);
    if (!newPatient->date) {
        printf("Memory allocation failed for date!\n");
        free(newPatient);
        return;
    }

    // Get user input
    printf("Enter Patient ID: ");
    scanf("%d", &newPatient->id);
    while (getchar() != '\n'); // Clear input buffer

    printf("Enter Patient Name: ");
    fgets(temp, sizeof(temp), stdin);
    temp[strcspn(temp, "\n")] = '\0';
    newPatient->name = strdup(temp);

    printf("Enter Medical History: ");
    fgets(temp, sizeof(temp), stdin);
    temp[strcspn(temp, "\n")] = '\0';
    newPatient->medical_history = strdup(temp);

    printf("Enter Contact Info: ");
    fgets(temp, sizeof(temp), stdin);
    temp[strcspn(temp, "\n")] = '\0';
    newPatient->contact_info = strdup(temp);

    printf("Enter Date of Birth (DD/MM/YYYY): ");
    fgets(temp, sizeof(temp), stdin);
    temp[strcspn(temp, "\n")] = '\0';
    newPatient->dob = strdup(temp);

    printf("Enter Doctor ID: ");
    scanf("%d", &newPatient->doctor_id);
    while (getchar() != '\n');

    printf("Enter Insurance Info: ");
    fgets(temp, sizeof(temp), stdin);
    temp[strcspn(temp, "\n")] = '\0';
    newPatient->insurance_info = strdup(temp);

    printf("Enter Emergency Contact: ");
    fgets(temp, sizeof(temp), stdin);
    temp[strcspn(temp, "\n")] = '\0';
    newPatient->emergency_contact = strdup(temp);

    // Save to file
    FILE *fp = fopen("patient.csv", "a");
    if (!fp) {
        printf("Error opening file!\n");
        free(newPatient);
        return;
    }

    fprintf(fp, "%d,%s,%s,%s,%s,%d,%s,%s,%s\n",
        newPatient->id, newPatient->name, newPatient->medical_history,
        newPatient->contact_info, newPatient->dob, newPatient->doctor_id,
        newPatient->insurance_info, newPatient->emergency_contact,
        newPatient->date);

    fclose(fp);
    printf("\nPatient Added Successfully!\n");

    // Free allocated memory
    char *records[] = {
	    newPatient->name, newPatient->medical_history, newPatient->contact_info,
	    newPatient->dob, newPatient->insurance_info, newPatient->emergency_contact,
	    newPatient->date
    };

    for (int i = 0; i < sizeof(records) / sizeof(records[0]); i++) {
	    free(records[i]);
    }
    free(newPatient);
}
