#include<iostream>
#include<conio.h>
#include"windows.h"
#include<chrono>
#include<fstream>
#include<vector>
#include<ctime>
#include"stdlib.h"
#include"Cancion.h"
#include"ListaCircular.h"
#include"Animaciones.h"

using namespace std;

//CREACION DE OBJETO Y LISTA
Cancion cancion;
ListaCircular *lista = new ListaCircular();

//VARIABLES PARA SETEAR VALORES
string nombre;
string album;
string artista;
string genero;
string direccion;
string minutos;
string segundos;

//VARIABLES GENERALES
int opc = 0;
int puntero = 1; //Toma el rol como si fuera id
fstream file; //De donde se leera

//PROTOTIPOS DE FUNCIONES
void displayMenu(); //Menu principal
void insertarDatos(); //Case 1
void editarDatos(); //Case 5
void llenarLista(); //Case 6
void interfazReproductor(); //Case 9

//Funciones de validación
bool validarCadena(string &entrada, int x, int y);
bool validarEntero(int &entrada, int min, int max, int x, int y);
bool validarMinutosSegundos(string &entrada, int x, int y);

int main()
{
    system("Title Spotify by sami");
    displayMenu();
    return 0;
}

void displayMenu(){
    do{
        system("cls");
        titulo();
        gotoxy(7,6); cout << "1.INGRESAR CANCION" << endl;
        gotoxy(7,8); cout << "2.ANTERIOR" << endl;
        gotoxy(7,10); cout << "3.SIGUIENTE" << endl;
        gotoxy(7,12); cout << "4.ELIMINAR CANCION" << endl;
        gotoxy(7,14); cout << "5.EDITAR CANCION" << endl;
        gotoxy(7,16); cout << "6.LLENAR LISTA" << endl;
        gotoxy(7,18); cout << "7.REPRODUCIR CANCION" << endl;
        gotoxy(7,20); cout << "8.PAUSAR CANCION" << endl;
        gotoxy(7,22); cout << "9.SPOTIFY" << endl;
        gotoxy(7,24); cout << "10.SALIR" << endl;
        lista->imprimirLista(puntero, cancion);
        lista->imprimirCancionActual(puntero);
        gotoxy(7,26); cout << "TU OPCION: ", cin >> opc;

        switch(opc){
            case 1: insertarDatos(); break;
            case 2:
                puntero--;
                if(puntero < 1){
                    puntero = lista->travel();
                }
                break;
            case 3:
                puntero++;
                if(puntero > lista->travel()){
                    puntero = 1;
                }
                break;
            case 4:
                if(!lista->estaVacia()){
                    lista->eliminar(puntero);
                    puntero = 1;
                }
                break;
            case 5: editarDatos(); break;
            case 6: llenarLista(); break;
            case 7:
                if(!lista->estaVacia()){
                    lista->reproducirCancion(puntero);
                }
                break;
            case 8: PlaySound(NULL, NULL, 0); break;
            case 9: interfazReproductor(); break;
        }
    } while(opc != 10);
}

//FUNCIONES///////////////////////////////////////////////////////////////////////////////////////////////////

bool validarCadena(string &entrada, int x, int y){
    while(entrada.empty()){
        gotoxy(x, y); // Mueve el cursor a la posición original
        cout << "Este campo no puede estar vacío. Por favor ingresa un valor: ";
        getline(cin, entrada);
    }
    return true;
}

bool validarEntero(int &entrada, int min, int max, int x, int y){
    while(true){
        gotoxy(x, y); // Regresa el cursor al mismo lugar
        if(cin >> entrada && entrada >= min && entrada <= max){
            return true;
        } else {
            cout << "Por favor ingresa un valor numérico entre " << min << " y " << max << ": ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

bool validarMinutosSegundos(string &entrada, int x, int y){
    int valor;
    while(true){
        gotoxy(x, y); // Mantiene el cursor en la misma posición
        if(cin >> valor && valor >= 0 && valor < 60){
            return true;
        } else {
            cout << "Por favor ingresa un valor entre 0 y 59 para minutos/segundos: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

void insertarDatos(){
    file.open("Canciones.txt", ios::app);

    interfazPrincipal_AgregandoCancion();

    cancion.setId(lista->travel() + 1);
    gotoxy(80, 7); cout << "ID: " << cancion.getId() << endl;

    // Validación del nombre
    gotoxy(34, 7); cout << "INGRESE NOMBRE: ";
    cin.sync();
    getline(cin, nombre);
    validarCadena(nombre, 34, 7);  // Pasamos la posición donde queremos que se quede el cursor
    cancion.setNombre(nombre);
    file << nombre << "\n";

    // Validación del artista
    gotoxy(34, 10); cout << "INGRESE ARTISTA: ";
    getline(cin, artista);
    validarCadena(artista, 34, 10);
    cancion.setArtista(artista);
    file << artista << "\n";

    // Validación del album
    gotoxy(34, 13); cout << "INGRESE ALBUM: ";
    getline(cin, album);
    validarCadena(album, 34, 13);
    cancion.setAlbum(album);
    file << album << "\n";

    // Validación del género
    gotoxy(34, 16); cout << "INGRESE GENERO: ";
    getline(cin, genero);
    validarCadena(genero, 34, 16);
    cancion.setGenero(genero);
    file << genero << "\n";

    // Validación de la dirección
    gotoxy(34, 19); cout << "INGRESE LA DIRECCION: ";
    getline(cin, direccion);
    validarCadena(direccion, 34, 19);
    cancion.setDireccion(direccion);
    file << direccion << "\n";

    // Validación de la duración (minutos y segundos)
    gotoxy(80, 10); cout << "INGRESE DURACION: ";

    gotoxy(80, 13); cout << "MINUTOS: ";
    getline(cin, minutos);
    while(!validarMinutosSegundos(minutos, 80, 13)){};
    cancion.setMinutos(minutos);
    file << minutos << "\n";

    gotoxy(80, 16); cout << "SEGUNDOS: ";
    getline(cin, segundos);
    while(!validarMinutosSegundos(segundos, 80, 16)){};
    cancion.setSegundos(segundos);
    file << segundos << "\n";

    lista->insertarUltimo(cancion);
    agregandoCancion();
    getch();
    file.close();
}

void editarDatos(){
    if(!lista->estaVacia()){
        interfazPrincipal_ModificandoCancion();

        gotoxy(80, 7); cout << "ID: " << cancion.getId() << endl;

        gotoxy(34, 7); cout << "INGRESE NOMBRE: ";
        cin.sync();
        getline(cin, nombre);
        validarCadena(nombre, 34, 7);  // Pasamos la posición donde queremos que se quede el cursor
        cancion.setNombre(nombre);

        gotoxy(34, 10); cout << "INGRESE ARTISTA: ";
        getline(cin, artista);
        validarCadena(artista, 34, 10);
        cancion.setArtista(artista);

        gotoxy(34, 13); cout << "INGRESE ALBUM: ";
        getline(cin, album);
        validarCadena(album, 34, 13);
        cancion.setAlbum(album);

        gotoxy(34, 16); cout << "INGRESE GENERO: ";
        getline(cin, genero);
        validarCadena(genero, 34, 16);
        cancion.setGenero(genero);

        gotoxy(34, 19); cout << "INGRESE LA DIRECCION: ";
        getline(cin, direccion);
        validarCadena(direccion, 34, 19);
        cancion.setDireccion(direccion);

        gotoxy(80, 10); cout << "INGRESE DURACION: ";

        gotoxy(80, 13); cout << "MINUTOS: ";
        getline(cin, minutos);
        while(!validarMinutosSegundos(minutos, 80, 13)){};
        cancion.setMinutos(minutos);

        gotoxy(80, 16); cout << "SEGUNDOS: ";
        getline(cin, segundos);
        while(!validarMinutosSegundos(segundos, 80, 16)){};
        cancion.setSegundos(segundos);

        lista->modificar(puntero, cancion);
        editandoCancion();
        getch();
    }
}

void llenarLista(){
    file.open("Canciones.txt", ios::in);

    if(file.fail()){
        cout << "Error al abrir el archivo";
    } else {
        while(!file.eof()){
            getline(file, nombre);
            cancion.setNombre(nombre);

            getline(file, artista);
            cancion.setArtista(artista);

            getline(file, album);
            cancion.setAlbum(album);

            getline(file, genero);
            cancion.setGenero(genero);

            getline(file, direccion);
            cancion.setDireccion(direccion);

            getline(file, minutos);
            cancion.setMinutos(minutos);

            getline(file, segundos);
            cancion.setSegundos(segundos);

            lista->insertarUltimo(cancion);
        }
        lista->eliminar(lista->travel());
    }

    file.close();
}

void interfazReproductor(){
    int op;
    do{
        reproductor();
        lista->reproducirCancion(puntero);
        lista->imprimirCancionActualReproductor(puntero);

        gotoxy(105, 26); cin >> op;

        switch(op){
            case 1:
                if(puntero > 1){
                    puntero = puntero - 1;
                } else {
                    puntero = lista->travel();
                }
                lista->reproducirCancion(puntero);
                break;
            case 2:
                PlaySound(NULL, NULL, 0);
                break;
            case 3:
                if(puntero < lista->travel()){
                    puntero = puntero + 1;
                } else {
                    puntero = 1;
                }
                lista->reproducirCancion(puntero);
                break;
        }
    } while(op != 4);
}
