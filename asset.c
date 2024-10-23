#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<mysql.h>
#include<windows.h>
#include<stdbool.h>
#include "asset.h"
int login(MYSQL *conn) {
    char username[50], password[50];
    char query[256];
    MYSQL_RES *res;
    MYSQL_ROW row;

    // Get user input
    printf("Enter Username: ");
    scanf("%s", username);
    printf("Enter Password: ");
    scanf("%s", password);

    // Prepare query to check username and password
    sprintf(query, "SELECT * FROM users WHERE username='%s' AND password='%s'", username, password);

    // Execute the query
    if (mysql_query(conn, query)) {
        printf("Query execution failed: %s\n", mysql_error(conn));
        return 0;
    }

    // Get the result
    res = mysql_store_result(conn);
    if (res == NULL) {
        printf("Error retrieving result: %s\n", mysql_error(conn));
        return 0;
    }

    // Check if the user exists
    row = mysql_fetch_row(res);
    if (row != NULL) {
        printf("Login successful! Welcome, %s.\n", username);
        mysql_free_result(res);
        return 1;  // Successful login
    } else {
        printf("Invalid username or password. Please try again.\n");
        mysql_free_result(res);
        return 0;  // Failed login
    }
}

void displayMenu() {
    printf("\n--- Welcome to Stock Management System ---\n");
    printf("1. Login\n");
    printf("2. Sign Up\n");
    printf("3. Exit\n");
    printf("Enter your choice: ");
}

void displayHotelMenu() {
//    // Clear the screen
//    #ifdef _WIN32
//        system("cls");  // For Windows
//    #else
//        system("clear");  // For Linux/Mac
//    #endif

    printf("Welcome to Hotel Management System\n\n");
    printf("Please select an option:\n");
    printf("1. Register a New Customer\n");
    printf("2. Book a Room\n");
    printf("3. View Customer Information\n");
    printf("4. View Rooms Allotted\n");
    printf("5. Edit Customer Details\n");
    printf("6. Order Food\n");
    printf("7. Checkout and Generate Bill\n");
    printf("8. Exit\n");
}

void signUp(MYSQL *conn) {
    char username[50], password[50];
    char query[256];

    // Get user input for new account
    printf("Enter New Username: ");
    scanf("%s", username);
    printf("Enter New Password: ");
    scanf("%s", password);

    // Check if the username already exists
    sprintf(query, "SELECT * FROM users WHERE username='%s'", username);
    if (mysql_query(conn, query)) {
        printf("Query execution failed: %s\n", mysql_error(conn));
        return;
    }

    MYSQL_RES *res = mysql_store_result(conn);
    if (mysql_fetch_row(res) != NULL) {
        printf("Username already exists. Please try another one.\n");
        mysql_free_result(res);
        return;
    }
    mysql_free_result(res);

    // Insert the new user into the database with default permission "update_only"
    sprintf(query, "INSERT INTO users (username, password) VALUES ('%s', '%s')", username, password);
    if (mysql_query(conn, query)) {
        printf("Query execution failed: %s\n", mysql_error(conn));
    } else {
        printf("Sign-Up successful! You can now login.\n");
    }
}

void register_customer(MYSQL *conn) {
    char name[100], phone[15], email[100], address[255];
    char query[1024];

    printf("Enter customer name: ");
    scanf(" %[^\n]", name);
    printf("Enter phone number: ");
    scanf("%s", phone);
    printf("Enter email: ");
    scanf("%s", email);
    printf("Enter address: ");
    scanf(" %[^\n]", address);

    // Create SQL query to insert customer data
    snprintf(query, sizeof(query),
             "INSERT INTO customers (name, phone, email, address) VALUES ('%s', '%s', '%s', '%s')",
             name, phone, email, address);

    // Execute the query
    if (mysql_query(conn, query)) {
        printf("Failed to register customer: %s\n", mysql_error(conn));
    } else {
        printf("Customer registered successfully!\n");
    }
}

void book_room(MYSQL *conn) {
    int customer_id;
    int room_number;
    char check_in_date[11]; // Format: YYYY-MM-DD
    char check_out_date[11]; // Format: YYYY-MM-DD
    int number_of_days;

    // Get customer ID
    printf("Enter Customer ID: ");
    scanf("%d", &customer_id);

    // Get room number
    printf("Enter Room Number: ");
    scanf("%d", &room_number);

    // Get check-in date
    printf("Enter Check-in Date (YYYY-MM-DD): ");
    scanf("%s", check_in_date);

    // Get check-out date
    printf("Enter Check-out Date (YYYY-MM-DD): ");
    scanf("%s", check_out_date);

    // Calculate number of days (this can be improved with date validation)
    printf("Enter Number of Days: ");
    scanf("%d", &number_of_days);

    // SQL query to insert booking details into the bookings table
    char query[512];
    snprintf(query, sizeof(query), 
             "INSERT INTO bookings (customer_id, room_number, check_in_date, check_out_date, days) "
             "VALUES (%d, %d, '%s', '%s', %d)", 
             customer_id, room_number, check_in_date, check_out_date, number_of_days);

    // Execute the insert query
    if (mysql_query(conn, query)) {
        fprintf(stderr, "Error inserting booking: %s\n", mysql_error(conn));
    } else {
        printf("Room booked successfully!\n");

        // SQL query to update the room's status to 'allotted' and assign the customer ID
        // After successfully booking a room, update the room status in the rooms table
	snprintf(query, sizeof(query), 
         "UPDATE rooms SET status = 'allotted', customer_id = %d WHERE room_number = %d", 
         customer_id, room_number);
	

        // Execute the update query for the rooms table
        if (mysql_query(conn, query)) {
            fprintf(stderr, "Error updating room status: %s\n", mysql_error(conn));
        } else {
            printf("Room status updated to 'allotted'.\n");
        }
    }
}


void view_customer_info(MYSQL *conn) {
    int customer_id;
    printf("Enter Customer ID to view information: ");
    scanf("%d", &customer_id);

    // SQL query to retrieve customer details
    char query[256];
    snprintf(query, sizeof(query), 
             "SELECT * FROM customers WHERE id = %d", customer_id);

    if (mysql_query(conn, query)) {
        fprintf(stderr, "Error: %s\n", mysql_error(conn));
        return;
    }

    MYSQL_RES *result = mysql_store_result(conn);
    if (!result) {
        fprintf(stderr, "Error: %s\n", mysql_error(conn));
        return;
    }

    int num_fields = mysql_num_fields(result);
    MYSQL_ROW row;
	int i;
    // Print column names
    for ( i= 0; i < num_fields; i++) {
        printf("%s\t", mysql_fetch_field(result)->name);
    }
    printf("\n");

    // Print customer details
    while ((row = mysql_fetch_row(result))) {
        for ( i = 0; i < num_fields; i++) {
            printf("%s\t", row[i] ? row[i] : "NULL");
        }
        printf("\n");
    }

    // Clean up
    mysql_free_result(result);
}

void view_rooms_alloted(MYSQL *conn) {
    char query[256];

    // Prepare SQL query to fetch allotted rooms
    snprintf(query, sizeof(query), "SELECT * FROM rooms WHERE status = 'allotted'");

    if (mysql_query(conn, query)) {
        fprintf(stderr, "Error: %s\n", mysql_error(conn));
        return;
    }

    MYSQL_RES *result = mysql_store_result(conn);
    if (!result) {
        fprintf(stderr, "Error retrieving result set: %s\n", mysql_error(conn));
        return;
    }

    MYSQL_ROW row;
    printf("Allotted Rooms Information:\n");
    printf("Room Number\tCustomer ID\tStatus\n");
    while ((row = mysql_fetch_row(result))) {
        printf("%s\t%s\t%s\n", row[0], row[1], row[2]); // Adjust according to your table structure
    }

    mysql_free_result(result);
}


void edit_customer_details(MYSQL *conn) {
    int customer_id;
    char new_name[100];
    char new_phone[15];
    char new_email[100];
    char new_address[255];

    printf("Enter Customer ID to edit: ");
    scanf("%d", &customer_id);

    // Get existing customer details
    char query[256];
    snprintf(query, sizeof(query), 
             "SELECT * FROM customers WHERE id = %d", customer_id);

    if (mysql_query(conn, query)) {
        fprintf(stderr, "Error: %s\n", mysql_error(conn));
        return;
    }

    MYSQL_RES *result = mysql_store_result(conn);
    if (!result || mysql_num_rows(result) == 0) {
        printf("No customer found with ID: %d\n", customer_id);
        return;
    }

    MYSQL_ROW row = mysql_fetch_row(result);
    printf("Current Details:\n");
    printf("Name: %s\n", row[1]);
    printf("Phone: %s\n", row[2]);
    printf("Email: %s\n", row[3]);
    printf("Address: %s\n", row[4]);

    // Get new details
    printf("Enter new name (or press enter to keep current): ");
    getchar(); // Clear the newline character from input buffer
    fgets(new_name, sizeof(new_name), stdin);
    if (new_name[0] == '\n') {
        strcpy(new_name, row[1]); // Keep current name if no new input
    } else {
        new_name[strcspn(new_name, "\n")] = 0; // Remove newline character
    }

    printf("Enter new phone (or press enter to keep current): ");
    fgets(new_phone, sizeof(new_phone), stdin);
    if (new_phone[0] == '\n') {
        strcpy(new_phone, row[2]);
    } else {
        new_phone[strcspn(new_phone, "\n")] = 0;
    }

    printf("Enter new email (or press enter to keep current): ");
    fgets(new_email, sizeof(new_email), stdin);
    if (new_email[0] == '\n') {
        strcpy(new_email, row[3]);
    } else {
        new_email[strcspn(new_email, "\n")] = 0;
    }

    printf("Enter new address (or press enter to keep current): ");
    fgets(new_address, sizeof(new_address), stdin);
    if (new_address[0] == '\n') {
        strcpy(new_address, row[4]);
    } else {
        new_address[strcspn(new_address, "\n")] = 0;
    }

    // SQL query to update customer details
    snprintf(query, sizeof(query), 
             "UPDATE customers SET name='%s', phone='%s', email='%s', address='%s' WHERE id=%d",
             new_name, new_phone, new_email, new_address, customer_id);

    // Execute the update query
    if (mysql_query(conn, query)) {
        fprintf(stderr, "Error: %s\n", mysql_error(conn));
    } else {
        printf("Customer details updated successfully!\n");
    }

    // Clean up
    mysql_free_result(result);
}

void order_food(MYSQL *conn) {
    int customer_id;
    char food_item[100];
    float price;
    int quantity;

    printf("Enter Customer ID for food order: ");
    scanf("%d", &customer_id);

    // Get food details
    printf("Enter Food Item: ");
    getchar(); // Clear the newline character from input buffer
    fgets(food_item, sizeof(food_item), stdin);
    food_item[strcspn(food_item, "\n")] = 0; // Remove newline character

    printf("Enter Price of the Food Item: ");
    scanf("%f", &price);

    printf("Enter Quantity: ");
    scanf("%d", &quantity);

    // SQL query to insert food order
    char query[256];
    snprintf(query, sizeof(query), 
             "INSERT INTO food_orders (customer_id, food_item, price, quantity) VALUES (%d, '%s', %.2f, %d)", 
             customer_id, food_item, price, quantity);

    // Execute the insert query
    if (mysql_query(conn, query)) {
        fprintf(stderr, "Error: %s\n", mysql_error(conn));
    } else {
        printf("Food order placed successfully!\n");
    }
}

void check_out(MYSQL *conn) {
    int customer_id;
    char query[256];

    printf("Enter Customer ID for check-out: ");
    scanf("%d", &customer_id);

    // SQL query to retrieve the booking details for the customer
    snprintf(query, sizeof(query),
             "SELECT room_number, check_in_date, check_out_date FROM bookings WHERE customer_id = %d", 
             customer_id);

    if (mysql_query(conn, query)) {
        fprintf(stderr, "Error: %s\n", mysql_error(conn));
        return;
    }

    MYSQL_RES *result = mysql_store_result(conn);
    if (!result || mysql_num_rows(result) == 0) {
        printf("No booking found for Customer ID: %d\n", customer_id);
        return;
    }

    MYSQL_ROW row;
    printf("Booking Details:\n");
    printf("Room Number\tCheck-In Date\tCheck-Out Date\n");
    while ((row = mysql_fetch_row(result))) {
        printf("%s\t%s\t%s\n", row[0], row[1], row[2]);
    }

    // Ask for confirmation to check out
    char confirmation;
    printf("Do you want to proceed with check-out? (Y/N): ");
    getchar(); // Clear the newline character from input buffer
    scanf("%c", &confirmation);

    if (confirmation == 'Y' || confirmation == 'y') {
        // SQL query to delete booking record
        snprintf(query, sizeof(query),
                 "DELETE FROM bookings WHERE customer_id = %d", customer_id);

        if (mysql_query(conn, query)) {
            fprintf(stderr, "Error: %s\n", mysql_error(conn));
        } else {
            printf("Customer checked out successfully!\n");
        }
    } else {
        printf("Check-out cancelled.\n");
    }

    // Clean up
    mysql_free_result(result);
}
