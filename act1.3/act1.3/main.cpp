//
//  main.cpp
//  act1.3
//
//  Created by Emma Gomez  on 12/09/20.
//

#include <string>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <vector>
#include <stdlib.h>
#include <cstdlib>
#include <iostream>

using namespace std;

struct Info{
    //struct, como class pero sin public y private
    int dateTime;
    string ipError;
    Info(int,string);
    string imprimir();
};

Info :: Info(int dateTime, string ipError){
    this -> dateTime = dateTime;
    this -> ipError = ipError;
}

string Info :: imprimir(){
    string dateHour = to_string(dateTime);
    int size = dateHour.size()-1;
    string hora,min,seg;
    seg = dateHour.substr(size-1,2);
    min = dateHour.substr(size-3,2);
    hora = dateHour.substr(size-5,2);
    string mes,dia;
    dia = dateHour.substr(size-7,2);
   
    if (dia.substr(0,1) == "0"){
    dia = dia.substr(1,1);
    }
      if (size == 8){
        mes = dateHour.substr(size-8,1);
      }
      else{
        mes = dateHour.substr(size-9,2);
      }
      //regresar el mes a su string original
      //lista que contiene los meses
      string meses[] = {"","Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
      mes = meses[stoi(mes)];
      return  mes + " " + dia + " " + hora + ":" + min + ":" + seg + " " + ipError;
}

//sort

int sort(vector<Info> &list, int left, int right){
    //el pivote --> left
    int pivote = list[left].dateTime;
    int first = left + 1;
    int last = right;
    while (first != last){
        if (list[first].dateTime <= pivote){
            first++;
        }
        else{
            while ((first != last)  && (pivote < list[last].dateTime)){
                last--;
            }
            //intercambio
            // con auxiliar
            Info aux = list[last];
            list[last] = list[first];
            list[first] = aux;
        }
    }
    
    if (list[first].dateTime > pivote){
    first--;
    }
    
    list[left] = list[first];
    list[first].dateTime = pivote;
    //retorna la posicion del pivote
    return first;
}

//quick sort
//left: datos que se encuentran a la izquierda, al principio
//right: datos que se encuentran a la derechea, al final
//pos: posición del pivote que se está analizando

void quickSort(vector<Info> &list, int left, int right){
    if(left < right) {
        int posP = sort(list, left, right);
        //parte la lista para el lado izquierdo del pivote
        quickSort(list, left, posP - 1); //-1 para correrlo
        quickSort(list, posP + 1, right);//para correrlo al final
    }
}

//binary search
//para encontrar el rango inicial y el rango final.

int busquedaBinaria (vector<Info> &list, int dateTime2, bool inicio){
    int inf = 0;
    int sup = list.size() - 1;
    while ((inf <= sup)){
        int mid = (inf + sup) / 2;
        //si buscamos el primer valor del rango
        if (inicio){
            if ((list[mid].dateTime >= dateTime2) && (list[mid-1].dateTime < dateTime2)){
                return mid;
            }
            //se encuentra del lado derecho
            else if(list[mid].dateTime < dateTime2){
                inf = mid + 1;
            }
            //se encuentra del lado izquierdo
            else{
                sup = mid - 1;
            }
        }
        //si buscamos el ultimo valor que entra en el rango
        else {
            if ((list[mid].dateTime <= dateTime2) && (list[mid+1].dateTime > dateTime2)){
                return mid;
            }
            //se encuentra del lado derecho
            else if(list[mid].dateTime < dateTime2){
                inf = mid + 1;
            }
            //se encuentra del lado izquierdo
            else{
                sup = mid - 1;
            }
        }
    }
    return -1;
}


int main() {
    //archivo
    fstream archivoEntrada;
    archivoEntrada.open("bitacora.txt", ios::in);
  
    string linea;
    vector <string> bitacora;
    
    while(getline(archivoEntrada,linea)){
        stringstream s(linea);
        bitacora.push_back(linea);
    }
    archivoEntrada.close();

    //de vector de strings a objetos en un vector
    vector<Info> informacion;
    for (int i=0; i<bitacora.size(); i++){
        string mes,fecha,hora,ipError;
        //mes string --> entero
        int pos = (int)bitacora[i].find(" ");
        string m = bitacora[i].substr(0,pos);
        bitacora[i].erase(0,pos + 1);
        string meses[] = {"","Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
        
        for (int i=1;i<=12;i++){ //para convertir el mes en numero (Jan = 01)
            if (m == meses[i]){
                mes = to_string(i); //se convierte a string
            }
        }
        
        //Dia
        pos = (int)bitacora[i].find(" ");
        string d = bitacora[i].substr(0,pos); //se eliminan los espacios
        bitacora[i].erase(0,pos + 1);
        int dia = std::stoi(d);
        //para que los meses enero, febrero, marzo, junio, julio, agosto y septiembre aparezcan con doble digito
        // de esta manera: 01,02,03,04,05,06,07,08,09
        if (dia<=9){
          d= "0" + d; //si es de enero a septiembre se cumple
        }
    
        //MMDD con enteros
        fecha = mes + d; // para juntar el mes y el dia ej: 0914 (sept 14)
        
        //Hora
        pos = (int)bitacora[i].find(":"); //se eliminan los dos puntos despues de la hora
        string h = bitacora[i].substr(0,pos);
        bitacora[i].erase(0,pos + 1);
        
        //Minutos
        pos = (int)bitacora[i].find(":"); //se eliminan los dos puntos despues de los minutos
        string mi = bitacora[i].substr(0,pos);
        bitacora[i].erase(0,pos + 1);
        
        //Segundos
        pos = (int)bitacora[i].find(" "); //se eliminan los dos puntos despues de los segundos
        string s = bitacora[i].substr(0,pos);
        bitacora[i].erase(0,pos + 1);
        
        //Tiempo
        hora = h + mi + s;
        string fechaHora = fecha + hora;
        int dateTime = std::stoi(fechaHora);
        
        //IP Error
        ipError = bitacora[i];
        //se crea un objeto con los parametros y se almacena en el vector
        informacion.push_back(Info(dateTime,ipError));
      }
      
    //Ordenamiento con quicksort
    cout << "La información del archivo se está analizando y será ordenada con método: Quick Sort..." << endl;
    quickSort(informacion,0,informacion.size()-1);
    cout << "La información del archivo 'bitacora.txt' ha sido ordenada de manera exitosa." << endl;
    
    //almacena el vector ordenado en un archivo nuevo
    ofstream archivoSalida("bitacora.Ordenada.txt");
    for (int i=0; i<informacion.size(); i++){
        archivoSalida << informacion[i].imprimir() << endl;
    }
    archivoSalida.close();
    cout << "Se ha creado un archivo con los datos de forma ordenada" << endl;
      
    //se pide el rango de busqueda
    // se busca encontrar el rango inicial y el rango final.
    bool not_valid = true;
    int rangoInicial = 0;
    int rangoFinal = 0;
    string mes, dia, hora, mess, diaa, horaa;
    bool mes_not_valid = true;
    while (not_valid){
        //datos de inicio de la búsqueda
        cout << "Teclea la fecha de inicio de tu búsqueda" << endl;
        
        while (mes_not_valid){
          cout << "Ingrese el mes (ej: Jan):";
          cin >> mes;
          
          string meses[] = {"","Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
          for (int i=1; i<=12; i++){
            if (mes == meses[i]){ //valida si el mes ingresado está en la lista
              mes = to_string(i);
              mes_not_valid = false;
            }
          }
          if (mes_not_valid){ //si el mes ingresado no está en la lista
            cout << "El mes ingresado no es válido para este tipo de busca. Vuelva ingresar el mes." << endl;
          }
        }
        cout << "Ingrese el día (de 1-31): ";
        cin >> dia;
        
          while (stoi(dia)<=0 || stoi(dia)>31){
          cout << "El dia ingresado no es válido para este tipo de busca. Vuelva ingresar el dia." << endl << "Día (de 1-31): ";
          cin >> dia;
        }
          
        if (stoi(dia)<=9){
          dia = "0" + dia;
        }
          
        cout << "Teclea la hora de inicio de tu búsqueda (de 0-23): ";
        cin >> hora;
          
        while (stoi(hora)<0 || stoi(hora)>23){
          cout << "La hora ingresada no es válida para este tipo de busca. Vuelva ingresar la hora." << endl << "Hora (de 0-23): ";
          cin >> hora;
        }
          
        if (stoi(hora)<=9){
          hora = "0" + hora;
        }
        
        //SEGUNDA PARTE
        //diaa --> dia 2 ingresado
        //mess --> mes 2 ingresado
        //horaa --> hora 2 ingresada
        
        cout << "Teclea la fecha final de tu búsqueda" << endl;
        mes_not_valid = true;
        while (mes_not_valid){
          cout << "Ingrese el mes (ej: Jan):: ";
          cin >> mess;
          //busca que el mes ingresado esté en la lista
          string meses[] = {"","Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
          for (int i=1; i<=12; i++){
            if (mess == meses[i]){
              mess = to_string(i);
              mes_not_valid = false;
            }
          }
          if (mes_not_valid){
            cout << "El mes ingresado no es válido para este tipo de busca. Vuelva ingresar el mes." << endl;
          }
        }
          
        cout << "Ingrese el día (de 1-31): ";
        cin >> diaa;
          
        while (stoi(diaa)<=0 || stoi(diaa)>31){
          cout << "El dia ingresado no es válido para este tipo de busca. Vuelva ingresar el dia." << endl << "Día (de 1-31): ";
          cin >> diaa;
        }
          
        if (stoi(diaa)<=9){
          diaa = "0" + diaa;
        }
          
        cout << "Teclea la hora de final de tu búsqueda (0-23): ";
        cin >> horaa;
          
        while (stoi(horaa)<0 || stoi(horaa)>23){
          cout << "La hora ingresada no es válida para este tipo de busca. Vuelva ingresar la hora." << endl << "Hora (0-23): ";
          cin >> horaa;
        }
          
        if (stoi(horaa)<=9){
          horaa = "0" + horaa;
        }
          
        rangoInicial = std::stoi(mes + dia + horaa + "00" + "00");
        rangoFinal = std::stoi(mess + diaa + horaa + "00" + "00");
        
        // se valida que la fecha inicial sea menor que la fecha final
        if (rangoInicial > rangoFinal){
          cout << "Tu búsqueda no es válida. La fecha y la hora inicial deben ser menores a la fecha y la hora final que se están buscando. Ingresar de nuevo los datos: " << endl;
          mes_not_valid = true;
        }
        else{
          not_valid = false;
        }
      
        //binary search
        //primer elemento
        int posInicial = busquedaBinaria(informacion, rangoInicial, true);
        //elemento final
        int posFinal = busquedaBinaria(informacion, rangoFinal, false);
        //si no se encontraron los datos
        if (posInicial == -1 || posFinal == -1){
            cout << "No hay datos con ese rango en el archivo 'bitacora.txt'" << endl;
        }
        else{
            for (int i=posInicial; i<=posFinal; i++){
                cout << informacion[i].imprimir() << endl;
            }
          }
          return 0;
        }
}
