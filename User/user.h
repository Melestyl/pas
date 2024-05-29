#include "../Common/common.h"

#define ADMIN_PASSWORD "1234"

/**
 * @brief Show the user menu
 * @param is_admin
 * @note If is_admin is true, the admin menu is displayed
 */
void menu(char is_admin);

/**
 * @brief Show the list of spaces
 */
void show_spaces();

/**
 * @brief Book a space
 */
void book_space();

/**
 * @brief Return a space
 */
void return_space();

/**
 * @brief Show the user's bookings
 */
void show_my_bookings();

/**
 * @brief Add a space
 */
void add_space();

/**
 * @brief Delete a space
 */
void delete_space();

/**
 * @brief End the program
 */
void end();