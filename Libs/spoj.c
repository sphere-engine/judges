#include <spoj.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>


/* file handles */
FILE
    *spoj_p_in,     /* problem's input */
    *spoj_p_out,    /* problem's output */
    *spoj_t_out,    /* tested program's output */
    *spoj_t_src,    /* tested program's source */
    *spoj_score,    /* score for the program, for challenge problems */
    *spoj_u_info,   /* additional info - psetter only */
    *spoj_p_info;   /* additional info - psetter and solution's owner */


#define __WARN_IF_NULL(x) if (errno=0, (x)==NULL)\
    fprintf(stderr, "warning: \"" #x "\" -> %s\n", strerror(errno))


#define __BUF_SIZE 1024*1024

static char buffer[__BUF_SIZE+1];

static void spoj_finalize();

void spoj_init() {
    __WARN_IF_NULL(spoj_p_in = fdopen(SPOJ_P_IN_FD,"r"));
    __WARN_IF_NULL(spoj_p_out = fdopen(SPOJ_P_OUT_FD,"r"));
    __WARN_IF_NULL(spoj_t_out = fdopen(SPOJ_T_OUT_FD,"r"));
    __WARN_IF_NULL(spoj_t_src = fdopen(SPOJ_T_SRC_FD,"r"));
    __WARN_IF_NULL(spoj_score = fdopen(SPOJ_SCORE_FD,"w"));
    __WARN_IF_NULL(spoj_p_info = fdopen(SPOJ_P_INFO_FD,"w"));
    __WARN_IF_NULL(spoj_u_info = fdopen(SPOJ_U_INFO_FD,"w"));
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
}


unsigned spoj_file_length(FILE *file) {
    int fd = fileno(file);
    unsigned size = 0;
    unsigned temp;
    while ((temp = read(fd, buffer, __BUF_SIZE))>0)
            size+=temp;
    return size;
}


void __spoj_assert_fail(char *s, int r) {
    if (!r) {
            fprintf(spoj_p_info, "%s\n", s);
            exit(SPOJ_RV_NEGATIVE);
    }
}
