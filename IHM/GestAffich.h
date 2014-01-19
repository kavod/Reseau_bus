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
