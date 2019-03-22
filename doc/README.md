# Extension PHP 7

## Fonction

### Définir un fonction.

la fonction hello accessible depuis PHP est définie dans l'extension de la manière suivante:

```c
PHP_FUNTION(hello) {
  ZVAL_STR(return_value, zend_init_string("hello world", 11, 0 ));
}
```
hello doit être définie dans la constante zend_function_entry

```c
const zend_function_entry test_functions[] = {
	  PHP_FE(hello, NULL)
    ...
	  PHP_FE_END
};
```
### valeur de retour d'une fonction.
 la valeur de retour d'une fonction est accessible dans le corps de la fonction par la variable return_value. return_value est un pointeur vers une ZVAL
Dans la fonction hello :
```c
ZVAL_STR(return_value, zend_init_string("hello world", 11, 0 ));
```
[ZVAL_STR](https://phpinternals.net/docs/zval_str) prends en premier argument un pointeur vers une ZVAL un zend_string en second argument et affecte le second au premier sous forme de string. Ainsi, on affecte un string à la valeur de retour de la fonction.

### Passer un argument à une fonction.

Pour passer un argument à une fonction, on peut typer l'argument, c'est a dire indiquer quelle type de valeur on attends, on peut aussi passer une référence, et enfin on peut passer un argument facultatif. Il existe des macros pour simplifier la lecture des arguments d'une fonction.
Prenons l'exemple suivant:

```c
PHP_FUNTION(test) {

  zval * t_param, * var;
  zend_string * str;

  ZEND_PARSE_PARAMETERS_START( 2, 3 )
    Z_PARAM_ARRAY( t_param )
    Z_PARAM_STR( str )
    Z_PARAM_OPTIONAL
    Z_PARAM_ZVAL( var )
  ZEND_PARSE_PARAMETERS_END();

  ...
}
```
la fonction pourra être appelée depuis php ainsi:
```php
test(['hello'], "string");
```
Z_PARAM_ARRAY( t_param) indique que le premier paramètre doit être de type array et sa référence est écrite dans t_param.
le second paramètre doit être de type string.
Les fonction qui permettent de lire les paramètres acceptent un argument de type pointeur.
### Arguments facultatifs.

On a ZEND_PARSE_PARAMETERS_START(2,3) le 2 indique qu'il y deux paramètres obligatoires, le 3 indique le nombre de paramètres totaux.
le dernier paramètre est de type facultatif. Ici il peut avoir n'importe quel type, c'est une ZVAL.
On peut tester le type de paramètre dans la fonction ainsi:

```c
Z_TYPE( var ) == IS_TRUE, IS_ARRAY, IS_NULL, IS_STRING etc ...
```

### Passer une valeur par référence.
```php
test(&$array, "hello")
```

Si on souhaite avoir des paramètres passés en référence il faut modifier l'appel de la macro PHP_FE et lui passer une second paramètre.

```c
PHP_FE(test, arginfo_test)
```
Et définir arginfo_test de la manière suivante :
```c
ZEND_BEGIN_ARG_INFO_EX(arginfo_test, 0,0,3)
	ZEND_ARG_INFO(1, t_param)
  ZEND_ARG_INFO(0, str)
  ZEND_ARG_INFO(0, var)
ZEND_END_ARG_INFO()
```
si Le premier argument de ZEND_ARG_INFO vaut 1 cela indique que le paramètre est passé par référence.

#### Tableau passé en référence:

Dans le cas d'un tableau passé en référence, cette configuration ne suffit pas, il faut aussi utiliser la macro

```c
ZEND_PARSE_PARAMETERS_START( 2, 3 )
  Z_PARAM_ARRAY_EX(t_param, 0, 1)
  ...
```
Avec le 3 ième paramètre qui vaut 1, qui réalise une copie du tableau et le déréférence si il est référencé plus d'une fois.


## Les string.

```c
  zend_string * value;
  value = zend_string_init("hello world", 11, 0 );
```
Le second paramètre est la longueur de la chaîne, et le dernier paramètre indique qu'on initie le string des manière volatile, dans une contexte propre à la requête, si on avait mis 1, il serait initialisé de manière globale dans un contexte propre à l'environnement PHP. ( il aurait fallu prévoir le déréférencement de la variable quand on désinitialise php ) Cette fonction zend_string_init alloue un espace mémoire pour le string. Par ailleurs on peut l'associer a une zval qui sera pour le coup une zval de type string.


## Array

Pour comprendre comment les array fonctionnent il faut comprendre les hastable, un array etant une zval qui pointe vers un hashtable. Je renvoie vers la lecture de [l'article](http://blog.jpauli.tech/2016-04-08-hashtables-html/) du blog de Julien Pauli.

Donc quand on travaille avec les Array PHP, en définitive il est plus simple d'utiliser les HashTable
qui ont une api définit dans [zend_hash.c](https://github.com/php/php-src/blob/master/Zend/zend_hash.c) du code source de PHP. Les fonction exposé sont préfixées de ZEND_API.

### initialiser un tableau.

L'initialisation d'un tableau ou d'un hashtable se fait en deux temps. D'abord on a alloue la mémoire, puis ensuite on l'initialise

#### Initilisation d'un array
```c
int size = 10;
Z_ARR * tab;
array_init_size( tab, size );
zend_hash_real_init( Z_ARRVAL_P(tab), 1);
```
Avec [Z_ARRVAL_P](https://phpinternals.net/docs/z_arrval) la macro PHP qui va récupérer la valeur de l'Array, c'est à dire le HashTable.

#### Initialisation d'un hashtable

```c
zend_hash_init( tab, 1, 0, ZVAL_PTR_DTOR , 0 );
zend_hash_real_init( tab, 1);
```

avec [zend_hash_init](https://phpinternals.net/docs/zend_hash_init)

### ajouter un élement à un tableau.

La fonction suivante permet d'ajouter un élément à la fin d'un tableau, le nouveau tableau est retourné:
```c
PHP_FUNCTION(add_element) {

	HashTable * tab;
	zval * index;
	zval * val;

	ZEND_PARSE_PARAMETERS_START(3, 3)
		Z_PARAM_ARRAY_HT_EX(tab, 0, 1)
		Z_PARAM_ZVAL(index)
		Z_PARAM_ZVAL(val)
	ZEND_PARSE_PARAMETERS_END();



	if(Z_TYPE_P(index) == IS_STRING ) {
		zend_hash_add(tab, Z_STR_P(index), val );
	} else if(Z_TYPE_P(index) == IS_LONG ) {
		zend_hash_index_add_new(tab, Z_LVAL_P(index) , val);
	}

	ZVAL_ARR(return_value, tab);
}
```
Noter que le tableau passé en argument est déréférencé. En effet, la fonction qui ajoute un élément ne peux fonctionner que si le tableau a une seule référence:
```c
Z_PARAM_ARRAY_HT_EX(tab, 0, 1)
```
c'est le paramètre 1 qui indique cela.
Par ailleurs, dans la fonction on utilise des HashTable : la fonction attend un Array en paramètre qui est transformé en HashTable ( c'est le HT de Z_PARAM_ARRAY_HT)

Les deux fonctions utilisées pour ajouter un élément au tableau parlent d'elle même. Elles fonctionent soit avec un paramètre de type LONG soit avec une chaîne de charactère. Cela est du à la manière dont sont gérés les HashTable, en effet, si tout les index sont de type integer, le HashTable est stocké sous une forme optimisée qui permet de s'affranchir de la gestion des clefs sous forme de chaine de charactère. Son emprunte mémoire est ainsi diminuée.

### supprimer un élément d'un tableau.

la fonction suivante supprime un élément d'un tableau:

```c
PHP_FUNCTION(array_unset) {

	HashTable * tab;
	zval * index;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ARRAY_HT_EX(tab, 0, 1)
		Z_PARAM_ZVAL(index)
	ZEND_PARSE_PARAMETERS_END();



	if(Z_TYPE_P(index) == IS_STRING ) {
		zend_hash_del(tab, Z_STR_P(index));
	} else if(Z_TYPE_P(index) == IS_LONG ) {
		zend_hash_index_del(tab, Z_LVAL_P(index));
	}

	ZVAL_ARR(return_value, tab);
}

```


### trouver un élément dans un tableau.


```c
PHP_FUNCTION(array_find_element) {

	HashTable * tab;
	zval * index;


	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ARRAY_HT(tab)
		Z_PARAM_ZVAL(index)
	ZEND_PARSE_PARAMETERS_END();

	if(Z_TYPE_P(index) == IS_STRING ) {
		return_value = zend_hash_find(tab, Z_STR_P( index));
	} else if(Z_TYPE_P(index) == IS_LONG ) {
		return_value = zend_hash_index_find(tab, Z_LVAL_P(index));
	}
}
```

## Appeler une fonction PHP existante.
