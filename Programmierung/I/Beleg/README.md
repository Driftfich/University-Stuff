# Library Directory Readme

## Author
Franz Rehschuh (Matr.-Nr.:MATRICULATION_NUMBER) (s-Nr.:s88216)

## Project outline

1. **s88216.c**
2. **web.c + web.h**
    - methods for parsing post request, handling get and post request
    - table printing method
3. **media.c + media.h**
    - defines tMedia structure
    - method for creating tMedia from query string
    - printing method
    - I/O methods
4. **idxlist.c + list.h**
    - double ring list implementation with pointer and index to current selected item
5. **utility.c + utility.h**
    - includes methods used through the project in different libraries
6. **logger.h**
7. **config.h**
    - macros for **debugging**, **media_path**, **html_path**, **MAX_INPUT_LENGTH**, **MAX_POSTD_LENGTH** and csv file **DELIMITER**
8. **index.html**

## Data Files

1. media.csv
2. log.txt (if flag -DDEBUG is set during compilation and debugging is run due to an error)

## Configuration options
**change...**
- path of image folder with the **IMAGE_BASE_URL** variable in the **index.html** file
- path of cgi file with the **POST_URL** variable in the **index.html** file
- the column delimiter in the media.csv file with the **DELIMITER** makro in **config.h** file
    - Attention: If the **DELIMITER** doesnt match the current delimiter in the **media.csv** file,
    the programm cannot parse the **media.csv** file correctly and will overwrite it with nothing in worst case
- path of **index.html** file in the **config.h** file
- path of **media.csv** file in the **config.h** file
- **MAX_INPUT_LENGTH** makro in the **config.h** file, to allow bigger/smaller inputs for the fields name, borrower, author and the search query. If **MAX_INPUT_LENGTH** equals a negative number, the limit will be removed. Try this option with caution, as it can be used for a buffer overflow. Default value is currently 800 char.
- **MAX_POSTD_LENGTH** makro in the **config.h** file, to allow larger/smaller post data requests. This maximum is set, to avoid buffer overflows. Change with caution. To disable limit, set to a negative int value. Default is 32000 Chars (32KB).

**activate/deactivate debuging...**
- **permanently** by outcommenting/commenting **#define DEBUG** in the **config.h** file
- **temporarily** by compiling with the **-DDEBUG** flag

## Compiling Command
gcc -o s88216.cgi s88216.c idxlist.c media.c web.c utility.c
## with DEBUGING activated
gcc -o s88216.cgi s88216.c idxlist.c media.c web.c utility.c -DDEBUG

## Usage
- check that permissions for the image folder, media.csv file and s88216.cgi file are set correctly
- in the browser direct to the server adress, where the s88216.cgi file is placed.
- For **searching**, just type the query into the top search bar. The search area includes all columns.
- **Sorting** can be changed via the blue dropdown menu
- **Add items** by clicking the plus button to open the add overlay. Type in the required attributes and press the submit button. Please check that the date is in the required format, for correct sorting by the date. The author and date field are optional.
- For **specific row deletion** click on the lines which you want to delete to color them permanently light blue. Then press the trashcan button.
- To **delete all** rows, unselect all rows which are currently selected and press the trashcan button. A popup occurs and asks for agreement. Press yes/ja to confirm the action.
