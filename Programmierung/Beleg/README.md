# Library Directory Readme

## Project outline

1. s88216.c
2. web.c
    - methods for parsing post request, handling get and post request
    - table printing method
3. media.c
    - defines tMedia structure
    - method for creating tMedia from query string
    - printing method
    - I/O methods
4. idxlist.c
    - double ring list implementation with pointer and index to current selected item
5. utility.c
    - includes methods used through the project in different libraries
6. logger.h
7. config.h
    - macros for **debugging**, **media_path**, **html_path** and csv **DELIMITER**
8. index.html

## Data Files

1. media.csv
2. log.txt (if flag -DEBUG is set during compilation)

## Configuration options
**change...**
- path of image folder with the **IMAGE_BASE_URL** variable in the **index.html** file
- path of cgi file with the **POST_URL** variable in the **index.html** file
- the column delimiter in the media.csv file with the **DELIMITER** makro in **config.h** file
    - Attention: If the **DELIMITER** doesnt match the current delimiter in the **media.csv** file,
    the programm cannot parse the **media.csv** file correctly and will overwrite it with nothing in worst case
- path of **index.html** file in the **config.h** file
- path of **media.csv** file in the **config.h** file

**activate/deactivate debuging...**
- **permanently** by outcommenting/commenting **#define DEBUG** in the **config.h** file
- **temporarily** by compiling with the **-DEBUG** flag

## Compiling Command
gcc -o s88216.cgi s88216.c idxlist.c media.c web.c utility.c
## with DEBUGING activated
gcc -o s88216.cgi s88216.c idxlist.c media.c web.c utility.c -DEBUG

## Usage
- check that permissions for the image folder, media.csv file and s88216.cgi file are set correctly
- in the browser direct to the server adress, where the s88216.cgi file is placed.
- For **searching**, just type the query into the top search bar
- **Sorting** can be changed via the blue dropdown menu
- **Add items** by clicking the plus button to open the add overlay. Type in the required attributes and press the submit button. Please check that the date is in the required format, for correct sorting by the date
- For **specific row deletion** click on the lines which you want to delete to color them permanently light blue. Then press the trashcan button.
- To **delete all** rows, unselect all rows which are currently selected and press the trashcan button. A popup occurs and asks for agreement. Press yes to confirm the action.
