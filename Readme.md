# Question 1
## Algorithme calculant le score d'un alignement optimal
Score d'un alignement optimal : nombre minimum d'indices **i** tq **x'[i] != y'[i]**
**Q1** : Proposer un algorithme en O(|x|.|y|) pour calculer le score d’un alignement optimal entre **x** et **y**.}

## Que fait leveinstein ?
Cet algorithme trouve le chemin minimisant le cout des opérations de deletion, d'insertion et de subtitution pour passer de **x** à **y**.
Son prototype est:
```C
int ** Levenstein(char *x, cahr *y, int i, int j, cI, cD, cS);
```
- **x** et **y** les séquences à comparer
- **i** et **j** les longueurs respéctives des séquences **x** et **y**
- **cD**, **cI** et **cS** les coûts respéctifs d'une délétion, insertion et substitution
- T[i][j] = distance d'édition

Autrement dit: **Leveisntein(x, y, |x|, |y|, cI, cD, cS) = min(nI.cI+nD.cD+nS.cS pour passer de x à y)** 
Avec **nI**, **nD** et **nS** le nombre d'insertions, de délétions et de substitutions.

## Distance d'édition ≠ Score d'un alignement optimal
Le résultat de cet algorithme (la distance d'édition) n'est absolument pas représentatif d'un score d'un alignement optimal entre **x** et **y**. 
```C
int res = Levenstein("a", "b", 1, 1, 18, 10, 42)[1][1];
```
Ici, nous voyons que la distance d'édition est de 42, alors que les deux chaines n'ont qu'une substitution de différence, dont le coût est de 42. Un score d'alignement optimal entre les deux chaines aurait pour valeur 1.

## Leveinstein réduit
Le score d'un alignement optimal entre x et y peut s'écrire ainsi:
Nombre minimum d'additions, de substitutions et de délétions pour passer de x à y.
**ScoreAlignementOptimal(x, y, |x|, |y|) = min(nI+cD+nS pour passer de x à y)**
Avec **nI**, **nD** et **nS** le nombre d'insertions, de délétions et de substitutions <=> nombre d'indices mal appareillés entre x' et y'.

Nous remarquons que:
**Leveinstein(x, y, |x|, |y|, 1, 1, 1) = ScoreAlignementOptimal(x, y, |x|, |y|) = min(nI.1+nD.1+nS.1 pour passer de x à y)**

On en déduit que fixer les coûts de délétion, d'insertion et de substition à 1 permet donc de transformer la distance d'édition de leveinstein en un score d'alignement maximum.

On a donc une nouvelle fonction, que nous appelons aussi leveinstein:
```C
int ** Levenstein(char *x, cahr *y, int i, int j){
    return Levenstein(x, y, i, j, 1, 1, 1);
}
```
La complexité de cet algorithme a déjà été démontrée en cours et est de l'ordre de **O(|x|.|y|)**, sa complexité n'étant pas influencée par les coûts associés aux différentes opérations.

# Q2
## Calcul d'un alignement optimal (x', y') de x et de y
A présent, Le leveinstein réduit de la Q1 sera celui utilisé par défaut.
Soit T[i][j] le score d'un alignement optimal entre xi et yj ou xi et yj désignent les préfixes de x et de y de longueur i et j respéctivement.
**Q2** : Étant donnée la matrice T (obtenue par exemple avec l’algorithme de la question 1) et les séquences x et y, proposez un schéma de programme retournant un alignement optimal (x′,y′) de x et y

Dans cette partie, je vais donner le schéma de l'algorithme, puis essayer de le réduire au calcul d'un alignement optimal. L'idée est de construire les chaines x' et y' en commencant par la fin, et de déduire les opérations à faire sur x' et y' en se déplacant dans la matrice.

## Algorithme pour l'alignement optimal
Paramètres: T la matrice décrite ci dessus, m sa longueur, n sa largeur
**\'\*\'** n'appartient pas à notre alphabet
**INITIALISATION :**
- m:=|x|, n:= |y|
- Se placer à T[m][n]
- créer deux chaines **x'** et **y'** vides
- placer deux compteurs xi et yi sur les derniers caracteres de **x** et **y**.

**CONDITION D'ARRET :**
Tant que x et y ne sont aps tous deux nuls:  => Tant que nous ne sommes pas en haut à gauche => fin de l'algorithme

**BOUCLE PRINCIPALE :**
Regarder ou se situe le minimum entre les cases à gauche, en haut et en diagonale, en priorisant la diagonale en cas d'égalité
**Si** le minimum est la diagonale (T[i-1][j-1]):
- ajouter à x' et y' x[xi] et y[yi] respéctivement.
- décrémenter de 1 xi et yi.
- décrémenter de 1 m et n

**Sinon si** le minimum est à gauche (T[i-1][j]):
- ajouter à x' et y' '*' et y[yi] respéctivement.
- décrémenter yi de 1
- décrémenter n de 1

**Sinon** (le minimum est en haut (T[i][j-1])):
- ajouter à x' et y' x[xi] et '*' respéctivement.
- Décrémenter xi de 1
- décrémenter m de 1

**FIN :**
inverser x' et y', puis les retourner

## En quoi cet algorithme est-il représentatif d'un alignement ?
En retournant en arrière dans la matrice T, sachant qu'elle donne le nombre minimal de délétions, d'additions et de substitutions pour transformer x en y, nous pouvons nous appercevoir d'une chose: 
- Une opération de délétion d'un caractere dans x sera traduite par l'ajout du caractere '\*' dans y'
- Une opération de d'insertion d'un caractere dans x sera traduite par l'ajout du caractere '\*' dans x'
- Une opération de substitution n'implique aucun ajout du caractère '\*'

On sait qu'à l'initialisation de l'algorithme, x' et y' ont la même taille (0)
Sachant qu'a chaque tour de boucle de celui-ci, un aractere sera ajouté à x' **et** y'. x' et y' feront toujours la même taille.
**(1)** On en déduit que |x'| = |y'|

En outre, les compteurs xi et yi sont décrémentés à chaque ajout d'un caractère de x dans x' ou de l'ajout d'un caractère de y dans y'. On voit aussi que, le cas échéant, le caractère '\*' est ajouté à x' ou y'. Pour rappel, '\*' n'appartient pas à l'alphabet.
**(2)** Cela implique que retirer les '\*' dans x' ou y' conduit à retrouver x et y respéctivement. 

Les résultats **(1)** et **(2)** sont les deux conditions nescessaires et suffisantes définissant un alignement (x', y') de x et y.

## Un alignement optimal ?
Comme démontré à la **Q1**, 
