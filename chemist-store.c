#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int id;
    char name[50];
    float price;
    int quantity;
    char expiry[11]; // Format: YYYY-MM-DD
} Medicine;

void addMedicine();
void viewMedicines();
void sellMedicine();
void deleteMedicine();

const char *FILENAME = "medicine.dat";

int main() {
    int choice;
    while (1) {
        printf("\n==== Medicine Shop ====\n");
        printf("1. Add Medicine\n2. View Medicines\n3. Sell Medicine\n4. Delete Medicine\n5. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1: addMedicine(); break;
            case 2: viewMedicines(); break;
            case 3: sellMedicine(); break;
            case 4: deleteMedicine(); break;
            case 5: exit(0);
            default: printf("Invalid choice!\n");
        }
    }
    return 0;
}

void addMedicine() {
    FILE *fp = fopen(FILENAME, "ab");
    Medicine m;
    printf("Enter ID: "); scanf("%d", &m.id);
    printf("Enter Name: "); scanf(" %[^\n]", m.name);
    printf("Enter Price: "); scanf("%f", &m.price);
    printf("Enter Quantity: "); scanf("%d", &m.quantity);
    printf("Enter Expiry (YYYY-MM-DD): "); scanf(" %s", m.expiry);

    fwrite(&m, sizeof(Medicine), 1, fp);
    fclose(fp);
    printf("Medicine added successfully.\n");
}

void viewMedicines() {
    FILE *fp = fopen(FILENAME, "rb");
    Medicine m;
    printf("\n%-5s %-20s %-8s %-10s %-12s\n", "ID", "Name", "Price", "Quantity", "Expiry");
    while (fread(&m, sizeof(Medicine), 1, fp)) {
        printf("%-5d %-20s %-8.2f %-10d %-12s\n", m.id, m.name, m.price, m.quantity, m.expiry);
    }
    fclose(fp);
}

void sellMedicine() {
    int id, qty;
    FILE *fp = fopen(FILENAME, "rb+");
    Medicine m;
    int found = 0;
    printf("Enter Medicine ID to sell: "); scanf("%d", &id);
    printf("Enter quantity to sell: "); scanf("%d", &qty);
    while (fread(&m, sizeof(Medicine), 1, fp)) {
        if (m.id == id) {
            found = 1;
            if (m.quantity >= qty) {
                m.quantity -= qty;
                fseek(fp, -sizeof(Medicine), SEEK_CUR);
                fwrite(&m, sizeof(Medicine), 1, fp);
                printf("Medicine sold.\n");
            } else {
                printf("Insufficient stock!\n");
            }
            break;
        }
    }
    fclose(fp);
    if (!found) printf("Medicine not found.\n");
}

void deleteMedicine() {
    int id;
    FILE *fp = fopen(FILENAME, "rb");
    FILE *temp = fopen("temp.dat", "wb");
    Medicine m;
    int found = 0;
    printf("Enter Medicine ID to delete: "); scanf("%d", &id);
    while (fread(&m, sizeof(Medicine), 1, fp)) {
        if (m.id != id) {
            fwrite(&m, sizeof(Medicine), 1, temp);
        } else {
            found = 1;
        }
    }
    fclose(fp);
    fclose(temp);
    remove(FILENAME);
    rename("temp.dat", FILENAME);
    if (found)
        printf("Medicine deleted.\n");
    else
        printf("Medicine not found.\n");
}
