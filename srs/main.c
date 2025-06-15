#include "mongoose.h"
#include "input/input.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

static bool uri_equal(struct mg_str uri, const char *str) {
    struct mg_str s = mg_str(str);
    return mg_strcmp(uri, s) == 0;
}

static void fn(struct mg_connection *c, int ev, void *ev_data) {
    if (ev == MG_EV_HTTP_MSG) {
        struct mg_http_message *hm = (struct mg_http_message *)ev_data;
        struct mg_http_serve_opts opts = {.root_dir = "."};

        // Обработка статических файлов
        if (uri_equal(hm->uri, "/styles.css")) {
            mg_http_serve_file(c, hm, "templates/styles.css", &opts);
            return;
        }

        // Главная страница
        if (uri_equal(hm->uri, "/")) {
            mg_http_serve_file(c, hm, "templates/login.html", &opts);
            return;
        }

        // Обработка логина
        if (uri_equal(hm->uri, "/login") && uri_equal(hm->method, "POST")) {
    char user[100], pass[100];
    mg_http_get_var(&hm->body, "username", user, sizeof(user));
    mg_http_get_var(&hm->body, "password", pass, sizeof(pass));

    if (validate_credentials(user, pass)) {
        mg_http_serve_file(c, hm, "templates/success.html", &opts);
    } else {
        // Вместо прямого HTML используем шаблон
        struct mg_http_serve_opts opts = {.root_dir = "."};
        mg_http_serve_file(c, hm, "templates/error.html", &opts);
    }
    return;
}

        // Калькулятор
        if (uri_equal(hm->uri, "/calculator")) {
            mg_http_serve_file(c, hm, "templates/webcalc.html", &opts);
            return;
        }

        // Обработка вычислений
        if (uri_equal(hm->uri, "/calculate") && uri_equal(hm->method, "POST")) {
            char a[100], b[100], op[100];
            mg_http_get_var(&hm->body, "a", a, sizeof(a));
            mg_http_get_var(&hm->body, "b", b, sizeof(b));
            mg_http_get_var(&hm->body, "op", op, sizeof(op));

            double result = calculate_expression(a, b, op);
            if (isnan(result)) {
                mg_http_serve_file(c, hm, "templates/calcerror.html", &opts);
                } else {
    
    // Формируем простой HTML ответ
    char buf[500];
    snprintf(buf, sizeof(buf),
        "<!DOCTYPE html>"
        "<html><head>"
        "<meta charset='UTF-8'>"
        "<title>Результат</title>"
        "<link rel='stylesheet' href='/styles.css'>"
        "</head><body style='text-align:center;margin-top:50px;'>"
        "<h1>Результат: %.2f</h1>"
        "<a href='/calculator' style='color:blue;text-decoration:none;font-size:18px;'>"
        "Назад к калькулятору</a>"
        "</body></html>",
        result);
    
    mg_http_reply(c, 200, "Content-Type: text/html; charset=UTF-8\r\n", buf);
    }
    return;
}

        // Все остальные запросы
        mg_http_reply(c, 404, "Content-Type: text/plain\r\n", "Not Found\n");
    }
}

int main(void) {
    struct mg_mgr mgr;
    mg_mgr_init(&mgr);

    if (mg_http_listen(&mgr, "http://0.0.0.0:8083", fn, NULL) == NULL) {
        printf("Failed to start server\n");
        return 1;
    }

    printf("Server running at http://localhost:8083\n");
    while (true) mg_mgr_poll(&mgr, 1000);

    return 0;
}
