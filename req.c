/* 
    * Algoritmo para realizar uma requisição à um servidor HTTP
    * Desenvolvido por Mateus Medeiros
    * Disponível também em https://github.com/mathmed/get-url-c
    * Última atualização em 10/09/2019
*/

/* Importações de bibliotecas necessárias */
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h> 
#include <errno.h> 
#include <netdb.h> 
#include <sys/types.h> 
#include <netinet/in.h> 
#include <arpa/inet.h> 

/* Definindo o tamanhp máximo da mensagem */
#define MAXIMOMSG 5000000

/*

    Utilizando no terminal:
    ./req <url> <path> <method> <post-fields>

    Exemplo:

    ./req www.sitelegal.com.br /index.php POST nome=mateus

*/


/* Assinatura das funções utilizadas no algoritmo */
int resolve_dns(char *hostname , char *ip);
int arg_lenght(char *string);

int main(int argc, char *argv[]){

    /* Verificando a quantidade de argumentos enviados é válida */
    if(argc >= 4 ){    

        char buffer[MAXIMOMSG + 1];
        int tamanho, meusocket;
        struct sockaddr_in destinatario;
        
        /* Resolvendo o DNS, transformando o hostname enviado em IP */
        char ip[100];
        resolve_dns(argv[1], ip);

        /* Criando um socket local */
        meusocket = socket(AF_INET, SOCK_STREAM, 0);

        destinatario.sin_family = AF_INET;

        /* Informando IP do servidor que será feita a requisição */
        destinatario.sin_addr.s_addr = inet_addr(ip);

        /* Informando a porta que será feita a requisição no servidor (80 - HTTP) */
        destinatario.sin_port = htons(80);

        /* Limpando o restante da estrutura que não será utilizado */
        memset(&(destinatario.sin_zero), "\0", sizeof(destinatario.sin_zero));

        /* Se conectando ao servidor destinatário */
        connect(meusocket, (struct sockaddr *) &destinatario, sizeof(struct sockaddr));

        /* Criando string base para o cabeçario */
        char header[2000];

        /* Verificando se o método para requisição no servidor é POST ou GET*/
        if(strcmp(argv[3], "GET") == 0){
            
            /* Criando string do HEADER com método GET para requisição */
            strcat(header, "GET ");
            strcat(header, argv[2]);
            strcat(header, " HTTP/1.1\r\nHost:");
            strcat(header, argv[1]);
            strcat(header, "\r\n\r\n");
        
        
        } else if(strcmp(argv[3], "POST") == 0){

            if(argc == 5){

                /* Criando string auxiliar para guardar o tamanho da query/post enviada no header */
                
                char length[30];

                sprintf(length, "%d", arg_lenght(argv[4]));

                /* Criando string do HEADER com método POST para requisição */
                strcat(header, "POST ");
                strcat(header, argv[2]);
                strcat(header, " HTTP/1.1\r\nHost: ");
                strcat(header, argv[1]);
                strcat(header, "\r\nContent-Type: application/x-www-form-urlencoded\r\n");
                strcat(header, "Content-Length: ");
                strcat(header, length);
                strcat(header, "\r\n\r\n");
                strcat(header, argv[4]);
                strcat(header, "\r\n\r\n");

            } else {

                printf("Modo de uso: \n ./req <url> <path> <method | GET/POST> <fields-post> \n \nExemplo: \n ./req www.sitelegal.com.br /index.php POST nome=mateus\n");

                return 0;

            }

        }

        /* Enviando a requisição */
        send(meusocket, header, strlen(header), 0);
        tamanho = recv(meusocket, buffer, MAXIMOMSG, 0);

        /* Exibindo o resultado na interface */
        printf("%s\n", buffer);

        /* Fechando o socket */
        close(meusocket);
    
        return 0;

        
    } else printf("Modo de uso: \n ./req <url> <path> <method> <fields-post> \n \nExemplo: \n ./req www.sitelegal.com.br /index.php POST nome=mateus\n");

}

/* Função para resolver um DNS, transformando um hostname em um IP */
int resolve_dns(char *hostname , char *ip){
    
    struct hostent *he;     
    struct in_addr **addr_list;     
    int i;     
    
    if ( (he = gethostbyname( hostname ) ) == NULL){ 
        herror("gethostbyname");         
        return 1;
    }

    addr_list = (struct in_addr **) he->h_addr_list;
    
    for(i = 0; addr_list[i] != NULL; i++){
        strcpy(ip , inet_ntoa(*addr_list[i]));
        return 0;
    }
    
    return 1;
}

/* Função para recuperar o tamanho da QUERY enviada para um método POST */
int arg_lenght(char *string){

    int i;
    
    for(i=0;string[i]!='\0';i++);

    return i;
}