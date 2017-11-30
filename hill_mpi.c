#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <mpi.h>

#define ALFABETO "ABCDEFGHIJKLMNOPQRSTUVWXYZ abcdefghijklmnopqrstuvwxyz"
#define TAMANO_ALFABETO 53

int *crea_arreglo_enteros(int n);
char *crea_cadena(int n);
void lecturaTeclado(int n, int *mat); 
int obten_longitud_cadena(int n);
int obten_grupos_por_proceso(int *grupos_por_proceso, int tamano_cadena, int grupo, int numero_procesos);
void lecturaDeArchivoTXT(char * cad, int n, int contador);
void convierte_cadena(char *palabra, int tamano_palabra, int *palabra_codificada, int n);
void multiplica_matriz(int *m, int n, int *w, int w_size, int *r);
void copia_parte_cadena(char *fuente, char *destino, int inicio, int fin);
void concatena_arreglo(int *fuente, int *extension, int inicio, int fin);
void convierte_codigo(int *codigo_cifrado, int tamano_codigo);

int main(int argc, char *argv[ ]){
	char *cad, *parte_cad;
	int *mat, n;
	//Para las pruebas se utilizará una matriz y una n estáticas
	//int mat[9] = {1, 2, 3, 0, 4, 5, 1, 0, 6};
	//int n = 3;
	int tam, gpp, ext, tam_parte;
	int *cad_cod, *parte_cad_cod;
	int *cad_cod_cif, *parte_cad_cod_cif;
	int id, nproc;
	double t0, t1;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &id);
	MPI_Comm_size(MPI_COMM_WORLD, &nproc);

	//El proceso 0 es el que lee los datos
	if(id == 0){
		printf("Ingrese el valor de n\n");
		scanf("%d",&n);
	}

	//Distribuyendo el valor de n a todos los procesos
	MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
	//Asignacion de espacio para la matriz (todos los procesos)
	mat = crea_arreglo_enteros(n * n);

	//leyendo los datos de la matriz
	if(id == 0){
		printf("Ingrese la matriz de %d*%d\n", n, n);
		lecturaTeclado(n, mat);
		tam = obten_longitud_cadena(n);
		//printf("Proceso %d: tamano_cadena = %d\n", id, tam);
		ext = obten_grupos_por_proceso(&gpp, tam, n, nproc);
		//printf("Proceso %d: excedentes = %d\n", id, ext);
		cad = crea_cadena(tam); 
		lecturaDeArchivoTXT(cad, n, tam);
		cad_cod_cif = crea_arreglo_enteros(tam);
		t0 = MPI_Wtime( );
	}

	//Distribuyendo los datos necesarios a los procesos
	MPI_Bcast(mat, n * n, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&gpp, 1, MPI_INT, 0, MPI_COMM_WORLD);

	//Calculando el tamaño de la parte que le tocara a cada proceso
	tam_parte = gpp * n;
	printf("Proceso %d: tamano_parte = %d\n", id, tam_parte);

	//Asignando espacio para cada parte
	parte_cad = crea_cadena(tam_parte);
	parte_cad_cod = crea_arreglo_enteros(tam_parte);
	parte_cad_cod_cif = crea_arreglo_enteros(tam_parte);

	//Dividiendo la cadena entre los procesos
	MPI_Scatter(cad, tam_parte, MPI_CHAR, parte_cad, tam_parte, MPI_CHAR, 0, MPI_COMM_WORLD);

	//Cada proceso hace su parte de la cadena
	convierte_cadena(parte_cad, tam_parte, parte_cad_cod, n);
	multiplica_matriz(mat, n, parte_cad_cod, tam_parte, parte_cad_cod_cif);

	//Juntando la cadena codificada cifrada a la raiz
	MPI_Gather(parte_cad_cod_cif, tam_parte, MPI_INT, cad_cod_cif, tam_parte, MPI_INT, 0, MPI_COMM_WORLD);

	//convierte_codigo debe ser atendida solo por 0 debido a que escribe en archivo
	if(id == 0){
		//Procesando grupos excedentes
		if(ext > 0){
			char *cad_aux;
			int *cad_cod_aux;
			int *cad_cod_cif_aux;
			int ini, fin;

			cad_aux = crea_cadena(ext * n);
			cad_cod_aux = crea_arreglo_enteros(ext * n);
			cad_cod_cif_aux = crea_arreglo_enteros(ext * n);
			ini = tam - (ext * n);
			fin = tam;
			copia_parte_cadena(cad, cad_aux, ini, fin);
			convierte_cadena(cad_aux, ext * n, cad_cod_aux, n);
			multiplica_matriz(mat, n, cad_cod_aux, ext * n, cad_cod_cif_aux);
			concatena_arreglo(cad_cod_cif, cad_cod_cif_aux, ini, fin);
		}
		convierte_codigo(cad_cod_cif, tam);
		t1 = MPI_Wtime( );
		printf("Tiempo de ejecucion = %1.3f ms\n", (t1 - t0) * 1000);
	}

	//Liberando espacio en memoria
	if(id == 0){
		free(cad);
		free(cad_cod_cif);
	}
	free(mat);
	free(parte_cad);
	free(parte_cad_cod);
	free(parte_cad_cod_cif);

	//Finalizando la región paralela
	MPI_Finalize( );

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

//Asigna en numero de grupos de la cadena que le tocara a cada proceso, devuelve los grupos restantes
int obten_grupos_por_proceso(int *grupos_por_proceso, int tamano_cadena, int grupo, int numero_procesos){
	int numero_grupos;

	numero_grupos = tamano_cadena / grupo;
	*grupos_por_proceso = numero_grupos / numero_procesos;

	return numero_grupos % numero_procesos;
}

void lecturaDeArchivoTXT(char *cad, int n, int contador){
	int letra, i,j,inicio,fin;
	FILE *fichero;

    fichero = fopen("ejemplo.txt","r");

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

	/*printf("Palabra codificada:\n");
	for(i = 0; i < tamano_palabra; i++){
		if(i % n == 0 && i != 0)
			printf("\n");
		printf("%3d", palabra_codificada[i]);
	}
	printf("\n");*/
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

	/*printf("Palabra codificada cifrada:\n");
	for(i = 0; i < w_size; i++){
		if(i % n == 0 && i != 0)
			printf("\n");
		printf("%3d", r[i]);
	}
	printf("\n");*/
}

void copia_parte_cadena(char *fuente, char *destino, int inicio, int fin){
	int i, j;

	j = 0;
	for(i = inicio; i < fin; i++){
		destino[j] = fuente[i];
		j++;
	}
}

void concatena_arreglo(int *fuente, int *extension, int inicio, int fin){
	int i, j;

	j = 0;
	for(i = inicio; i < fin; i++){
		fuente[i] = extension[j];
		j++;
	}
}

void convierte_codigo(int *codigo_cifrado, int tamano_codigo){
	int i, j;
	char *cadena_cifrada;
	FILE *salida;

	salida = fopen("cadena_cifrada_mpi.txt", "w");

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