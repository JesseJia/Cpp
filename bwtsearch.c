//
//  main.c
//  bwtsearch
//
//  Created by Zhuang Jia on 25/04/2014.
//  Copyright (c) 2014 Zhuang Jia. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>

int main(int argc, const char * argv[])
{
    if (argc == 5) {
        FILE *fp_bwtfile;
        if((fp_bwtfile = fopen(argv[1], "r")) == NULL) {
            printf("cannot open bwt file\n");
            exit(0);
        }
        int last_char; //index of last character
        fread(&last_char, 1, 4, fp_bwtfile);
        //printf("last=%ld", last_char);
        int ch;
        int array[256] = {0};
        FILE *fp_indexfile;
        if((fp_indexfile = fopen(argv[2], "w+")) == NULL) {
            printf("cannot open index file\n");
            exit(0);
        }
        //open output file
        FILE *fp_output;
        if((fp_output = fopen(argv[4], "w+")) == NULL) {
            printf("cannot open output file\n");
            exit(0);
        }
        bool flag = false;
        
        while (true) {
            //read bwt file 2048 bytes by 2048 bytes and put them into an array
	    int i = 0;
            for (i=0; i<2048; i++) {
                if ((ch=fgetc(fp_bwtfile)) != EOF) {
                    array[ch]++;
                    fprintf(fp_output, "%c", ch);
                }
                else {
                    flag = true;
                    break;
                }
            }
            //wrtie arrays into index file
	    int j = 0;
            for (j=0; j<256; j++) {
                fwrite(&array[j], sizeof(int), 1, fp_indexfile);
            }
            
            if (flag) {
                break;
            }
        }
        
        int chara;
        int index = last_char;
        
        int output_index = -1;
        int temp = 999;
        int temp_array[1000] = {0};
        
        do {
            int b_nb = index/2048;
            int r_nb = index%2048;
            
            fseek(fp_bwtfile, 4+index, SEEK_SET); //find the char
            chara = fgetc(fp_bwtfile);
            //write this char into a file
            temp_array[temp] = chara;
            temp--;
            if (temp < 0) {
		output_index = output_index - 1000;
                fseek(fp_output, output_index--, SEEK_END);
		fprintf(fp_output, "%s", temp_array);
                temp = 999;
            }
            
            //printf("%d ",chara);
            int pre_b_nb = 0;
            int cur_r_nb = 0;
            int matching_nb = 0;
            int less_than_nb = 0;
            //get matching number
            if (b_nb>0) {
                int temp = chara*4+1024*(b_nb-1);
                fseek(fp_indexfile, temp, SEEK_SET);
                fread(&pre_b_nb, 1, 4, fp_indexfile);
            }
            int i;
            for (i=b_nb*2048; i<b_nb*2048+r_nb; i++) {
                fseek(fp_bwtfile, i+4, SEEK_SET);
                unsigned char temp = fgetc(fp_bwtfile);
                if (chara == temp) {
                    cur_r_nb++;
                }
            }
            matching_nb = pre_b_nb + cur_r_nb;
	    int j;
            for (j=0; j<chara; j++) {
                less_than_nb += array[j];
            }
            index = matching_nb + less_than_nb;
        } while (last_char != index);
	if(temp >= 0) {
	    output_index = output_index - (999-temp);
	    fseek(fp_output, output_index, SEEK_END);
	    fprintf(fp_output, "%s", temp_array);
	}
    }
    else {
        
    }

}

