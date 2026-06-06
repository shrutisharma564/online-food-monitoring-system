#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct FoodItem {
    int id;
    char name[50];
    int quantity;
    float price;
};

struct Feedback {
    char name[50];
    char comment[200];
};

void showInventory() {
    FILE *fp = fopen("inventory.txt", "rb");
    struct FoodItem item;

    if (fp == NULL) {
        printf("No inventory found.\n");
        return;
    }

    printf("\n--- Inventory ---\n");
    while (fread(&item, sizeof(item), 1, fp)) {
        printf("ID: %d | Name: %s | Qty: %d | Price: %.2f\n", item.id, item.name, item.quantity, item.price);
    }

    fclose(fp);
}

void placeOrder() {
    int id, qty, found = 0;
    struct FoodItem item;

    FILE *fp = fopen("inventory.txt", "rb");
    FILE *temp = fopen("temp.txt", "wb");

    if (fp == NULL || temp == NULL) {
        printf("Error opening files.\n");
        return;
    }

    printf("Enter Food ID: ");
    scanf("%d", &id);
    printf("Enter Quantity: ");
    scanf("%d", &qty);

    while (fread(&item, sizeof(item), 1, fp)) {
        if (item.id == id) {
            found = 1;
            if (item.quantity >= qty) {
                item.quantity -= qty;
                printf("Order placed for %d x %s\n", qty, item.name);
            } else {
                printf("Not enough stock for %s\n", item.name);
            }
        }
        fwrite(&item, sizeof(item), 1, temp);
    }

    fclose(fp);
    fclose(temp);

    remove("inventory.txt");
    rename("temp.txt", "inventory.txt");

    if (!found) {
        printf("Food item not found.\n");
    }
}

void addFeedback() {
    struct Feedback fb;
    FILE *fp = fopen("feedback.txt", "ab");

    if (fp == NULL) {
        printf("Error opening feedback file.\n");
        return;
    }

    printf("Enter your name: ");
    scanf(" %[^\n]", fb.name);
    printf("Enter your feedback: ");
    scanf(" %[^\n]", fb.comment);

    fwrite(&fb, sizeof(fb), 1, fp);
    fclose(fp);
    printf("Feedback added.\n");
}

void viewFeedback() {
    struct Feedback fb;
    FILE *fp = fopen("feedback.txt", "rb");

    if (fp == NULL) {
        printf("No feedback found.\n");
        return;
    }

    printf("\n--- Feedback ---\n");
    while (fread(&fb, sizeof(fb), 1, fp)) {
        printf("Name: %s\nFeedback: %s\n\n", fb.name, fb.comment);
    }

    fclose(fp);
}

void initializeInventory() {
    FILE *fp = fopen("inventory.txt", "wb");
    struct FoodItem items[7] = {
        {1, "Burger", 20, 120.0},
        {2, "Pizza", 15, 250.0},
        {3, "Pasta", 10, 180.0},
        {4, "momos", 20, 100.0},
        {5, "pastry", 50, 70.0},
        {6, "non veg", 20, 120.0},
        {7, "noodles", 70, 50.0}
    };

    if (fp == NULL) {
        printf("Error creating inventory.\n");
        return;
    }

    for (int i = 0; i < 7; i++) {
        fwrite(&items[i], sizeof(items[i]), 1, fp);
    }

    fclose(fp);
    printf("Inventory initialized.\n");
}

int main() {
    int choice;

    do {
        printf("\n--- Online Food Monitoring System ---\n");
        printf("1. Initialize Inventory\n");
        printf("2. Show Inventory\n");
        printf("3. Place Order\n");
        printf("4. Add Feedback\n");
        printf("5. View Feedback\n");
        printf("6. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: initializeInventory(); break;
            case 2: showInventory(); break;
            case 3: placeOrder(); break;
            case 4: addFeedback(); break;
            case 5: viewFeedback(); break;
            case 6: printf("Exiting...\n"); break;
            default: printf("Invalid choice.\n");
        }
    } while (choice != 6);

    return 0;
}
