# HTTP Request (GET/POST) using C

C algorithm to perform HTTP requests on servers using GET and POST methods

## Compile
```html
$ gcc -g req.c -o req
```

## Usage

```html
$ ./req <url> <path> <method> <fields-post>
```

## Usage examples

```html
$ ./req https://postman-echo.com /post POST somefield=somevalue&somefield2=somevalue2
```
```html
$ ./req https://postman-echo.com /get GET
```
