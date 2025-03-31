# Hospital Patient Record System & Traffic Management System

## Table of Contents
1. [Hospital Patient Record System](#1-hospital-patient-record-system)
   - [Overview](#overview)
   - [Core Data Structures](#core-data-structures)
   - [Workflow](#workflow)
   - [Error Handling](#error-handling)
   - [Memory Management](#memory-management)
2. [Traffic Management System](#2-traffic-management-system)
   - [Overview](#overview-1)
   - [Core Data Structures](#core-data-structures-1)
   - [Workflow](#workflow-1)
   - [Traffic Signal Adjustment](#traffic-signal-adjustment)
   - [Memory Management](#memory-management-1)
   - [Error Handling](#error-handling-1)

---

# 1. Hospital Patient Record System

## Overview
This C program manages hospital patient records with dynamic memory allocation for efficient handling of patient data. It supports:
- Adding new patients
- Updating medical histories
- Removing discharged patients
- Viewing all records

## Core Data Structures
### Medical History
Dynamically stores medical notes.

| Field    | Type  | Description |
|----------|-------|-------------|
| details  | char* | Medical notes |
| capacity | size_t | Total allocated memory |
| length   | size_t | Used memory |

### Patient
Stores patient details.

| Field         | Type  | Description |
|--------------|-------|-------------|
| id           | int   | Unique identifier |
| name         | char[MAX_NAME_LENGTH] | Patient name |
| age          | int   | Age (1-119) |
| diagnosis    | char[MAX_DIAGNOSIS_LENGTH] | Medical condition |
| medicalHistory | MedicalHistory | Medical notes |

### Patient Database
Manages patients dynamically.

| Field    | Type  | Description |
|----------|------|-------------|
| patients | Patient* | Dynamic patient list |
| count    | size_t | Current count |
| capacity | size_t | Allocated slots |

## Workflow
### Adding a Patient
1. User inputs name, age, diagnosis, and ID.
2. System validates input, expands database, and initializes history.

### Updating Medical History
1. User provides patient ID and new notes.
2. System appends notes dynamically.

### Removing a Patient
1. User provides ID.
2. System frees memory and updates the database.

## Error Handling
- **Failed Allocations**:
  ```c
  if (newPatients == NULL) {
      fprintf(stderr, "Memory allocation failed!\n");
      exit(EXIT_FAILURE);
  }
  ```
- **Invalid Inputs**: Re-prompts user.
- **NULL Checks**: Prevents crashes.

## Memory Management
- `malloc()` and `free()` ensure proper allocation.
- **Automatic Shrinking** reduces memory use.

---

# 2. Traffic Management System

## Overview
This C program simulates a traffic management system with dynamic sensor handling and adaptive traffic signal control. It:
- Manages dynamic sensors
- Updates traffic signals based on sensor data
- Frees unused resources

## Core Data Structures
### Traffic Signal
Manages signal states and duration.

| Field    | Type  | Description |
|----------|------|-------------|
| state    | LightState | Signal state (RED, GREEN, YELLOW) |
| duration | int  | Signal duration |

### Traffic Sensor
Tracks vehicle counts using a linked list.

| Field        | Type       | Description |
|-------------|-----------|-------------|
| id          | int       | Unique ID |
| vehicle_count | int    | Vehicle count |
| last_update  | time_t   | Last update time |
| next        | SensorNode* | Next sensor in list |

## Workflow
### Adding a Sensor
1. `create_sensor(id)` adds a sensor dynamically.
2. Sensor starts with vehicle count `0`.

### Updating Sensor Data
1. Updates vehicle count for a sensor.
2. Validates input and stores timestamps.

### Deleting a Sensor
1. Frees memory and updates the linked list.

## Traffic Signal Adjustment
1. Calculates median vehicle count.
2. Adjusts signal based on:
   - **Green (45s)**: Median > 10
   - **Yellow (5s)**: Median 5-10
   - **Red (20s)**: Median ≤ 5

## Memory Management
- **Dynamic Allocation**:`malloc()` and `free()` ensure proper allocation.
- **Cleanup**: `cleanup_resources()` frees all memory on exit.

## Error Handling
- **Memory Allocation Failure**: Shows an error message.
- **Invalid Inputs**: Prevents buffer overflow and ensures valid data.


