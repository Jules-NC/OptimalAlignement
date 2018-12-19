#include<stdio.h>
#include<stdlib.h>
#include <sys/stat.h>
#include <string.h>


struct alignement{
  char * x;
  char * y;
};

//	AMOI !!!!
int ** create_table(int X, int Y);
int ** levenstein(char *x, char * y, int m, int n);
void affiche_Table(int **T, int x, int y);
void affiche_backtrack(int **T, int x, int y);
struct alignement backtrack(int **T, char* X, char* Y);





char * readtextfile(char * filename){
  /* Retourne le contenu du fichier texte filename */
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


int Imax(int a, int b)
{
  if (a < b) return b;
  else return a;	       
}


int Imin2(int a, int b)
{
  if (a < b) return a;
  else return b;	       
}


int Imin3(int a, int b, int c){
  return Imin2(Imin2(a,b),c);
}


void retourne(char *c){
/* Retourner la chaîne de caractère c */
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


void afficheSeparateurHorizontal(int nbcar){
  int i;
  printf("|-");
  for(i=0; i < nbcar; i++)
    printf("-");
  printf("-|-");
  for(i=0; i < nbcar; i++)
    printf("-");
  printf("-|\n");
}


void affiche(char* texte1, char* texte2, int nbcar){
  /* Affiche simultanément texte1 et texte 2 en positionnnant nbcar  
     caractères sur chaque ligne. */
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


void affiche2(char* texte1, char* texte2, int nbcar){

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


int main(int argc, char **argv){
  char *X, *Y; 
  int x, y, lev_dist;
  int ** T;
  struct alignement Prime;

  
  if(argc != 3){
    printf("usage: %s text1 text2\n", argv[0]);
  }
  
  X = readtextfile(argv[1]);
  Y = readtextfile(argv[2]);

  x = strlen(X);
  y = strlen(Y);
  
  // 	Q1 : LEVENSTEIN DISTANCE (ya pas les options pour les coûts car inutile ici, je le met pas, tout est à 1)
  T = levenstein(X, Y, x, y);
  lev_dist = T[x][y];
  
  //	Q2-3 : ALIGNEMENT OPTIMAL X' et Y', commenté ds la fct°.
  Prime = backtrack(T, X, Y);
  
  // Affichage
  printf("Chaines de bases (X et Y):\n");
  affiche(X, Y, 100);
  printf("Distance d'édition: %d\n", lev_dist);
  printf("\nAlignement optimal (X' et Y'):\n");
  affiche(Prime.x, Prime.y, 100);//1 + Imax(strlen(Prime.x), strlen(Prime.y)));

  free(X);
  free(Y);
  //	Celui-la je m'en souvenais pas tiens !
  for(int i=0; i<=x; i++){
	free(T[i]);
  }
  free(T);
  free(Prime.x);
  free(Prime.y);
  return 0;
}


int ** create_table(int m, int n){
	/*	Créé un tableau d'int de taille m*n	*/
  int ** T;
  T=malloc((m+1)*sizeof(int*));
  for(int i=0; i<m+1; i++)
          T[i]=malloc((n+1)*sizeof(int));
  return T;
}


int ** levenstein(char * x, char * y, int m, int n){
	/*	Oui, toutes les opérations sont à 1, parsque ca marche très bien comme ca, on travaille sur des textes, sacrebleu !!!!	*/
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
      // Bien recopié !
    }
  }
  return T;
}


void affiche_Table(int ** T, int x, int y){
	/*	Affiche... le tableau int** ...   ..     .*/
  printf("Lev:\n");  
  for(int i=0; i<=x;++i){
    for(int j=0;j<=y;++j)
      printf("%d ", T[i][j]) ;
    printf("\n");
  }
}


struct alignement backtrack(int **T, char* X, char* Y){
	//	T : la matrice d'édition, x : taille x de T, y : taille y T
	int x, y, xi, yi, haut, gauche, diag, min, max, i;
	char *Xp, *Yp;
	
	//	Initialisation des paramètres
	x = strlen(X);							// 	|X|
	y = strlen(Y);		
						//	|Y|
	//	On connait déjà la longueur max de X' et de Y' (alignement => |X'| = |Y'|), n délétions + n additions, mais bon en vrai je penche sur du Max(|X|, |Y|) + 1 pasque ca c'est des substitutions du cp
	Xp = malloc((x+y+1)*sizeof(char));		// 	X'
	Yp = malloc((x+y+1)*sizeof(char));		//	Y'
	strcpy(Xp, "");
	strcpy(Yp, "");
	
	//	 Compteurs pour trouver le bon char dans X et Y pour X' et Y'
	xi = x-1;
	yi = y-1;
	
	// 	La distance d'édition va toujours être décroissante, donc on sait que T[x][y]+1 ne sera jamais dépassé (valeur initiale du backtrack => valeur maximale non stricte du chemin suivi par celui-ci)
	max = T[x][y]+1;	
	i = 0;
	
	//	BACKTRACKING !!!
	while(x!=0 || y!=0){	// Tant que l'on ne se situe pas en haut à gauche (fin du backtracing)
		//	Cas de merde des bords (évite la segfault), j'ai condensé pasque sinon le code était vraiment immonde
		if(x!=0){	gauche = T[x-1][y];} else {gauche = max;}
		if(y!=0){haut = T[x][y-1];} else {haut = max;}
		if(x!=0 && y!=0){diag = T[x-1][y-1];} else {diag = max;}
		min = Imin3(gauche, haut, diag);	//	Valeur séléctionnée
		
		//	 Priorisation de la diagonale dans le backtracking => minimisation du nomrbe d'éditions (une subst = n*add + n*del du coup on choisit le chemin le + court)
		if(min == diag){
			Xp[i] = X[xi];
			Yp[i] = Y[yi];
			xi--;
			yi--;
			x--;
			y--;
		}
		// 	La les 2 peuvent êtres inversés, l'idée c'est de ne pas se tromper sur ou mettre le caractere *
		else if(min == haut){
			Xp[i] = '*';
			Yp[i] = Y[yi];
			yi--;
			y--;
		}
		else{
			Xp[i] = X[xi];
			Yp[i] = '*';
			xi--;
			x--;
		}
	// 	Et oui lui faut pas l'oublier, sert à itérer sur les chars de X' et Y' (à la fin, l'espace supplémentaire est utilisé)
	i++;
	}
	//	Utilisé ici ! On met les \0 sinon C pas content
	Xp[i] = '\0';
	Yp[i] = '\0';
	retourne(Xp);
	retourne(Yp);
	
	struct alignement res;
	res.x = Xp;
	res.y = Yp;
	return res;
}
