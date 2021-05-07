#ifndef UTILS_H
#define UTILS_H

#include <arpa/inet.h>
#include <stdbool.h>

void print_sockaddr_in_info (const struct sockaddr_in *sockaddr);
void print_raw_string (const char *str);
bool set_uint_from_string (char *str, unsigned int *nb);

#endif
