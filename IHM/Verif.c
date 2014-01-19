//*****************************************************
//**  Correction des erreurs dans une chaine
//**  SILR-1   17/01/03
//**  Richaud Guillaume - Lavoix Pierre 
//*****************************************************
#include <stdio.h>

//*****************************************************
// Supression des caracteres invalides et passage minuscule>majuscule
// Mot: la chaine a verifier 
// Vmot: la chaine qui sert au retour
//*****************************************************
void convertS( char* mot, char* motConvert)
{
  char lettre;    // caractere en cours de traitement
  int indM = -1;  // indice de la lettre en cours dans mot
  int indC = 0;   // indice dans le motConvert

  do {
    indM = indM + 1;
    lettre = mot[ indM ];
 
    if ( lettre >64  &&  lettre < 91 ) { lettre = lettre + 32; }
    
    if ((( lettre > 47 ) && ( lettre < 58 )) ||
       (( lettre > 96 ) && ( lettre < 123))  ||
      lettre == 0 || lettre == 45 ) 
      {motConvert[ indC ] = lettre; indC = indC + 1 ;}
    
  }while(  mot[ indM ] != 0 );
}

//*****************************************************
// Nombre de caracteres dans la chaine
//*****************************************************
int longueur ( char* mot )
{
  int longmot = 0; int i;
  for (i = 0; ( mot[ i ] != 0) ; i = i + 1) { longmot = longmot + 1; }
  return( longmot );
}
//*****************************************************
// Calcul le nombre de lettres differents entre 2 chaines
// motRef: la chaine de reference
// motV  : la chaine a corriger
//*****************************************************
int Correlation ( char* motRef, char* motV )
{
  int indRef = 0;  int indV = 0;
  int indC = 0;  int memeLettre = 0;
  int longRef = longueur ( motRef );
  int longV = longueur ( motV );
  
  for( indV = 0; (indV < longV ); indV = indV + 1 )
    {
      for( indC = indRef; (indC < longRef) && (indC < indRef + 4 ); indC = indC+1 )
	{
	  if ( motRef[ indC ] == motV[ indV ] ) { memeLettre = memeLettre + 1; indRef = indC+1; break; }
	}
    }

  if (longRef > longV) { return( longRef - memeLettre ); }
  else               { return( longV - memeLettre ); }
}


//*****************************************************
// Remplace chaque caractere de la chaine par \0
//*****************************************************
void effaceMot (char* mot )
{
  int indCaractere = 0; 
  
  while( indCaractere < 30 )
    {
      mot[ indCaractere ] = 0;
      indCaractere = indCaractere + 1;
    }
}

//*****************************************************
// Recherche dans le tableau Arret, la chaine qui a le
// moins de caracteres distincts de arretSaisi (apres convertion)
// renvoi un int == indice + 1 si le mot est ok
// renvoi int    == 0 si plus de nbre_max_faute
// renvoi int    == -( indice + 1 ) si moins de 3 fautes
//*****************************************************
int Verification ( char* motSaisi, char* dictionnaire[], int nbrMot, int nbre_max_faute )
{
  int indMot   = 0;
  int Diff     = 0;
  
  int indMotMin  = 0;
  int DiffMin  = 99;
  char nomArret[30];
  char nomEntre[30];

  effaceMot( nomEntre );
  convertS( motSaisi, nomEntre ); 
  for ( indMot = 0; indMot < nbrMot; indMot = indMot + 1)
    {
      effaceMot( nomArret );
      convertS( dictionnaire[ indMot ], nomArret );
      Diff = Correlation ( nomArret, nomEntre );
      if ( DiffMin > Diff )	{ DiffMin = Diff; indMotMin = indMot; }
    }

   if ( DiffMin > nbre_max_faute ) { return( 0 ); }
  if  ( DiffMin == 0 ) { return( indMotMin + 1); }
  else { return( -(indMotMin + 1)); }
}
