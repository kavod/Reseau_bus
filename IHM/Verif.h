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
