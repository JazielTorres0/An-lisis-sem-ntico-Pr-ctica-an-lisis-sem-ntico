#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Adelantos (IMPORTANTE)
class DefVar;
class DefFunc;
class Parametro;


class ElementoTabla {
public:
    char tipo;
    string nombre;
    string ambito;

    ElementoTabla(char t, string n, string a)
        : tipo(t), nombre(n), ambito(a) {}
};

class Variable : public ElementoTabla {
public:
    Variable(char t, string n, string a)
        : ElementoTabla(t, n, a) {}
};

class Funcion : public ElementoTabla {
public:
    Funcion(char t, string n)
        : ElementoTabla(t, n, "") {}
};


class TablaSimbolos; // forward

class Nodo {
public:
    char tipoDato;
    Nodo *sig = NULL;

    static TablaSimbolos *tablaSimbolos;
    static string ambito;

    virtual void validaTipos(){
        tipoDato = 'v';
        if(sig) sig->validaTipos();
    }
};


class TablaSimbolos {
public:
    vector<ElementoTabla*> tabla;
    vector<string> *listaErrores;

    ElementoTabla *varLocal = NULL;
    ElementoTabla *varGlobal = NULL;

    TablaSimbolos(vector<string> *errores){
        listaErrores = errores;
    }

    void agrega(ElementoTabla *e){
        tabla.push_back(e);
    }

    void agrega(DefVar *defVar);
    void agrega(Parametro *param);
    void agrega(DefFunc *defFunc);

    bool varGlobalDefinida(string nombre){
        for(auto e : tabla)
            if(e->nombre == nombre && e->ambito == "")
                return true;
        return false;
    }

    bool varLocalDefinida(string nombre, string ambito){
        for(auto e : tabla)
            if(e->nombre == nombre && e->ambito == ambito)
                return true;
        return false;
    }

    bool funcionDefinida(string nombre){
        for(auto e : tabla)
            if(e->nombre == nombre && e->ambito == "")
                return true;
        return false;
    }

    void buscaIdentificador(string nombre){
        varLocal = varGlobal = NULL;

        for(auto e : tabla){
            if(e->nombre == nombre){
                if(e->ambito == Nodo::ambito)
                    varLocal = e;
                else if(e->ambito == "")
                    varGlobal = e;
            }
        }
    }

    void muestra(){
        cout << "\n--- TABLA ---\n";
        for(auto e : tabla)
            cout << e->nombre << " | " << e->tipo << " | " << e->ambito << endl;
    }
};


class Tipo {
public:
    string simbolo;

    char dimeTipo(){
        if(simbolo == "int") return 'i';
        if(simbolo == "float") return 'f';
        if(simbolo == "string") return 's';
        return 'v';
    }
};

class Identificador : public Nodo {
public:
    string simbolo;

    Identificador(string s){ simbolo = s; }

    void validaTipos(){
        Nodo::tablaSimbolos->buscaIdentificador(simbolo);

        if(Nodo::tablaSimbolos->varLocal)
            tipoDato = Nodo::tablaSimbolos->varLocal->tipo;
        else if(Nodo::tablaSimbolos->varGlobal)
            tipoDato = Nodo::tablaSimbolos->varGlobal->tipo;
        else{
            tipoDato = 'v';
            Nodo::tablaSimbolos->listaErrores->push_back(
                "Error: variable \"" + simbolo + "\" no definida"
            );
        }
    }
};

class DefVar : public Nodo {
public:
    Tipo *tipo;
    Identificador *listaVar;

    void validaTipos(){
        tipoDato = tipo->dimeTipo();
        Nodo::tablaSimbolos->agrega(this);
    }
};

class Parametro : public Nodo {
public:
    Tipo *tipo;
    Identificador *identificador;
};

class DefFunc : public Nodo {
public:
    Tipo *tipo;
    Identificador *identificador;
    Parametro *parametros;
    Nodo *bloque;
};


void TablaSimbolos::agrega(DefVar *defVar){
    char tipo = defVar->tipo->dimeTipo();
    Identificador *p = defVar->listaVar;

    while(p){
        if(Nodo::ambito == ""){
            if(varGlobalDefinida(p->simbolo))
                listaErrores->push_back("Error: variable global redefinida");
        } else {
            if(varLocalDefinida(p->simbolo, Nodo::ambito))
                listaErrores->push_back("Error: variable local redefinida");
        }

        agrega(new Variable(tipo, p->simbolo, Nodo::ambito));
        p = (Identificador*) p->sig;
    }
}

void TablaSimbolos::agrega(Parametro *param){
    while(param){
        char tipo = param->tipo->dimeTipo();
        string nombre = param->identificador->simbolo;

        if(varLocalDefinida(nombre, Nodo::ambito))
            listaErrores->push_back("Error: parametro redefinido");

        agrega(new Variable(tipo, nombre, Nodo::ambito));
        param = (Parametro*) param->sig;
    }
}

void TablaSimbolos::agrega(DefFunc *defFunc){
    string nombre = defFunc->identificador->simbolo;

    if(funcionDefinida(nombre)){
        listaErrores->push_back("Error: funcion redefinida");
        return;
    }

    agrega(new Funcion(defFunc->tipo->dimeTipo(), nombre));

    Nodo::ambito = nombre;

    if(defFunc->parametros)
        agrega(defFunc->parametros);

    if(defFunc->bloque)
        defFunc->bloque->validaTipos();

    Nodo::ambito = "";
}



class Semantico {
public:
    Nodo *arbol;
    TablaSimbolos *tablaSimbolos;
    vector<string> errores;

    Semantico(){
        Nodo::tablaSimbolos = tablaSimbolos = new TablaSimbolos(&errores);
    }

    void analiza(Nodo *arbol){
        this->arbol = arbol;
        arbol->validaTipos();

        tablaSimbolos->muestra();

        cout << "\n--- ERRORES ---\n";
        for(auto &e : errores)
            cout << e << endl;
    }
};


TablaSimbolos* Nodo::tablaSimbolos = NULL;
string Nodo::ambito = "";

int main(){
    Semantico sem;

    Nodo *arbol = new Nodo(); // prueba básica

    sem.analiza(arbol);

    return 0;
}

/*
int main(){
    Semantico sem;

    // Simular: int x;
    Tipo *tipo = new Tipo();
    tipo->simbolo = "int";

    Identificador *id = new Identificador("x");

    DefVar *var = new DefVar();
    var->tipo = tipo;
    var->listaVar = id;

    sem.analiza(var);

    return 0;
}
*/

/*
int main(){
    Semantico sem;

    Tipo *tipo = new Tipo();
    tipo->simbolo = "int";

    Identificador *id1 = new Identificador("x");//variable duplicada
    Identificador *id2 = new Identificador("x");

    id1->sig = id2;

    DefVar *var = new DefVar();
    var->tipo = tipo;
    var->listaVar = id1;

    sem.analiza(var);

    return 0;
}
*/
