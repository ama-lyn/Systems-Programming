#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

FILE *fp;

typedef struct medicalHistory {
	int patient_id;
	char *last_diagnosis;
	char *date_of_last_visit;
	struct medicalHistory *next;
} medicalHistory;

typedef struct Patient {
	int id;
	char *name;
	char *contact_info;
	char *dob;
	char *insurance_info;
	char *emergency_contact;
	char *date;
	struct Patient *next;
	medicalHistory *history;
} Patient;

void admitPatient();
void patientList();
void dischargePatient();
void updatePatient();
void getInput(char **field, const char *prompt, size_t size);

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

				//case 2:
				//   patientList();
				// break;

				//case 3:
				//  dischargePatient();
				//break;

				//case 4:
				//  updatePatient();
				//break;

			default:
				printf("Invalid Choice...\n\n");

		}
		printf("\nPress Enter to continue...");
		while (getchar() != '\n'); // Wait for user input
		getchar();
	}

	return 0;
}
void getInput(char **field, const char *prompt, size_t size) {
	char temp[size];
	printf("%s", prompt);
	fgets(temp, sizeof(temp), stdin);
	temp[strcspn(temp, "\n")] = '\0';
	*field = strdup(temp);
	if (!*field) {
		printf("Memory allocation failed!\n");
		exit(1);
	}
}

int patientExists(int id) {
	FILE *fp = fopen("patient.csv", "r");
	if (!fp) return 0;

	char line[512];
	while (fgets(line, sizeof(line), fp)) {
		int existing_id;
		sscanf(line, "%d,", &existing_id);
		if (existing_id == id) {
			fclose(fp);
			return 1;
		}
	}
	fclose(fp);
	return 0;
}

void admitPatient() {
	char myDate[20];
	time_t t = time(NULL);
	struct tm *tm_info = localtime(&t);
	strftime(myDate, sizeof(myDate), "%d/%m/%Y", tm_info);

	Patient *newPatient = (Patient *)malloc(sizeof(Patient));
	if (!newPatient) {
		printf("Memory allocation failed for patient!\n");
		return;
	}
	newPatient->date = strdup(myDate);

	printf("Enter Patient ID: ");
	scanf("%d", &newPatient->id);

	void getInput(char **field, const char *prompt, size_t size) {
		char temp[size];
		printf("%s", prompt);
		fgets(temp, sizeof(temp), stdin);
		temp[strcspn(temp, "\n")] = '\0';
		*field = strdup(temp);
		if (!*field) {
			printf("Memory allocation failed!\n");
			exit(1);
		}
	}
	int patientExists(int id) {
		FILE *fp = fopen("patient.csv", "r");
		if (!fp) return 0;

		char line[512];
		while (fgets(line, sizeof(line), fp)) {
			int existing_id;
			sscanf(line, "%d,", &existing_id);
			if (existing_id == id) {
				fclose(fp);
				return 1;
			}
		}
		fclose(fp);
		return 0;
	}

	void admitPatient() {
		char myDate[20];
		time_t t = time(NULL);
		struct tm *tm_info = localtime(&t);
		strftime(myDate, sizeof(myDate), "%d/%m/%Y", tm_info);

		Patient *newPatient = (Patient *)malloc(sizeof(Patient));
		if (!newPatient) {
			printf("Memory allocation failed for patient!\n");
			return;
		}
		newPatient->date = strdup(myDate);

		printf("Enter Patient ID: ");
		scanf("%d", &newPatient->id);
		while (getchar() != '\n');

		if (patientExists(newPatient->id)) {
			printf("Patient found. Update medical history.\n");

			medicalHistory *newHistory = (medicalHistory *)malloc(sizeof(medicalHistory));
			if (!newHistory) {
				while (getchar() != '\n');

				if (patientExists(newPatient->id)) {
					printf("Existing patient found. Only updating medical history.\n");

					medicalHistory *newHistory = (medicalHistory *)malloc(sizeof(medicalHistory));
					if (!newHistory) {
						printf("Memory allocation failed for medical history!\n");
						free(newPatient);
						return;
					}
					newHistory->patient_id = newPatient->id;
					newHistory->date_of_last_visit = strdup(myDate);
					getInput(&newHistory->last_diagnosis, "Enter Last Diagnosis: ", 200);

					FILE *fp = fopen("medical_history.csv", "a");
					if (!fp) {
						printf("Error opening medical history file!\n");
						free(newHistory);
						free(newPatient);
						return;
					}
					fprintf(fp, "%d,%s,%s\n", newHistory->patient_id,
							newHistory->last_diagnosis,
							newHistory->date_of_last_visit);
					fclose(fp);

					printf("\nMedical History Updated Successfully!\n");

					free(newHistory->last_diagnosis);
					free(newHistory->date_of_last_visit);
					free(newHistory);
					free(newPatient);
				} else {
					printf("New patient detected. Collecting full details.\n");
					getInput(&newPatient->name, "Enter Patient Name: ", 100);
					getInput(&newPatient->contact_info, "Enter Contact Info: ", 100);
					getInput(&newPatient->dob, "Enter Date of Birth (DD/MM/YYYY): ", 20);
					getInput(&newPatient->insurance_info, "Enter Insurance Info: ", 100);
					getInput(&newPatient->emergency_contact, "Enter Emergency Contact: ", 100);

					FILE *fp = fopen("patient.csv", "a");
					if (!fp) {
						printf("Error opening file!\n");
						free(newPatient);
						return;
					}
					fprintf(fp, "%d,%s,%s,%s,%s,%s,%s\n",
							newPatient->id, newPatient->name, newPatient->contact_info, newPatient->dob,
							newPatient->insurance_info, newPatient->emergency_contact, newPatient->date);
					fclose(fp);

					printf("\nPatient Added Successfully!\n");

					char *records[] = {
						newPatient->name, newPatient->contact_info,
						newPatient->dob, newPatient->insurance_info,
						newPatient->emergency_contact, newPatient->date
					};
					for (int i = 0; i < sizeof(records) / sizeof(records[0]); i++) {
						free(records[i]);
					}
					free(newPatient);
				}
			}
		}
	}
}
