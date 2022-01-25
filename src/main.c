#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define boolean unsigned char
#define true 1
#define false 0
#define Maxrow 100
#define Maxcol 100
typedef char ElType;
typedef struct
{
   ElType contents[Maxrow][Maxcol];
   int rowEff;
   int colEff;
} Matrix;
#define ELMT(M, i, j) (M).contents[(i)][(j)]

//Mentransformasikan puzzle pada file ke bentuk matrix
void loadPuzzle(int *rows, int *cols, int *nword, Matrix *puzzle, Matrix *words){
    FILE *fptr;
    char now,prev;
    int row = *rows, col = *cols, count= *nword;
    char* file;
    char path[] = "../test/";
    char fname[50]="";
    printf("Please enter puzzle file name: ");
    scanf("%s",&fname);
    strcat(path,fname);

    fptr = fopen(path,"r");
    now = fgetc(fptr);
    
    while (now!= EOF){
        if(prev != now){
            if (now == '\n'){
                row++; 
                col = 0;
                *rows = row; 
            }
            else{
                if (now != ' ' ){
                    ELMT(*puzzle,row,col) = now;
                    col++;
                    if (*cols< col)
                        *cols = col;
                }
            }
            prev = now;
            now = fgetc(fptr);
        }
        else{
            int row = 0, col=0;
            now = fgetc(fptr);
            while(now!= EOF){
                if(now != '\n'){
                   ELMT(*words,row,col) = now;
                    col++;
                    now = fgetc(fptr);
                }
                else{
                    ELMT(*words,row,col) = '.';
                    row++;
                    col = 0;
                    *nword = row;
                    now = fgetc(fptr);
                }
            }
            ELMT(*words,row,col) = '.';
        } 
    }
}

//Menghasilkan matrix berukuran Row  x Col dengan isi '_'
void createSolvePuzzle(int rows, int cols, Matrix *puzsol){
    for (int i=0; i<rows; i++){
        for(int j=0; j<cols*2; j+=2){
            ELMT(*puzsol,i,j) = '-';
            ELMT(*puzsol,i,j+1) = ' ';
        }
    }
}

//Pencarian dari kiri ke kanan
boolean searchWtoE(Matrix puzzle, Matrix words, int i, int *row, int *col, int *len, int *count){
    boolean found = false;
    char now;
    int rows = *row, cols = *col;
    int p = 0, q=0,passp=0, passw=0, hitung=0;
    while (p < rows && !found && ELMT(words,i,passw)!='.'){
        while(q< cols && !found && ELMT(words,i,passw)!='.'){
            hitung++;
            if(ELMT(puzzle,p,passp) == ELMT(words,i,passw)){
                passp++;
                passw++;
            }
            else{
                passw = 0;
                q++;
                passp = q;
            }
            now = ELMT(words,i,passw); 
        }
        if (now == '.'){
            *len = passw;
            found = true;
            *row = p;
            *col = q;
        }
        else{
            passw = 0;
            p++;
            q=0;
            passp=0;
        }
    }
    *count = hitung;
    return found;
}

//pencarian dari kanan ke kiri
boolean searchEtoW(Matrix puzzle, Matrix words, int i, int *row, int *col, int *len, int *count){
    boolean found = false;
    char now;
    int rows = *row, cols = *col;
    int p = 0, q= cols-1,passp=cols-1, passw=0, hitung=0;
    while (p < rows && !found && ELMT(words,i,passw)!='.'){
        while(q>=0 && !found && ELMT(words,i,passw)!='.'){
            hitung++;
            if(ELMT(puzzle,p,passp) == ELMT(words,i,passw)){
                passp--;
                passw++;
            }
            else{
                passw = 0;
                q--;
                passp = q;
            }
            now = ELMT(words,i,passw);
        }
        
        if (now == '.'){
            *len = passw;
            found = true;
            *row = p;
            *col = q;
        }
        else{
            passw = 0;
            p++;
            q=cols-1;
            passp=q;
        }
    }
    *count = hitung;
    return found;
}

//pencarian dari atas ke bawah
boolean searchNtoS(Matrix puzzle, Matrix words, int i, int *row, int *col, int *len, int *count){
    boolean found = false;
    char now;
    int rows = *row, cols = *col;
    int p = 0, q=0,passp=0, passw=0,hitung=0;
    while (p < cols && !found && ELMT(words,i,passw)!='.'){
        while(q< rows && !found && ELMT(words,i,passw)!='.'){
            hitung++;
            if(ELMT(puzzle,passp,p) == ELMT(words,i,passw)){
                passp++;
                passw++;
            }
            else{
                passw = 0;
                q++;
                passp = q;
            }
            now = ELMT(words,i,passw);
        }
        
        if (now == '.'){
            *len = passw;
            found = true;
            *row = q;
            *col = p;
        }
        else{
            passw = 0;
            p++;
            q=0;
            passp=0;
        }
    }
    *count=hitung;
    return found;
}

//pencarian dari bawah ke atas
boolean searchStoN(Matrix puzzle, Matrix words, int i, int *row, int *col, int *len, int *count){
    boolean found = false;
    char now;
    int rows = *row, cols = *col;
    int p = 0, q=rows-1,passp=rows-1, passw=0, hitung=0;
    while (p < cols && !found && ELMT(words,i,passw)!='.'){
        while(q>=0 && !found && ELMT(words,i,passw)!='.'){
            hitung++;
            if(ELMT(puzzle,passp,p) == ELMT(words,i,passw)){
                passp--;
                passw++;
            }
            else{
                passw = 0;
                q--;
                passp = q;
            }
            now = ELMT(words,i,passw);
        }
        
        if (now == '.'){
            *len = passw;
            found = true;
            *row = q;
            *col = p;
        }
        else{
            passw = 0;
            p++;
            q=rows-1;
            passp=q;
        }
    }
    *count=hitung;
    return found;
}

//pencarian dari kanan bawah ke kiri atas secara diagonal
boolean searchSEtoNW(Matrix puzzle, Matrix words, int i, int *row, int *col, int *len, int *count){
    boolean found = false;
    char now;
    int rows = *row, cols = *col;
    int p = rows-1, q=cols-1,passh=q,passv=p,passw=0,hitung=0;
    while (p > 0 && !found && ELMT(words,i,passw)!='.'){
        while(q >=0 && !found && ELMT(words,i,passw)!='.'){
            hitung++;
            if(ELMT(puzzle,passv,passh) == ELMT(words,i,passw)){
                passh--;
                passv--;
                passw++;
            }
            else{
                passw = 0;
                q--;
                passh = q;
                passv = p;
            }
            now = ELMT(words,i,passw);
        }
        
        if (now == '.'){
            *len = passw;
            found = true;
            *row = p;
            *col = q;
        }
        else{
            passw = 0;
            p--;
            q=cols-1;
            passv=p;
            passh=q;
        }
    }
    *count=hitung;
    return found;
}

//pencarian dari kiri atas ke kanan bawah secara diagonal
boolean searchNWtoSE(Matrix puzzle, Matrix words, int i, int *row, int *col, int *len, int *count){
    boolean found = false;
    char now;
    int rows = *row, cols = *col;
    int p = 0, q=0,passh=0,passv=0,passw=0,hitung=0;
    while (p < rows && !found && ELMT(words,i,passw)!='.'){
        while(q< cols && !found && ELMT(words,i,passw)!='.'){
            hitung++;
            if(ELMT(puzzle,passv,passh) == ELMT(words,i,passw)){
                passh++;
                passv++;
                passw++;
            }
            else{
                passw = 0;
                q++;
                passh = q;
                passv = p;
            }
            now = ELMT(words,i,passw);
        }
        
        if (now == '.'){
            *len = passw;
            found = true;
            *row = p;
            *col = q;
        }
        else{
            passw = 0;
            p++;
            q=0;
            passv=p;
            passh=q;
        }    
    }
    *count = hitung;
    return found;
}

//pencarian dari kiri bawah ke kanan atas secara diagonal
boolean searchSWtoNE(Matrix puzzle, Matrix words, int i, int *row, int *col, int *len, int *count){
    boolean found = false;
    char now;
    int rows = *row, cols = *col;
    int p = rows-1, q=0,passh=0,passv=p,passw=0,hitung=0;
    while (p >=0 && !found && ELMT(words,i,passw)!='.'){
        while(q< cols && !found && ELMT(words,i,passw)!='.'){
            hitung++;
            if(ELMT(puzzle,passv,passh) == ELMT(words,i,passw)){
                passh++;
                passv--;
                passw++;
            }
            else{
                passw = 0;
                q++;
                passh = q;
                passv = p;
            }
            now = ELMT(words,i,passw);
        }
        
        if (now == '.'){
            *len = passw;
            found = true;
            *row = p;
            *col = q;
        }
        else{
            passw = 0;
            p--;
            q=0;
            passv=p;
            passh=q;
        }
    }
    *count=hitung;
    return found;
}

//pencarian dari kiri atas ke kanan bawah secara diagonal
boolean searchNEtoSW(Matrix puzzle, Matrix words, int i, int *row, int *col, int *len, int *count){
    boolean found = false;
    char now;
    int rows = *row, cols = *col;
    int p = 0, q=cols-1,passh=q,passv=p,passw=0,hitung=0;
    while (p < rows && !found && ELMT(words,i,passw)!='.'){
        while(q>=0 && !found && ELMT(words,i,passw)!='.'){
            hitung++;
            if(ELMT(puzzle,passv,passh) == ELMT(words,i,passw)){
                passh--;
                passv++;
                passw++;
            }
            else{
                passw = 0;
                q--;
                passh = q;
                passv = p;
            }
            now = ELMT(words,i,passw);
        }
        
        if (now == '.'){
            *len = passw;
            found = true;
            *row = p;
            *col = q;
        }
        else{
            passw = 0;
            p++;
            q=cols-1;
            passv=p;
            passh=q;
        }
    }
    *count=hitung;
    return found;
}

//mengisi matrix solusi
void result(Matrix *puzsol, Matrix puzzle, int rows, int cols,int len, int option){
    if(option>=1 && option <=4){
        for (int i=0; i<len; i++){
            ELMT(*puzsol,rows,cols*2) = ELMT(puzzle,rows,cols);
            if (option == 1){
                cols++;
            }else if(option == 2){
                cols--;
            }else if(option == 3){
                rows++;
            }else{
                rows--;
            }
        }
    }
    else{
        for (int i=0; i<len; i++){
            ELMT(*puzsol,rows,cols*2) = ELMT(puzzle,rows,cols);
            if (option == 6){
                cols++;
                rows++;
            }else if(option == 5){
                cols--;
                rows--;
            }else if(option == 7){
                cols++;
                rows--;
            }else{
                cols--;
                rows++;
            }
        }
    }
}

//mencetak solusi
void displayResult(Matrix puzsol, Matrix puzzle,int ori_row,int ori_col){
    for (int i=0; i<ori_row; i++){
        for(int j=0; j<ori_col*2;j++){
            printf("%c",ELMT(puzsol,i,j));
        }
        printf("\n");
    }
}

boolean searchWord(Matrix puzzle, Matrix words, int i, int *row, int *col, int *len, int search_option, int *count){
    boolean found = false;
    if (search_option == 1){found = searchWtoE(puzzle,words,i,row,col,len, count);}
    else if (search_option == 2){found = searchEtoW(puzzle,words,i,row,col,len, count);}
    else if (search_option == 3){found = searchNtoS(puzzle,words,i,row,col,len,count);}
    else if (search_option == 4){found = searchStoN(puzzle,words,i,row,col,len,count);}
    else if (search_option == 5){found = searchSEtoNW(puzzle,words,i,row,col,len,count);}
    else if (search_option == 6){found = searchNWtoSE(puzzle,words,i,row,col,len,count);}
    else if (search_option == 7){found = searchSWtoNE(puzzle,words,i,row,col,len,count);}
    else if (search_option == 8){found = searchNEtoSW(puzzle,words,i,row,col,len,count);}
    return found;
}

void execute(Matrix puzzle, Matrix puzsol, Matrix words, int Row, int Col, int nword, int len, int *count){
    boolean found;
    int rows, cols, sum=0;
    for (int i=0; i<=nword;i++){
        rows = Row; cols = Col;
        createSolvePuzzle(Row, Col, &puzsol);
        int j=1;
        found = false;
        while(j<=9 && !found){
            found = searchWord(puzzle,words,i,&rows,&cols,&len,j,count);
            sum = sum + *count;
            if (found) {
                result(&puzsol,puzzle,rows,cols,len,j);
                displayResult(puzsol,puzzle,Row,Col);
                printf("\n");
            }
            j++;
        }
    }
    *count = sum;
}

int main(){
    Matrix puzzle, puzsol, words;
    int rows=0, cols=0, nword=0, len, Row, Col,count;
    loadPuzzle(&rows, &cols, &nword, &puzzle, &words);
    Row = rows;
    Col = cols;

    clock_t t;
    t = clock();
    execute(puzzle, puzsol,words,Row,Col,nword,len,&count);
    t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC;
    printf("%d times comparison \n", count);
    printf("Execution time: %f seconds", time_taken);
    return 0;
}
