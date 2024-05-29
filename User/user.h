#include "../Common/common.h"

#define ADMIN_PASSWORD "1234"

/**
 * @brief Show the user menu
 * @param is_admin
 * @note If is_admin is true, the admin menu is displayed
 */
void menu(char is_admin);

/**
 * @brief Show the list of areas
 */
void show_areas();

/**
 * @brief Book a area
 */
void book_area();

/**
 * @brief Return a area
 */
void return_area();

/**
 * @brief Show the user's bookings
 */
void show_my_bookings();

/**
 * @brief Add a area
 */
void add_area();

/**
 * @brief Delete a area
 */
void delete_area();

/**
 * @brief End the program
 */
void end();