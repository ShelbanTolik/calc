#include "input.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

bool validate_credentials(const char *user, const char *pass) {
    const char *env_user = getenv("LOGIN_USER");
    const char *env_pass = getenv("LOGIN_PASS");
    return env_user && env_pass && 
           strcmp(user, env_user) == 0 && 
           strcmp(pass, env_pass) == 0;

}

double calculate_expression(const char *a, const char *b, const char *op) {
    double num1 = atof(a);
    double num2 = atof(b);
    
    if (strcmp(op, "add") == 0) return num1 + num2;
    if (strcmp(op, "sub") == 0) return num1 - num2;
    if (strcmp(op, "mul") == 0) return num1 * num2;
    if (strcmp(op, "div") == 0) return num2 != 0 ? num1 / num2 : NAN;
    
    return NAN;
}
