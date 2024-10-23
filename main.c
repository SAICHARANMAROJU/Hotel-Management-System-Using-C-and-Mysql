#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<mysql.h>
#include<windows.h>
#include<stdbool.h>
#include "asset.h"

#define HOST "localhost"
#define USER "root"
#define PASS "23022006"
#define DB "hotelManagement"

MYSQL* connectDB() {
    MYSQL *conn = mysql_init(NULL);
    if (conn == NULL) {
        printf("mysql_init() failed\n");
        return NULL;
    }

    if (mysql_real_connect(conn, HOST, USER, PASS, DB, 0, NULL, 0) == NULL) {
        printf("mysql_real_connect() failed\n");
        mysql_close(conn);
        return NULL;
    }

    return conn;
}

int main() {
    MYSQL *conn = connectDB();
    if (conn == NULL) {
        return 1;
    }

    int choice, loginSuccess = 0;

    while (1) {
        displayMenu();
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                loginSuccess = login(conn);
                if (loginSuccess) {
                    printf("Login successful. Loading Hotel Management System...\n");
                    
                    // Clear screen and display hotel management menu
                    displayHotelMenu();

                    int hotelChoice;
                    scanf("%d", &hotelChoice);

                    switch (hotelChoice) {
                        case 1:
                            register_customer(conn);
                            break;
                        // Add other options (booking, view info, etc.) here
                        case 2:
                        	book_room(conn);
                        	break;
                        case 3:
                        	view_customer_info(conn);
                        	break;
                        case 4:
                        	view_rooms_alloted(conn);
                        	break;
                        case 5:
                        	edit_customer_details(conn);
                        	break;
                        case 6:
                        	order_food(conn);
                        	break;
                        case 7:
                        	check_out(conn);
                        case 8:
                            printf("Exiting system. Goodbye!\n");
                            mysql_close(conn);
                            return 0;

                        default:
                            printf("Invalid choice. Please try again.\n");
                    }
                }
                break;

            case 2:
                signUp(conn);
                break;

            case 3:
                printf("Exiting program. Goodbye!\n");
                mysql_close(conn);
                return 0;

            default:
                printf("Invalid choice. Please enter 1, 2, or 3.\n");
			}
    }

    return 0;
}	
