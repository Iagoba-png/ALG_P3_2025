#include <stdio.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

void inicializar_semilla() {      
    srand(time(NULL));
}

double microsegundos() {
    struct timeval t;
    if (gettimeofday(&t, NULL) < 0 )
        return 0.0;
    return (t.tv_usec + t.tv_sec * 1000000.0);
}

void aleatorio(int v[], int n) {
    int i, m=2*n+1;
    for (i=0; i < n; i++)
        v[i] = (rand() % m) - n;
}

void ascendente(int v [], int n) {
    int i;
    for (i=0; i < n; i++)
        v[i] = i;
}

void descendente(int v[], int n){
    int i,j=0;
    for (i=n; i > 0; i--){
        v[j] = i;
        j++;
    }
}

void ord_ins(int v[], int n){
    int i, x, j;
    for(i = 1; i < n;  i++){
        x = v[i];
        j = i-1;
        while(j >= 0 && v[j] > x){
            v[j+1] = v[j];
            j = j-1;
        }
        v[j+1] = x;
    }
}

void intercambiar(int *a, int *b){
    int temp = *a;
    *a = *b;
    *b = temp;
}

void mediana3 (int v[], int i, int j){
    int k = (i+j)/2;
    
    if(v[k] > v[j]) intercambiar(&v[k], &v[j]);
    if(v[k] > v[i]) intercambiar(&v[k], &v[i]);
    if(v[i] > v[j]) intercambiar(&v[i], &v[j]);
}

void ordenar_aux(int v[], int izq, int der, int umbral){
    int pivote, i, j;
    if(izq + umbral <= der){
        mediana3(v, izq, der);
        pivote = v[izq];
        i = izq;
        j = der;
        do{
            do { i = i + 1; } while (v[i] < pivote);
            do { j = j - 1; } while (v[j] > pivote);
            intercambiar(&v[i], &v[j]);
        } while (j > i);
        intercambiar(&v[i], &v[j]); //deshacemos el último intercambio
        intercambiar(&v[izq], &v[j]);
        ordenar_aux(v, izq, j - 1, umbral);
        ordenar_aux(v, j + 1, der, umbral);
    }
}

void ord_rapida (int v[], int n, int umbral) {
    ordenar_aux(v, 0, n-1, umbral);
    if(umbral > 1)  ord_ins(v, n);
}

bool isOrdenado(int v[], int n) {
    int i;
    for (i = 1; i < n; i++) {
        if (v[i] < v[i - 1]) {
            return false;
        }
    }
    return true;
}

void imprimir_vector(int v[], int n) {
    for (int i = 0; i < n; i++)
        printf("%d%s", v[i], (i == n - 1) ? "" : ", ");
}

//TESTS
void testOrd_rapida_general(int v[], int n, int umbral, 
                        void (*inicializar)(int[], int)) {
    int cont1 = 0, cont2 = 0, i;
    inicializar(v, n);
    for (i = 0; i < n; i++) {
        printf(" %d", v[i]);
        cont1 += v[i];
    }
    printf("\nordenado? %d\n", isOrdenado(v, n));
    printf("ordenando...\n");

    ord_rapida(v, n, umbral);

    for (i = 0; i < n; i++) {
        printf(" %d", v[i]);
        cont2 += v[i];
    }

    if (cont1 != cont2) {
        printf("\nError: los valores no coinciden.\n");
    } else {
        printf("\nordenado? %d\n", isOrdenado(v, n));
    }
}

void testOrd_rapida(){
    int v[20], i, j, n, umbral, tam, umb;
    int tamaños[] ={10,15,20};
    int umbrales[] = {1, 10, 100};
    tam = sizeof(tamaños) / sizeof(tamaños[0]);
    umb = sizeof(umbrales) / sizeof(umbrales[0]);
    
    for(i = 0; i < tam; i++){
        n = tamaños[i];
        printf("\n----TAMAÑO DEL VECTOR = %d ----\n", n);
        for(j = 0; j < umb; j++) {
            umbral = umbrales[j];
            printf("\n---- UMBRAL = %d ----\n", umbral);

            printf("\nCaso ALEATORIO:\n");
            testOrd_rapida_general(v, n, umbral, aleatorio);
            printf("\nCaso ASCENDENTE:\n");
            testOrd_rapida_general(v, n, umbral, ascendente);
            printf("\nCaso DESCENDENTE:\n");
            testOrd_rapida_general(v, n, umbral, descendente);
        }
    }
}

double calcularTiempo(int n, bool *bucle, void (*inicializar) (int[], int), int umbral){
    int i;
    double a, b, t, t1, t2;
    int *v = malloc(n * sizeof(int));

    *bucle = false;
    inicializar(v, n);
    a = microsegundos();
    ord_rapida(v, n, umbral);
    b = microsegundos();
    t = b - a;

    if (t < 500) {
        *bucle = true;

        a = microsegundos();
        for (i = 0; i < 1000; i++) {
            inicializar(v, n);
            ord_rapida(v, n, umbral);
        }
        b = microsegundos();
        t1 = b - a;

        a = microsegundos();
        for (i = 0; i < 1000; i++) {
            inicializar(v, n);
        }
        b = microsegundos();
        t2 = b - a;

        t = (t1 - t2) / 1000.0;
    }

    free(v);
    return t;
}

void tiempos_generico(void (*inicializar)(int[], int), int umbral,
                      double cInf, double cAjust, double cSup,
                      const char *fInf, const char *fAjust, const char *fSup) {
    int n;
    double t;
    bool repeat;

    printf("%15s%20s%20s%20s%20s\n", "n", "t(n)", fInf, fAjust, fSup);
    for (n = 500; n <= 64000; n *= 2) {
        t = calcularTiempo(n, &repeat, inicializar, umbral);
        if (repeat) printf("(*) "); else printf("    ");
        double valInf = (strcmp(fInf, "t(n)/n*log(n)") == 0) ? t / (n * log(n))
                        : (strcmp(fInf, "t(n)/n") == 0) ? t / n
                        : t / pow(n, cInf);

        double valAjust = (strcmp(fAjust, "t(n)/n*log(n)") == 0) ? t / (n * log(n))
                          : t / pow(n, cAjust);

        double valSup = (strcmp(fSup, "t(n)/n*log(n)") == 0) ? t / (n * log(n))
                        : t / pow(n, cSup);

        printf("%14d%19.4f%19.7f%19.7f%19.7f\n", n, t, valInf, valAjust, valSup);
    }
}

void tiempo_alg() {
    int umbrales[] = {1, 10, 100};
    printf("\nOrdenación rápida:\n");

    for (int i = 0; i < 3; i++) {
        int umbral = umbrales[i];
        printf("\n---- UMBRAL = %d ----\n", umbral);

        printf("\n\tVector ascendente:\n");
        if (umbral == 1)
            tiempos_generico(ascendente, umbral, 1.0, 1.1, 1.3, 
                            "t(n)/n", "t(n)/n^1.1", "t(n)/n^1.3");
        else if (umbral == 10)
            tiempos_generico(ascendente, umbral, 0.9, 1.05, 1.2, 
                            "t(n)/n^0.9", "t(n)/n^1.05", "t(n)/n^1.2");
        else
            tiempos_generico(ascendente, umbral, 1.0, 1.1, 1.3, 
                            "t(n)/n", "t(n)/n^1.1", "t(n)/n^1.3");

        printf("\n\tVector descendente:\n");
        if (umbral == 1)
            tiempos_generico(descendente, umbral, 1.0, 1.12, 1.3, 
                            "t(n)/n", "t(n)/n^1.12", "t(n)/n^1.3");
        else if (umbral == 10)
            tiempos_generico(descendente, umbral, 1.0, 1.15, 1.3, 
                            "t(n)/n", "t(n)/n^1.15", "t(n)/n^1.3");
        else
            tiempos_generico(descendente, umbral, 1.0, 1.12, 1.3, 
                            "t(n)/n", "t(n)/n^1.12", "t(n)/n^1.3");

        printf("\n\tVector aleatorio:\n");
        if (umbral == 1)
            tiempos_generico(aleatorio, umbral, 1.0, 1.1, 1.3, 
                            "t(n)/n", "t(n)/n^1.1", "t(n)/n^1.3");
        else if (umbral == 10)
            tiempos_generico(aleatorio, umbral, 1.0, 1.1, 1.2, 
                            "t(n)/n", "t(n)/n*log(n)", "t(n)/n^1.2");
        else
            tiempos_generico(aleatorio, umbral, 1.0, 1.1, 1.2, 
                            "t(n)/n", "t(n)/n*log(n)", "t(n)/n^1.2");
    }
}

int main() {
    int i;
    inicializar_semilla();
    testOrd_rapida();
   for(i = 0; i < 3; i++) {
    tiempo_alg();
   }

}