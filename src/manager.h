#ifndef MANAGER_H
#define MANAGER_H

struct manager;

struct manager* create_manager (int port);
int start_manager (struct manager* man);
void destroy_manager(struct manager* man);

#endif
