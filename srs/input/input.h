#ifndef INPUT_H
#define INPUT_H

#include <stdbool.h>
#include <math.h>  // Для NAN

bool validate_credentials(const char *user, const char *pass);
double calculate_expression(const char *a, const char *b, const char *op);

#endif  // INPUT_H
