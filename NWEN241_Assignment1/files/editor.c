#include "editor.h"
#include<string.h>


int editor_insert_char(char editing_buffer[], int editing_buflen, char to_insert, int pos){
    // Check if the position is valid
	if(pos < 0 || pos >=editing_buflen){
		return 0;
	}

    // If the position is at the end of the buffer, simply insert the character
	if(pos == editing_buflen - 1){
		editing_buffer[pos] = to_insert;
		return 1;
	}

	// Shift characters to the right starting from the end up to the insertion position
	for (int i = editing_buflen - 2; i >= pos; --i)
	{
		editing_buffer[i+1] = editing_buffer[i];
	}

	// Insert the character at the specified position
	editing_buffer[pos] = to_insert;

	return 1;
}

int editor_delete_char(char editing_buffer[], int editing_buflen, char to_delete, int offset){
	
   // Check if the offset is valid
	if(offset < 0 || offset >= editing_buflen){
		return 0;
	}

	int deletion_occured = 0;

    // Iterate through the buffer starting from the offset
	for(int i = offset; i < editing_buflen - 1; ++i){
		// If the character to delete is found and deletion hasn't occurred yet
		if(editing_buffer[i] == to_delete && deletion_occured == 0  ){
			// Shift remaining characters to the left to overwrite the deleted character
			for(int j = i; j<editing_buflen-1;++j){
				editing_buffer[j] = editing_buffer[j+1];
			}
			deletion_occured = 1;	// Mark deletion occurred
			break;     // Exit the loop after deleting the first occurrence
		}

	}
		
		 // Null terminate the string
		editing_buffer[editing_buflen-1] ='\0';

		return deletion_occured;
}

int editor_replace_str(char editing_buffer[], int editing_buflen, const char *str, const char *replacement, int offset){
	int str_len = strlen(str);
	int replacement_len = strlen(replacement);

	
	int index = offset;
	int found = 0;

	 // Check for invalid parameters
	if(str_len == 0 || offset<0 || offset >=editing_buflen){
		return -1;
	}

	// Search for the substring in the buffer
	for(int i = offset; i<=editing_buflen - str_len;++i){
		int j;
		// Check if the substring matches at the current position
		for(j = 0; j<str_len; ++j){
			if(editing_buffer[i+j] != str[j]){
				break;
			}
		}
		
		 // If the substring is found, mark the index and set the found flag
		if(j == str_len){
			index = i;
			found = 1;
			break;
		}
	}

	 // If the substring is not found, return -1
	if(!found){
		return -1;
	}

	  // Delete the found substring from the buffer
	for(int i = 0; i<str_len; ++i){
		editor_delete_char(editing_buffer,editing_buflen,str[i], index);
	}

	// Determine the end index after replacement
	int end_index = index + replacement_len - 1;
	if(end_index >= editing_buflen){
		end_index = editing_buflen - 1;
	}

	// Insert the replacement substring into the buffer
	for(int i = 0;i<replacement_len;++i){
		if(!editor_insert_char(editing_buffer,editing_buflen,replacement[i],index+i)){ //if returns 0
			return editing_buflen - 1; //The index of the last character that was successfully inserted before the failure occurred.
		}
	}

	return end_index; //Returning the index of the last character of the replaced substring

}

void editor_view(int rows, int cols, char viewing_buffer[rows][cols], const char editing_buffer[], int editing_buflen, int wrap){
	 // Initialize viewing buffer to null characters
	for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            viewing_buffer[i][j] = '\0';
        }
    }

    int row = 0;   // Current row in the viewing buffer
    int col = 0;   // Current column in the viewing buffer

     // Iterate through the editing buffer
    for (int i = 0; i < editing_buflen && editing_buffer[i] != '\0'; i++) {
        if (editing_buffer[i] == '\n') {
            // If newline character encountered, move to the next row and reset column
            row++;
            col = 0;

             // If wrapping is enabled and the row exceeds the buffer, exit loop
            if (row >= rows) {
                break; 
            }
        } else {
             // Insert character into viewing buffer if within column limit or wrapping is enabled
            if (col < cols - 1 || wrap) {
                viewing_buffer[row][col] = editing_buffer[i];
                col++;
            }
        }

        // Wrap to the next row if wrapping is enabled and the column exceeds the buffer
        if (wrap && col >= cols - 1) {
            row++;
            col = 0;

            // If wrapping is enabled and the row exceeds the buffer, exit loop
            if (row >= rows) {
                break; 
            }
        }
    }
}
	


