#ifndef __IRIS__H_
#define __ISIS__H_
#include <time.h>

/* Structure de Liste chainnée */
typedef struct List{
    BMU* value;
    struct List *next;
} List;

/* Ajout en fin de list */
void addBack (List* list, BMU* value){
    List *node= (List*)malloc(sizeof(List));
    if(node == NULL) return;
    List* end = list;
    while(end->next != NULL) end =end->next;
    node->value = value;
    node->next = NULL;
    end->next = node;
}
/* Suppression en fin de List */
void eraseBack (List* list){
    List* last = list;
    if(list->next == NULL) exit( EXIT_FAILURE);
    while(last->next->next != NULL) last = last->next;
    free(last->next->next);
    last->next = NULL;
}

/* Fonction pour générer 150 valeurs aléatoires parmi [0,149]*/
int* Rand_Table ( int b ){
    int *Rand = (int*) malloc(b*sizeof(int));
    int i, random, t, a=0;
    for( i=0; i<b; i++) Rand[i] = i;
    for( i= 0; i<b; i++){
        random = rand()%(b-a)+a;
        t= Rand[i];
        Rand[i]=Rand[random];
        Rand[random]=t;
    }
    return Rand;
} 

//Fonction pour calculer la distance entre deux vecteurs
double distance_neuronne ( double *Tab1, double *Tab2){
    int i;
    double distance = 0;
    for (i=0; i < size.columnIris; i++)
        distance += pow (Tab1[i]-Tab2[i], 2);
    distance = sqrt (distance);
    return distance;
}

//Find the Best Match Unit
BMU* Winner_Neuronne (double* normalizeIris, double*** neuronne){
    double distance = DBL_MAX;
    double distance1 = 0;
    int cpt = 0;
    List  *list = &(List){0, NULL};
    for(int i=0; i< size.vertical; i++){
         for(int j=0; j< size.horizontal; j++){
             distance1 = distance_neuronne (normalizeIris, neuronne[i][j]);
             if(distance1 < distance ){
                 while(list->next != NULL) eraseBack (list);            /* on vide la liste dès qu'ont trouve */
                 BMU* bmu = (BMU*) malloc (sizeof(BMU));               /* un autre neuronne plus proche      */
                 bmu->i = i;
                 bmu->j = j;
                 cpt = 1;
                 addBack(list, bmu);
                 distance = distance1; 
             }else if(distance1 == distance ){
                 BMU* bmu = (BMU*) malloc (sizeof(BMU)); 
                 bmu->i = i;
                 bmu->j = j;
                 cpt ++;
                 addBack(list, bmu);
                 distance = distance;
             }
         }
    }
    //Choix aleatoire parmi les neuronnes les plus proche s'il y en a plusieurs
        srand(time(NULL));
        cpt = rand()%cpt+1;
        int pos=0;
        while(pos != cpt && list->next != NULL){ list = list->next; pos++;}
   return list->value;
} 

//Propagation to neighbors
void Propagation_Neighbors (double*** neuronne, double* normalized, BMU* bmu, double alpha, int rayon, int pos){
    //Find neighbors' raduis
    int i_start, i_end, j_start, j_end;
    //Top
    if(bmu->i < rayon) i_start = 0;
        else i_start = bmu->i - rayon;
    //Bottom
    if(bmu->i > size.vertical - rayon-1) i_end = size.vertical-1;
        else i_end = bmu->i + rayon;
    //Left
    if(bmu->j < rayon) j_start = 0;
        else j_start = bmu->j - rayon;
    //Right
    if(bmu->j >= size.horizontal - rayon-1) j_end = size.horizontal-1;
        else j_end = bmu->j + rayon; 
         //propagation to neighbor
    for(int i=i_start; i<= i_end; i++){
        for(int j=j_start; j<= j_end; j++){
            for(int k=0; k< size.columnIris; k++){
                neuronne[i][j][k] = neuronne[i][j][k] + alpha * (normalized[k] - neuronne[i][j][k]);
             }
        }
    }
}

//Function pour apprentissage Learnning

double*** Learning_Neuronnes (DataIris *data, double*** neuronne, int itteration, int rayon, double alpha){
    double alpha_max = alpha;
    int periode = itteration / rayon;
    for(int i=0; i< itteration; i++){
        srand(time(NULL));
        int *Rand = Rand_Table ( size.lineIris );                       /* random              */
        alpha = alpha_max * (1 - (double)i / (double)itteration);      /* alpha               */
        if(rayon != 1 && !((i+1)%periode)) rayon--;                   /* rayon de voisinage  */
        for(int k=0; k< size.lineIris; k++){
            BMU* bmu = (BMU*) malloc (sizeof(BMU)); 
            bmu = Winner_Neuronne (data[Rand[k]].normalized, neuronne);                    /* recherche du best match unit */
            Propagation_Neighbors (neuronne, data[Rand[k]].normalized, bmu, alpha, rayon, i);     /* propagation aux voisins      */
        }
    }
    //printf("END");
    return neuronne;
}

//Etiquettage
void Etiquettage (DataIris *data, DataNeuronne *dataNeuronne){
    for(int i=0; i< size.lineIris; i++){
        BMU* bmu = (BMU*) malloc (sizeof(BMU)); 
        bmu = Winner_Neuronne (data[i].normalized, dataNeuronne->neuronne); 
        int x = bmu->i;
        int y = bmu->j;
        if(dataNeuronne->result[x][y] == 0 ){
            if(strcmp(data[i].nameIris, categories[0]) == 0) dataNeuronne->result[x][y] = 1;
            if(strcmp(data[i].nameIris, categories[1]) == 0) dataNeuronne->result[x][y] = 2;
            if(strcmp(data[i].nameIris, categories[2]) == 0) dataNeuronne->result[x][y] = 3;
        }
    }
}
#endif