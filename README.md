I built **ParcelTrack** as a logistics management system for tracking package delivery chains across cities. The project started with a scenario where a database failure wiped out all delivery tracking data. I had to rebuild the entire system using C and linked lists from scratch.
### Key features

- **Dynamic route management**: Ability to add and remove delivery hubs in real-time.
- **Performance analytics**: A system to find bottlenecks and calculate total distance.
- **Flexible search**: Pattern matching to help find specific hubs.
- **Memory-efficient design**: Proper memory allocation and cleanup.
## Problem Statement

When I first read this assignment I thought it would be straightforward. Then I realized how many edge cases exist in a real logistics system. What happens when the list is empty? How do you handle memory allocation failures? These questions shaped my entire approach.

The scenario simulates what happens when database systems fail. Companies need backup tracking methods during outages. My system rebuilds delivery tracking using fundamental data structures instead of relying on external databases.

## Technical Architecture

### Data structure design (provided)

```c
#define MAX_NAME_LEN 100
typedef struct hub {
 char name[MAX_NAME_LEN]; // city or location name
 int distance; // distance from previous hub
 int hour; // simplified timestamp (0–23)
 struct hub* next;
} Hub;
```

### Core components

| Component             | Purpose                             | Implementation                        |
| --------------------- | ----------------------------------- | ------------------------------------- |
| **Hub management**    | Add and remove delivery points      | Linked list with head/tail operations |
| **Route analysis**    | Calculate distances and find delays | Traversal algorithms                  |
| **Search engine**     | Find hubs by name patterns          | String matching functions             |
| **Memory management** | Prevent memory leaks                | Allocation and cleanup routines       |
## Implementation Highlights

### 1. Dynamic hub insertion

**Head insertion** - O(1) complexity for urgent deliveries:

```c
void insert_at_head(Hub **head, const char *name, int distance, int hour)
```

I implemented head insertion first because it's simpler. The tricky part was remembering to update the head pointer correctly. 

**Tail insertion** - O(n) complexity for standard route building:

```c
void insert_at_tail(Hub **head, const char *name, int distance, int hour)
```

Tail insertion gave me more trouble. My first attempt used recursion but I switched to iteration for better memory usage. 
### 2. Route analysis

**Bottleneck detection** - Finds delivery delays: 
This function analyzes time differences between consecutive hubs. I iterate through the chain and track the maximum time difference. The algorithm identifies which segment has the longest delay.

**Total distance calculation** - Measures complete route length: 
The function traverses the entire chain once and adds up all distance values. I considered using recursion here but iteration felt more natural for this problem.

### 3. Search system

**Pattern matching** - Hub identification: I use `strstr()` for substring matching. The function returns complete hub information when matches are found. My professor suggested this approach.

The search function taught me about returning struct values versus pointers. I return a struct by value with a sentinel "None" value when no match exists.

### 4. Memory management

**Cleanup** - Memory handling:

```c
void deleteList(Hub *head)  // Complete chain deallocation
void remove_first_match()   // Selective node removal with proper cleanup
```

Memory management was where I learned the most. My first version had memory leaks everywhere. I spent hours with debugging tools figuring out where I forgot to call `free()`. The remove function was particularly tricky because I had to handle three cases: removing the head, removing the tail, and removing a middle node.

## Testing & Validation

### Test case execution

I wrote the test cases to match the expected output format. The main function tests all the major operations in sequence.

**Input scenario:**

```c
insert_at_head(&chain, "Orlando", 0, 9);
insert_at_tail(&chain, "Atlanta", 400, 14);
insert_at_tail(&chain, "Charlotte", 350, 17);
```

**Expected output:**

```md
ParcelTrack Delivery Chain Test

Delivery chain:
Orlando - 0 km, 9h
Atlanta - 400 km, 14h
Charlotte - 350 km, 17h

Total distance: 750 km

Bottleneck found: Orlando to Atlanta, Hours spent: 5
Search result: Found "{Atlanta, 400, 14}"

Reverse delivery chain:
Charlotte - 350 km, 17h
Atlanta - 400 km, 14h
Orlando - 0 km, 9h

After removing first match for "lanta":
Orlando - 0 km, 9h
Charlotte - 350 km, 17h
```
## Performance Characteristics

| Operation            | Time Complexity | Space Complexity | Use Case                 |
| -------------------- | --------------- | ---------------- | ------------------------ |
| Insert at Head       | O(1)            | O(1)             | Priority routing         |
| Insert at Tail       | O(n)            | O(1)             | Standard delivery        |
| Search Hub           | O(n)            | O(1)             | Hub lookup               |
| Total Distance       | O(n)            | O(1)             | Route analysis           |
| Bottleneck Detection | O(n)            | O(1)             | Performance optimization |
| Reverse Print        | O(n)            | O(n)             | Route verification       |

### Quality assurance

I check for memory allocation failures in every function that calls `malloc()`. Edge cases like empty lists get handled with early returns. For string safety I use `snprintf()` instead of `strcpy()` to prevent buffer overflows.

The most challenging bug was in my remove function. I wasn't updating the previous pointer correctly which caused segmentation faults. Debugging that taught me to always draw diagrams when working with linked lists.
