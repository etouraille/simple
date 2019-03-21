# Extension PHP 7

## Fonction

### Définir un fonction.

la fonction hello accessible depuis PHP est définie dans l'extension de la manière suivante:

```
PHP_FUNTION(hello) {
  ZVAL_STR(return_value, zend_init_string("hello world", 11, 0 ));
}
```
hello doit être définit dans la constante zend_function_entry
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
```
ZVAL_STR(return_value, zend_init_string("hello world", 11, 0 ));
```
[ZVAL_STR](https://phpinternals.net/docs/zval_str) prends en premier argument un pointeur vers une ZVAL un zend_string en second argument et affecte le second au premier sous forme de string. Ainsi, on affecte un string à la valeur de retour de la fonction.

### Passer un argument à une fonction.

Pour passer un argument à une fonction, on peut typer l'argument, c'est a dire indiquer quelle type de valeur on attends, on peut aussi passer une référence, et enfin on peut passer un argument facultatif. Il existe des macro pour simplifier la lecture des arguments d'une fonction.
Prenons l'exemple suivant:

```
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
la fonction pourra être appelée depuis php Ainsi
```
test(['hello'], "string");
```
Z_PARAM_ARRAY( t_param) indique que le premier paramètre doit être de type array et sa référence est ecrite dans t_param.
le second paramètre doit être de type string.
Les fonction qui permettent de lire les paramètres acceptent un argument de type pointeur.
### Arguments facultatifs.

On a ZEND_PARSE_PARAMETERS_START(2,3) le deux indique qu'il y deux paramètres obligatoires, le 3 indique le nombre de paramètres totaux.
le dernier paramètre est de type facultatif. Il peut avoir n'importe quel type.
On peut tester le type de paramètre dans la fonction ainsi:

```
Z_TYPE( var ) == IS_TRUE, IS_ARRAY, IS_NULL, IS_STRING etc ...
```

### Passer une valeur par référence.
```
test(&$array, "hello")
```

Si on souhaite avoir des paramètres passés en référence il faut modifier l'appel de la macro PHP_FE et lui passer une second paramètre.

```
PHP_FE(test, arginfo_test)
```
Et définir arginfo_test de la manière suivante :
```
ZEND_BEGIN_ARG_INFO_EX(arginfo_test, 0,0,3)
	ZEND_ARG_INFO(1, t_param)
  ZEND_ARG_INFO(0, str)
  ZEND_ARG_INFO(0, var)
ZEND_END_ARG_INFO()
```
si Le premier argument de ZEND_ARG_INFO vaut 1 cela indique que le paramètre est passée par référence.

#### Tableau passé en référence:

Dans le cas d'un tableau passé en référence, cette configuration ne suffit pas, il faut aussi utiliser la macro

```
ZEND_PARSE_PARAMETERS_START( 2, 3 )
  Z_PARAM_ARRAY_EX(t_param, 0, 1)
  ...
```
Avec le 3 ième paramètre qui vaut 1, pour ce que j'en ai compris cela réalise une copie du tableau et le déréférence si il est référencé plus d'une fois.


## Les string.

```
  zend_string * value;
  value = zend_string_init("hello world", 11, 0 );
```
Le second paramètre est la longueur de la chaîne, et le dernier paramètre indique qu'on initie le string des manière volatile, dans une contexte propre à la requête, si on avait mis 1, il serait initialisé de manière globale dans un contexte propre à l'environnement PHP. ( il aurait fallu prévoir le déréférencement de la variable quand on désinitialise php ) Cette fonction zend_string_init alloue un espace mémoire pour le string. Par ailleurs on peut l'associer a une zval qui sera pour le coup une zval de type string.


## Array

Pour comprendre comment les array fonctionnent il faut comprendre les hastable, un array etant une zval qui pointe vers un hastable. Je renvoie vers la lecture de [l'article](http://blog.jpauli.tech/2016/04/08/hashtables.html) du blog de Julien Pauli.


### initialiser un tableau.

### ajouter un élement à un tableau.

ajouter a la fin.
choix de l'index.

### supprimer un élément d'un tableau.

### ajouter un tableau a un tableau.

## Appeler une fonction PHP existante.
