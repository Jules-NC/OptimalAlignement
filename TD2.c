#include<stdio.h>
#include<stdlib.h>
#include <sys/stat.h>
#include <string.h>

struct alignement
{
  char * x;
  char * y;
};


int ** create_table(int X, int Y);
int ** levenstein(char * x, char * y, int m, int n);
void affiche_Table(int ** T, int x, int y);
void affiche_backtrack(int **T, int x, int y);
void backtrack(int **T, int x, int y);

/* =============================================================== */
char * readtextfile(char * filename)
  /* Retourne le contenu du fichier texte filename */
/* =============================================================== */
{
  struct stat monstat;
  int N;
  char * text = NULL;
  FILE *fd = NULL;

  N = stat(filename, &monstat);
  if (N == -1)
  {
    fprintf(stderr, "error : bad file %s\n", filename);
    exit(0);
  }
  N = monstat.st_size;
  text = (char *)malloc(N+1);
  if (text == NULL)
  {   fprintf(stderr,"readtextfile() : malloc failed for text\n");
      exit(0);
  }
  fd = fopen(filename,"r");
  if (!fd)
  {
    fprintf(stderr, "readtextfile: can't open file %s\n", filename);
    exit(0);
  }
  
  fread(text, sizeof(char), N, fd);
  if((N>0) && (text[N-1] == '\n') ) text[N-1] = '\0';
  else text[N-1] = '\0';
  fclose(fd);
  return text;
}

/* =============================================================== */
int Imax(int a, int b)
/* Retourne  le maximum de a et b                                  */
/* =============================================================== */
{
  if (a < b) return b;
  else return a;	       
}

/* =============================================================== */
int Imin2(int a, int b)
/* Retourne  le minimum de a et b                                  */
/* =============================================================== */
{
  if (a < b) return a;
  else return b;	       
}

/* =============================================================== */
int Imin3(int a, int b, int c)
/* Retourne  le minimum de a, b et c                               */
/* =============================================================== */
{
  return Imin2(Imin2(a,b),c);
}

/* =============================================================== */
void retourne(char *c)
/* Retourner la chaîne de caractère c                              */
/* =============================================================== */
{
  char tmp;
  int m, j, i;
  m = strlen(c);
  j = m/2;
  for(i = 0; i < j; i++ ){
    tmp = c[i];
    c[i] = c[m-i-1];
    c[m-i-1] = tmp;
  }
}
/* =============================================================== */
void afficheSeparateurHorizontal(int nbcar)
/* =============================================================== */
{
  int i;
  printf("|-");
  for(i=0; i < nbcar; i++)
    printf("-");
  printf("-|-");
  for(i=0; i < nbcar; i++)
    printf("-");
  printf("-|\n");
}


/* =============================================================== */
void affiche(char* texte1, char* texte2, int nbcar)
  /* Affiche simultanément texte1 et texte 2 en positionnnant nbcar  
     caractères sur chaque ligne. */
/* =============================================================== */
{
  int i, l1, l2, l;
  
  char *t1,*t2;
  
  char out[512];
  
  l1 = strlen(texte1);
  l2 = strlen(texte2);

  t1 = (char*) malloc(sizeof(char) * (nbcar + 1));
  t2 = (char*)malloc(sizeof(char) * (nbcar + 1));

  l = Imax(l1, l2);
  afficheSeparateurHorizontal(nbcar);
  for(i = 0; i < l; i+= nbcar){
    if (i < l1) {
      strncpy(t1, &(texte1[i]), nbcar);
      t1[nbcar] = '\0';
    } else t1[0] = '\0';
    if (i < l2) {
      strncpy(t2, &(texte2[i]),nbcar);
      t2[nbcar] = '\0';
    } else t2[0] = '\0';
    
    sprintf(out, "| %c-%ds | %c-%ds |\n",'%', nbcar, '%', nbcar);
    printf(out, t1,t2);
  }
  afficheSeparateurHorizontal(nbcar);
  free(t1);
  free(t2);
}



/* =============================================================== */
void affiche2(char* texte1, char* texte2, int nbcar)
  /* idem affiche, mais avec un formattage différent
/* =============================================================== */
{

  int i, l1, l2, l;
  
  char *t1,*t2;
  
  char out[512];
  
  l1 = strlen(texte1);
  l2 = strlen(texte2);

  t1 = (char*) malloc(sizeof(char) * (nbcar + 1));
  t2 = (char*)malloc(sizeof(char) * (nbcar + 1));

  l = Imax(l1, l2);

  for(i = 0; i < l; i+= nbcar){
    if (i < l1) {
      strncpy(t1, &(texte1[i]), nbcar);
      t1[nbcar] = '\0';
    } else t1[0] = '\0';
    if (i < l2) {
      strncpy(t2, &(texte2[i]),nbcar);
      t2[nbcar] = '\0';
    } else t2[0] = '\0';
    
    sprintf(out, "x: %c-%ds \ny: %c-%ds\n",'%', nbcar, '%', nbcar);
    printf(out, t1,t2);

  }
  free(t1);
  free(t2);
}


/* =============================================================== */
int main(int argc, char **argv)
/* =============================================================== */
{
  char *x, *y; 
  
  if(argc != 3){
    printf("usage: %s text1 text2\n", argv[0]);
  }  
  
  //x = readtextfile(argv[1]);
  //y = readtextfile(argv[2]);;
  x = "chien";
  y = "maison";

  //affiche(x, y, 50);
  printf("\n");
  int ** T;
  T = levenstein(x, y, strlen(x), strlen(y));
  affiche_Table(T, strlen(x), strlen(y));
  backtrack(T, strlen(x), strlen(y));

  return 0;
  free(x);
  free(y);
  free(T);
}

int ** create_table(int m, int n){
  int ** T;
  T=malloc((m+1)*sizeof(int*));
  for(int i=0; i<m+1; i++)
          T[i]=malloc((n+1)*sizeof(int));
  return T;
}

int ** levenstein(char * x, char * y, int m, int n){
  int ** T = create_table(m,n);
  T[0][0] = 0;
  for(int j=1; j<=n; j++)
    T[0][j] = T[0][j-1] + 1;
  for(int i=1; i<=m; i++)
    T[i][0] = T[i-1][0] + 1;
  for(int i=1; i<=m; i++){
    for(int j=1; j<=n; j++){
      int fC;
      if(x[i-1] == y[j-1])
        fC = T[i-1][j-1];
      else
        fC = T[i-1][j-1] + 1;
      int sC = T[i][j-1] + 1;
      int tC = T[i-1][j] + 1;
      T[i][j] = Imin3(fC, sC, tC);
    }
  }
  return T;
}

void affiche_Table(int ** T, int x, int y){
  printf("Lev:\n");  
  for(int i=0; i<=x;++i){
    for(int j=0;j<=y;++j)
      printf("%d ", T[i][j]) ;
    printf("\n");
  }
}

void backtrack(int **T, int x, int y){
	printf("\n\n");
	//int haut, gauche, diag, min;
	while(x!=0 || y!=0){
		int haut, gauche, diag, min;
		if(x!=0){	gauche = T[x-1][y];} else {gauche = 10000;}
		if(y!=0){haut = T[x][y-1];} else {haut = 1000;}
		if(x!=0 && y!=0){diag = T[x-1][y-1];} else {diag = 10000;}
		min = Imin3(gauche, haut, diag);
		//printf("%d\n", min);
		if(min == diag){
			//printf("%d\n", T[x-1][y-1]);
			printf("X|Y\n");
			x--;
			y--;
		}
		else if(min == haut){
			printf("*|Y\n");
			//printf("%d\n", T[x][y-1]);
			y--;
		}
		else{
			printf("X|*\n");
			//printf("%d\n", T[x-1][y]);
			x--;
		}
	}
}
