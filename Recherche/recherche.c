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
