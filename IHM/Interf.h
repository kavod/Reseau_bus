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
