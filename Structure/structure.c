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


