#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>

#define ALFABETO "ABCDEFGHIJKLMNOPQRSTUVWXYZ abcdefghijklmnopqrstuvwxyz"
#define TAMANO_ALFABETO 53

int *crea_arreglo_enteros(int n);
char *crea_cadena(int n);
void lecturaTeclado(int n, int *mat); 
int obten_longitud_cadena(int n);
void lecturaDeArchivoTXT(char * cad, int n, int contador);
void convierte_cadena(char *palabra, int tamano_palabra, int *palabra_codificada, int n);
void multiplica_matriz(int *m, int n, int *w, int w_size, int *r);
void convierte_codigo(int *codigo_cifrado, int tamano_codigo);

int main(int argc, char *argv[ ]){
	struct timeval t0, t1;
	char *cad;
	int tam,*mat,n;
	int *cad_cod, n_cad_cod;
	int *cad_cod_cif; 

	printf("Ingrese el valor de n\n");
	scanf("%d",&n);
	mat = crea_arreglo_enteros(n*n);
	printf("Ingrese la matriz de %d*%d\n",n,n);
	lecturaTeclado(n,mat);
	tam = obten_longitud_cadena(n);
	cad = crea_cadena(tam); 
	lecturaDeArchivoTXT(cad, n, tam);
	cad_cod = crea_arreglo_enteros(tam);
	cad_cod_cif = crea_arreglo_enteros(tam);
	gettimeofday(&t0, 0);
	convierte_cadena(cad, tam, cad_cod, n);
	multiplica_matriz(mat, n, cad_cod, tam, cad_cod_cif);
	convierte_codigo(cad_cod_cif, tam);
	gettimeofday(&t1, 0);
	printf("Tiempo de ejecucion: %1.3f ms\n", (double) (t1.tv_usec - t0.tv_usec) / 1000);

	free(cad);
	free(mat);
	free(cad_cod);
	free(cad_cod_cif);

	return 0;

}

int *crea_arreglo_enteros(int n){
	return (int *) malloc(n * sizeof(int));
}

char *crea_cadena(int n){
	return (char *) malloc(n * sizeof(int));
}

void lecturaTeclado(int n, int *mat){
	//funcion que permite la lectura de datos de teclado de n y de la matriz
	int i,j,val; 
	//validar si es transponible?
	// el scanf no permite el ingreso de una matriz menor al tamaño establecido
	for(i=0; i<(n*n); i++){
			scanf("%d",&val);
			mat[i]=val;
	}
}

int obten_longitud_cadena(int n){
	FILE *archivo;
	int contador, modulo;

	archivo = fopen("ejemplo.txt", "r");
	if(archivo == NULL){
		printf("No se encuentra el archivo de entrada\n");
		return 0;
	}

	contador = 0;
	while(getc(archivo) != EOF)
		contador++;
	contador=contador-1;
	modulo = contador%n;//revisar si se pueden agrupar en grupos de n
   	printf("modulo=> %d mod %d = %d\n",contador,n, modulo);
   	if(modulo>0){
    		contador=contador+(n-modulo); 
    	}
	fclose(archivo);

	//Regresa el tamaño menos el carcater de fin de archivo
	return contador;
}

void lecturaDeArchivoTXT(char *cad, int n, int contador){
	int letra, i,j,inicio,fin;
	FILE *fichero;

    fichero = fopen("ejemplo.txt","r");
    /*for(i=0; i<contador; i++){
    	cad[i]=getc(fichero);
		/*if(cad[i] == 'Ñ')
			cad[i] = 'N';
		else if (cad[i] == 'ñ')
			cad[i] == 'n';
		if()
			cad[i]='x';
    }*/

    i = 0;
    while((letra = getc(fichero)) != EOF){
    	cad[i] = letra;
    	i++;
    }
    if(i < contador)
    	for(j = i - 1; j < contador; j++)
    		cad[j] = 'x';
	fclose(fichero);
	inicio=0;
	fin=n;
	printf("\n");
    
    //para imprimir en periodos de n 
	for(i=0; i<(contador/n); i++){
		for (j =inicio; j < fin; ++j){
			printf("%c",cad[j]);	
		}
		printf("\n");
		inicio=inicio+n;
		fin=inicio+n; 
	}
}


void convierte_cadena(char *palabra, int tamano_palabra, int *palabra_codificada, int n){
	int i, j, bandera;

	for(i = 0; i < tamano_palabra; i++){
		bandera = 0;
		//Busqueda en el alfabeto
		for(j = 0; j < TAMANO_ALFABETO; j++)
			if(palabra[i] == ALFABETO[j]){
				palabra_codificada[i] = j;
				bandera = 1;
				break;
			}

		//Si el caracter se encuentra fuera del ASCII, pone 50 (X)
		if(bandera == 0)
			palabra_codificada[i] = 50;
	}

	printf("Palabra codificada:\n");
	for(i = 0; i < tamano_palabra; ++i){
		if(i % n == 0 && i != 0)
			printf("\n");
		printf("%3d", palabra_codificada[i]);
	}
	printf("\n");
}

/* Entradas: m es la matriz, n es el tamaño de la matriz y w es la palabra codificada
 * Salida: apuntador al arreglo de enteros cifrado
 */
void multiplica_matriz(int *m, int n, int *w, int w_size, int *r){
	int m_aux[n][n];
	int w_aux[n];
	int r_aux[n];
	int i, j, k;
	int start, end, count;

	//copiando lo que tiene la memoria dinámica en una matriz estática para evitar problemas al usar mpi
	k = 0;
	for(i = 0; i < n; i++)
		for(j = 0; j < n; j++){
			m_aux[i][j] = m[k];
			k++;
		}

	//iniciando el proceso de la multiplicacion de la matriz
	for(i = 0; i < w_size / n; i++){
		start = i * n;
		end = start + n;

		//Copiando los grupos de n en el arreglo auxiliar
		count = 0;
		for(j = start; j < end; j++){
			w_aux[count] = w[j];
			count++;
		}

		//limpiando el arreglo auxiliar del resultado;
		for(j = 0; j < n; j++)
			r_aux[j] = 0;

		//multiplicando la matriz con el grupo de la palabra
		for(j = 0; j < n; j++)
			for(k = 0; k < n; k++)
				r_aux[j] += m_aux[j][k] * w_aux[k];

		//se agrega al arreglo del resultado la parte de la palabra cifrada
		count = 0;
		for(j = start; j < end; j++){
			r[j] = r_aux[count] % TAMANO_ALFABETO;
			count++;
		}
	}

	printf("Palabra codificada cifrada:\n");
	for(i = 0; i < w_size; i++){
		if(i % n == 0 && i != 0)
			printf("\n");
		printf("%3d", r[i]);
	}
	printf("\n");
}

void convierte_codigo(int *codigo_cifrado, int tamano_codigo){
	int i, j;
	char *cadena_cifrada;
	FILE *salida;

	salida = fopen("cadena_cifrada.txt", "w");

	if(salida == NULL){
		printf("Error al abrir el archivo de salida\n");
	}else{
		cadena_cifrada = (char *) malloc(tamano_codigo + 1 * sizeof(char));
		//Proceso inverso de busqueda en el alfabeto
		for(i = 0; i < tamano_codigo; i++)
			cadena_cifrada[i] = ALFABETO[codigo_cifrado[i]];
		cadena_cifrada[tamano_codigo] = '\0';

		printf("Cadena cifrada: %s\n", cadena_cifrada);
		fputs(cadena_cifrada, salida);
		fclose(salida);
		free(cadena_cifrada);
	}
}