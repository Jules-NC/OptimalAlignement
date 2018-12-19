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

La complexité de cet algorithme a déjà été démontrée en cours et est de l'ordre de **O(|x|.|y|)**, sa complexité n'étant pas influencée par les coûts associés aux différentes opérations.
