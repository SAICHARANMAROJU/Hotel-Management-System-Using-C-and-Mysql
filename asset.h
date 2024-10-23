#ifndef ASSEST_H
#define ASSEST_H

#include<mysql.h>
#include<ctype.h>

int login(MYSQL *conn);

void displayMenu();

void signUp(MYSQL *conn);
void register_customer(MYSQL *conn);
void displayHotelMenu();
void book_room(MYSQL *conn);
void view_customer_info(MYSQL *conn);
void view_rooms_allotted(MYSQL *conn);
void edit_customer_details(MYSQL *conn);
void order_food(MYSQL *conn);
void check_out(MYSQL *conn);

#endif
