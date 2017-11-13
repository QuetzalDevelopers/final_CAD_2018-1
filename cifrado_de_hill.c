#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void lecturaTeclado(int n, int *mat); 
void lecturaDeArchivoTXT(FILE * fichero, char * cad, int tam);
void convierte_cadena(char *palabra, int tamano_palabra, int *palabra_codificada, int *tamano_palabra_codificada, int n);

int main(int argc, char const *argv[])
{
	char *cad;
	int tam,*mat,n;
	int *cad_cod, n_cad_cod;
	FILE* fichero; 

	lecturaTeclado(n,mat);	
	lecturaDeArchivoTXT(fichero, cad, tam);
	convierte_cadena(cad, tam, cad_cod, &n_cad_cod, n);

	return 0;

}
void lecturaTeclado(int n, int * mat){
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
}
void lecturaDeArchivoTXT(FILE * fichero, char * cad, int tam){
	int letra, contador,i;
	fichero = fopen("ejemplo.txt","r");
	contador=0; 
	while(letra!=EOF)//para saber el tamaño de la cadena
    {
        letra= getc(fichero);  //obtienes un caracter y lo guardas, el valor es su equivalente en ascii, por eso es de tipo int
        contador++;
        printf("%c\n",letra);
    }
    fclose(fichero);
    printf("\n contador %d\n",contador);
    cad  = (char *)malloc(contador * sizeof(char));
    fichero = fopen("ejemplo.txt","r");
    for(i=0; i<contador; i++){cad[i]=getc(fichero);printf("%c",cad[i]);}
	fclose(fichero);
}

void convierte_cadena(char *palabra, int tamano_palabra, int *palabra_codificada, int *tamano_palabra_codificada, int n){
	char alfabeto[ ] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ abcdefghijklmnopqrstuvwxyz";
	int tamano_alfabeto, i, j;

	tamano_alfabeto = strlen(alfabeto) + 1;

	if(tamano_palabra % n == 0)
		*tamano_palabra_codificada = tamano_palabra;
	else
		*tamano_palabra_codificada = tamano_palabra + n;

	palabra_codificada = (int *) malloc(*tamano_palabra_codificada * sizeof(int));

	for(i = 0; i < tamano_palabra; i++){
		if(palabra[i] == 'Ñ')
			palabra[i] = 'N';
		for(j = 0; j < tamano_alfabeto; j++)
			if(palabra[i] == alfabeto[j]){
				palabra_codificada[i] = j;
				break;
			}
	}

	if(tamano_palabra != *tamano_palabra_codificada)
		for(i = tamano_palabra; i < *tamano_palabra_codificada; i++)
			palabra_codificada[i] = 'X';
}
