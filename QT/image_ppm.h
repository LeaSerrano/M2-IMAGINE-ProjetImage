// ----------------------------------------------------------------------------
// Filename        : image_ppm.c
// Description     :
// Created On      : Tue Mar 31 13:26:36 2005
// ----------------------------------------------------------------------------

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <vector>

#define allocation_tableau(nom, type, nombre) \
if( (nom = (type*) calloc (nombre, sizeof(type) ) ) == NULL ) \
    {\
            printf("\n Allocation dynamique impossible pour un pointeur-tableau \n");\
            exit(1);\
    }

typedef unsigned char OCTET;

// Ensemble de fonctions pour convertir une couleur dans l'espace RGB en une couleur dans l'espace LAB

float f_CIEXYZ_to_CIELAB( float t )
{
    float delta = 6./29.;
    if( t > delta*delta*delta )
        return cbrt(t);
    else
        return (t / 3 * delta*delta) + (4./29.);
}

void RGB2LAB( float R, float G, float B, float& l, float& a, float& b )
{
    float cX, cY, cZ;
    cX = 0.4887180 * R + 0.3106803 * G + 0.2006017 * B;
    cY = 0.1762044 * R + 0.8129847 * G + 0.0108109 * B;
    cZ = 0.0102048 * G + 0.9897952 * B;

    float Xn = 95.0489;
    float Yn = 100.;
    float Zn = 108.8840;

    l = 116 * f_CIEXYZ_to_CIELAB( cY/Yn ) - 16;
    a = 500 * ( f_CIEXYZ_to_CIELAB( cX/Xn ) - f_CIEXYZ_to_CIELAB( cY/Yn ) );
    b = 200 * ( f_CIEXYZ_to_CIELAB( cY/Yn ) - f_CIEXYZ_to_CIELAB( cZ/Zn ) );
}

// Structure cluster
struct cluster
{
    int i, j;                 // Coordonnées du centre
    unsigned int* pixels;     // Ensemble des pixels appartenant au clusters
        // Stocke un pixel sous la forme de ( i, j )
    unsigned int nb_pixels;   // Nombre d'éléments dans pixels

    // Initialise une nouvelle structure à partir d'un centre
    static cluster* new_cluster( int iC, int jC )
    {
        cluster* c = (cluster*) malloc( sizeof( cluster ) );

        c->i = iC;
        c->j = jC;

        c->pixels = NULL;
        c->nb_pixels = 0;

        return c;
    }

    // Perturbe la position du centre du cluster en prenant le pixel ayant
    // le plus petit gradient dans un voisinnage carré de n*n à partir du centre
   void perturb( float* ImgLAB, int nW, int nH, int n )
    {
        float lowest_gradient = 1e30;
        int LGi, LGj;

        for( int k = -n; k <= n; k++ )
            for( int l = -n; l <= n; l++ )
                if( 0 < i+k && i+k < nH-1 && 0 < j+l && j+l < nW-1 )
                {
                    float Vec_Down_N = sqrt( pow( ImgLAB[((i+k+1)*nW+(j+l))*3  ] - ImgLAB[((i+k-1)*nW+(j+l))*3  ] , 2.)
                                            + pow( ImgLAB[((i+k+1)*nW+(j+l))*3+1] - ImgLAB[((i+k-1)*nW+(j+l))*3+1] , 2.)
                                            + pow( ImgLAB[((i+k+1)*nW+(j+l))*3+2] - ImgLAB[((i+k-1)*nW+(j+l))*3+2] , 2.) );

                    float Vec_Left_N = sqrt( pow( ImgLAB[((i+k)*nW+(j+l+1))*3  ] - ImgLAB[((i+k)*nW+(j+l+1))*3  ] , 2.)
                                            + pow( ImgLAB[((i+k)*nW+(j+l+1))*3+1] - ImgLAB[((i+k)*nW+(j+l+1))*3+1] , 2.)
                                            + pow( ImgLAB[((i+k)*nW+(j+l+1))*3+2] - ImgLAB[((i+k)*nW+(j+l+1))*3+2] , 2.) );

                    float G = Vec_Down_N + Vec_Left_N;

                    if( G < lowest_gradient )
                    {
                        lowest_gradient = G;
                        LGi = i+k;
                        LGj = j+l;
                    }
                }

        i = LGi;
        j = LGj;
    }

    // Calcule la distance entre le centre du cluster et un pixel candidat
    // décrit par la position ( i_cand, j_cand ) selon m et S
    float compare( float* ImgLAB, int i_cand, int j_cand, int nW, float m, float S )
    {
        float dLAB = sqrt( pow( ImgLAB[(i*nW+j)*3  ] - ImgLAB[(i_cand*nW+j_cand)*3  ], 2. )
                          + pow( ImgLAB[(i*nW+j)*3+1] - ImgLAB[(i_cand*nW+j_cand)*3+1], 2. )
                          + pow( ImgLAB[(i*nW+j)*3+2] - ImgLAB[(i_cand*nW+j_cand)*3+2], 2. ) );

        float dij = sqrt( pow( i - i_cand, 2. ) + pow( j - j_cand, 2. ));

        return dLAB + (m*dij)/S;
    }

    // Calcule le nouveau centre d'un cluster à partir de la moyenne
    // des coordonnées des pixels inclus dans ce dernier
   // Retourne la distance de Manhattan entre l'ancien centre et le nouveau
    int update()
    {
        if( nb_pixels == 0 )
            return 0;

        float new_i, new_j;
        new_i = new_j = 0.;

        for( unsigned long l = 0; l < nb_pixels; l += 2 )
        {
            new_i += pixels[l];
            new_j += pixels[l+1];
        }

        new_i = (new_i*2.)/(float)nb_pixels;
        new_j = (new_j*2.)/(float)nb_pixels;

        int old_i = i;
        int old_j = j;
        i = (int) floor( new_i );
        j = (int) floor( new_j );

        return abs( i - old_i ) + abs( j - old_j );
    }

    // Calcule la moyenne de la couleur RGB des pixels contenus dans le cluster
    void calcul_couleur_moyenne( OCTET* ImgIn, int nH, int nW, OCTET &R, OCTET &G, OCTET &B )
    {
        float avg[3] = { 0., 0., 0. };

        for (unsigned int l = 0; l < nb_pixels; l += 2)
        {
            avg[0] += ImgIn[ (pixels[l]*nW + pixels[l+1])*3    ];
            avg[1] += ImgIn[ (pixels[l]*nW + pixels[l+1])*3 + 1];
            avg[2] += ImgIn[ (pixels[l]*nW + pixels[l+1])*3 + 2];
        }

        R = floor( (avg[0]*2.)/(float)nb_pixels );
        G = floor( (avg[1]*2.)/(float)nb_pixels );
        B = floor( (avg[2]*2.)/(float)nb_pixels );
    }
};

typedef struct cluster* CLUSTER;

// Ecrit dans ImgOut la moyenne des pixels dans tous les pixels de tous les K clusters de C
void get_final_image( OCTET* ImgIn, OCTET* ImgOut, int nH, int nW, CLUSTER* C, int K )
{
    int iOut, jOut;
    OCTET R, G, B;

    for( int k = 0; k < K; k++ )
    {
        C[k]->calcul_couleur_moyenne(ImgIn, nH, nW, R, G, B);

        for( int l = 0; l < C[k]->nb_pixels; l += 2 )
        {
            iOut = C[k]->pixels[l];
            jOut = C[k]->pixels[l + 1];

            ImgOut[ ( iOut * nW + jOut ) * 3    ] = R;
            ImgOut[ ( iOut * nW + jOut ) * 3 + 1] = G;
            ImgOut[ ( iOut * nW + jOut ) * 3 + 2] = B;


        }
    }
}

/*===========================================================================*/
void ignorer_commentaires(FILE * f)
{
    unsigned char c;
    while((c=fgetc(f)) == '#')
        while((c=fgetc(f)) != '\n');
    fseek(f, -sizeof(unsigned char), SEEK_CUR);
}
/*===========================================================================*/


/*===========================================================================*/
void ecrire_image_ppm(char  nom_image[], OCTET *pt_image, int nb_lignes, int nb_colonnes)
{
    FILE *f_image;
    int taille_image = 3*nb_colonnes * nb_lignes;

    if( (f_image = fopen(nom_image, "wb")) == NULL)
    {
        printf("\nPas d'acces en ecriture sur l'image %s \n", nom_image);
        exit(EXIT_FAILURE);
    }
    else
    {
        fprintf(f_image,"P6\r") ;                               /*ecriture entete*/
        fprintf(f_image,"%d %d\r255\r", nb_colonnes, nb_lignes) ;

        if( (fwrite((OCTET*)pt_image, sizeof(OCTET), taille_image, f_image))
            != (size_t)(taille_image))
        {
            printf("\nErreur d'ecriture de l'image %s \n", nom_image);
            exit(EXIT_FAILURE);
        }
        fclose(f_image);
    }
}
/*===========================================================================*/

/*===========================================================================*/
void lire_nb_lignes_colonnes_image_ppm(char nom_image[], int *nb_lignes, int *nb_colonnes)
{
    FILE *f_image;
    int max_grey_val;

    /* cf : l'entete d'une image .ppm : P6                   */
    /*				       nb_colonnes nb_lignes */
    /*    			       max_grey_val          */


    if( (f_image = fopen(nom_image, "rb")) == NULL)
    {
        printf("\nPas d'acces en lecture sur l'image %s \n", nom_image);
        exit(EXIT_FAILURE);
    }
    else
    {
        fscanf(f_image, "P6 ");
        ignorer_commentaires(f_image);
        fscanf(f_image, "%d %d %d%*c", nb_colonnes, nb_lignes, &max_grey_val);
        fclose(f_image);
    }
}
/*===========================================================================*/
/*===========================================================================*/
void lire_image_ppm(char  nom_image[], OCTET *pt_image, int taille_image)
{
    FILE *f_image;
    int  nb_colonnes, nb_lignes, max_grey_val;
    taille_image=3*taille_image;

    if( (f_image = fopen(nom_image, "rb")) == NULL)
    {
        printf("\nPas d'acces en lecture sur l'image %s \n", nom_image);
        exit(EXIT_FAILURE);
    }
    else
    {
        fscanf(f_image, "P6 ");
        ignorer_commentaires(f_image);
        fscanf(f_image, "%d %d %d%*c",
               &nb_colonnes, &nb_lignes, &max_grey_val); /*lecture entete*/

        if( (fread((OCTET*)pt_image, sizeof(OCTET), taille_image, f_image))
            !=  (size_t)(taille_image))
        {
            printf("\nErreur de lecture de l'image %s \n", nom_image);
            exit(EXIT_FAILURE);
        }
        fclose(f_image);
    }
}

/*===========================================================================*/
/*===========================================================================*/

void planR(OCTET *pt_image, OCTET *src, int taille_image){
    int i;
    for (i=0; i<taille_image; i++){
        pt_image[i]=src[3*i];
    }
}

/*===========================================================================*/
/*===========================================================================*/

void planV(OCTET *pt_image, OCTET *src, int taille_image){
    int i;
    for (i=0; i<taille_image; i++){
        pt_image[i]=src[3*i+1];
    }
}

/*===========================================================================*/
/*===========================================================================*/

void planB(OCTET *pt_image, OCTET *src, int taille_image){
    int i;
    for (i=0; i<taille_image; i++){
        pt_image[i]=src[3*i+2];
    }
}

/*===========================================================================*/
/*===========================================================================*/

void ecrire_image_pgm(char  nom_image[], OCTET *pt_image, int nb_lignes, int nb_colonnes)
{
    FILE *f_image;
    int taille_image = nb_colonnes * nb_lignes;

    if( (f_image = fopen(nom_image, "wb")) == NULL)
    {
        printf("\nPas d'acces en ecriture sur l'image %s \n", nom_image);
        exit(EXIT_FAILURE);
    }
    else
    {
        fprintf(f_image,"P5\r") ;                               /*ecriture entete*/
        fprintf(f_image,"%d %d\r255\r", nb_colonnes, nb_lignes) ;

        if( (fwrite((OCTET*)pt_image, sizeof(OCTET), taille_image, f_image))
            != (size_t) taille_image)
        {
            printf("\nErreur de lecture de l'image %s \n", nom_image);
            exit(EXIT_FAILURE);
        }
        fclose(f_image);
    }
}
/*===========================================================================*/

void lire_nb_lignes_colonnes_image_pgm(char nom_image[], int *nb_lignes, int *nb_colonnes)
{
    FILE *f_image;
    int max_grey_val;

    /* cf : l'entete d'une image .pgm : P5                    */
    /*				       nb_colonnes nb_lignes */
    /*    			       max_grey_val          */


    if( (f_image = fopen(nom_image, "rb")) == NULL)
    {
        printf("\nPas d'acces en lecture sur l'image %s \n", nom_image);
        exit(EXIT_FAILURE);
    }
    else
    {
        fscanf(f_image, "P5 ");
        ignorer_commentaires(f_image);
        fscanf(f_image, "%d %d %d%*c", nb_colonnes, nb_lignes, &max_grey_val);
        fclose(f_image);
    }
}
/*===========================================================================*/
/*===========================================================================*/
void lire_image_pgm(char  nom_image[], OCTET *pt_image, int taille_image)
{
    FILE *f_image;
    int  nb_colonnes, nb_lignes, max_grey_val;

    if( (f_image = fopen(nom_image, "rb")) == NULL)
    {
        printf("\nPas d'acces en lecture sur l'image %s \n", nom_image);
        exit(EXIT_FAILURE);
    }
    else
    {
        fscanf(f_image, "P5 ");
        ignorer_commentaires(f_image);
        fscanf(f_image, "%d %d %d%*c",
               &nb_colonnes, &nb_lignes, &max_grey_val); /*lecture entete*/

        if( (fread((OCTET*)pt_image, sizeof(OCTET), taille_image, f_image))
            !=  (size_t) taille_image)
        {
            printf("\nErreur de lecture de l'image %s \n", nom_image);
            exit(EXIT_FAILURE);
        }
        fclose(f_image);
    }
}
/*===========================================================================*/
