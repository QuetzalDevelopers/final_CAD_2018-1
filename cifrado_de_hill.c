#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define ALFABETO "ABCDEFGHIJKLMNOPQRSTUVWXYZ abcdefghijklmnopqrstuvwxyz"
#define TAMANO_ALFABETO 53

//<<<<<<< HEAD
int lecturaTeclado(int n, int *mat); 
int lecturaDeArchivoTXT(FILE * fichero, char * cad, int n);
//=======
//int lecturaTeclado(int n, int *mat); 
//int lecturaDeArchivoTXT(FILE * fichero, char * cad, int tam);
void convierte_cadena(char *palabra, int tamano_palabra, int *palabra_codificada, int n);
void multiplica_matriz(int *m, int n, int *w, int w_size, int *r);
void convierte_codigo(int *codigo_cifrado, int tamano_codigo);
//>>>>>>> 8eb471138fe7297e1b3c5e033a409cbe4dc2980b

int main(int argc, char const *argv[])
{
	char *cad;
	int tam,*mat,n;
	int *cad_cod, n_cad_cod;
	int *cad_cod_cif;
	FILE* fichero; 

//<<<<<<< HEAD
	n=lecturaTeclado(n,mat);
	tam=lecturaDeArchivoTXT(fichero, cad, n);
//=======
	//lecturaTeclado(n,mat);	
	//lecturaDeArchivoTXT(fichero, cad, tam);
	convierte_cadena(cad, tam, cad_cod, n);
	printf("Voy a multiplicar la matriz\n");
	multiplica_matriz(mat, n, cad_cod, tam, cad_cod_cif);
	printf("Voy a recodificar\n");
	convierte_codigo(cad_cod_cif, tam);
//>>>>>>> 8eb471138fe7297e1b3c5e033a409cbe4dc2980b
	free(cad);
	free(mat);
	free(cad_cod);
	free(cad_cod_cif);

	return 0;

}
int lecturaTeclado(int n, int *mat){
	//funcion que permite la lectura de datos de teclado de n y de la matriz
	int i,j,val; 
	printf("Ingrese el valor de n\n");
	scanf("%d",&n);
	mat = (int *)malloc((n*n)* sizeof(int));
	printf("Ingrese la matriz de %d*%d\n",n,n);
	//validar si es transponible?
	// el scanf no permite el ingreso de una matriz menor al tamaño establecido
	for(i=0; i<(n*n); i++){
			scanf("%d",&val);
			mat[i]=val;
	}
	return n;
}

int lecturaDeArchivoTXT(FILE * fichero, char * cad, int n){
	int letra, contador,i,j,inicio,fin,modulo;
	fichero = fopen("ejemplo.txt","r");
	contador=0; 
	while(letra!=EOF)//para saber el tamaño de la cadena
    {
        letra= getc(fichero);  //obtienes un caracter y lo guardas, el valor es su equivalente en ascii, por eso es de tipo int
        contador++;
    }
    fclose(fichero);
    contador=contador-1;
    modulo= contador%n;//revisar si se pueden agrupar en grupos de n
   	printf("modulo=> %d mod %d = %d\n",contador,n, modulo);
   	if(modulo>0){
    		contador=contador+(n-modulo); 
    	}
    
    cad  = (char *)malloc(contador * sizeof(char));
    fichero = fopen("ejemplo.txt","r");
    for(i=0; i<contador; i++){
    	cad[i]=getc(fichero);
    	if(i>(contador-n)){
    		cad[i]='x';
    	}
    }
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
	
	return contador;
}

void convierte_cadena(char *palabra, int tamano_palabra, int *palabra_codificada, int n){
	int i, j;

	palabra_codificada = (int *) malloc(tamano_palabra * sizeof(int));

	for(i = 0; i < tamano_palabra; i++){
		if(palabra[i] == 'Ñ')
			palabra[i] = 'N';
		for(j = 0; j < TAMANO_ALFABETO; j++)
			if(palabra[i] == ALFABETO[j]){
				palabra_codificada[i] = j;
				break;
			}
	}
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

	r = (int *) malloc(w_size * sizeof(int));

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
		count = 0;

		//Copiando los grupos de n en el arreglo auxiliar
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
		for(j = start; j < end; j++)
			r[j] = r_aux[j] % TAMANO_ALFABETO;
	}
}

void convierte_codigo(int *codigo_cifrado, int tamano_codigo){
	int i, j;
	char *cadena_cifrada;
	FILE *salida;

	salida = fopen("cadena_cifrada.txt", "w");

	if(salida == NULL){
		printf("Error al abrir el archivo de salida\n");
	}else{
		cadena_cifrada = (char *) malloc(tamano_codigo * sizeof(char));

		for(i = 0; i < tamano_codigo; i++)
			for(j = 0; j < TAMANO_ALFABETO; j++)
				if(codigo_cifrado[i] == ALFABETO[j]){
					cadena_cifrada[i] = ALFABETO[j];
					break;
				}

		fputs(cadena_cifrada, salida);
		fclose(salida);
		free(cadena_cifrada);
	}
}