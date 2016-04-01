/*
* CSCI3280 Introduction to Multimedia Systems
*
* --- Declaration ---
*
* I declare that the assignment here submitted is original except for source
* material explicitly acknowledged. I also acknowledge that I am aware of
* University policy and regulations on honesty in academic work, and of the
* disciplinary guidelines and procedures applicable to breaches of such policy
* and regulations, as contained in the website
* http://www.cuhk.edu.hk/policy/academichonesty/
*
* Assignment 2
* Name: Yung Man Lee
* Student ID : 1155032377
* Email Addr : mlyung3@cse.cuhk.edu.hk
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

#define CODE_SIZE  12
#define TRUE 1
#define FALSE 0
#define MAX_DICT_SIZE (1<<CODE_SIZE)
#define CODE_EOF (MAX_DICT_SIZE - 1)

/* function prototypes */
unsigned int read_code(FILE*, unsigned int);
void write_code(FILE*, unsigned int, unsigned int);
void writefileheader(FILE *,char**,int);
void readfileheader(FILE *,char**,int *);
void compress(FILE*, FILE*);
void decompress(FILE*, FILE*);


/* Variables */
struct node {
    int pos;
    struct node *next[256];
    char *content;
};

struct node dictionary_root;
struct node dictionary[MAX_DICT_SIZE];
unsigned int dict_size;

unsigned char read_buffer;

char* concat(char *prefix, char suffix) {
    int old_len = strlen(prefix);
    char *text = (char *)malloc(sizeof(char) * (old_len + 2));

    strcpy(text, prefix);
    text[old_len] = suffix;
    text[old_len + 1] = '\0';

    return text;
}

void add_new_node(struct node *parent, char suffix, int is_set_content) {
    int i;
    struct node *new_node = (dictionary + dict_size);

    for(i=0; i<256; i++) {
        new_node->next[i] = NULL;
    }

    if (is_set_content) {
        new_node->content = concat(parent->content, (char)suffix);
    }
    new_node->pos = dict_size++;
    parent->next[(int)suffix] = new_node;
}

void init_dict() {
    int i;

    dict_size = 0;
    dictionary_root.content = "";

    for(i=0; i<256; i++) {
        add_new_node(&dictionary_root, (char)i, TRUE);
    }
}

int main(int argc, char **argv)
{
    int printusage = 0;
    int	no_of_file;
    int current_input_file;
    char **input_file_names;
    char *output_file_names;
    FILE *lzw_file;

    init_dict();

    if (argc >= 3)
    {
        if ( strcmp(argv[1],"-c") == 0)
        {
            /* compression */
            lzw_file = fopen(argv[2] ,"wb");

            /* write the file header */
            input_file_names = argv + 3;
            no_of_file = argc - 3;
            writefileheader(lzw_file,input_file_names,no_of_file);

            /* TODO ADD CODES HERE */
            for(current_input_file = 0; current_input_file < no_of_file; current_input_file++) {
                FILE *f = fopen(input_file_names[current_input_file], "rb");
                compress(f, lzw_file);
                fclose(f);
            }

            fclose(lzw_file);
        } else
            if ( strcmp(argv[1],"-d") == 0)
            {
                /* decompress */
                lzw_file = fopen(argv[2] ,"rb");

                /* read the file header */
                no_of_file = 0;
                readfileheader(lzw_file,&output_file_names,&no_of_file);

                /* TODO ADD CODES HERE */
                char *tok = NULL;
                tok = strtok(output_file_names, "\n");
                fprintf(stderr, "file names: %s\n", output_file_names);
                for (current_input_file = 0; current_input_file < no_of_file; current_input_file++) {
                    FILE *f = fopen(output_file_names, "wb");
                    decompress(lzw_file, f);
                    fclose(f);
                    tok = strtok(NULL, "\n");
                }

                fclose(lzw_file);
                free(output_file_names);
            }else
                printusage = 1;
    }else
        printusage = 1;

    if (printusage)
        printf("Usage: %s -<c/d> <lzw filename> <list of files>\n",argv[0]);

    return 0;
}

/*****************************************************************
 *
 * writefileheader() -  write the lzw file header to support multiple files
 *
 ****************************************************************/
void writefileheader(FILE *lzw_file,char** input_file_names,int no_of_files)
{
    int i;
    /* write the file header */
    for ( i = 0 ; i < no_of_files; i++)
    {
        fprintf(lzw_file,"%s\n",input_file_names[i]);

    }
    fputc('\n',lzw_file);

}

/*****************************************************************
 *
 * readfileheader() - read the fileheader from the lzw file
 *
 ****************************************************************/
void readfileheader(FILE *lzw_file,char** output_filenames,int * no_of_files)
{
    int noofchar;
    char c,lastc;

    noofchar = 0;
    lastc = 0;
    *no_of_files=0;
    /* find where is the end of double newline */
    while((c = fgetc(lzw_file)) != EOF)
    {
        noofchar++;
        if (c =='\n')
        {
            if (lastc == c )
            /* found double newline */
                break;
            (*no_of_files)++;
        }
        lastc = c;
    }

    if (c == EOF)
    {
        /* problem .... file may have corrupted*/
        *no_of_files = 0;
        return;

    }
    /* allocate memeory for the filenames */
    *output_filenames = (char *) malloc(sizeof(char)*noofchar);
    /* roll back to start */
    fseek(lzw_file,0,SEEK_SET);

    fread((*output_filenames),1,(size_t)noofchar,lzw_file);

    return;
}

/*****************************************************************
 *
 * read_code() - reads a specific-size code from the code file
 *
 ****************************************************************/
unsigned int read_code(FILE *input, unsigned int code_size)
{
    unsigned int return_value;
    static int input_bit_count = 0;
    static unsigned long input_bit_buffer = 0L;

    /* The code file is treated as an input bit-stream. Each     */
    /*   character read is stored in input_bit_buffer, which     */
    /*   is 32-bit wide.                                         */

    /* input_bit_count stores the no. of bits left in the buffer */

    while (input_bit_count <= 24) {
        input_bit_buffer |= (unsigned long) getc(input) << (24-input_bit_count);
        input_bit_count += 8;
    }

    return_value = input_bit_buffer >> (32 - code_size);
    input_bit_buffer <<= code_size;
    input_bit_count -= code_size;

    return(return_value);
}


/*****************************************************************
 *
 * write_code() - write a code (of specific length) to the file
 *
 ****************************************************************/
void write_code(FILE *output, unsigned int code, unsigned int code_size)
{
    static int output_bit_count = 0;
    static unsigned long output_bit_buffer = 0L;

    /* Each output code is first stored in output_bit_buffer,    */
    /*   which is 32-bit wide. Content in output_bit_buffer is   */
    /*   written to the output file in bytes.                    */

    /* output_bit_count stores the no. of bits left              */

    output_bit_buffer |= (unsigned long) code << (32-code_size-output_bit_count);
    output_bit_count += code_size;

    while (output_bit_count >= 8) {
        putc(output_bit_buffer >> 24, output);
        output_bit_buffer <<= 8;
        output_bit_count -= 8;
    }


    /* only < 8 bits left in the buffer                          */

}

void write_node_content_to_file(struct node *n, FILE *output) {
    int len = strlen(n->content);
    int i;
    for(i=0; i<len; i++) {
        fprintf(output, "%c", n->content[i]);
        fprintf(stderr, "= Write code: %c\n", n->content[i]);
    }
}

/*****************************************************************
 *
 * compress() - compress the source file and output the coded text
 *
 ****************************************************************/
void compress(FILE *input, FILE *output)
{

    /* TODO ADD CODES HERE */
    struct node *last_code = &dictionary_root;
    size_t byte_read = 0;

    while((byte_read = fread(&read_buffer, sizeof(unsigned char), 1, input)) == 1) {
        struct node *next_node = last_code->next[read_buffer];

        fprintf(stderr, "| Read code: %c\n", read_buffer);

        if (next_node == NULL) {
            // Pattern not found
            write_code(output, last_code->pos, CODE_SIZE);
            fprintf(stderr, "= Write code: %d (%c)\n", last_code->pos, last_code->pos);
            add_new_node(last_code, read_buffer, FALSE);
            last_code = dictionary_root.next[read_buffer];
        } else {
            // Pattern found
            last_code = next_node;
        }
    }
    write_code(output, last_code->pos, CODE_SIZE);
    write_code(output, CODE_EOF, CODE_SIZE);
    fprintf(stderr, "= Write code: %d (%c)\n", last_code->pos, last_code->pos);
}


/*****************************************************************
 *
 * decompress() - decompress a compressed file to the orig. file
 *
 ****************************************************************/
void decompress(FILE *input, FILE *output)
{

    /* TODO ADD CODES HERE */
    unsigned int cW = read_code(input, CODE_SIZE) & (MAX_DICT_SIZE - 1);
    unsigned int pW = cW;
    fprintf(stderr, "| Read code: %u (%c)\n", cW, cW);

    if (cW != CODE_EOF) {
        struct node *cW_dict = (dictionary + cW);
        write_node_content_to_file(cW_dict, output);

        while((cW = (read_code(input, CODE_SIZE) & (MAX_DICT_SIZE - 1))) != CODE_EOF) {
            cW_dict = (dictionary + cW);
            fprintf(stderr, "| Read code: %u (%c)\n", cW, cW);

            if (cW < dict_size) {
                // Pattern found
                write_node_content_to_file(cW_dict, output);
                char C = cW_dict->content[0];
                struct node *P = (dictionary + pW);
                add_new_node(P, C, TRUE);
            } else {
                // Pattern not found
                struct node *P = (dictionary + pW);
                char C = P->content[0];
                add_new_node(P, C, TRUE);
                write_node_content_to_file(dictionary + dict_size - 1, output);
            }
            pW = cW;
        }
        fprintf(stderr, "| Read code: %u (%c)\n", cW, cW);
    }
}

