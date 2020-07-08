#include <stdio.h>
#include <stdlib.h>
#include "iris_in.h"
#include "learning.h"
#include "iris_out.h"

/* Exécution   >gcc -lm main.c    */

int main(){
    char* fileName = "iris.data"; /*https://archive.ics.uci.edu/ml/machine-learning-databases/iris/iris.data*/

        /* Find size line and colum of dataIris */
    SetSizeDataIris ( fileName );

        /* set space for DataIris */
    DataIris* data = (DataIris*) malloc (sizeof(DataIris));
    data = reserveSpaceDataIris (data);

        /* Charge datas from database */
    data = ChargeDatabase (fileName, data);

        /*  Normalize the datas          */
    NormalizeMatrix (data);
  
        /*  Calculate the average of vectors */
    double*  average =  AverageMatrix (data);
    
        /*  Drow the neuronal space */
    DataNeuronne* dataNeuronne = (DataNeuronne*) malloc (sizeof(DataNeuronne));
    dataNeuronne = reserveSpaceDataNeuronne (dataNeuronne);
    dataNeuronne = EnvDonneeNeuronne (dataNeuronne, average);

        /*  Déterminer les neuronnes gagnants */
    
    Learning_Neuronnes (data, dataNeuronne->neuronne, size.ordonnencement, size.rayon, size.alpha_ordonn);
    Learning_Neuronnes (data, dataNeuronne->neuronne, size.affinage,        1 ,             size.alpha_affin);

        /* Etquettage */
    Etiquettage (data, dataNeuronne);


        /*  Displays */
    //display_database (data );
    //display_nameflower (data );
    //display_normalise (data );
    //display_average ( average);
    //display_neuronne_space ( dataNeuronne );
    display_result_neuronne ( dataNeuronne );
    
    //Free space
    freeSpace (data, dataNeuronne, average);




    return 0;
}