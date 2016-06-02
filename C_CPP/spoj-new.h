#ifndef __SPOJ_H__                                                                                                                                       
#define __SPOJ_H__                                                                                                                                       
                                                                                                                                                         
#include <stdio.h>                                                                                                                                       
#include <errno.h>                                                                                                                                       
#include <unistd.h>                                                                                                                                      
#include <string.h>                                                                                                                                      
#include <stdlib.h>                                                                                                                                      
                                                                                                                                                         
/* return values */                                                                                                                                      
#define SPOJ_RV_POSITIVE                0                                                                                                                
#define SPOJ_RV_NEGATIVE                1                                                                                                                
#define SPOJ_RV_IE                      2                                                                                                                

#define SPOJ_RV_AC              0
#define SPOJ_RV_WA              1
#define SPOJ_RV_TLE             2
#define SPOJ_RV_CE              3
#define SPOJ_RV_SE              4
#define SPOJ_RV_RE              5
#define SPOJ_RV_MLE             6
#define SPOJ_RV_DE              7
#define SPOJ_RV_EOF             8
#define SPOJ_RV_IE              9

/* descriptor numbers */                                                                                                                                 
#define SPOJ_P_IN_FD                    0                                                                                                                
#define SPOJ_P_OUT_FD                   4                                                                                                                
#define SPOJ_T_OUT_FD                   3                                                                                                                
#define SPOJ_T_SRC_FD                   5                                                                                                                
#define SPOJ_SCORE_FD                   1                                                                                                                
#define SPOJ_P_INFO_FD                  6                                                                                                                
#define SPOJ_U_INFO_FD                  7                                                                                                                
#define SPOJ_FOR_TESTED 				8                                                                                                                               
#define SPOJ_P_METAIN_FD                  9                                                                                                              
#define SPOJ_P_METAOUT_FD                  10                                                                                                            
                                                                                                                                                         
/* file handles */                                                                                                                                       
FILE                                                                                                                                                     
        *spoj_p_in,     /* problem's input */                                                                                                            
        *spoj_p_out,    /* problem's output */                                                                                                           
        *spoj_t_out,    /* tested program's output */                                                                                                    
        *spoj_t_src,    /* tested program's source */
        *spoj_score,    /* score for the program, for challenge problems */
        *spoj_u_info,   /* additional info - psetter only */
        *spoj_p_info,   /* additional info - psetter and solution's owner */
        *spoj_for_tested;       /* data for the tested program */
FILE
        *spoj_p_metain,
        *spoj_p_metaout;


/* must be called before using other functions or variables */
void spoj_init();

/* spoj "files" are really pipes, this is the recommended way to tell size */
/* the file's data is unaccessible later, so use it only if it doesn't matter */
unsigned spoj_file_length(FILE*);

/* use spoj_assert()! */
/* usual assert() from assert.h will cause "judge's internal error" in the future! */
#define spoj_assert(x) __spoj_assert_fail(__FILE__ ":" __spoj_xstr(__LINE__) ": SPOJ assertion `" #x "' failed.", x)
#define __spoj_str(x) #x
#define __spoj_xstr(x) __spoj_str(x)
void __spoj_assert_fail(char*, int);

#define __WARN_IF_NULL(x) if (errno=0, (x)==NULL)\
        fprintf(stderr, "warning: \"" #x "\" -> %s\n", strerror(errno))

#define __BUF_SIZE 1024*1024

static char spoj_buffer[__BUF_SIZE+1];

static void spoj_finalize();

void spoj_init() {
        __WARN_IF_NULL(spoj_p_in = fdopen(SPOJ_P_IN_FD,"r"));
        __WARN_IF_NULL(spoj_p_out = fdopen(SPOJ_P_OUT_FD,"r"));
        __WARN_IF_NULL(spoj_t_out = fdopen(SPOJ_T_OUT_FD,"r"));
        __WARN_IF_NULL(spoj_t_src = fdopen(SPOJ_T_SRC_FD,"r"));
        __WARN_IF_NULL(spoj_score = fdopen(SPOJ_SCORE_FD,"w"));
        __WARN_IF_NULL(spoj_p_info = fdopen(SPOJ_P_INFO_FD,"w"));
        __WARN_IF_NULL(spoj_u_info = fdopen(SPOJ_U_INFO_FD,"w"));
        __WARN_IF_NULL(spoj_for_tested = fdopen(SPOJ_FOR_TESTED,"w"));

        spoj_p_metain = fdopen(SPOJ_P_METAIN_FD,"r");
        spoj_p_metaout = fdopen(SPOJ_P_METAOUT_FD,"r");
        atexit(spoj_finalize);
}

static void spoj_finalize() {
        fclose(spoj_p_in);
        fclose(spoj_p_out);
        fclose(spoj_t_out);
        fclose(spoj_t_src);
        fclose(spoj_score);
        fclose(spoj_p_info);
        fclose(spoj_u_info);
        fclose(spoj_for_tested);

        if(spoj_p_metain != NULL) fclose(spoj_p_metain);
        if(spoj_p_metaout != NULL) fclose(spoj_p_metaout);
}

unsigned spoj_file_length(FILE *file) {
        int fd = fileno(file);
        unsigned size = 0;
        unsigned temp;
        while ((temp = read(fd, spoj_buffer, __BUF_SIZE))>0)
                size+=temp;
        return size;
}

void __spoj_assert_fail(char *s, int r) {
        if (!r) {
                fprintf(spoj_p_info, "%s\n", s);
                exit(SPOJ_RV_NEGATIVE);
        }
}

#endif
