#include <stdio.h>      /* printf, sprintf */
#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "helpers.h"
#include "requests.h"

int main(int argc, char *argv[])
{
    char *message;
    char *response;
    int sockfd, weather;

    char *host = "ec2-3-8-116-10.eu-west-2.compute.amazonaws.com";
    char *formEncoded = "application/x-www-form-urlencoded";
    char *cookie = "connect.sid=s%3AE5CZ4-WoNaZIE2AUijTvZFDY3PI7lxqO.Qm8maPw04wvSeEjg8rjSN5UQoEKL%2FEnJHlsqzmmpbPs";
    char *details = "lat=25&lon=45&appid=b912dd495585fbf756dc6d8f415a7649";
    int sendDetailCount = 1;
    char **detailsCook = malloc(sendDetailCount * sizeof(char*));
    detailsCook[0] = malloc(strlen(details) + 1);
    strcpy(detailsCook[0], details);
    int cookiesAuthCount = 1;
    char **cookiesAuth = malloc(cookiesAuthCount * sizeof(char*));
    cookiesAuth[0] = malloc(strlen(cookie) + 1);
    strcpy(cookiesAuth[0], cookie);

    int loginDataSize = 2;
    char **data = malloc(loginDataSize * sizeof(char*));
    data[0] = malloc(BUFLEN);
    data[1] = malloc(BUFLEN);
    strcpy(data[0], "username=student");
    strcpy(data[1], "password=student");
    
    sockfd = open_connection("34.118.48.238", 8080, AF_INET, SOCK_STREAM, 0);
    
    // Ex 1.1: GET dummy from main server
    printf("Exercitiul 1.1##############\n");
    message = compute_get_request("34.118.48.238", "/api/v1/dummy", NULL, NULL, 0);
    puts(message);
    printf("\n");
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    printf("Response:\n");
    puts(response);
    printf("\n");

    // Ex 1.2: POST dummy and print response from main server
    printf("Exercitiul 1.2##############\n");
    message = compute_post_request("34.118.48.238", "/api/v1/dummy", formEncoded, data, 2, NULL, 0);
    puts(message);
    printf("\n");
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    printf("Response:\n");
    puts(response);
    printf("\n");
    // Ex 2: Login into main server
    printf("Exercitiul 2##############\n");
    message = compute_post_request("34.118.48.238", "/api/v1/auth/login", formEncoded, data, 2, NULL, 0);
    puts(message);
    printf("\n");
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    printf("Response:\n");
    puts(response);
    printf("\n");
    // Ex 3: GET weather key from main server
    printf("Exercitiul 3##############\n");
    message = compute_get_request(host, "/api/v1/weather/key", NULL, cookiesAuth, cookiesAuthCount);
    puts(message);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    puts(response);
    
    // Ex 4: GET weather data from OpenWeather API
    printf("\nExercitiul 4##############\n");
    weather = open_connection("37.139.20.5", 80, AF_INET, SOCK_STREAM, 0);

    message = compute_get_request("api.openweathermap.org", "/data/2.5/weather", details, NULL, 0);
    puts(message);
    send_to_server(weather, message);
    response = receive_from_server(weather);
    puts(response);

    close_connection(weather);
    // Ex 5: POST weather data for verification to main server


    // BONUS: make the main server return "Already logged in!"
    printf("\n\nverificare conexiune!!!!##############\n\n");
    if (cookie != NULL && strlen(cookie) > 0) {
        puts("Already logged in!");
    } else {
        message = compute_post_request(host, "/api/v1/auth/login", formEncoded, data, 2, NULL, 0);
        puts(message);
        send_to_server(sockfd, message);
        response = receive_from_server(sockfd);
        puts(response);
    }

    // Ex 6: Logout from main server
    printf("\nExercitiul 6##############\n");
    message = compute_get_request(host, "/api/v1/auth/logout", NULL, NULL, 0);
    puts(message);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    puts(response);

    // free the allocated data at the end!
    free(message);
    free(response);
    free(data[0]);
    free(data[1]);
    free(data);
    free(cookiesAuth[0]);
    free(cookiesAuth);
    close_connection(sockfd);
    return 0;
}
