//*****************************************************
//**  Interface 
//**  SILR-1   17/01/03
//**  Richaud Guillaume - Lavoix Pierre 
//*****************************************************
#include <stdio.h>
#include "GestAffich.h"
#include "Verif.h"

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

