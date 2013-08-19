//This module is responsible for converting the item name into a set of coordinates using a hash functiondepending 
//on the dimensions
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

char result_str[1000]="";
float cartesian[20];
/*
main() {
    stringtocart("hello",2);
}
*/

stringtocart(char *input,int d) {

    int padlen = 0,i,j,k,l,num,m;
    char *newstr = NULL;
    char *nullstr = "0";
    int  count = 0;
    int result = 0x0;
    char hexstr[d+1];
    char word[d+1];
    char word1[d+1];
    int len =0;


    // if the string is null return null
    // if the dimensions are zero return null

    if((input == NULL) || (d <= 0)) {
        printf("No string found or dimensions are invalid");
        return -1;
    }
   newstr = input;

   result_str[0]= '\0';
    //set the result to all zeros
    for(i=0;i<2*d;i++) {
        strcat(result_str,"0000");
    }

    //now calculate the cartesian
    for(i=0;i<strlen(newstr);) {
        k =0;
        l =0;
        num =0;
        hexstr[k]='\0';
        word1[k]='\0';
        for(j =i ; j<i+ d; j++) {
        //    printf("%c",newstr[j]);
            sprintf(word,"%X",newstr[j]);
            strcat(hexstr,word);
            word1[k++] = newstr[j];
        }
        word1[k]='\0';
        len = strlen(word1);
        if(len != d ) {
            for(m=0;m<d - len;m++)
                strcat(hexstr,"0");
        }
        count ++;
        i = i + d;
       /*
        if(count%2 == 0)
            printf(" %s even %d",hexstr,count);
        else
            printf(" %s odd %d",hexstr,count);
        printf("\n");
     */
        heaxlbitchange(hexstr,count);
    }
    cartesian_cord();
    /*
    for(i=0;i<d;i++) {
        printf(" %f ",cartesian[i]);
    }
    */
    return 0;
}

cartesian_cord() {
    int i,j,k,l,m;
    float num=0;

    for(i=0,m=0;i<strlen(result_str);i +=8,m++) {
        num = 0;
        l =7;
        for(j=i;j<i+8;j++,l--) {
            k = result_str[j] == '0' ? 0:1;
            num = num + k*pow(2,l);
            //printf("%c",result_str[j]);
        }
        cartesian[m] = num/256;
    }
}

const char *hexbin[] = { "0000","0001","0010","0011","0100","0101", "0110", "0111", "1000", "1001", "1010", "1011", "1100", "1101", "1110", "1111"};

char* rev(char* str)
{
  int end= strlen(str)-1;
  int start = 0;

  while( start<end )
  {
    str[start] ^= str[end];
    str[end] ^=   str[start];
    str[start]^= str[end];

    ++start;
    --end;
  }

  return str;
}

xorstr(char *binstr) {
    int i,j,k,l;

    for(i=0;i<strlen(binstr);i++) {
        j = result_str[i] == '0' ? 0:1;
        k = binstr[i] == '0' ? 0:1;
        l = j^k;
        result_str[i] = l == 0 ? '0':'1';
    }
    //printf(" %s %s \n",binstr,result_str);
}


heaxlbitchange(char *hexstr,int count) {

    int i,offset;
    char binstr[1000]="";
    char *c;

    for (i=0;i<strlen(hexstr);i++) {
        if (hexstr[i] >= 65 && hexstr[i] <= 71 )
            offset = 55;
        if (hexstr[i] >= 48 && hexstr[i] <= 58 )
            offset = 48;
        strcat(binstr,hexbin[(int)hexstr[i]- offset]);
       // printf(" %c %s",hexstr[i],hexbin[(int)hexstr[i]- offset]);
    }
    if (count%2 == 1)
        //printf("%s\n",rev(binstr));
        xorstr(rev(binstr));
    else
        //printf("%s\n",(binstr));
        xorstr((binstr));
}

