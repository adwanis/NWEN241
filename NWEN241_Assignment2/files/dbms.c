#include "dbms.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int db_show_row(const struct db_table *db, unsigned int row){
	// Check if the row number is valid
    if (row >= db->rows_used) {
        return 0; // Row does not exist
    }
    
    // Get the album record at the specified row
    const struct album *record = &(db->table[row]);

  	char id_str[20]; // Assuming the ID won't exceed 20 characters
    snprintf(id_str, sizeof(id_str), "%lu", record->id);
    size_t id_length = strlen(id_str);
    
    if (id_length > 6) { // Check if ID length exceeds 6 characters
        printf("%.6lu:", record->id); // Print only the first 6 digits of the ID
    }else{
        printf("%*s%lu:", (int)(6 - id_length), "", record->id); // Print ID with padding
    }
  	
  	
  	int title_length = strlen(record->title);
  	if(title_length>20){
  		printf("%.20s:", record->title);
  	}else{
  		printf("%*s%s:", (int)(20 - title_length), "", record->title);
  	}
  	

  	int artist_length = strlen(record->artist);
  	if(artist_length>20){
  		printf("%.20s:", record->artist);
  	}else{
  		printf("%*s%s:", (int)(20 - artist_length), "", record->artist); //"" is an empty string, serving as a placeholder for the artist's name.
  	}

  	char year_str[20]; 
    snprintf(year_str, sizeof(year_str), "%hu", record->year);
    size_t year_length = strlen(year_str);
    if(year_length > 4){ //Check if year length exceeds 4 characters
         printf("%.4hu\n", (int)(4 - year_length), "", record->year);// Print only the first 4 digits of the year
    }else{
         printf("%*s%hu\n", (int)(4 - year_length), "", record->year); //Print year with padding
    }
   

    return 1; // Row displayed successfully
}

int db_add_row(struct db_table *db, struct album *a){
    // Check if there is enough space in the table
    if(db->rows_used >= db->rows_total){
        // If not enough space, allocate memory for 5 additional records
        struct album *temp = realloc(db->table,(db->rows_total+5)*sizeof(struct album));
        if(temp == NULL){
            // Allocation failed, return 0
            return 0;
        }
        // Update the table pointer and rows_total
        db->table = temp;
        db->rows_total += 5;
    }
     // Store the record in the next unused row
    db->table[db->rows_used] = *a;

     // Update the rows_used
    db->rows_used++;

    return 1; // Record added successfully
}


int db_remove_row(struct db_table *db, unsigned long id){
    // Search for the record with the specified ID
    int index = -1;
    for (int i = 0; i < db->rows_used; i++) {
        if (db->table[i].id == id) {
            index = i;
            break;
        }
    }
    
    // If the record is not found, return 0 (failure)
    if (index == -1) {
        return 0;
    }
    
    // Remove the record by shifting records after the removed record up by one row
    for (int i = index; i < db->rows_used - 1; i++) {
        db->table[i] = db->table[i + 1];
    }
    
    // Decrement rows_used
    db->rows_used--;
    
    // If the number of unused rows reaches 5, release the unused memory
    if (db->rows_total - db->rows_used >= 5) {
        struct album *temp = realloc(db->table, db->rows_used * sizeof(struct album));
        if (temp != NULL) {
            db->table = temp;
            db->rows_total = db->rows_used;
        }
    }
    
    // If all records are removed, free the array
    if (db->rows_used == 0) {
        free(db->table);
        db->table = NULL;
        db->rows_total = 0;
    }

    return 1; // Removal successful
}