#ifndef _CONFIG_H
#define _CONFIG_H

#include <climits>

// Settings for Person params
// age
#define MAX_AGE 120
#define MIN_AGE 0

// Settings for Borrower params
#define MAX_ITEMS_PER_BORROWER_HARD 10 // Hard limit, cannot be changed higher. Can be set to -1 for disabled
#define MAX_ITEMS_PER_BORROWER_DEFAULT 5 // Default at init, dont set to -1

// Constant for media id
#define INVALID_MEDIA_ID ULONG_MAX // Use maximum value as sentinel

#endif