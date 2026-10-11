

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>



typedef struct TreeNode {
    char text[150];          
    char treatment[200];     
    struct TreeNode *yes;
    struct TreeNode *no;
} TreeNode;


TreeNode* question(const char *text, TreeNode *yes, TreeNode *no) {
    TreeNode *n = (TreeNode*)malloc(sizeof(TreeNode));
    strcpy(n->text, text);
    strcpy(n->treatment, "");
    n->yes = yes;
    n->no = no;
    return n;
}


TreeNode* diagnosis(const char *disease, const char *treatment) {
    TreeNode *n = (TreeNode*)malloc(sizeof(TreeNode));
    strcpy(n->text, disease);
    strcpy(n->treatment, treatment);
    n->yes = NULL;
    n->no = NULL;
    return n;
}

TreeNode* buildDiseaseTree() {
    
    TreeNode *leafSpots =
        question("Is there a white or grey powdery coating on the leaves?",
            diagnosis("Powdery Mildew",
                      "Spray sulphur-based fungicide and improve air flow between plants."),
        question("Are the spots orange or rust coloured?",
            diagnosis("Rust Disease",
                      "Remove infected leaves and spray a recommended fungicide."),
        question("Are the spots brown or black with ring-like patterns?",
            diagnosis("Early Blight",
                      "Remove lower infected leaves and avoid wetting leaves while watering."),
        question("Are there large dark patches that look wet and spread quickly?",
            diagnosis("Late Blight",
                      "Remove and destroy infected plants immediately and spray fungicide."),
        question("Is there a fuzzy grey or purple growth under the leaves?",
            diagnosis("Downy Mildew",
                      "Avoid overhead watering, space plants apart and spray fungicide."),
            diagnosis("Bacterial Leaf Spot",
                      "Use copper-based spray and avoid overcrowding of plants."))))));

    
    TreeNode *wilting =
        question("Are the roots brown, soft and mushy?",
            diagnosis("Root Rot",
                      "Reduce watering and improve soil drainage."),
        question("When you cut the stem, are there brown streaks inside?",
            diagnosis("Fusarium Wilt",
                      "Remove infected plants, rotate crops and use resistant varieties."),
        question("Is the soil very dry?",
            diagnosis("Water Stress",
                      "Irrigate the field and use mulch to keep moisture in the soil."),
            diagnosis("Bacterial Wilt",
                      "Remove affected plants and avoid planting the same crop there next season."))));

    
    TreeNode *yellowing =
        question("Do the leaves have a patchy light and dark green (mosaic) pattern?",
            diagnosis("Mosaic Virus",
                      "Remove infected plants and control insects that spread the virus."),
        question("Are mostly the older, lower leaves turning yellow?",
            diagnosis("Nitrogen Deficiency",
                      "Apply nitrogen-rich fertilizer such as urea or compost."),
        question("Are there tiny insects under the leaves?",
            question("Do the insects fly up when the plant is shaken?",
                diagnosis("Whitefly Infestation",
                          "Use yellow sticky traps and spray neem oil."),
                diagnosis("Aphid Infestation",
                          "Spray neem oil or soap solution on the leaves.")),
            diagnosis("Iron Deficiency",
                      "Apply iron fertilizer and check that soil pH is not too high."))));

    
    TreeNode *otherDamage =
        question("Are there holes in the stem or sawdust-like waste near it?",
            diagnosis("Stem Borer",
                      "Cut and destroy affected stems and use pheromone traps."),
        question("Are there sunken dark spots on the fruits?",
            diagnosis("Anthracnose",
                      "Remove infected fruits and spray a recommended fungicide."),
        question("Are the leaves chewed or full of holes?",
            diagnosis("Caterpillar Attack",
                      "Hand-pick caterpillars and spray neem oil."),
            diagnosis("Healthy Plant",
                      "No disease found. Keep monitoring regularly."))));

   
    return question("Are there spots or patches on the leaves?",
               leafSpots,
           question("Is the plant wilting or drooping?",
               wilting,
           question("Are the leaves turning yellow or losing colour?",
               yellowing,
               otherDamage)));
}


char getAnswer(const char *questionText) {
    char input[50];
    while (1) {
        printf("%s (y/n, e to exit): ", questionText);
        scanf("%49s", input);

        if (strlen(input) == 1) {
            char c = tolower(input[0]);
            if (c == 'y' || c == 'n' || c == 'e')
                return c;
        }
        printf("Invalid input! Please enter only 'y' or 'n' (or 'e' to exit).\n");
    }
}



#define MAX_FIELDS 50

typedef struct {
    char name[30];
    int moisture;            
} Field;

Field heap[MAX_FIELDS];
int heapSize = 0;

void swapFields(Field *a, Field *b) {
    Field temp = *a;
    *a = *b;
    *b = temp;
}

void heapInsert(const char *name, int moisture) {
    if (heapSize == MAX_FIELDS) {
        printf("Cannot add more fields.\n");
        return;
    }
    int i = heapSize++;
    strcpy(heap[i].name, name);
    heap[i].moisture = moisture;

    /
    while (i > 0 && heap[(i - 1) / 2].moisture > heap[i].moisture) {
        swapFields(&heap[i], &heap[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

Field heapExtractMin() {
    Field min = heap[0];
    heap[0] = heap[--heapSize];

    
    int i = 0;
    while (1) {
        int left = 2 * i + 1, right = 2 * i + 2, smallest = i;
        if (left < heapSize && heap[left].moisture < heap[smallest].moisture)
            smallest = left;
        if (right < heapSize && heap[right].moisture < heap[smallest].moisture)
            smallest = right;
        if (smallest == i) break;
        swapFields(&heap[i], &heap[smallest]);
        i = smallest;
    }
    return min;
}



typedef struct HarvestNode {
    char season[30];
    char crop[30];
    float quantity;         
    float income;            
    struct HarvestNode *prev;
    struct HarvestNode *next;
} HarvestNode;

HarvestNode *harvestHead = NULL;   
HarvestNode *harvestTail = NULL;   
int harvestCount = 0;


void addHarvest(const char *season, const char *crop, float quantity, float income) {
    HarvestNode *n = (HarvestNode*)malloc(sizeof(HarvestNode));
    strcpy(n->season, season);
    strcpy(n->crop, crop);
    n->quantity = quantity;
    n->income = income;
    n->next = NULL;
    n->prev = harvestTail;

    if (harvestHead == NULL)
        harvestHead = n;
    else
        harvestTail->next = n;
    harvestTail = n;
    harvestCount++;
}


void deleteHarvest(const char *season) {
    HarvestNode *cur = harvestHead;

    while (cur != NULL && strcmp(cur->season, season) != 0)
        cur = cur->next;

    if (cur == NULL) {
        printf("No record found for %s.\n", season);
        return;
    }

    if (cur->prev != NULL) cur->prev->next = cur->next;
    else harvestHead = cur->next;            

    if (cur->next != NULL) cur->next->prev = cur->prev;
    else harvestTail = cur->prev;            

    free(cur);
    harvestCount--;
    printf("Record for %s deleted.\n", season);
}

void printHarvest(HarvestNode *h) {
    printf("Season   : %s\n", h->season);
    printf("Crop     : %s\n", h->crop);
    printf("Quantity : %.1f quintals\n", h->quantity);
    printf("Income   : Rs. %.2f\n", h->income);
}


void showAllHarvests(int newestFirst) {
    if (harvestHead == NULL) {
        printf("No harvest records.\n");
        return;
    }
    HarvestNode *cur = newestFirst ? harvestTail : harvestHead;
    float total = 0;

    while (cur != NULL) {
        printf("\n");
        printHarvest(cur);
        total += cur->income;
        cur = newestFirst ? cur->prev : cur->next;
    }
    printf("\nTotal records: %d\n", harvestCount);
    printf("Total income : Rs. %.2f\n", total);
}



#define GODOWN_CAPACITY 10   

typedef struct {
    char crop[30];
    float weight;            
} Sack;

Sack godown[GODOWN_CAPACITY];
int top = -1;               

int isGodownEmpty() {
    return top == -1;
}

int isGodownFull() {
    return top == GODOWN_CAPACITY - 1;
}


void pushSack(const char *crop, float weight) {
    if (isGodownFull()) {
        printf("Godown is full! Cannot store more sacks. (Stack Overflow)\n");
        return;
    }
    top++;
    strcpy(godown[top].crop, crop);
    godown[top].weight = weight;
}


void popSack() {
    if (isGodownEmpty()) {
        printf("Godown is empty! No sacks to remove. (Stack Underflow)\n");
        return;
    }
    printf("Removed sack: %s (%.1f kg)\n", godown[top].crop, godown[top].weight);
    top--;
}


void peekSack() {
    if (isGodownEmpty()) {
        printf("Godown is empty.\n");
        return;
    }
    printf("Top sack: %s (%.1f kg)\n", godown[top].crop, godown[top].weight);
}


void showGodown() {
    if (isGodownEmpty()) {
        printf("Godown is empty.\n");
        return;
    }
    float total = 0;
    printf("\nSacks from TOP to BOTTOM:\n");
    for (int i = top; i >= 0; i--) {
        printf("%2d. %-12s %.1f kg%s\n", top - i + 1, godown[i].crop,
               godown[i].weight, (i == top) ? "   <-- TOP" : "");
        total += godown[i].weight;
    }
    printf("\nSacks stored : %d / %d\n", top + 1, GODOWN_CAPACITY);
    printf("Total weight : %.1f kg\n", total);
}



#define WINDOW 5             

float readings[WINDOW];
int front = 0, rear = -1, count = 0;

void enqueue(float x) {
    rear = (rear + 1) % WINDOW;
    readings[rear] = x;
    count++;
}

float dequeue() {
    float x = readings[front];
    front = (front + 1) % WINDOW;
    count--;
    return x;
}

float averageReading() {
    float sum = 0;
    for (int i = 0; i < count; i++)
        sum += readings[(front + i) % WINDOW];
    return sum / count;
}

void showReadings() {
    if (count == 0) {
        printf("No readings yet.\n");
        return;
    }
    printf("Last %d readings: ", count);
    for (int i = 0; i < count; i++)
        printf("%.1f  ", readings[(front + i) % WINDOW]);
    printf("\nAverage: %.1f C\n", averageReading());
}

void addReading(float temp) {
    if (count > 0) {
        float avg = averageReading();
        if (temp - avg > 5 || avg - temp > 5)
            printf("ALERT: Sudden temperature change! (average was %.1f C)\n", avg);
    }
    if (temp <= 4)
        printf("ALERT: Frost risk! Cover sensitive crops.\n");

    if (count == WINDOW)
        dequeue();                
    enqueue(temp);

    showReadings();
}



void diseaseMenu(TreeNode *root) {
    char again;

    do {
        TreeNode *node = root;
        printf("\n--- Crop Disease Diagnosis ---\n");
        printf("Answer each question with y or n. Enter e at any time to exit.\n\n");

        while (node->yes != NULL) {          
            char ans = getAnswer(node->text);
            if (ans == 'e') {
                printf("Exiting diagnosis.\n");
                return;
            }
            node = (ans == 'y') ? node->yes : node->no;
        }

        printf("\nDiagnosis : %s\n", node->text);
        printf("Treatment : %s\n", node->treatment);

        again = getAnswer("\nDiagnose another plant?");
    } while (again == 'y');
}

void irrigationMenu() {
    int choice, moisture;
    char name[30];

    do {
        printf("\n--- Irrigation Scheduler ---\n");
        printf("1. Add field\n2. Water the driest field\n3. View all fields\n0. Back\n");
        printf("Choice: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            printf("Field name (one word): ");
            scanf("%29s", name);
            printf("Soil moisture (%%): ");
            scanf("%d", &moisture);
            heapInsert(name, moisture);
            printf("Field added.\n");
            break;
        case 2:
            if (heapSize == 0) {
                printf("No fields waiting.\n");
            } else {
                Field f = heapExtractMin();
                printf("Watering %s (moisture %d%%).\n", f.name, f.moisture);
            }
            break;
        case 3:
            if (heapSize == 0) printf("No fields waiting.\n");
            for (int i = 0; i < heapSize; i++)
                printf("%s - %d%%\n", heap[i].name, heap[i].moisture);
            break;
        }
    } while (choice != 0);
}

void browseHarvests() {
    if (harvestHead == NULL) {
        printf("No harvest records.\n");
        return;
    }
    HarvestNode *cur = harvestHead;
    int position = 1;
    char input[50];

    while (1) {
        printf("\n--- Record %d of %d ---\n", position, harvestCount);
        printHarvest(cur);
        printf("\nn = next, p = previous, e = exit: ");
        scanf("%49s", input);

        char c = (strlen(input) == 1) ? tolower(input[0]) : '?';

        if (c == 'n') {
            if (cur->next != NULL) { cur = cur->next; position++; }
            else printf("This is the latest record.\n");
        } else if (c == 'p') {
            if (cur->prev != NULL) { cur = cur->prev; position--; }
            else printf("This is the oldest record.\n");
        } else if (c == 'e') {
            return;
        } else {
            printf("Invalid input! Please enter only 'n', 'p' or 'e'.\n");
        }
    }
}

void harvestMenu() {
    int choice;
    char season[30], crop[30];
    float quantity, income;

    do {
        printf("\n--- Harvest Records ---\n");
        printf("1. Add harvest record\n");
        printf("2. Browse records (next/previous)\n");
        printf("3. View all (oldest first)\n");
        printf("4. View all (newest first)\n");
        printf("5. Delete a record\n");
        printf("0. Back\n");
        printf("Choice: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            printf("Season (one word, e.g. Monsoon2025): ");
            scanf("%29s", season);
            printf("Crop: ");
            scanf("%29s", crop);
            printf("Quantity (quintals): ");
            scanf("%f", &quantity);
            printf("Income (Rs.): ");
            scanf("%f", &income);
            addHarvest(season, crop, quantity, income);
            printf("Record added.\n");
            break;
        case 2:
            browseHarvests();
            break;
        case 3:
            showAllHarvests(0);
            break;
        case 4:
            showAllHarvests(1);
            break;
        case 5:
            printf("Season to delete: ");
            scanf("%29s", season);
            deleteHarvest(season);
            break;
        }
    } while (choice != 0);
}

void godownMenu() {
    int choice;
    char crop[30];
    float weight;

    do {
        printf("\n--- Grain Storage Godown ---\n");
        printf("1. Store a sack (push)\n");
        printf("2. Remove top sack (pop)\n");
        printf("3. Check top sack (peek)\n");
        printf("4. View all sacks\n");
        printf("0. Back\n");
        printf("Choice: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            if (isGodownFull()) {
                printf("Godown is full! Cannot store more sacks. (Stack Overflow)\n");
                break;
            }
            printf("Crop name: ");
            scanf("%29s", crop);
            printf("Weight (kg): ");
            scanf("%f", &weight);
            if (weight <= 0) {
                printf("Weight must be more than 0.\n");
                break;
            }
            pushSack(crop, weight);
            printf("Sack of %s stored on top.\n", crop);
            break;
        case 2:
            popSack();
            break;
        case 3:
            peekSack();
            break;
        case 4:
            showGodown();
            break;
        }
    } while (choice != 0);
}

void sensorMenu() {
    int choice;
    float temp;

    do {
        printf("\n--- Sensor Monitoring ---\n");
        printf("1. Add temperature reading\n2. View readings\n0. Back\n");
        printf("Choice: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            printf("Temperature (C): ");
            scanf("%f", &temp);
            addReading(temp);
            break;
        case 2:
            showReadings();
            break;
        }
    } while (choice != 0);
}

int main() {
    
    TreeNode *diseaseTree = buildDiseaseTree();

    heapInsert("Sugarcane", 35);
    heapInsert("Cotton", 18);
    heapInsert("Onion", 27);
    heapInsert("Soybean", 12);

    addHarvest("Monsoon2023", "Soybean", 12, 54000);
    addHarvest("Winter2023", "Wheat", 15, 33000);
    addHarvest("Monsoon2024", "Cotton", 10, 70000);
    addHarvest("Winter2024", "Chickpea", 8, 44000);

    pushSack("Wheat", 50);
    pushSack("Rice", 50);
    pushSack("Soybean", 40);

    enqueue(28.0);
    enqueue(29.5);
    enqueue(30.0);

    int choice;
    do {
        printf("\n==============================\n");
        printf("      SMART FARM MANAGER\n");
        printf("==============================\n");
        printf("1. Crop Disease Diagnosis\n");
        printf("2. Irrigation Scheduler\n");
        printf("3. Harvest Records\n");
        printf("4. Grain Storage Godown\n");
        printf("5. Sensor Monitoring\n");
        printf("0. Exit\n");
        printf("Choice: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1: diseaseMenu(diseaseTree); break;
        case 2: irrigationMenu(); break;
        case 3: harvestMenu(); break;
        case 4: godownMenu(); break;
        case 5: sensorMenu(); break;
        case 0: printf("Goodbye!\n"); break;
        default: printf("Invalid choice.\n");
        }
    } while (choice != 0);

    return 0;
}
