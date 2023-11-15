#include <stdio.h>
#include <tgmath.h>
#include <queue>
#include <QString>
#include "image_ppm.h"
#include "qdebug.h"

/*<---------- Fonctions ou elements utilitaires ---------->*/

#define NB_CALL_REC_LIMIT 50000

struct Element {
    int lab[3];
    int i;
    int j;
    int k;
    float dist;

    Element() {
        this->lab[0] = 0;
        this->lab[1] = 0;
        this->lab[2] = 0;

        this->i = 0;
        this->j = 0;
        this->k = 0;
        this->dist = 0;
    }

    Element(int l, int a, int b, int i, int j, int k, float dist) {
        this->lab[0] = l;
        this->lab[1] = a;
        this->lab[2] = b;

        this->i = i;
        this->j = j;
        this->k = k;
        this->dist = dist;
    }

    void afficher() {
        printf("i = %d ; j = %d\n", i, j);
    }
};

void add_update_cluster(CLUSTER C, Element E) {
    C->pixels = (unsigned int*) realloc( (void*) C->pixels,  sizeof(unsigned int)*(C->nb_pixels+2));
    C->pixels[C->nb_pixels++] = E.i;
    C->pixels[C->nb_pixels++] = E.j;
    C->update();
}

void tri_bulle_distances(Element* tab, int size) {
    for (int i = 0; i < size - 1; i++)
        for (int j = 0; j < size - i - 1; j++)
            if (tab[j].dist > tab[j+1].dist) {
                Element temp = tab[j];
                tab[j] = tab[j + 1];
                tab[j + 1] = temp;
            }
}

float calcul_distance(int rgb_j[3], int rgb_k[3], int i_j, int j_j, int i_k, int j_k, float m, float s) {
    float dist_rgb = sqrt(((rgb_k[0] - rgb_j[0]) * (rgb_k[0] - rgb_j[0]))
                          + ((rgb_k[1] - rgb_j[1]) * (rgb_k[1] - rgb_j[1]))
                          + ((rgb_k[2] - rgb_j[2]) * (rgb_k[2] - rgb_j[2])));
    float dist_coord = sqrt(((i_k - i_j) * (i_k - i_j))
                            + ((j_k - j_j) * (j_k - j_j)));
    return sqrt( (dist_coord * dist_coord)/s + (dist_rgb * dist_rgb)/m );
}

double compute_PSNR(char* cNomImgLue1, char* cNomImgLue2) {
    int nH1, nW1, nH2, nW2;
    lire_nb_lignes_colonnes_image_ppm(cNomImgLue1, &nH1, &nW1);
    lire_nb_lignes_colonnes_image_ppm(cNomImgLue2, &nH2, &nW2);

    int nTaille3 = nH1 * nW1 * 3;
    OCTET *ImgIn1, *ImgIn2;
    allocation_tableau(ImgIn1, OCTET, nTaille3);
    lire_image_ppm(cNomImgLue1, ImgIn1, nH1 * nW1);
    allocation_tableau(ImgIn2, OCTET, nTaille3);
    lire_image_ppm(cNomImgLue2, ImgIn2, nH2 * nW2);

    double EQM = 0.;

    for( int i = 0; i < nTaille3; i++)
        EQM += pow(ImgIn1[i]- ImgIn2[i], 2.);

    EQM /= (double) nTaille3;

    double PSNR = 10.*log10(pow(255., 2.)/EQM);

    free(ImgIn1);
    free(ImgIn2);

    return PSNR;
}

void compute_contours(char* cNomImgLue, char* cNomImgEcrite) {
    int nH, nW, nTaille;

    OCTET *ImgIn, *ImgOut;

    lire_nb_lignes_colonnes_image_ppm(cNomImgLue, &nH, &nW);
    nTaille = nH * nW;

    allocation_tableau(ImgIn, OCTET, nTaille*3);
    lire_image_ppm(cNomImgLue, ImgIn, nH * nW);
    allocation_tableau(ImgOut, OCTET, nTaille*3);

    for (int i=0; i < nH; i++)
        for (int j=0; j < nW; j++)
        {
            bool diff_found = false;
            OCTET cur_R = ImgIn[(i*nW+j)*3];
            OCTET cur_G = ImgIn[(i*nW+j)*3+1];
            OCTET cur_B = ImgIn[(i*nW+j)*3+2];

            for( int k = -1; k <= 1; k++ )
                for( int l = -1; l <= 1; l++ )
                    if( 0 <= i+k && i+k < nH && 0 <= j+l && j+l < nW )
                    {
                        if( cur_R != ImgIn[((i+k)*nW+(j+l))*3  ]
                            || cur_G != ImgIn[((i+k)*nW+(j+l))*3+1]
                            || cur_B != ImgIn[((i+k)*nW+(j+l))*3+2] )
                            diff_found = true;
                    }
            if( diff_found )
            {
                ImgOut[(i*nW+j)*3]   = ( 255 - cur_R );
                ImgOut[(i*nW+j)*3+1] = ( 255 - cur_G );
                ImgOut[(i*nW+j)*3+2] = ( 255 - cur_B );
            }
            else
            {
                ImgOut[(i*nW+j)*3]   = cur_R;
                ImgOut[(i*nW+j)*3+1] = cur_G;
                ImgOut[(i*nW+j)*3+2] = cur_B;
            }
        }



    ecrire_image_ppm(cNomImgEcrite, ImgOut,  nH, nW);
    free(ImgIn);
    free(ImgOut);
}

void draw_rec( OCTET* ImgOut, OCTET* ImgCtr, OCTET* ImgMark, unsigned short nH, unsigned short nW, unsigned short i, unsigned short j, OCTET R, OCTET G, OCTET B, unsigned short nb_call_rec ) {
    if( nb_call_rec >= NB_CALL_REC_LIMIT )
        return;

    int idx = i*nW+j;

    if( !ImgMark[idx] || nb_call_rec == 1 )
    {
        ImgOut[idx*3  ] = R;
        ImgOut[idx*3+1] = G;
        ImgOut[idx*3+2] = B;

        ImgMark[idx] = 255;

        if( ImgCtr[idx] ) // on est sur un contour, on s'arrête
            return;

        if( i > 0 )
        {
            draw_rec( ImgOut, ImgCtr, ImgMark, nH, nW, i-1, j, R, G, B, nb_call_rec+1 );

            if( j > 0 )
                draw_rec( ImgOut, ImgCtr, ImgMark, nH, nW, i-1, j-1, R, G, B, nb_call_rec+1 );

            if( j < nW-1 )
                draw_rec( ImgOut, ImgCtr, ImgMark, nH, nW, i-1, j+1, R, G, B, nb_call_rec+1 );
        }

        if( j > 0 )
            draw_rec( ImgOut, ImgCtr, ImgMark, nH, nW, i, j-1, R, G, B, nb_call_rec+1 );

        if( j < nW-1 )
            draw_rec( ImgOut, ImgCtr, ImgMark, nH, nW, i, j+1, R, G, B, nb_call_rec+1 );

        if( i < nH-1 )
        {
            draw_rec( ImgOut, ImgCtr, ImgMark, nH, nW, i+1, j, R, G, B, nb_call_rec+1 );

            if( j > 0 )
                draw_rec( ImgOut, ImgCtr, ImgMark, nH, nW, i+1, j-1, R, G, B, nb_call_rec+1 );

            if( j < nW-1 )
                draw_rec( ImgOut, ImgCtr, ImgMark, nH, nW, i+1, j+1, R, G, B, nb_call_rec+1 );
        }
    }

}

int fill_in_blank( OCTET* ImgOut, OCTET* ImgMark, OCTET* ImgMarkTmp, int nH, int nW ) {
    int change_happened = 0;

    for( int i = 0; i < nH; i++ )
        for( int j = 0; j < nW; j++ )
            if( ImgMark[(i*nW+j)] == 0 )
            {
                int index = (i*nW+j)*3;

                int col_occur[8];
                OCTET R[8], G[8], B[8];

                OCTET cur_R = ImgOut[index  ];
                OCTET cur_G = ImgOut[index+1];
                OCTET cur_B = ImgOut[index+2];

                int nb_elem = 0;

                for( int k = -1; k <= 1; k++ )
                    for( int l = -1; l <= 1; l++ )
                        if( 0 <= i+k && i+k < nH && 0 <= j+l && j+l < nW && ImgMark[(i+k)*nW+(j+l)] )
                        {
                            bool col_found = false;
                            OCTET cur_R = ImgOut[((i+k)*nW+(j+l))*3];
                            OCTET cur_G = ImgOut[((i+k)*nW+(j+l))*3+1];
                            OCTET cur_B = ImgOut[((i+k)*nW+(j+l))*3+2];

                            for( int m = 0; m < nb_elem; m++ )
                                if( R[m] == cur_R && G[m] == cur_G && B[m] == cur_B )
                                {
                                    col_occur[m]++;
                                    col_found = true;
                                }

                            if( !col_found )
                            {
                                R[nb_elem] = cur_R;
                                G[nb_elem] = cur_G;
                                B[nb_elem] = cur_B;
                                col_occur[nb_elem] = 1;
                                nb_elem++;
                            }
                        }

                if( nb_elem == 0 )
                    continue;

                int best_col_idx = 0;

                for( int m = 1; m < nb_elem; m++ )
                    if( col_occur[m] > col_occur[best_col_idx] )
                        best_col_idx = m;

                ImgOut[index  ] = R[best_col_idx];
                ImgOut[index+1] = G[best_col_idx];
                ImgOut[index+2] = B[best_col_idx];

                change_happened = 1;
                ImgMarkTmp[(i*nW+j)] = 255;
            }

    return change_happened;
}

int bin2clust( FILE* f, unsigned long& i, unsigned long& j, OCTET& R, OCTET& G, OCTET& B )  {
    OCTET log;
    fscanf(f, "%c", &log );

    int log_i = (log>>4)&0xf;
    int log_j = log&0xf;

    static int mm = 0;

    i = j = 0;

    unsigned int coeff;

    for( int k = 0, coeff = 0x1; k < log_i; k++, coeff *= 0x100 )
    {
        OCTET i_char;
        fscanf(f, "%c", &i_char );
        i += i_char*coeff;
    }

    for( int k = 0, coeff = 0x1; k < log_j; k++, coeff *= 0x100 )
    {
        OCTET j_char;
        fscanf(f, "%c", &j_char );
        j += j_char*coeff;
    }

    OCTET Rc, Gc, Bc;

    int rv = fscanf( f, "%c%c%c", &Rc, &Gc, &Bc );

    R = Rc;
    G = Gc;
    B = Bc;

    return rv;
}

void clust2bin( FILE* f, const unsigned long i, const unsigned long j, const OCTET R, const OCTET G, const OCTET B ) {
    // Calcul du nombre d'octets pris par chaque indice
    // Allocation du string selon ces valeurs
    OCTET log_i = (i>255)?(2):(1);
    OCTET log_j = (j>255)?(2):(1);
    OCTET log = ((log_i<<4)&0xf0) + (log_j&0xf);

    fprintf(f, "%c", log );

    // Déclaration du masque
    unsigned long mask;

    // Conversion de i en str
    for( int k = 0, mask = 0xff; k < log_i; k++, mask *= 0x100 )
        fprintf(f, "%c", (OCTET) (((i&mask)>>(k*8))&0xff) );

    // Conversion de j en str
    for( int k = 0, mask = 0xff; k < log_j; k++, mask *= 0x100 )
        fprintf(f, "%c", (OCTET) (((j&mask)>>(k*8))&0xff) );

    fprintf(f, "%c%c%c", R, G, B );
}

/*<---------- Algorithmes de compression  ---------->*/

void compress_palette_256( OCTET* ImgIn, OCTET* ImgOut, OCTET* ImgPalette, int nH, int nW, CLUSTER* C, int K ) {
    int iOut, jOut;
    OCTET R, G, B;

    for ( int k = 0; k < K; k++ )
    {
        C[k]->calcul_couleur_moyenne(ImgIn, nH, nW, R, G, B);

        ImgPalette[3 * k] = R;
        ImgPalette[3 * k + 1] = G;
        ImgPalette[3 * k + 2] = B;

        for (int p = 0; p < C[k]->nb_pixels; p += 2)
        {
            iOut = C[k]->pixels[p];
            jOut = C[k]->pixels[p + 1];

            ImgOut[ iOut * nW + jOut ] = k;
        }
    }
}

// Compresse une image segmentée en maximum 65536 superpixels à l'aide d'une palette de couleur (moins efficace que la précédente)
void compress_palette_65536( OCTET* ImgIn, OCTET* ImgOut1, OCTET* ImgOut2, OCTET* ImgPalette, int nH, int nW, CLUSTER* C, int K ) {
    int iOut, jOut;
    OCTET R, G, B;

    for ( int k = 0; k < K; k++ )
    {
        C[k]->calcul_couleur_moyenne(ImgIn, nH, nW, R, G, B);

        ImgPalette[3 * k] = R;
        ImgPalette[3 * k + 1] = G;
        ImgPalette[3 * k + 2] = B;

        for (int p = 0; p < C[k]->nb_pixels; p += 2)
        {
            iOut = C[k]->pixels[p];
            jOut = C[k]->pixels[p + 1];

            ImgOut1[ iOut * nW + jOut ] = k/256;
            ImgOut2[ iOut * nW + jOut ] = k%256;
        }
    }
}

void compress_txt( OCTET* ImgOut, char* TxtStr, int nH, int nW, CLUSTER* C, int K ) {
    /********************* CREATION DE L'IMAGE BINAIRE DES CONTOURS *********************/

    int size_str = ceil( (float)(nW * nH)/8. );
    OCTET* str = (OCTET*) malloc( sizeof( char ) * size_str );
    memset( str, 0, size_str );
    /*
    int* min_j_in_line = (int*) malloc( sizeof( int ) * nH );
    int* max_j_in_line = (int*) malloc( sizeof( int ) * nH );
    int* min_i_in_column = (int*) malloc( sizeof( int ) * nW );
    int* max_i_in_column = (int*) malloc( sizeof( int ) * nW );

    int k_min_i, k_max_i, k_min_j, k_max_j;
    k_min_i = k_min_j = 0;
    k_max_i = nH-1; k_max_j = nW-1;

    for( int k = 0; k < K; k++ )
    {
         for( int i = k_min_i; i <= k_max_i; i++ )
         {
            min_j_in_line[i] = nW+1;
            max_j_in_line[i] = -1;
         }

         for( int j = k_min_j; j <= k_max_j; j++ )
         {
            min_i_in_column[j] = nW+1;
            max_i_in_column[j] = -1;
         }

         for( int l = 0; l < C[k]->nb_pixels; l += 2 )
         {
            int i = C[k]->pixels[l];
            int j = C[k]->pixels[l+1];

            if( i < k_min_i ) k_min_i = i;
            else if( i > k_max_i ) k_max_i = i;

            if( j < k_min_j ) k_min_j = j;
            else if( j > k_max_j ) k_max_j = j;

            if( i < min_i_in_column[j] ) min_i_in_column[j] = i;
            else if( i > max_i_in_column[j] ) max_i_in_column[j] = i;

            if( j < min_j_in_line[i] ) min_j_in_line[i] = j;
            else if( j > max_j_in_line[i] ) max_j_in_line[i] = j;
         }

         for( int i = k_min_i; i <= k_max_i; i++ )
         {
            int idx = i*nW+min_j_in_line[i];
            str[idx/8] |= 1<<(idx%8);

            idx = i*nW+max_j_in_line[i];
            str[idx/8] |= 1<<(idx%8);
         }

         for( int j = k_min_j; j <= k_max_j; j++ )
         {
            int idx = min_i_in_column[j]*nW+j;
            str[idx/8] |= 1<<(idx%8);

            idx = max_i_in_column[j]*nW+j;
            str[idx/8] |= 1<<(idx%8);
         }
    }

    free(max_j_in_line);
    free(min_j_in_line);
    free(max_i_in_column);
    free(min_i_in_column);
    */

    for (int i=0; i < nH; i++)
        for (int j=0; j < nW; j++)
        {
            int diff_found = 0;
            int idx = i*nW+j;
            OCTET cur_R = ImgOut[idx*3];
            OCTET cur_G = ImgOut[idx*3+1];
            OCTET cur_B = ImgOut[idx*3+2];

            for( int k = -1; k <= 1; k++ )
                for( int l = -1; l <= 1; l++ )
                    if( 0 <= i+k && i+k < nH && 0 <= j+l && j+l < nW )
                    {
                        if( cur_R != ImgOut[((i+k)*nW+(j+l))*3  ]
                            || cur_G != ImgOut[((i+k)*nW+(j+l))*3+1]
                            || cur_B != ImgOut[((i+k)*nW+(j+l))*3+2] )
                            diff_found = 1;
                    }

            str[idx/8] += diff_found<<(idx%8);
        }

    /*************************** CREATION DU FICHIER DE SORTIE ***************************/

    FILE* f = fopen( TxtStr, "w" );
    if( !f )
    {
        printf("Could not open/create %s !\n", TxtStr );
        return;
    }

    fprintf( f, "%d %d ", nH, nW );

    for( int i = 0; i < size_str; i++ )
        fprintf(f, "%c", str[i]);

    for( int k = 0; k < K; k++ )
    {
        int idx = C[k]->i*nW + C[k]->j;
        //fprintf( f, "U%dU%dU%c%c%c", C[k]->i, C[k]->j, ImgOut[idx*3], ImgOut[idx*3+1], ImgOut[idx*3+2] );

        clust2bin(f,  C[k]->i, C[k]->j, ImgOut[idx*3], ImgOut[idx*3+1], ImgOut[idx*3+2] );
    }

    fclose(f);
    free(str);
}

/*<---------- Algorithmes de decompression  ---------->*/

void decompress_palette_256( OCTET* ImgIn, OCTET* ImgOut, OCTET* ImgPalette, int nH, int nW) {
    int nTaille = nH * nW;

    OCTET R, G, B;

    for (int i = 0; i < nTaille; i++) {
        R = ImgPalette[ 3 * ImgIn[i] ];
        G = ImgPalette[ 3 * ImgIn[i] + 1 ];
        B = ImgPalette[ 3 * ImgIn[i] + 2 ];

        ImgOut[ 3 * i ] = R;
        ImgOut[ 3 * i + 1 ] = G;
        ImgOut[ 3 * i + 2 ] = B;
    }
}

void decompress_256(char* NomImageLue, char* NomPalette, char* NomImageEcrite) {
    int nH, nW, nTaille, nH_pal, nW_pal, nTaille_pal;
    OCTET *ImgIn, *ImgPalette, *ImgOut;

    lire_nb_lignes_colonnes_image_pgm(NomImageLue, &nH, &nW);
    nTaille = nH * nW;
    lire_nb_lignes_colonnes_image_ppm(NomPalette, &nH_pal, &nW_pal);
    nTaille_pal = nH_pal * nW_pal;

    allocation_tableau(ImgIn, OCTET, nTaille);
    allocation_tableau(ImgPalette, OCTET, nTaille_pal * 3);
    allocation_tableau(ImgOut, OCTET, nTaille * 3);

    lire_image_pgm(NomImageLue, ImgIn, nTaille);
    lire_image_ppm(NomPalette, ImgPalette, nTaille_pal);

    decompress_palette_256(ImgIn, ImgOut, ImgPalette, nH, nW);

    ecrire_image_ppm(NomImageEcrite, ImgOut, nH, nW);

    free(ImgIn);
    free(ImgOut);
    free(ImgPalette);
}

void decompress_palette_65536( OCTET* ImgIn1, OCTET* ImgIn2, OCTET* ImgOut, OCTET* ImgPalette, int nH, int nW) {
    int nTaille = nH * nW;

    OCTET R, G, B;
    int indiceCouleur;

    for (int i = 0; i < nTaille; i++) {
        indiceCouleur = ImgIn1[i] * 256 + ImgIn2[i];

        R = ImgPalette[ 3 * indiceCouleur ];
        G = ImgPalette[ 3 * indiceCouleur + 1];
        B = ImgPalette[ 3 * indiceCouleur + 2];

        ImgOut[ 3 * i ] = R;
        ImgOut[ 3 * i + 1 ] = G;
        ImgOut[ 3 * i + 2 ] = B;
    }
}

void decompress_65536(char* NomImageLue1, char* NomImageLue2, char* NomPalette, char* NomImageEcrite) {
    int nH, nW, nTaille, nH_pal, nW_pal, nTaille_pal;
    OCTET *ImgIn1, *ImgIn2, *ImgPalette, *ImgOut;

    lire_nb_lignes_colonnes_image_pgm(NomImageLue1, &nH, &nW);
    nTaille = nH * nW;
    lire_nb_lignes_colonnes_image_ppm(NomPalette, &nH_pal, &nW_pal);
    nTaille_pal = nH_pal * nW_pal;

    allocation_tableau(ImgIn1, OCTET, nTaille);
    allocation_tableau(ImgIn2, OCTET, nTaille);
    allocation_tableau(ImgPalette, OCTET, nTaille_pal * 3);
    allocation_tableau(ImgOut, OCTET, nTaille * 3);

    lire_image_pgm(NomImageLue1, ImgIn1, nTaille);
    lire_image_pgm(NomImageLue2, ImgIn2, nTaille);
    lire_image_ppm(NomPalette, ImgPalette, nTaille_pal);

    decompress_palette_65536(ImgIn1, ImgIn2, ImgOut, ImgPalette, nH, nW);

    ecrire_image_ppm(NomImageEcrite, ImgOut, nH, nW);

    free(ImgIn1);
    free(ImgIn2);
    free(ImgOut);
    free(ImgPalette);
}

void decompress_txt(char* NomFichierTxt, char* NomImgEcrite) {
    FILE* f = fopen( NomFichierTxt, "r" );
    if( !f )
    {
        printf("Could not open %s !\n", NomFichierTxt);
    }

    int nW, nH, nTaille, K, keep_filling;
    OCTET* str,*ImgOut, *ImgCtr, *ImgMark, *ImgMarkTmp;

    fscanf(f, "%d %d ", &nH, &nW );

    nTaille = nW * nH;

    int size_str = ceil( (float)(nTaille)/8. );
    str = (OCTET*) malloc( sizeof( OCTET ) * size_str );
    memset( str, 0, size_str );

    for( int i = 0; i < size_str; i++ )
        fscanf(f, "%c", &(str[i]) );

    allocation_tableau( ImgOut, OCTET, nTaille*3 );
    allocation_tableau( ImgCtr, OCTET, size_str*8 );
    allocation_tableau( ImgMark, OCTET, nTaille );
    allocation_tableau( ImgMarkTmp, OCTET, nTaille );

    for (int i=0; i < nH; i++)
        for (int j=0; j < nW; j++)
        {
            int idx = i*nW+j;

            if( (str[idx/8])&(1<<(idx%8)) )
                ImgCtr[i*nW+j] = 255;
            else
                ImgCtr[i*nW+j] = 0;
        }

    long unsigned i, j;
    OCTET R, G, B;

    while( bin2clust(f, i, j, R, G, B ) != EOF )
    {
        draw_rec( ImgOut, ImgCtr, ImgMark, nH, nW, i, j, R, G, B, 1 );
    }

    for( int i = 0; i < nTaille; i++ )
        ImgMarkTmp[i] = ImgMark[i];

    while( fill_in_blank( ImgOut, ImgMark, ImgMarkTmp, nH, nW ) )
    {
        for( int i = 0; i < nTaille; i++ )
            ImgMark[i] = ImgMarkTmp[i];
    }

    ecrire_image_ppm(NomImgEcrite, ImgOut, nH, nW);

    free(ImgCtr);
    free(ImgOut);
    fclose(f);
}

/*<---------- Algorithmes de segmentation  ---------->*/

void compute_SLIC(char* cNomImgLue, char* cNomImgEcrite, int KX, int KY, int Seuil, float m, QString compress) {
    int nH, nW, nTaille, old_E, E, S;

    /********************* LECTURE ET CREATION DES IMAGES *********************/

    OCTET *ImgIn, *ImgOut;
    float* ImgLAB;

    lire_nb_lignes_colonnes_image_ppm(cNomImgLue, &nH, &nW);
    nTaille = nH * nW;

    allocation_tableau(ImgIn, OCTET, nTaille*3);
    lire_image_ppm(cNomImgLue, ImgIn, nH * nW);

    allocation_tableau(ImgOut, OCTET, nTaille*3);
    allocation_tableau(ImgLAB, float, nTaille*3);

    for( int i = 0; i < nTaille*3; i += 3 )
        RGB2LAB( ImgIn[i], ImgIn[i+1], ImgIn[i+2], ImgLAB[i], ImgLAB[i+1], ImgLAB[i+2] );

    /********************* ALGORITHME SLIC *********************/

    const int K = KX * KY;
    S = ceil( sqrt( (float)nTaille/(float)K  ) );

    // Initialisation des clusters et de leur centre

    CLUSTER* C = (CLUSTER*) malloc( sizeof( CLUSTER ) * K );
    int idx = 0;
    for( int i = 1; i <= KX; i++ )
        for( int j = 1; j <= KY; j++ )
        {
            C[idx] = cluster::new_cluster( (i*nH)/(KX+1), (j*nW)/(KY+1) );
            C[idx]->perturb( ImgLAB, nW, nH, 3 );
            idx++;
        }

    // Boucle principale

    do
    {
        // Ré-initialisation des pixels de chaque cluster

        for( int k = 0; k < K; k++ )
        {
            free( C[k]->pixels );
            C[k]->pixels = NULL;
            C[k]->nb_pixels = 0;
        }

        // Affetation des pixels dans le meilleur cluster dans un voisinnage de carré de 2S*2S

        for( int i = 0; i < nH; i++ )
            for( int j = 0; j < nW; j++ )
            {
                int new_clust_idx = -1;
                float dist = 1e30;

                for( int k = 0; k < K; k++ )
                {
                    if( C[k]->i-(2*S) <= i && i <= C[k]->i+(2*S)
                        && C[k]->j-(2*S) <= j && j <= C[k]->j+(2*S) )
                    {
                        float cur_dist = C[k]->compare( ImgLAB, i, j, nW, m, S );

                        if( dist > cur_dist )
                        {
                            dist = cur_dist;
                            new_clust_idx = k;
                        }
                    }
                }

                if( new_clust_idx == -1 )
                    printf("Should not happen !\n");
                else
                {
                    C[new_clust_idx]->pixels = (unsigned int*) realloc( (void*) C[new_clust_idx]->pixels,  sizeof(unsigned int)*(C[new_clust_idx]->nb_pixels+2) );
                    C[new_clust_idx]->pixels[C[new_clust_idx]->nb_pixels++] = i;
                    C[new_clust_idx]->pixels[C[new_clust_idx]->nb_pixels++] = j;
                }
            }

        // Mise à jour des cluster et calcul de l'erreur résiduel

        old_E = E;
        E = 0;

        for( int k = 0; k < K; k++ )
            E += C[k]->update();

        printf("%d\n", E );
    }
    while( abs(old_E - E )> Seuil );


    /*********************  ECRITURE ET LIBAERATION DES DONNEES *********************/

    get_final_image( ImgIn, ImgOut, nH, nW, C, K );

    ecrire_image_ppm(cNomImgEcrite, ImgOut, nH, nW);

    if (compress == "256") {
        OCTET* final_img;
        OCTET* palette;
        allocation_tableau(final_img, OCTET, nTaille);
        allocation_tableau(palette, OCTET, KX*KY);
        compress_palette_256(ImgOut, final_img, palette, nH, nW, C, K);
    }
    if (compress == "65536") {
        OCTET* final_img1;
        OCTET* final_img2;
        OCTET* palette;
        allocation_tableau(final_img1, OCTET, nTaille);
        allocation_tableau(final_img2, OCTET, nTaille);
        allocation_tableau(palette, OCTET, KX*KY);
        compress_palette_65536(ImgOut, final_img1, final_img2, palette, nH, nW, C, K);
    }
    if (compress == "txt")
        compress_txt(ImgOut, (char*) "comp_res.txt", nH, nW, C, K);

    free(ImgIn);
    free(ImgOut);
    free(ImgLAB);

    for( int i = 0; i < K; i++ )
    {
        free(C[i]->pixels);
        free(C[i]);
    }
    free(C);
}

void compute_SNIC(char* cNomImgLue, char* cNomImgEcrite, int KX, int KY, float m, QString compress) {
    int nH, nW, nTaille, S;

    /********************* LECTURE ET CREATION DES IMAGES *********************/

    OCTET *ImgIn, *ImgOut;
    float* ImgLAB;
    int *ImgMap;

    lire_nb_lignes_colonnes_image_ppm(cNomImgLue, &nH, &nW);
    nTaille = nH * nW;

    allocation_tableau(ImgIn, OCTET, nTaille*3);
    lire_image_ppm(cNomImgLue, ImgIn, nH * nW);

    allocation_tableau(ImgOut, OCTET, nTaille*3);
    allocation_tableau(ImgMap, int, nTaille);
    allocation_tableau(ImgLAB, float, nTaille*3);

    for( int i = 0; i < nTaille*3; i += 3 )
        RGB2LAB( ImgIn[i], ImgIn[i+1], ImgIn[i+2], ImgLAB[i], ImgLAB[i+1], ImgLAB[i+2] );

    /********************* ALGORITHME SLIC *********************/

    const int K = KX * KY;
    S = ceil( sqrt( (float)nTaille/(float)K  ) );

    // Initialisation des clusters et de leur centre

    CLUSTER* C = (CLUSTER*) malloc( sizeof( CLUSTER ) * K );
    int idx = 0;
    for( int i = 1; i <= KX; i++ )
        for( int j = 1; j <= KY; j++ )
        {
            C[idx] = cluster::new_cluster( (i*nH)/(KX+1), (j*nW)/(KY+1) );
            C[idx]->perturb( ImgLAB, nW, nH, 3 );
            idx++;
        }

    float s = sqrt((float)nTaille/(float)K);

    std::queue<Element> queue;
    for (int k = 0; k < K; k++) {
        Element new_E = Element(ImgLAB[3 * (C[k]->i * nW + C[k]->j)],
                                ImgLAB[3 * (C[k]->i * nW + C[k]->j) + 1],
                                ImgLAB[3 * (C[k]->i * nW + C[k]->j) + 2],
                                C[k]->i, C[k]->j, k + 1, 0.);
        queue.push(new_E);
    }

    int indVoisin;
    int i_voisin, j_voisin;

    int lab_voisin[3];

    for (int i = 0; i < nTaille; i++) {
        ImgMap[i] = 0;
    }

    // Boucle principale

    int nb_voisins;
    Element tab_voisins[8];

    while (!queue.empty()) {
        Element temp_E = queue.front();
        queue.pop();
        if (ImgMap[temp_E.i * nW + temp_E.j] == 0) {
            ImgMap[temp_E.i * nW + temp_E.j] = temp_E.k;
            add_update_cluster(C[temp_E.k - 1], temp_E);
            nb_voisins = 0;
            for (int v_i = -1; v_i <= 1; v_i++) {
                for (int v_j = -1; v_j <= 1; v_j++) {
                    i_voisin = temp_E.i + v_i;
                    j_voisin = temp_E.j + v_j;
                    if (!(i_voisin < 0 || i_voisin >= nH || j_voisin < 0 || j_voisin >= nW)) {
                        indVoisin = 3 * (i_voisin * nW + j_voisin);
                        lab_voisin[0] = ImgLAB[indVoisin];
                        lab_voisin[1] = ImgLAB[indVoisin + 1];
                        lab_voisin[2] = ImgLAB[indVoisin + 2];
                        float distance = calcul_distance(lab_voisin, temp_E.lab, i_voisin, j_voisin, temp_E.i, temp_E.j, m, s);
                        Element new_E2 = Element(ImgLAB[indVoisin], ImgLAB[indVoisin+1], ImgLAB[indVoisin],
                                                 i_voisin, j_voisin, temp_E.k, distance);
                        if (ImgMap[indVoisin/3] == 0) {
                            tab_voisins[nb_voisins] = new_E2;
                            nb_voisins += 1;
                        }
                    }
                }
            }
            tri_bulle_distances(tab_voisins, nb_voisins);
            for (int i = 0; i < nb_voisins; i++)
                queue.push(tab_voisins[i]);
        }
    }

    // /*********************  ECRITURE ET LIBERATION DES DONNEES *********************/

    get_final_image( ImgIn, ImgOut, nH, nW, C, K );

    if (compress == "256") {
        OCTET* final_img;
        OCTET* palette;
        allocation_tableau(final_img, OCTET, nTaille);
        allocation_tableau(palette, OCTET, 3*KX*KY);
        compress_palette_256(ImgOut, final_img, palette, nH, nW, C, K);
        ecrire_image_pgm((char*) "index.pgm", final_img, nH, nW);
        ecrire_image_ppm((char*) "palette.ppm", palette, KX, KY);
    }
    if (compress == "65536") {
        OCTET* final_img1;
        OCTET* final_img2;
        OCTET* palette;
        allocation_tableau(final_img1, OCTET, nTaille);
        allocation_tableau(final_img2, OCTET, nTaille);
        allocation_tableau(palette, OCTET, 3*KX*KY);
        compress_palette_65536(ImgOut, final_img1, final_img2, palette, nH, nW, C, K);
        ecrire_image_pgm((char*) "index.pgm", final_img1, nH, nW);
        ecrire_image_pgm((char*) "indexbis.pgm", final_img2, nH, nW);
        ecrire_image_ppm((char*) "palette.ppm", palette, KX, KY);
    }
    if (compress == "txt")
        compress_txt(ImgOut, (char*) "comp_res.txt", nH, nW, C, K);

    ecrire_image_ppm(cNomImgEcrite, ImgOut, nH, nW);

    free(ImgIn);
    free(ImgOut);
    free(ImgLAB);

    for( int i = 0; i < K; i++ )
        free(C[i]);
    free(C);
}
