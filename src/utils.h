#ifndef UTILS_H
#define UTILS_H

#include <arpa/inet.h>

void print_sockaddr_in_info (const struct sockaddr_in *sockaddr);
void print_raw_string (const char *str);

#endif
