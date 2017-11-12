#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void lecturaTeclado(int n, int *mat); 
void lecturaDeArchivoTXT(FILE * fichero, char * cad, int tam);

int main(int argc, char const *argv[])
{
	char *cad;
	int tam,*mat,n;
	FILE* fichero; 

	lecturaTeclado(n,mat);	
	lecturaDeArchivoTXT(fichero, cad, tam);

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


