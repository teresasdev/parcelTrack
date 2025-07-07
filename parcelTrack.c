#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_NAME_LENGTH 100

typedef struct hub
{
    char name[MAX_NAME_LENGTH]; // name of city or location.
    int distance;               // distance from previous hub in km.
    int hour;                   // simple time in hours (0-23).
    struct hub *next;
} Hub;

int is_empty(Hub *head);
void insert_at_head(Hub **head, const char *name, int distance, int hour);
void insert_at_tail(Hub **head, const char *name, int distance, int hour);
void print_chain(Hub *head);
int total_distance(Hub *head);
void print_chain_reverse(Hub *head);
Hub search_hub(Hub *head, const char *keyword);
void display_bottleneck(Hub *head);
void remove_first_match(Hub **head, const char *keyword);
void deleteList(Hub *head);

int is_empty(Hub *head)
{
    return head == NULL;
}

// creates a new hub node and inserts it at beginning of list.
void insert_at_head(Hub **head, const char *name, int distance, int hour)
{
    // create a new hub node.
    Hub *new_hub = (Hub *)malloc(sizeof(Hub));
    // check if memory allocation was successful.
    if (new_hub == NULL)
    {
        printf("Memory allocation failed\n");
        return;
    }

    snprintf(new_hub->name, MAX_NAME_LENGTH, "%s", name);
    new_hub->distance = distance;
    new_hub->hour = hour;
    new_hub->next = *head; // point new hub to current head.
    *head = new_hub;       // update head to point to new hub.
}

// creates a new hub node and inserts it at end of list recursively.
void insert_at_tail(Hub **head, const char *name, int distance, int hour)
{
    // creates new hub node.
    Hub *new_hub = (Hub *)malloc(sizeof(Hub));
    // checks if memory allocation was successful.
    if (!new_hub)
    {
        printf("Memory allocation failed\n");
        return;
    }

    snprintf(new_hub->name, MAX_NAME_LENGTH, "%s", name); // uses snprintf to safely copy name.
    new_hub->distance = distance;                         // sets distance.
    new_hub->hour = hour;                                 // sets hour.
    new_hub->next = NULL;
    // if empty, set new hub as head.
    if (is_empty(*head))
    {
        *head = new_hub;
        return;
    }
    // otherwise, finds last hub and link new hub.
    Hub *current = *head;
    while (current->next)
    {
        current = current->next; // traverses to last hub.
    }
    current->next = new_hub; // links new hub at end.
}

// iteratively prints all hubs in order. each to be displayed in the format: [HubName] - [Distance] km, [Hour]h
void print_chain(Hub *head)
{
    Hub *current = head;
    while (current)
    {
        printf("%s - %d km, %dh\n", current->name, current->distance, current->hour);
        current = current->next; // moves to next hub.
    }
}

// returns total sum of all distance values across list.
// Note: This function can be implemented either iteratively or recursively.
int total_distance(Hub *head)
{
    int total = 0;       // initializes total distance.
    Hub *current = head; // starts from head of list.
    while (current)
    {                               // traverses list.
        total += current->distance; // adds distance of current hub.
        current = current->next;    // moves to next hub.
    }
    return total; // returns total distance.
}

// recursively prints all hubs from end to beginning.
void print_chain_reverse(Hub *head)
{
    if (head == NULL)
    {
        return; // base case: if list is empty, return.
    }
    else
    {
        print_chain_reverse(head->next);                                     // recursive call to next hub.
        printf("%s - %d km, %dh\n", head->name, head->distance, head->hour); // print current hub after recursive call.
    }
}

// finds and prints two hubs in chain with max hours in between.
void display_bottleneck(Hub *head)
{
    if (is_empty(head))
    {
        printf("Delivery chain is empty.\n");
        return; // if list is empty, return.
    }

    Hub *current = head;
    int max_hours = 0;
    Hub *bottleneck_start = NULL;
    Hub *bottleneck_end = NULL;

    while (current && current->next)
    {
        int hours_spent = current->next->hour - current->hour; // calculates hours spent between hubs.
        if (hours_spent > max_hours)
        {
            max_hours = hours_spent;        // updates max hours if found a new maximum.
            bottleneck_start = current;     // updates start hub of bottleneck.
            bottleneck_end = current->next; // updates end hub of bottleneck.
        }
        current = current->next; // moves to next hub.
    }

    if (bottleneck_start && bottleneck_end)
    {
        printf("Bottleneck found: %s to %s, Hours spent: %d\n",
               bottleneck_start->name,
               bottleneck_end->name,
               max_hours);
    }
    else
    {
        printf("No bottleneck found.\n");
    }
}

Hub search_hub(Hub *head, const char *keyword)
{
    Hub *current = head;
    while (current)
    {
        if (strstr(current->name, keyword))
        {
            return *current;
        }
        current = current->next;
    }

    Hub not_found = {"None", -1, -1, NULL};
    return not_found;
}

// removes first hub that matches keyword from list.
// Frees removed node. If no match is found, original list is unchanged.
void remove_first_match(Hub **head, const char *keyword)
{
    if (is_empty(*head))
    {
        return; // If list is empty, do nothing.
    }

    Hub *current = *head;
    Hub *previous = NULL;

    while (current)
    {
        if (strstr(current->name, keyword))
        { // checks if current hub's name contains the keyword.
            if (previous)
            {
                previous->next = current->next; // bypasses current hub.
            }
            else
            {
                *head = current->next; // if it's the head, updates head pointer.
            }
            free(current); // frees memory of removed hub.
            return;        // exits after removing first match.
        }
        previous = current; // move to next hub.
        current = current->next;
    }
    printf("No match found for keyword: %s\n", keyword);
}

int main()
{
    printf("=== ParcelTrack Delivery Chain Test ===\n\n");
    Hub *chain = NULL;

    // Test insert_at_head and insert_at_tail
    insert_at_head(&chain, "Orlando", 0, 9);
    insert_at_tail(&chain, "Atlanta", 400, 14);
    insert_at_tail(&chain, "Charlotte", 350, 17);

    printf("Delivery chain:\n");
    print_chain(chain);
    printf("\n");

    // Test total_distance
    int total = total_distance(chain);
    printf("Total distance: %d km\n\n", total);

    display_bottleneck(chain);

    // Test search_hub
    Hub found = search_hub(chain, "lanta");
    if (strcmp(found.name, "None"))
    {
        printf("Search result: Found ");
        printf("\"{%s, %d, %d}\"\n\n", found.name, found.distance, found.hour);
    }
    else
        printf("Search result: No match found\n\n");

    // Test print_chain_reverse
    printf("Reverse delivery chain:\n");
    print_chain_reverse(chain);
    printf("\n");

    // Test remove_first_match
    remove_first_match(&chain, "lanta");
    printf("After removing first match for \"lanta\":\n");
    print_chain(chain);
    printf("\n");

    deleteList(chain);
    chain = NULL;
    return 0;
}

void deleteList(Hub *head)
{
    Hub *temp;
    while (head != NULL)
    {
        temp = head;
        head = head->next;
        free(temp);
    }
}
