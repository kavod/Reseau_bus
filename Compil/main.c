

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#define MAX_LONG 40  // longueur maximum de caractères pour une lecture 


/*********************************************************************************************************/
/* Stringtoint : fonction convertissant la chaine de caractere str en entier                             */
/*               valeur retournee : conversion de str en entier si succes                                */
/*                                  -1 si str contient un caractere autre qu'un nombre                   */
/*********************************************************************************************************/
int stringtoint(char* str) {

   int i=0;
   int lettreDebut=0;
   int tailleChaine=strlen(str);
   
   // comptage des espaces au debut de str
   while(isspace(str[lettreDebut])) {
     lettreDebut++;}
   // test si str ne contient que des espaces
   if (lettreDebut==tailleChaine) return -1;
   //pour tous les caracteres suivants de str 
   for(i=lettreDebut;i<tailleChaine;i++) {
     // test si le caractere n'est pas un chiffre
     if (! isdigit(str[i])) {
       // test si le caractere est un espace
       if (isspace(str[i])) {
	 // test si le caractere n'est pas le dernier (pour ne pas provoquer de depassement de tableau)
	 if (i<tailleChaine-1) {
	   // test si le caractere suivant n'est pas un espace (permettant de reperer un espace entre deux caracteres ou la fin de str)  
	   if (! isspace(str[i+1])) { 
	     return -1;
	   }
	 }
       }
       else return -1;
     }
   }
   return atoi(str);
}


/*********************************************************************************************************/
/* ModifieLecture : fonction modifiant la chaine de caractere str (correspondant a un arret) pour pouvoir*/
/*                  l'inserer correctement dans le tableau                                               */
/*                  action : suppression des espaces de debut et de fin, et du caractere entree          */
/*********************************************************************************************************/
void modifieLecture(char* str) {
  
  int lettreDebut=0;
  int j=0;
  int lettreFin=strlen(str)-1;
 
 
  // comptage des espaces au debut de str
  while(isspace(str[lettreDebut])) {
    lettreDebut++;}
  // comptage des espaces a la fin de str
  while(isspace(str[lettreFin])) {
    lettreFin--;}
  // test si str ne contient que des espaces
  if (lettreFin-lettreDebut<0) {
    str[0]='\0';
  }
  else {
    // pour tous les caracteres compris entre ces deux valeurs
    for(j=lettreDebut;j<=lettreFin;j++) {
      // decalage des caracteres
      str[j-lettreDebut]=str[j];
    }
    // placement du caractere de fin de chaine de caractere
    str[lettreFin-lettreDebut+1]='\0';
  }
}


/*********************************************************************************************************/
/* LiberePointeur : fonction liberant tous les pointeurs utilises                                        */
/*********************************************************************************************************/
void liberePointeur(int *ligne, char **arret, int **mat) {
  free(mat); 
  free(arret);
  free(ligne);
}


/*********************************************************************************************************/
/* ConstruitStructure : fonction mettant en place la structure de données                                */
/*********************************************************************************************************/
int construitStructure(int *ligne, char **arret, int **mat, int nbLigne, int *nbArret, FILE *fichier) {

  char* temp_string;           // chaine de caractere stockant les lectures
  int temp_int=0;              // entier stockant une valeur temporaire
  int nbArretLigne=0;       // nombre d'arret de la ligne en cours de lecture
  int i=0;                     // compteur de boucle
  int j=0;                     // compteur de boucle
  int k=0;                     // compteur de boucle
 
        
  temp_string=(char *) calloc(MAX_LONG,sizeof(char));
  *nbArret=0;
  rewind(fichier);
  
  // lecture du nombre de ligne
  fgets(temp_string,MAX_LONG,fichier);
  // pour toutes les lignes
  for(i=0;i<nbLigne;i++) {
    // lecture du numero de la ligne et stockage dans le tableau ligne
    fgets(temp_string,MAX_LONG,fichier);
    temp_int=stringtoint(temp_string);
    ligne[i]=temp_int;
    // lecture du nombre d'arret de la ligne
    fgets(temp_string,MAX_LONG,fichier);
    nbArretLigne=stringtoint(temp_string);
    // pour tous les arrets
    for(j=0;j<nbArretLigne;j++) {
      strcpy(temp_string,"");
      // lecture des arrets
      fgets(temp_string,MAX_LONG,fichier);
      modifieLecture(temp_string);
      // test si temp_string est vide
      if (strlen(temp_string)==0) {
	free(temp_string);
	return -1;
      }
      k=0;
      // test la presence de l'arret dans le tableau arret
      while ((k<*nbArret) && (strcmp(arret[k],temp_string)!=0)) {
	k=k+1;
      }
      // si l'arret n'est pas present
      if (k==*nbArret) {
	// tableau arret : allocation dynamique du nombre de caractere de la lecture (jusqu'au caractere /0 compris)
	arret[*nbArret]=(char *) calloc(strlen(temp_string)+1,sizeof(char));
	// copie de l'arret dans le tableau arret
	arret[*nbArret]=strcpy(arret[*nbArret],temp_string);
	// remplissage de la matrice [ligne][arret] 
	mat[i][*nbArret]=j+1;
	*nbArret=*nbArret+1;
      }
      // si l'arret est present
      else {
	// remplissage de la matrice [ligne][arret]
      	mat[i][k]=j+1;
      }
    }
  }
  free(temp_string);
  return 0;
}


/*********************************************************************************************************/
/*  TesteOuvertureFichier : teste l'existence, les droits et le type du fichier                          */
/*                          valeur renvoyée : 0 si succes                                                */
/*                                            1 nombre négatif si echec                                  */
/*********************************************************************************************************/
int testeOuvertureFichier(char *nomFichier, int nbArgument) {  
  
  struct stat buffer;
  
  // test du nombre d'argument
  if (nbArgument!=2) return -1;
  // test de l'existence du fichier
  if (access(nomFichier,00)==0) {
    // test si le fichier dispose du droit de lecture
    if (access(nomFichier,04)!=0) return -3;
  }
  else return -2;
  stat(nomFichier,&buffer);
  // test si le fichier est un fichier regulier
  if (buffer.st_mode>>12!=0x8) {
    return -4;
  }
  return 0;
}


/*********************************************************************************************************/
/* RecupereValeur : fonction récuperant à partir du fichier le nombre de ligne et le nombre d'arret      */
/*                  valeur renvoyée : 0 si succes                                                        */
/*                                    1 nombre negatif sinon                                             */
/*********************************************************************************************************/
int recupereValeurs(int *nbLigne,int *nbArret, FILE *fichier) {

  char *temp_string;
  int nbArretLigne=0;
  int numeroLigne=0;
  int i,j;

  temp_string=(char *) calloc(MAX_LONG,sizeof(char));
  
  // test de fin de fichier
  if (feof(fichier)) {
    free(temp_string);
    return -1;
  }
  // lecture du nombres lignes
  fgets(temp_string,MAX_LONG,fichier); 
  *nbLigne=stringtoint(temp_string);
  if (*nbLigne==-1) {
    free(temp_string);
    return -1;
  }
  
  if (*nbLigne==0) {
    free(temp_string);
    return -2;
  }
  // test de fin de fichier
  if (feof(fichier)) {
    free(temp_string);
    return -1;
  }
  // pour toutes les lignes
  for (i=0;i<*nbLigne;i++) {
    // lecture du numeros de la lignes
    fgets(temp_string,MAX_LONG,fichier);
    // test la fin de fichier
    if (feof(fichier)) {
      free(temp_string);
      return -1;
    }
    numeroLigne=stringtoint(temp_string);
    if (numeroLigne<0) {
      free(temp_string);
      return -1;
    }
    // test la fin de fichier
    if (feof(fichier)) {
      free(temp_string);
      return -1;
    }
    // lecture du nombre d'arret de la ligne
    fgets(temp_string,MAX_LONG,fichier);
    nbArretLigne=stringtoint(temp_string);
    if (nbArretLigne<0) {
      free(temp_string);
      return -1;
    }
    // test la fin de fichier
    if (feof(fichier)) {
      free(temp_string);
      return -1;
    }
    // incrementation du nombre total d'arret du nombre d'arret de la ligne en cours
    *nbArret=*nbArret+nbArretLigne;
    // pour tous les arrets de la ligne
    for (j=0;j<nbArretLigne;j++) {
      // lecture du nom de l'arret
      fgets(temp_string,MAX_LONG,fichier);
      // test la fin de fichier sauf pour le dernier arret de la derniere ligne
      if ((i!=*nbLigne-1)&&(feof(fichier))&&(j!=nbArretLigne)) {
	free(temp_string);
	return -1;
      }
    }
  }
  free(temp_string);
  return 0; 
}




//int construitStructure(int *ligne, char **arret, int **mat, int nbLigne, int *nbArret, FILE *fichier);
void liberePointeur(int *ligne, char **arret, int **mat);
//int testeOuvertureFichier(char *nomFichier, int nbArgument);
//int recupereValeurs(int *nbLigne,int *nbArret, FILE *fichier);
//void recherche(int depart, int arrivee,int nb_ligne,int nb_arret,int* ligne,char** arret,int **mat);
////////////////////////////////////
// Projet d'algorithmie N°1       //
// Année 2002-2003                //
// -Brice GRICHY                  //
// -Arnaud BRUNET                 //
// -Timothée CONSTENSOUX          //
////////////////////////////////////
#include <stdlib.h>
#include <stdio.h>
//#include "recherche.h"
#include <unistd.h>



int recherche0(int *dep,int *arr,int *lg,int **mat,int nb_ligne) // Fonction de recherche de ligne contenant le départ et l'arrivée.
     // Retourne 1 si on en trouve une, 0 sinon
     // Pour chaque ligne, on regarde si cette ligne contient le départ et l'arrivée
{
  int i=0;
   for(i=0;i<nb_ligne;i++)
    {
      if (mat[i][*dep]!=0)
	{
	  if (mat[i][*arr]!=0)
	    {
	      *lg=i;
	      return 1;
	    }
	}
    }
   return 0;
}


int recherche1(int *dep, int *arr,int *chg,int *lge1,int *lge2,int **mat,int nb_ligne,int nb_arret)   // Fonction de recherche d'itinéaire à 1 changement
     // On cherche deux lignes d'intersection a, l'une contenant l'arrêt de départ, l'autre contenant l'arrêt d'arrivée
     // Renvoi 1 si l'itinéraire est trouvé, 0 sinon
     // Pour chaque arrêt, on effectue un recherche0 (aucun changement) entre cet arrêt et le départ puis entre cet arrêt et l'arrivée.
{
 int a;  

 for ( a=0; a<nb_arret; a++)
 {
   int t;
   int u;
   t=recherche0(&a,arr,lge2,mat,nb_ligne);

   u=recherche0(dep,&a,lge1,mat,nb_ligne);
   if (t!=0 && u!=0)
   { 
     *chg=a;
    return 1;
  }
 }
 return 0;
}



int recherche2 (int *dep, int *arr,int *chgp1,int *chgp2,int *lgp1, int *lgp2, int *lgp3,int **mat,int nb_ligne,int nb_arret)  // Fonction de recherche d'itinéraires à 2 changements
     // Pour chaque ligne contenant l'arrêt de départ, on teste s'il existe un itinéraire à 1 changement à partir d'un des arrêts de cette ligne.
     // Cette fonction renvoie 1 si un tel itinéraire est trouvé, 0 sinon.
     // lg (lg1, voir recherche() ) est la première ligne à prendre
     // chg (chg1 voir recherche() ) est le premier changement à effectuer
     // chg2 (changement suivant) et lg2 et lg3 (lignes suivantes) sont affectées dans le sous programme recherche1
{
 int i,j;
 for (i=0; i<nb_ligne; i++)
 {
   if (mat[i][*dep]!=0)     
  {
   
   *lgp1=i;
   for (j=0;j<nb_arret;j++)
   {
    if (mat[i][j]!=0)      
    {
    * chgp1=j;
     if (recherche1(chgp1,arr,chgp2,lgp2,lgp3,mat,nb_ligne,nb_arret)!=0)
     {
      return 1;
     }
    }
   }
  }
 }
 return 0;
}


int terminus(int *lg, int *dep, int *arr,int **mat,int nb_arret) // Fonction qui retourne l'identifiant de l'arrêt "terminus" vers lequel on veut se diriger
     // On recherche le sens de direction de l'itinéraire sur la ligne (positif ou négatif)
     // On retourne le maximun ou le minimum selon le cas
{
  int i, max, term;
 if (mat[*lg][*dep]<mat[*lg][*arr])
   {
     max=0;
     term=0;
     for ( i=1;i<nb_arret;i++)
       {
	 if (mat[*lg][i]>max)
	   {
	     term =i;
	     max=mat[*lg][i];
	   }
       }
     return term;
   }
 else
   {
     i=0;
     while (mat[*lg][i]!=1)
       {
	 i++;
       }
     return i;
   }
}

 
 

 
 
 
 
void recherche(int depart, int arrivee,int nb_ligne,int nb_arret,int* ligne,char** arret,int **mat) // On test la possibilité d'un itinéraire sans changement, puis avec un changement, puis avec 2 changements
     // Puis affiche le résultat
{
                 // identifiants des arrêts de changement
               // identifiants des lignes à prendre
  int * chg1=(int*)malloc(sizeof(int));
  int * chg2=(int*)malloc(sizeof (int));
  int * lg1=(int*)malloc(sizeof (int));
  int * lg2=(int*)malloc(sizeof (int));
  int * lg3=(int*)malloc(sizeof (int));
  int direction;
  

 if (recherche0(&depart,&arrivee,lg1,mat,nb_ligne)!=0)
   {
      direction=terminus(lg1,&depart,&arrivee,mat,nb_arret);
     affiche("Pas de changement\n Monter a");
     affiche(arret[depart]);
     affiche(" \nPrendre ligne");
     afficheInt(ligne[*lg1]);
     affiche("vers");
     affiche(arret[direction]);
     affiche(" \n  Descendre a");
     affiche(arret[arrivee]);
     affiche(" \n");
   }
 else
   {      
     if (recherche1(&depart,&arrivee,chg1,lg1,lg2,mat,nb_ligne,nb_arret)!=0)
       {
 
	   direction=terminus(lg1,&depart,chg1,mat,nb_arret);
           affiche("1 changement :\n  Monter a");
	   affiche(arret[depart]);
	   affiche("\n  Prendre ligne");
	   afficheInt(ligne[*lg1]);
	   affiche(" vers ");
	   affiche(arret[direction]);
	   affiche("\n  Descendre a ");
	   affiche(arret[*chg1]);
	   affiche("\n");
           direction=terminus(lg2,chg1,&arrivee,mat,nb_arret);
           affiche("Prendre ligne");
	   afficheInt(ligne[*lg2]);
	   affiche(" vers ");
	   affiche(arret[direction]);
	   affiche(" \n   Descendre a ");
	   affiche(arret[arrivee]);
	   affiche(" \n");
       }
     else
       {        
	 if (recherche2(&depart,&arrivee,chg1,chg2,lg1,lg2,lg3,mat,nb_ligne,nb_arret)==0) 
	   {
	     affiche("Pas d'itinéraire possible avec moins de deux changements\n");
	   }
	 else
	   {
         direction=terminus(lg1,&depart,chg1,mat,nb_arret);
         affiche("2 changements :\n  Monter a ");
	 affiche(arret[depart]);
	 affiche("\n\tPrendre ligne ");
	 afficheInt(ligne[*lg1]);
	 affiche(" vers ");
	 affiche(arret[direction]);
	 affiche("\n  Descendre a ");
	 affiche(arret[*chg1]);
	 affiche("\n");
         direction=terminus(lg2,chg1,chg2,mat,nb_arret);
	 affiche("\tPrendre ligne ");
	 afficheInt(ligne[*lg2]);
	 affiche(" vers ");
	 affiche(arret[direction]);
	 affiche(" \n  Descendre a ");
	 affiche(arret[*chg2]);
	 affiche(" \n");
         direction=terminus(lg3,chg2,&arrivee,mat,nb_arret);
         affiche("\tPrendre ligne ");
	 afficheInt(ligne[*lg3]);
	 affiche(" vers ");
	 affiche(arret[direction]);
	 affiche("\n  Descendre a ");
	 affiche(arret[arrivee]);
	 affiche(" \n");
	 }
       }
   }


}
#ifndef RECHERCHE
#define RECHERCHE

//int recherche2 (int *dep, int *arr,int *chgp1,int *chgp2,int *lgp1, int *lgp2, int *lgp3,int **mat,int nb_ligne,int nb_arret);
//int recherche1(int *dep, int *arr,int *chg,int *lge1,int *lge2,int **mat,int nb_ligne,int nb_arret);
//int recherche0(int *dep,int *arr,int *lg,int **mat,int nb_ligne);
void recherche(int depart, int arrivee,int nb_ligne,int nb_arret,int* ligne,char** arret,int **mat); 
//int terminus(int *lg, int *dep, int *arr,int **mat,int nb_arret);




#endif
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
//*****************************************************
//**  Interface 
//**  SILR-1   17/01/03
//**  Richaud Guillaume - Lavoix Pierre 
//*****************************************************
#include <stdio.h>
#include "..\IHM\GestAffich.h"
#include "..\IHM\Verif.h"

#define NBRFAUTE  3  // Nombre max de lettres differentes entre 2 mots
#define NORAW system("stty echo -raw");
#define RAW system("stty -echo raw");

// Variables globales ( Arret d'origine, Arret de destination ) 
char* savOrigine;
char* savDestination;

void fin( void )
{    
  NORAW;
  EFFACE_ECRAN;
  GOTO(1,1);
  printf("Exit\n");
  // LIBERATION DES POINTEURS UTILISESs
  //  liberation_pointeur();
}



// Initialisation de l'affichage
//*****************************************************
void debut( void )
{
  EFFACE_ECRAN;
  GOTO(6,1);
}


// Affichage de l'entete
//*****************************************************
void cadre( void )
{
  SAUVE_CURSEUR;
  GOTO(1,1); EFFACE_LIGNE;
  GOTO(2,1); EFFACE_LIGNE;
  GOTO(3,1); EFFACE_LIGNE;
  GOTO(4,1); EFFACE_LIGNE;
  GOTO(5,1); EFFACE_LIGNE;  
  GOTO(6,1); EFFACE_LIGNE;  

  PROPRIETE_ECRAN(AVANT_ROUGE);
  GOTO( 1,1); printf("_____[ RECHERCHE DE TRAJET ]______________________________________\n");
  GOTO( 4,1); printf("__________________________________________________________________\n");

  PROPRIETE_ECRAN(AVANT_BLANC);
  GOTO(2,10); printf("Origine :\n");
  GOTO(2,25); printf("%s\n", savOrigine);
  GOTO(3,10); printf("Destination :\n");
  GOTO(3,25); printf("%s\n", savDestination);
  RESTORE_CURSEUR;
}

// Effacage de l'entete
//*****************************************************
void effaceCadre( void )
{
  SAUVE_CURSEUR;
  GOTO(1,1); EFFACE_LIGNE;
  GOTO(2,1); EFFACE_LIGNE;
  GOTO(3,1); EFFACE_LIGNE;
  GOTO(4,1); EFFACE_LIGNE;
  GOTO(5,1); EFFACE_LIGNE;  
  GOTO(6,1); EFFACE_LIGNE;  
  RESTORE_CURSEUR;
}

//  Gestion de l'affichage des chaines
//*****************************************************
void affiche( char* chaine )
{
  effaceCadre();
  printf("  %s",chaine);
  cadre();
}

//  Gestion de l'affichage des entiers
//*****************************************************
void afficheInt( int entier )
{
  effaceCadre();
  printf("  %u\n", entier);
  cadre();
}

// Affichage des chaines en jaune
//*****************************************************
void message( char* chaine )
{
  effaceCadre();
  PROPRIETE_ECRAN(AVANT_JAUNE);
  printf( "\n[>%s\n",chaine);
  PROPRIETE_ECRAN(AVANT_BLANC);
  cadre();
}


// Capture de la chaine entrée par l'utilisateur
//*****************************************************
void saisiChaine( char mot [30] )
{
    char ch;
  char index = 0;
  RAW;
  memset( mot, 0, 30 );
  while( (ch != 10) &&  (ch != 13 ))
    {
      if ( index >= 29 ) ch = 13;
      else
	{
	  ch = getchar();
	  //	  	  printf("(%x)",ch);
	  //	  if((ch==0x1b)) printf("%d",fflush(stdin));

	  if ( (ch == 3) || (ch ==4) || (ch ==0x1a) || (ch==0x1b))
	    {
	      fin();
	      exit(0);
	    }

	  if (( ch == 0x20 ) || ( ch == 0x28 ) || ( ch == 0x29 )  || ( ch ==0x2D ) || ( ch == 0x2E ) || (( ch > 0x60 ) && ( ch < 0x7B )) || (( ch > 0x29 ) && ( ch < 0x39 )) )
	    {
	      printf("%c",ch);
	      fflush( stdout );
	      mot[index] = ch;
	      index = index + 1;
	    }
	  if (( ch > 0x40 ) && ( ch < 0x5B ))
	    {
	      ch = ch + 0x20;
	      printf("%c",ch);
	      fflush( stdout );
	      //	      fflush( stdin );
	      mot[index] = ch;
	      index++;
	    }
	  if ( ch == 127 )
	    {
	      mot[index] = 0;
	      if ( index != 0 )
		{
		  index = index - 1;
		  printf("\b \b");
		}
	    }

	}
    }
  mot[index] = 0;
  printf("\n");
  //  printf("%s\n",mot);
  //  getchar();
  NORAW;
}


// Gestion de l'entrée de l'arret au clavier
//*****************************************************
//   arret[] = tableau dans le quel sera recherche l'arret saisi
//   nbre_arret = nombre d'arret dans le tableau
//   msg  = message affiché lors de la demande de saisi
//*****************************************************
int saisi( char** arret, int nbre_arret, int position_ecran )
{
  
  char nomArret [30];
  int numArret = -1;


  do {
    SAUVE_CURSEUR;
    GOTO(position_ecran,25);
    EFFACE_LIGNE;
    saisiChaine( nomArret );
    RESTORE_CURSEUR;
    numArret =  Verification(  nomArret, arret, nbre_arret, NBRFAUTE ); 
    
    
    if (numArret  == 0 )
      {
	message("Arret incorrect");
      }

    if (numArret > 0 )
      {
	message("Saisi confirmée");
	affiche( "Arret:" );
	affiche( arret[ numArret -1 ]);
      }

    if ( numArret < 0 )
      {
	message( "Erreur de saisi. Arret possible: " );
	affiche( arret[ (-(numArret +1)) ]);
      }
    
    
  } while( numArret <= 0 );
    
    return( numArret - 1 );
}


// PAS ENCORE FAIT !!!
//*****************************************************
int continuer( void )
{
  char ch;
  
  message(" Pour quitter presser 'q' \n");
  RAW; 
  ch =  getchar();
  NORAW;
  return  ( (ch != 'q') && (ch != 'Q') ); 
}

//*****************************************************
//**  Correction des erreurs dans une chaine
//**  SILR-1   17/01/03
//**  Richaud Guillaume - Lavoix Pierre 
//*****************************************************


//*****************************************************
// Recherche dans le tableau Arret, la chaine qui a le
// moins de caracteres distincts de motV (apres convertion)
// renvoi un int == indice + 1 si le mot est ok
// renvoi int    == 0 si plus de nbre_max_faute
// renvoi int    == -( indice + 1 ) si moins de 3 fautes
// Verfification( mot_a_verifie ,tableau_contenant_les mots, taille_du_tableau, nombre_maximal_de_faute)
//*****************************************************

#ifndef VERIF
#define VERIF


int Verification ( char* , char**, int, int );

#endif
//*****************************************************
//**  Interface 
//**  SILR-1   17/01/03
//**  Richaud Guillaume - Lavoix Pierre 
//*****************************************************
#ifndef INTERFACE
#define INTERFACE

// Variables globales ( Arret d'origine, Arret de destination ) 
extern char* savOrigine;
extern char* savDestination;


// Initialisation de l'affichage
void debut( void );

// Affichage de l'entete
void cadre( void );

//  Gestion de l'affichage des chaines
void affiche( char* );

//  Gestion de l'affichage des entiers
void afficheInt( int );


// Affichage des chaines en jaune
void message( char* );

// Gestion de l'entrée de l'arret au clavier
//*****************************************************
//   arret[] = tableau dans le quel sera recherche l'arret saisi
//   nbre_arret = nombre d'arret dans le tableau
//   msg  = message affiché lors de la demande de saisi
//*****************************************************
int saisi( char** , int, int );

// Renvoit
int continuer( void );


#endif
#define  GOTO(ligne, colonne)            printf("\033[%d;%dH", ligne, colonne)

#define  MONTE_CURSEUR(nbreligne)        printf("\033[%dA", nbreligne)
#define  DESCEND_CURSEUR(nbreligne)      printf("\033[%dB", nbreligne)
#define  RECULE_CURSEUR(nbrecolonne)     printf("\033[%dC", nbrecolonne)
#define  AVANCE_CURSEUR(nbrecolonne)     printf("\033[%dD", nbrecolonne)

#define  SAUVE_CURSEUR                   printf("\033[s")
#define  RESTORE_CURSEUR                 printf("\033[u")

#define  EFFACE_ECRAN                    printf("\033[2J")
#define  EFFACE_LIGNE                    printf("\033[K")

#define  PROPRIETE_ECRAN(attribut)       printf("\033[%dm", attribut)

#define  ATTR_OFF          0
#define  BOLD_ON           1
#define  SOULIGNE          4
#define  CLIGNOTE_ON       5
#define  INVERSE_PROPRIETE 7
#define  CONCEALED_ON      8

#define  AVANT_NOIR    30
#define  AVANT_ROUGE   31
#define  AVANT_VERT    32
#define  AVANT_JAUNE   33
#define  AVANT_BLEU    34
#define  AVANT_MAGENTA 35
#define  AVANT_CYAN    36
#define  AVANT_BLANC   37

#define  FOND_NOIR    40
#define  FOND_ROUGE   41
#define  FOND_VERT    42
#define  FOND_JAUNE   43
#define  FOND_BLEU    44
#define  FOND_MAGENTA 45
#define  FOND_CYAN    46
#define  FOND_BLANC   47
#include <stdio.h>
#include "../Structure/primitives.h"
#include "../IHM/Interf.h"
#include "../Recherche/recherche.h"


int main(int argc,char * argv[]) {

  /***************************   DECLARATIONS DES VARIABLES *********************************************/
  int nbLigne=0; // nombre total de lignes
  int nbArret=0; //nombre total d'arret  (sans doublons)
  int *ligne;   // tableau contenant les numeros de lignes par ordre d'apparition
  char **arret; // tableau contenant tous les arrets (sans doublons pour les intersections)
  int **mat;    // matrice [ligne][arret]
  FILE *fichier;
  
  int i,j;
  int ind_origine;
  int ind_destination;

  /*****************************     OUVERTURE DU FICHIER    *********************************************/
  switch (testeOuvertureFichier(argv[1], argc)) {
  case -1 : {
    puts("ERREUR : Nombre d'arguments incorrect");
    printf("\t Syntaxe : %s chemin_absolu_fichier \n",argv[0]);
    return -1;
  }
  case -2 : {
    printf("ERREUR : %s inexistant \n",argv[1]);
    return -1;
  }
  case -3 : {
    printf("ERREUR : %s permission insuffisante \n",argv[1]);
    printf("\t Permission de lecture obligatoire \n");
    return -1;
  }
  case -4 : {
    printf("ERREUR : %s n'est pas un fichier régulier \n",argv[1]);
    return -1;
  }
  }
  fichier=fopen(argv[1],"r");


  /*******************    RECUPERATION DES VALEURS NBLIGNE ET NBARRET  ***********************************/
  switch (recupereValeurs(&nbLigne, &nbArret, fichier)) {
  case -1 : {
    printf("ERREUR : %s mal construit \n",argv[1]);
    return -1;
  }
  case -2 : {
    puts("Nombre total de ligne nulle : FIN D'EXECUTION");
    return -1;
  }
  }

  /**********************   INITIALISATION DES TABLEAUX ET DE LA MATRICE   *******************************/
  // tableau des numeros de ligne : allocation dynamique de nbLigne entiers
  ligne=(int *) calloc(nbLigne,sizeof(int));
  // matrice [ligne][arret] : allocation dynamique de nbLigne pointeurs d'entiers 
  mat=(int ** ) calloc(nbLigne,sizeof(int *));
  arret=(char **) calloc(nbArret,sizeof(char *));
  for(i=0;i<nbLigne;i++) {
    ligne[i]=0;
    // matrice [ligne][arret] : allocation dynamique pour chaque ligne de nbArretTotale entiers
    mat[i]=(int *) calloc(nbArret,sizeof(int));
    for(j=0;j<nbArret;j++) {
      mat[i][j]=0;
    }
  }
  
  /****************************  MISE EN PLACE DE LA STRUCTURE   *****************************************/
  if (construitStructure(ligne, arret, mat, nbLigne, &nbArret, fichier)==-1) {
    printf("ERREUR : %s mal construit \n",argv[1]);
    liberePointeur(ligne, arret, mat);
    return -1;
  }

  /****************************   FERMETURE DU FICHIER   *************************************************/
  fclose(fichier);

  /*************************************    AFFICHAGE      ***********************************************/
  /*  printf("ligne \t");
  for(i=0;i<nbLigne;i++) {
    printf("%d ",ligne[i]);
  }
  puts("\narret");
  for(j=0;j<nbArret;j++) {
    printf("%s \t",&(*arret[j]));
    for(i=0;i<nbLigne;i++) {
      printf(" %d ",mat[i][j]);
    }
    puts("\n");
    }*/
    


  //------------------ INTERFACE -----------------------------
  debut();
  
  do {
  savOrigine = " ";
  savDestination=" ";
  cadre();
  
  ind_origine=saisi(arret, nbArret, 2 );
  savOrigine= arret[ ind_origine ];
  cadre();

  ind_destination=saisi(arret, nbArret, 3 );
  savDestination= arret[ ind_destination ];
  message("Resultat : \n"); 

  if (ind_origine != ind_destination) 
    {  recherche(ind_origine, ind_destination, nbLigne,nbArret,ligne,arret,mat); }
  else
    { affiche(" Origine et destination identiques ");}
  } while( continuer() );
  //----------------------------------------------------------
  fin(); 
  
  return 0;
}
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#define MAX_LONG 40  // longueur maximum de caractères pour une lecture 


/*********************************************************************************************************/
/* Stringtoint : fonction convertissant la chaine de caractere str en entier                             */
/*               valeur retournee : conversion de str en entier si succes                                */
/*                                  -1 si str contient un caractere autre qu'un nombre                   */
/*********************************************************************************************************/
int stringtoint(char* str) {

   int i=0;
   int lettreDebut=0;
   int tailleChaine=strlen(str);
   
   // comptage des espaces au debut de str
   while(isspace(str[lettreDebut])) {
     lettreDebut++;}
   // test si str ne contient que des espaces
   if (lettreDebut==tailleChaine) return -1;
   //pour tous les caracteres suivants de str 
   for(i=lettreDebut;i<tailleChaine;i++) {
     // test si le caractere n'est pas un chiffre
     if (! isdigit(str[i])) {
       // test si le caractere est un espace
       if (isspace(str[i])) {
	 // test si le caractere n'est pas le dernier (pour ne pas provoquer de depassement de tableau)
	 if (i<tailleChaine-1) {
	   // test si le caractere suivant n'est pas un espace (permettant de reperer un espace entre deux caracteres ou la fin de str)  
	   if (! isspace(str[i+1])) { 
	     return -1;
	   }
	 }
       }
       else return -1;
     }
   }
   return atoi(str);
}


/*********************************************************************************************************/
/* ModifieLecture : fonction modifiant la chaine de caractere str (correspondant a un arret) pour pouvoir*/
/*                  l'inserer correctement dans le tableau                                               */
/*                  action : suppression des espaces de debut et de fin, et du caractere entree          */
/*********************************************************************************************************/
void modifieLecture(char* str) {
  
  int lettreDebut=0;
  int j=0;
  int lettreFin=strlen(str)-1;
 
 
  // comptage des espaces au debut de str
  while(isspace(str[lettreDebut])) {
    lettreDebut++;}
  // comptage des espaces a la fin de str
  while(isspace(str[lettreFin])) {
    lettreFin--;}
  // test si str ne contient que des espaces
  if (lettreFin-lettreDebut<0) {
    str[0]='\0';
  }
  else {
    // pour tous les caracteres compris entre ces deux valeurs
    for(j=lettreDebut;j<=lettreFin;j++) {
      // decalage des caracteres
      str[j-lettreDebut]=str[j];
    }
    // placement du caractere de fin de chaine de caractere
    str[lettreFin-lettreDebut+1]='\0';
  }
}


/*********************************************************************************************************/
/* LiberePointeur : fonction liberant tous les pointeurs utilises                                        */
/*********************************************************************************************************/
void liberePointeur(int *ligne, char **arret, int **mat) {
  free(mat); 
  free(arret);
  free(ligne);
}


/*********************************************************************************************************/
/* ConstruitStructure : fonction mettant en place la structure de données                                */
/*********************************************************************************************************/
int construitStructure(int *ligne, char **arret, int **mat, int nbLigne, int *nbArret, FILE *fichier) {

  char* temp_string;           // chaine de caractere stockant les lectures
  int temp_int=0;              // entier stockant une valeur temporaire
  int nbArretLigne=0;       // nombre d'arret de la ligne en cours de lecture
  int i=0;                     // compteur de boucle
  int j=0;                     // compteur de boucle
  int k=0;                     // compteur de boucle
 
        
  temp_string=(char *) calloc(MAX_LONG,sizeof(char));
  *nbArret=0;
  rewind(fichier);
  
  // lecture du nombre de ligne
  fgets(temp_string,MAX_LONG,fichier);
  // pour toutes les lignes
  for(i=0;i<nbLigne;i++) {
    // lecture du numero de la ligne et stockage dans le tableau ligne
    fgets(temp_string,MAX_LONG,fichier);
    temp_int=stringtoint(temp_string);
    ligne[i]=temp_int;
    // lecture du nombre d'arret de la ligne
    fgets(temp_string,MAX_LONG,fichier);
    nbArretLigne=stringtoint(temp_string);
    // pour tous les arrets
    for(j=0;j<nbArretLigne;j++) {
      strcpy(temp_string,"");
      // lecture des arrets
      fgets(temp_string,MAX_LONG,fichier);
      modifieLecture(temp_string);
      // test si temp_string est vide
      if (strlen(temp_string)==0) {
	free(temp_string);
	return -1;
      }
      k=0;
      // test la presence de l'arret dans le tableau arret
      while ((k<*nbArret) && (strcmp(arret[k],temp_string)!=0)) {
	k=k+1;
      }
      // si l'arret n'est pas present
      if (k==*nbArret) {
	// tableau arret : allocation dynamique du nombre de caractere de la lecture (jusqu'au caractere /0 compris)
	arret[*nbArret]=(char *) calloc(strlen(temp_string)+1,sizeof(char));
	// copie de l'arret dans le tableau arret
	arret[*nbArret]=strcpy(arret[*nbArret],temp_string);
	// remplissage de la matrice [ligne][arret] 
	mat[i][*nbArret]=j+1;
	*nbArret=*nbArret+1;
      }
      // si l'arret est present
      else {
	// remplissage de la matrice [ligne][arret]
      	mat[i][k]=j+1;
      }
    }
  }
  free(temp_string);
  return 0;
}


/*********************************************************************************************************/
/*  TesteOuvertureFichier : teste l'existence, les droits et le type du fichier                          */
/*                          valeur renvoyée : 0 si succes                                                */
/*                                            1 nombre négatif si echec                                  */
/*********************************************************************************************************/
int testeOuvertureFichier(char *nomFichier, int nbArgument) {  
  
  struct stat buffer;
  
  // test du nombre d'argument
  if (nbArgument!=2) return -1;
  // test de l'existence du fichier
  if (access(nomFichier,00)==0) {
    // test si le fichier dispose du droit de lecture
    if (access(nomFichier,04)!=0) return -3;
  }
  else return -2;
  stat(nomFichier,&buffer);
  // test si le fichier est un fichier regulier
  if (buffer.st_mode>>12!=0x8) {
    return -4;
  }
  return 0;
}


/*********************************************************************************************************/
/* RecupereValeur : fonction récuperant à partir du fichier le nombre de ligne et le nombre d'arret      */
/*                  valeur renvoyée : 0 si succes                                                        */
/*                                    1 nombre negatif sinon                                             */
/*********************************************************************************************************/
int recupereValeurs(int *nbLigne,int *nbArret, FILE *fichier) {

  char *temp_string;
  int nbArretLigne=0;
  int numeroLigne=0;
  int i,j;

  temp_string=(char *) calloc(MAX_LONG,sizeof(char));
  
  // test de fin de fichier
  if (feof(fichier)) {
    free(temp_string);
    return -1;
  }
  // lecture du nombres lignes
  fgets(temp_string,MAX_LONG,fichier); 
  *nbLigne=stringtoint(temp_string);
  if (*nbLigne==-1) {
    free(temp_string);
    return -1;
  }
  
  if (*nbLigne==0) {
    free(temp_string);
    return -2;
  }
  // test de fin de fichier
  if (feof(fichier)) {
    free(temp_string);
    return -1;
  }
  // pour toutes les lignes
  for (i=0;i<*nbLigne;i++) {
    // lecture du numeros de la lignes
    fgets(temp_string,MAX_LONG,fichier);
    // test la fin de fichier
    if (feof(fichier)) {
      free(temp_string);
      return -1;
    }
    numeroLigne=stringtoint(temp_string);
    if (numeroLigne<0) {
      free(temp_string);
      return -1;
    }
    // test la fin de fichier
    if (feof(fichier)) {
      free(temp_string);
      return -1;
    }
    // lecture du nombre d'arret de la ligne
    fgets(temp_string,MAX_LONG,fichier);
    nbArretLigne=stringtoint(temp_string);
    if (nbArretLigne<0) {
      free(temp_string);
      return -1;
    }
    // test la fin de fichier
    if (feof(fichier)) {
      free(temp_string);
      return -1;
    }
    // incrementation du nombre total d'arret du nombre d'arret de la ligne en cours
    *nbArret=*nbArret+nbArretLigne;
    // pour tous les arrets de la ligne
    for (j=0;j<nbArretLigne;j++) {
      // lecture du nom de l'arret
      fgets(temp_string,MAX_LONG,fichier);
      // test la fin de fichier sauf pour le dernier arret de la derniere ligne
      if ((i!=*nbLigne-1)&&(feof(fichier))&&(j!=nbArretLigne)) {
	free(temp_string);
	return -1;
      }
    }
  }
  free(temp_string);
  return 0; 
}




//int construitStructure(int *ligne, char **arret, int **mat, int nbLigne, int *nbArret, FILE *fichier);
void liberePointeur(int *ligne, char **arret, int **mat);
//int testeOuvertureFichier(char *nomFichier, int nbArgument);
//int recupereValeurs(int *nbLigne,int *nbArret, FILE *fichier);
//void recherche(int depart, int arrivee,int nb_ligne,int nb_arret,int* ligne,char** arret,int **mat);
////////////////////////////////////
// Projet d'algorithmie N°1       //
// Année 2002-2003                //
// -Brice GRICHY                  //
// -Arnaud BRUNET                 //
// -Timothée CONSTENSOUX          //
////////////////////////////////////
#include <stdlib.h>
#include <stdio.h>
//#include "recherche.h"
#include <unistd.h>



int recherche0(int *dep,int *arr,int *lg,int **mat,int nb_ligne) // Fonction de recherche de ligne contenant le départ et l'arrivée.
     // Retourne 1 si on en trouve une, 0 sinon
     // Pour chaque ligne, on regarde si cette ligne contient le départ et l'arrivée
{
  int i=0;
   for(i=0;i<nb_ligne;i++)
    {
      if (mat[i][*dep]!=0)
	{
	  if (mat[i][*arr]!=0)
	    {
	      *lg=i;
	      return 1;
	    }
	}
    }
   return 0;
}


int recherche1(int *dep, int *arr,int *chg,int *lge1,int *lge2,int **mat,int nb_ligne,int nb_arret)   // Fonction de recherche d'itinéaire à 1 changement
     // On cherche deux lignes d'intersection a, l'une contenant l'arrêt de départ, l'autre contenant l'arrêt d'arrivée
     // Renvoi 1 si l'itinéraire est trouvé, 0 sinon
     // Pour chaque arrêt, on effectue un recherche0 (aucun changement) entre cet arrêt et le départ puis entre cet arrêt et l'arrivée.
{
 int a;  

 for ( a=0; a<nb_arret; a++)
 {
   int t;
   int u;
   t=recherche0(&a,arr,lge2,mat,nb_ligne);

   u=recherche0(dep,&a,lge1,mat,nb_ligne);
   if (t!=0 && u!=0)
   { 
     *chg=a;
    return 1;
  }
 }
 return 0;
}



int recherche2 (int *dep, int *arr,int *chgp1,int *chgp2,int *lgp1, int *lgp2, int *lgp3,int **mat,int nb_ligne,int nb_arret)  // Fonction de recherche d'itinéraires à 2 changements
     // Pour chaque ligne contenant l'arrêt de départ, on teste s'il existe un itinéraire à 1 changement à partir d'un des arrêts de cette ligne.
     // Cette fonction renvoie 1 si un tel itinéraire est trouvé, 0 sinon.
     // lg (lg1, voir recherche() ) est la première ligne à prendre
     // chg (chg1 voir recherche() ) est le premier changement à effectuer
     // chg2 (changement suivant) et lg2 et lg3 (lignes suivantes) sont affectées dans le sous programme recherche1
{
 int i,j;
 for (i=0; i<nb_ligne; i++)
 {
   if (mat[i][*dep]!=0)     
  {
   
   *lgp1=i;
   for (j=0;j<nb_arret;j++)
   {
    if (mat[i][j]!=0)      
    {
    * chgp1=j;
     if (recherche1(chgp1,arr,chgp2,lgp2,lgp3,mat,nb_ligne,nb_arret)!=0)
     {
      return 1;
     }
    }
   }
  }
 }
 return 0;
}


int terminus(int *lg, int *dep, int *arr,int **mat,int nb_arret) // Fonction qui retourne l'identifiant de l'arrêt "terminus" vers lequel on veut se diriger
     // On recherche le sens de direction de l'itinéraire sur la ligne (positif ou négatif)
     // On retourne le maximun ou le minimum selon le cas
{
  int i, max, term;
 if (mat[*lg][*dep]<mat[*lg][*arr])
   {
     max=0;
     term=0;
     for ( i=1;i<nb_arret;i++)
       {
	 if (mat[*lg][i]>max)
	   {
	     term =i;
	     max=mat[*lg][i];
	   }
       }
     return term;
   }
 else
   {
     i=0;
     while (mat[*lg][i]!=1)
       {
	 i++;
       }
     return i;
   }
}

 
 

 
 
 
 
void recherche(int depart, int arrivee,int nb_ligne,int nb_arret,int* ligne,char** arret,int **mat) // On test la possibilité d'un itinéraire sans changement, puis avec un changement, puis avec 2 changements
     // Puis affiche le résultat
{
                 // identifiants des arrêts de changement
               // identifiants des lignes à prendre
  int * chg1=(int*)malloc(sizeof(int));
  int * chg2=(int*)malloc(sizeof (int));
  int * lg1=(int*)malloc(sizeof (int));
  int * lg2=(int*)malloc(sizeof (int));
  int * lg3=(int*)malloc(sizeof (int));
  int direction;
  

 if (recherche0(&depart,&arrivee,lg1,mat,nb_ligne)!=0)
   {
      direction=terminus(lg1,&depart,&arrivee,mat,nb_arret);
     affiche("Pas de changement\n Monter a");
     affiche(arret[depart]);
     affiche(" \nPrendre ligne");
     afficheInt(ligne[*lg1]);
     affiche("vers");
     affiche(arret[direction]);
     affiche(" \n  Descendre a");
     affiche(arret[arrivee]);
     affiche(" \n");
   }
 else
   {      
     if (recherche1(&depart,&arrivee,chg1,lg1,lg2,mat,nb_ligne,nb_arret)!=0)
       {
 
	   direction=terminus(lg1,&depart,chg1,mat,nb_arret);
           affiche("1 changement :\n  Monter a");
	   affiche(arret[depart]);
	   affiche("\n  Prendre ligne");
	   afficheInt(ligne[*lg1]);
	   affiche(" vers ");
	   affiche(arret[direction]);
	   affiche("\n  Descendre a ");
	   affiche(arret[*chg1]);
	   affiche("\n");
           direction=terminus(lg2,chg1,&arrivee,mat,nb_arret);
           affiche("Prendre ligne");
	   afficheInt(ligne[*lg2]);
	   affiche(" vers ");
	   affiche(arret[direction]);
	   affiche(" \n   Descendre a ");
	   affiche(arret[arrivee]);
	   affiche(" \n");
       }
     else
       {        
	 if (recherche2(&depart,&arrivee,chg1,chg2,lg1,lg2,lg3,mat,nb_ligne,nb_arret)==0) 
	   {
	     affiche("Pas d'itinéraire possible avec moins de deux changements\n");
	   }
	 else
	   {
         direction=terminus(lg1,&depart,chg1,mat,nb_arret);
         affiche("2 changements :\n  Monter a ");
	 affiche(arret[depart]);
	 affiche("\n\tPrendre ligne ");
	 afficheInt(ligne[*lg1]);
	 affiche(" vers ");
	 affiche(arret[direction]);
	 affiche("\n  Descendre a ");
	 affiche(arret[*chg1]);
	 affiche("\n");
         direction=terminus(lg2,chg1,chg2,mat,nb_arret);
	 affiche("\tPrendre ligne ");
	 afficheInt(ligne[*lg2]);
	 affiche(" vers ");
	 affiche(arret[direction]);
	 affiche(" \n  Descendre a ");
	 affiche(arret[*chg2]);
	 affiche(" \n");
         direction=terminus(lg3,chg2,&arrivee,mat,nb_arret);
         affiche("\tPrendre ligne ");
	 afficheInt(ligne[*lg3]);
	 affiche(" vers ");
	 affiche(arret[direction]);
	 affiche("\n  Descendre a ");
	 affiche(arret[arrivee]);
	 affiche(" \n");
	 }
       }
   }


}
#ifndef RECHERCHE
#define RECHERCHE

//int recherche2 (int *dep, int *arr,int *chgp1,int *chgp2,int *lgp1, int *lgp2, int *lgp3,int **mat,int nb_ligne,int nb_arret);
//int recherche1(int *dep, int *arr,int *chg,int *lge1,int *lge2,int **mat,int nb_ligne,int nb_arret);
//int recherche0(int *dep,int *arr,int *lg,int **mat,int nb_ligne);
void recherche(int depart, int arrivee,int nb_ligne,int nb_arret,int* ligne,char** arret,int **mat); 
//int terminus(int *lg, int *dep, int *arr,int **mat,int nb_arret);




#endif
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
//*****************************************************
//**  Interface 
//**  SILR-1   17/01/03
//**  Richaud Guillaume - Lavoix Pierre 
//*****************************************************
#include <stdio.h>
#include "..\IHM\GestAffich.h"
#include "..\IHM\Verif.h"

#define NBRFAUTE  3  // Nombre max de lettres differentes entre 2 mots
#define NORAW system("stty echo -raw");
#define RAW system("stty -echo raw");

// Variables globales ( Arret d'origine, Arret de destination ) 
char* savOrigine;
char* savDestination;

void fin( void )
{    
  NORAW;
  EFFACE_ECRAN;
  GOTO(1,1);
  printf("Exit\n");
  // LIBERATION DES POINTEURS UTILISESs
  //  liberation_pointeur();
}



// Initialisation de l'affichage
//*****************************************************
void debut( void )
{
  EFFACE_ECRAN;
  GOTO(6,1);
}


// Affichage de l'entete
//*****************************************************
void cadre( void )
{
  SAUVE_CURSEUR;
  GOTO(1,1); EFFACE_LIGNE;
  GOTO(2,1); EFFACE_LIGNE;
  GOTO(3,1); EFFACE_LIGNE;
  GOTO(4,1); EFFACE_LIGNE;
  GOTO(5,1); EFFACE_LIGNE;  
  GOTO(6,1); EFFACE_LIGNE;  

  PROPRIETE_ECRAN(AVANT_ROUGE);
  GOTO( 1,1); printf("_____[ RECHERCHE DE TRAJET ]______________________________________\n");
  GOTO( 4,1); printf("__________________________________________________________________\n");

  PROPRIETE_ECRAN(AVANT_BLANC);
  GOTO(2,10); printf("Origine :\n");
  GOTO(2,25); printf("%s\n", savOrigine);
  GOTO(3,10); printf("Destination :\n");
  GOTO(3,25); printf("%s\n", savDestination);
  RESTORE_CURSEUR;
}

// Effacage de l'entete
//*****************************************************
void effaceCadre( void )
{
  SAUVE_CURSEUR;
  GOTO(1,1); EFFACE_LIGNE;
  GOTO(2,1); EFFACE_LIGNE;
  GOTO(3,1); EFFACE_LIGNE;
  GOTO(4,1); EFFACE_LIGNE;
  GOTO(5,1); EFFACE_LIGNE;  
  GOTO(6,1); EFFACE_LIGNE;  
  RESTORE_CURSEUR;
}

//  Gestion de l'affichage des chaines
//*****************************************************
void affiche( char* chaine )
{
  effaceCadre();
  printf("  %s",chaine);
  cadre();
}

//  Gestion de l'affichage des entiers
//*****************************************************
void afficheInt( int entier )
{
  effaceCadre();
  printf("  %u\n", entier);
  cadre();
}

// Affichage des chaines en jaune
//*****************************************************
void message( char* chaine )
{
  effaceCadre();
  PROPRIETE_ECRAN(AVANT_JAUNE);
  printf( "\n[>%s\n",chaine);
  PROPRIETE_ECRAN(AVANT_BLANC);
  cadre();
}


// Capture de la chaine entrée par l'utilisateur
//*****************************************************
void saisiChaine( char mot [30] )
{
    char ch;
  char index = 0;
  RAW;
  memset( mot, 0, 30 );
  while( (ch != 10) &&  (ch != 13 ))
    {
      if ( index >= 29 ) ch = 13;
      else
	{
	  ch = getchar();
	  //	  	  printf("(%x)",ch);
	  //	  if((ch==0x1b)) printf("%d",fflush(stdin));

	  if ( (ch == 3) || (ch ==4) || (ch ==0x1a) || (ch==0x1b))
	    {
	      fin();
	      exit(0);
	    }

	  if (( ch == 0x20 ) || ( ch == 0x28 ) || ( ch == 0x29 )  || ( ch ==0x2D ) || ( ch == 0x2E ) || (( ch > 0x60 ) && ( ch < 0x7B )) || (( ch > 0x29 ) && ( ch < 0x39 )) )
	    {
	      printf("%c",ch);
	      fflush( stdout );
	      mot[index] = ch;
	      index = index + 1;
	    }
	  if (( ch > 0x40 ) && ( ch < 0x5B ))
	    {
	      ch = ch + 0x20;
	      printf("%c",ch);
	      fflush( stdout );
	      //	      fflush( stdin );
	      mot[index] = ch;
	      index++;
	    }
	  if ( ch == 127 )
	    {
	      mot[index] = 0;
	      if ( index != 0 )
		{
		  index = index - 1;
		  printf("\b \b");
		}
	    }

	}
    }
  mot[index] = 0;
  printf("\n");
  //  printf("%s\n",mot);
  //  getchar();
  NORAW;
}


// Gestion de l'entrée de l'arret au clavier
//*****************************************************
//   arret[] = tableau dans le quel sera recherche l'arret saisi
//   nbre_arret = nombre d'arret dans le tableau
//   msg  = message affiché lors de la demande de saisi
//*****************************************************
int saisi( char** arret, int nbre_arret, int position_ecran )
{
  
  char nomArret [30];
  int numArret = -1;


  do {
    SAUVE_CURSEUR;
    GOTO(position_ecran,25);
    EFFACE_LIGNE;
    saisiChaine( nomArret );
    RESTORE_CURSEUR;
    numArret =  Verification(  nomArret, arret, nbre_arret, NBRFAUTE ); 
    
    
    if (numArret  == 0 )
      {
	message("Arret incorrect");
      }

    if (numArret > 0 )
      {
	message("Saisi confirmée");
	affiche( "Arret:" );
	affiche( arret[ numArret -1 ]);
      }

    if ( numArret < 0 )
      {
	message( "Erreur de saisi. Arret possible: " );
	affiche( arret[ (-(numArret +1)) ]);
      }
    
    
  } while( numArret <= 0 );
    
    return( numArret - 1 );
}


// PAS ENCORE FAIT !!!
//*****************************************************
int continuer( void )
{
  char ch;
  
  message(" Pour quitter presser 'q' \n");
  RAW; 
  ch =  getchar();
  NORAW;
  return  ( (ch != 'q') && (ch != 'Q') ); 
}

//*****************************************************
//**  Correction des erreurs dans une chaine
//**  SILR-1   17/01/03
//**  Richaud Guillaume - Lavoix Pierre 
//*****************************************************


//*****************************************************
// Recherche dans le tableau Arret, la chaine qui a le
// moins de caracteres distincts de motV (apres convertion)
// renvoi un int == indice + 1 si le mot est ok
// renvoi int    == 0 si plus de nbre_max_faute
// renvoi int    == -( indice + 1 ) si moins de 3 fautes
// Verfification( mot_a_verifie ,tableau_contenant_les mots, taille_du_tableau, nombre_maximal_de_faute)
//*****************************************************

#ifndef VERIF
#define VERIF


int Verification ( char* , char**, int, int );

#endif
//*****************************************************
//**  Interface 
//**  SILR-1   17/01/03
//**  Richaud Guillaume - Lavoix Pierre 
//*****************************************************
#ifndef INTERFACE
#define INTERFACE

// Variables globales ( Arret d'origine, Arret de destination ) 
extern char* savOrigine;
extern char* savDestination;


// Initialisation de l'affichage
void debut( void );

// Affichage de l'entete
void cadre( void );

//  Gestion de l'affichage des chaines
void affiche( char* );

//  Gestion de l'affichage des entiers
void afficheInt( int );


// Affichage des chaines en jaune
void message( char* );

// Gestion de l'entrée de l'arret au clavier
//*****************************************************
//   arret[] = tableau dans le quel sera recherche l'arret saisi
//   nbre_arret = nombre d'arret dans le tableau
//   msg  = message affiché lors de la demande de saisi
//*****************************************************
int saisi( char** , int, int );

// Renvoit
int continuer( void );


#endif
#define  GOTO(ligne, colonne)            printf("\033[%d;%dH", ligne, colonne)

#define  MONTE_CURSEUR(nbreligne)        printf("\033[%dA", nbreligne)
#define  DESCEND_CURSEUR(nbreligne)      printf("\033[%dB", nbreligne)
#define  RECULE_CURSEUR(nbrecolonne)     printf("\033[%dC", nbrecolonne)
#define  AVANCE_CURSEUR(nbrecolonne)     printf("\033[%dD", nbrecolonne)

#define  SAUVE_CURSEUR                   printf("\033[s")
#define  RESTORE_CURSEUR                 printf("\033[u")

#define  EFFACE_ECRAN                    printf("\033[2J")
#define  EFFACE_LIGNE                    printf("\033[K")

#define  PROPRIETE_ECRAN(attribut)       printf("\033[%dm", attribut)

#define  ATTR_OFF          0
#define  BOLD_ON           1
#define  SOULIGNE          4
#define  CLIGNOTE_ON       5
#define  INVERSE_PROPRIETE 7
#define  CONCEALED_ON      8

#define  AVANT_NOIR    30
#define  AVANT_ROUGE   31
#define  AVANT_VERT    32
#define  AVANT_JAUNE   33
#define  AVANT_BLEU    34
#define  AVANT_MAGENTA 35
#define  AVANT_CYAN    36
#define  AVANT_BLANC   37

#define  FOND_NOIR    40
#define  FOND_ROUGE   41
#define  FOND_VERT    42
#define  FOND_JAUNE   43
#define  FOND_BLEU    44
#define  FOND_MAGENTA 45
#define  FOND_CYAN    46
#define  FOND_BLANC   47
#include <stdio.h>
#include "../Structure/primitives.h"
#include "../IHM/Interf.h"
#include "../Recherche/recherche.h"


int main(int argc,char * argv[]) {

  /***************************   DECLARATIONS DES VARIABLES *********************************************/
  int nbLigne=0; // nombre total de lignes
  int nbArret=0; //nombre total d'arret  (sans doublons)
  int *ligne;   // tableau contenant les numeros de lignes par ordre d'apparition
  char **arret; // tableau contenant tous les arrets (sans doublons pour les intersections)
  int **mat;    // matrice [ligne][arret]
  FILE *fichier;
  
  int i,j;
  int ind_origine;
  int ind_destination;

  /*****************************     OUVERTURE DU FICHIER    *********************************************/
  switch (testeOuvertureFichier(argv[1], argc)) {
  case -1 : {
    puts("ERREUR : Nombre d'arguments incorrect");
    printf("\t Syntaxe : %s chemin_absolu_fichier \n",argv[0]);
    return -1;
  }
  case -2 : {
    printf("ERREUR : %s inexistant \n",argv[1]);
    return -1;
  }
  case -3 : {
    printf("ERREUR : %s permission insuffisante \n",argv[1]);
    printf("\t Permission de lecture obligatoire \n");
    return -1;
  }
  case -4 : {
    printf("ERREUR : %s n'est pas un fichier régulier \n",argv[1]);
    return -1;
  }
  }
  fichier=fopen(argv[1],"r");


  /*******************    RECUPERATION DES VALEURS NBLIGNE ET NBARRET  ***********************************/
  switch (recupereValeurs(&nbLigne, &nbArret, fichier)) {
  case -1 : {
    printf("ERREUR : %s mal construit \n",argv[1]);
    return -1;
  }
  case -2 : {
    puts("Nombre total de ligne nulle : FIN D'EXECUTION");
    return -1;
  }
  }

  /**********************   INITIALISATION DES TABLEAUX ET DE LA MATRICE   *******************************/
  // tableau des numeros de ligne : allocation dynamique de nbLigne entiers
  ligne=(int *) calloc(nbLigne,sizeof(int));
  // matrice [ligne][arret] : allocation dynamique de nbLigne pointeurs d'entiers 
  mat=(int ** ) calloc(nbLigne,sizeof(int *));
  arret=(char **) calloc(nbArret,sizeof(char *));
  for(i=0;i<nbLigne;i++) {
    ligne[i]=0;
    // matrice [ligne][arret] : allocation dynamique pour chaque ligne de nbArretTotale entiers
    mat[i]=(int *) calloc(nbArret,sizeof(int));
    for(j=0;j<nbArret;j++) {
      mat[i][j]=0;
    }
  }
  
  /****************************  MISE EN PLACE DE LA STRUCTURE   *****************************************/
  if (construitStructure(ligne, arret, mat, nbLigne, &nbArret, fichier)==-1) {
    printf("ERREUR : %s mal construit \n",argv[1]);
    liberePointeur(ligne, arret, mat);
    return -1;
  }

  /****************************   FERMETURE DU FICHIER   *************************************************/
  fclose(fichier);

  /*************************************    AFFICHAGE      ***********************************************/
  /*  printf("ligne \t");
  for(i=0;i<nbLigne;i++) {
    printf("%d ",ligne[i]);
  }
  puts("\narret");
  for(j=0;j<nbArret;j++) {
    printf("%s \t",&(*arret[j]));
    for(i=0;i<nbLigne;i++) {
      printf(" %d ",mat[i][j]);
    }
    puts("\n");
    }*/
    


  //------------------ INTERFACE -----------------------------
  debut();
  
  do {
  savOrigine = " ";
  savDestination=" ";
  cadre();
  
  ind_origine=saisi(arret, nbArret, 2 );
  savOrigine= arret[ ind_origine ];
  cadre();

  ind_destination=saisi(arret, nbArret, 3 );
  savDestination= arret[ ind_destination ];
  message("Resultat : \n"); 

  if (ind_origine != ind_destination) 
    {  recherche(ind_origine, ind_destination, nbLigne,nbArret,ligne,arret,mat); }
  else
    { affiche(" Origine et destination identiques ");}
  } while( continuer() );
  //----------------------------------------------------------
  fin(); 
  
  return 0;
}
