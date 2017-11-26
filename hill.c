#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define ALFABETO "ABCDEFGHIJKLMNOPQRSTUVWXYZ abcdefghijklmnopqrstuvwxyz"
#define TAMANO_ALFABETO 53

int *mat;
char *cad;
int *palabra_codificada;
int *codigo_cifrado;

int lecturaTeclado(int n); 
int lecturaDeArchivoTXT(FILE * fichero, int n);
void convierte_cadena(int tamano_palabra, int n);
void multiplica_matriz(int n, int w_size);
void convierte_codigo(int tamano_codigo);

int main(int argc, char const *argv[])
{
	int tam,n;
	int *cad_cod_cif;
	FILE* fichero; 

	n=lecturaTeclado(n);
	tam=lecturaDeArchivoTXT(fichero, n);
	convierte_cadena(tam, n);
	multiplica_matriz(n, tam);
	convierte_codigo(tam);

	free(cad);
	free(mat);
	free(palabra_codificada);
	free(codigo_cifrado);

	return 0;

}
int lecturaTeclado(int n){
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

int lecturaDeArchivoTXT(FILE * fichero, int n){
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

void convierte_cadena(int tamano_palabra, int n){
	int i, j;

	palabra_codificada = (int *) malloc(tamano_palabra * sizeof(int));

	for(i = 0; i < tamano_palabra; i++){
		if(cad[i] == 'Ñ')
			cad[i] = 'N';
		for(j = 0; j < TAMANO_ALFABETO; j++)
			if(cad[i] == ALFABETO[j]){
				palabra_codificada[i] = j;
				break;
			}
	}

	printf("Palabra codificada:");
	for(i = 0; i < tamano_palabra; ++i){
		if(i % n == 0)
			printf("\t");
		printf("%3d", palabra_codificada[i]);
	}
	printf("\n");
}

/* Entradas: m es la matriz, n es el tamaño de la matriz y w es la palabra codificada
 * Salida: apuntador al arreglo de enteros cifrado
 */
void multiplica_matriz(int n, int w_size){
	int m_aux[n][n];
	int w_aux[n];
	int r_aux[n];
	int i, j, k;
	int start, end, count;

	codigo_cifrado = (int *) malloc(w_size * sizeof(int));

	//copiando lo que tiene la memoria dinámica en una matriz estática para evitar problemas al usar mpi
	k = 0;
	for(i = 0; i < n; i++)
		for(j = 0; j < n; j++){
			m_aux[i][j] = mat[k];
			k++;
		}

	//iniciando el proceso de la multiplicacion de la matriz
	for(i = 0; i < w_size / n; i++){
		start = i * n;
		end = start + n;
		count = 0;

		//Copiando los grupos de n en el arreglo auxiliar
		for(j = start; j < end; j++){
			w_aux[count] = palabra_codificada[j];
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
			codigo_cifrado[j] = abs(r_aux[j] % 53);
			printf("%3d", codigo_cifrado[j]);
	}
}

void convierte_codigo(int tamano_codigo){
	int i, j;
	char *cadena_cifrada;
	FILE *salida;

	salida = fopen("cadena_cifrada.txt", "w");

	if(salida == NULL){
		printf("Error al abrir el archivo de salida\n");
	}else{
		cadena_cifrada = (char *) malloc(tamano_codigo * sizeof(char));

		for(i = 0; i < tamano_codigo; i++)
			cadena_cifrada[i] = ALFABETO[codigo_cifrado[i]];

		fputs(cadena_cifrada, salida);
		fclose(salida);
		free(cadena_cifrada);
	}
}