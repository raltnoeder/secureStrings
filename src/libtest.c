/**
 * secureStrings library - some basic test methods
 *
 * libtest version 1.0 (2010-11-11_01)
 */

#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <securestr.h>
#include <securestr_conv.h>

/* size of the test strings */
#define STR_A_SIZE   200
#define STR_B_SIZE   200

/* size of the function string */
#define FUNC_SIZE    32

int  main(int, char*[], char*[]);
void syntax_exit(void);
void test_sstrCpy(sString*, sString*);
void test_sstrAppd(sString*, sString*);
void test_sstrSubstr(sString*, sString*, char*[]);
void test_sstrAppdSubstr(sString*, sString*, char*[]);
void test_sstrCmp(sString*, sString*);
void test_sstrStartsWith(sString*, sString*);
void test_sstrEndsWith(sString*, sString*);
void test_sstrIndexOf(sString*, sString*);
void test_sstrSwap(sString*, sString*);
void chkArgs(int, int);
void dspStr(const char*, sString*);

sstr_rc argCmp(sString*, const char*);

/**
 * secureStrings tests
 */
int main(
    int   argc,
    char* argv[],
    char* envp[]
)
{
    sString*  func;
    sString*  str_a;
    sString*  str_b;
    sString*  versionInfo;
    
    extern const sString* sstrVersion;
    
    versionInfo = sstr_alloc( (size_t) 100 );
    if (versionInfo == NULL)
    {
        fputs("Out of memory\n", stderr);
        exit(1);
    }
    sstrCpyCstr( (const char*) "secureStrings version ", versionInfo,
        (size_t) 22);
    sstr_appd((sString*) sstrVersion, versionInfo);
    sstrAppdCstr( (const char*) "\n", versionInfo, (size_t) 1);

    fputs(versionInfo->chars, stdout);

    if (argc < 2)
    {
        syntax_exit();
    }
    
    func  = sstr_alloc( (size_t) FUNC_SIZE  );
    str_a = sstr_alloc( (size_t) STR_A_SIZE );
    str_b = sstr_alloc( (size_t) STR_B_SIZE );
    if (func == NULL || str_a == NULL || str_b == NULL)
    {
        fputs("Out of memory\n", stderr);
        exit(1);
    }
    
    /* copy first argument into function string */
    sstrCpyCstr(
        (const char*) argv[1],
        func,
        strlen(argv[1])
    );
    if (argc >= 3)
    {
        sstrCpyCstr(
            (const char*) argv[2],
            str_a,
            strlen(argv[2])
        );
    }
    if (argc >= 4)
    {
        sstrCpyCstr(
            (const char*) argv[3],
            str_b,
            strlen(argv[3])
        );
    }
    
    if ( argCmp(str_a, "=null") == SSTR_TRUE )
    {
        sstr_dealloc(str_a);
        str_a = NULL;
    }
    if ( argCmp(str_b, "=null") == SSTR_TRUE )
    {
        sstr_dealloc(str_b);
        str_b = NULL;
    } else
    if (argCmp(str_b, "=string_A") == SSTR_TRUE)
    {
        sstr_dealloc(str_b);
        str_b = str_a;
    }
    
    if ( argCmp(func, "sstrCpy") == SSTR_TRUE )
    {
        chkArgs(argc, 4);
        test_sstrCpy(str_a, str_b);
    } else
    if ( argCmp(func, "sstrSubstr") == SSTR_TRUE )
    {
        chkArgs(argc, 6);
        test_sstrSubstr(str_a, str_b, argv);
    } else
    if ( argCmp(func, "sstrAppd") == SSTR_TRUE )
    {
        chkArgs(argc, 4);
        test_sstrAppd(str_a, str_b);
    } else
    if ( argCmp(func, "sstrAppdSubstr") == SSTR_TRUE )
    {
        chkArgs(argc, 6);
        test_sstrAppdSubstr(str_a, str_b, argv);
    } else
    if ( argCmp(func, "sstrCmp") == SSTR_TRUE )
    {
        chkArgs(argc, 4);
        test_sstrCmp(str_a, str_b);
    } else
    if ( argCmp(func, "sstrStartsWith") == SSTR_TRUE )
    {
        chkArgs(argc, 4);
        test_sstrStartsWith(str_a, str_b);
    } else
    if ( argCmp(func, "sstrEndsWith") == SSTR_TRUE )
    {
        chkArgs(argc, 4);
        test_sstrEndsWith(str_a, str_b);
    } else
    if ( argCmp(func, "sstrIndexOf") == SSTR_TRUE )
    {
        chkArgs(argc, 4);
        test_sstrIndexOf(str_a, str_b);
    } else
    if ( argCmp(func, "sstrSwap") == SSTR_TRUE ){
        chkArgs(argc, 4);
        test_sstrSwap(str_a, str_b);
    } else {
        syntax_exit();
    }
    
    sstr_dealloc(func);

    if (str_a != NULL)
    {
        if (str_a == str_b)
        {
            /* two pointers to the same area of memory
             * deallocate once, then invalidate both pointers
             */
            sstr_dealloc(str_a);
            str_a = NULL;
            str_b = NULL;
        } else {
            sstr_dealloc(str_a);
            str_a = NULL;
        }
    }

    if (str_b != NULL)
    {
        sstr_dealloc(str_b);
    }
    
    return 0;
}

/**
 * show syntax help and exit
 */
void syntax_exit(void)
{
    fputs("Syntax: libtest <function> <arguments...>\n", stderr);
    fputs("  sstrCpy          <string_A> <string_B>\n"
          "  sstrAppd         <string_A> <string_B>\n"
          "  sstrSubstr       <string_A> <string_B> <offset> <length>\n"
          "  sstrAppdSubstr   <string_A> <string_B> <offset> <length>\n"
          "  sstrCmp          <string_A> <string_B>\n"
          "  sstrStartsWith   <string_A> <string_B>\n"
          "  sstrEndsWith     <string_A> <string_B>\n"
          "  sstrIndexOf      <string_A> <string_B>\n"
          "  sstrSwap         <string_A> <string_B>\n", stderr);

    exit(1);
}

void test_sstrCpy(
    sString* str_a,
    sString* str_b
)
{
    sstr_rc rc;
    
    fputs("sstrCpy(string_A, string_B): ", stdout);
    fflush(stdout);
    rc = sstr_cpy(str_a, str_b);
    if (rc == SSTR_PASS)
    {
        fputs("SSTR_PASS\n", stdout);
    } else
    if (rc == SSTR_FAIL)
    {
        fputs("SSTR_FAIL\n", stdout);
    } else {
        fputs("!! INVALID RETURN CODE !!\n", stdout);    
    }
    dspStr("string_A", str_a);
    dspStr("string_B", str_b);
}

void test_sstrSubstr(
    sString* str_a,
    sString* str_b,
    char*    argv[]
)
{
    sstr_rc rc;
    sstr_pos   offset;
    size_t  length;
    
    offset = 0;
    length = 0;
    
    fputs("sstrSubstr(string_A, string_B, offset, length): ", stdout);
    fflush(stdout);
    
    offset = (sstr_pos)  strtoll(argv[4], NULL, 10);
    length = (size_t) strtoll(argv[5], NULL, 10);
    
    rc = sstr_substr(str_a, str_b, offset, length);
    if (rc == SSTR_PASS)
    {
        fputs("SSTR_PASS\n", stdout);
    } else
    if (rc == SSTR_FAIL)
    {
        fputs("SSTR_FAIL\n", stdout);
    } else {
        fputs("!! INVALID RETURN CODE !!\n", stdout);    
    }
    dspStr("string_A", str_a);
    dspStr("string_B", str_b);
}

void test_sstrAppd(
    sString* str_a,
    sString* str_b
)
{
    sstr_rc rc;
    
    fputs("sstrAppd(string_A, string_B): ", stdout);
    fflush(stdout);
    rc = sstr_appd(str_a, str_b);
    if (rc == SSTR_PASS)
    {
        fputs("SSTR_PASS\n", stdout);
    } else
    if (rc == SSTR_FAIL)
    {
        fputs("SSTR_FAIL\n", stdout);
    } else {
        fputs("!! INVALID RETURN CODE !!\n", stdout);    
    }
    dspStr("string_A", str_a);
    dspStr("string_B", str_b);
}

void test_sstrAppdSubstr(
    sString* str_a,
    sString* str_b,
    char*    argv[]
)
{
    sstr_rc rc;
    sstr_pos   offset;
    size_t  length;
    
    offset = 0;
    length = 0;
    
    fputs("sstrAppdSubstr(string_A, string_B, offset, length): ", stdout);
    fflush(stdout);
    
    offset = (sstr_pos)  strtoll(argv[4], NULL, 10);
    length = (size_t) strtoll(argv[5], NULL, 10);
    
    rc = sstr_appdsubstr(str_a, str_b, offset, length);
    if (rc == SSTR_PASS)
    {
        fputs("SSTR_PASS\n", stdout);
    } else
    if (rc == SSTR_FAIL)
    {
        fputs("SSTR_FAIL\n", stdout);
    } else {
        fputs("!! INVALID RETURN CODE !!\n", stdout);    
    }
    dspStr("string_A", str_a);
    dspStr("string_B", str_b);
}

void test_sstrCmp(
    sString* str_a,
    sString* str_b
)
{
    sstr_rc rc;
    
    fputs("sstrCmp(string_A, string_B): ", stdout);
    fflush(stdout);
    rc = sstr_cmp(str_a, str_b);
    if (rc == SSTR_TRUE)
    {
        fputs("SSTR_TRUE\n", stdout);
    } else
    if (rc == SSTR_FALSE)
    {
        fputs("SSTR_FALSE\n", stdout);
    } else
    if (rc == SSTR_FAIL)
    {
        fputs("SSTR_FAIL\n", stdout);
    } else {
        fputs("!! INVALID RETURN CODE !!\n", stdout);    
    }
    dspStr("string_A", str_a);
    dspStr("string_B", str_b);
}

void test_sstrStartsWith(
    sString* str_a,
    sString* str_b
)
{
    sstr_rc rc;
    
    fputs("sstrStartsWith(string_A, string_B): ", stdout);
    fflush(stdout);
    rc = sstr_startswith(str_a, str_b);
    if (rc == SSTR_TRUE)
    {
        fputs("SSTR_TRUE\n", stdout);
    } else
    if (rc == SSTR_FALSE)
    {
        fputs("SSTR_FALSE\n", stdout);
    } else
    if (rc == SSTR_FAIL)
    {
        fputs("SSTR_FAIL\n", stdout);
    } else {
        fputs("!! INVALID RETURN CODE !!\n", stdout);    
    }
    dspStr("string_A", str_a);
    dspStr("string_B", str_b);
}

void test_sstrEndsWith(
    sString* str_a,
    sString* str_b
)
{
    sstr_rc rc;
    
    fputs("sstrEndsWith(string_A, string_B): ", stdout);
    fflush(stdout);
    rc = sstr_endswith(str_a, str_b);
    if (rc == SSTR_TRUE)
    {
        fputs("SSTR_TRUE\n", stdout);
    } else
    if (rc == SSTR_FALSE)
    {
        fputs("SSTR_FALSE\n", stdout);
    } else
    if (rc == SSTR_FAIL)
    {
        fputs("SSTR_FAIL\n", stdout);
    } else {
        fputs("!! INVALID RETURN CODE !!\n", stdout);    
    }
    dspStr("string_A", str_a);
    dspStr("string_B", str_b);
}


void test_sstrIndexOf(
    sString* str_a,
    sString* str_b
)
{
    sstr_pos rc;
    
    fputs("sstrIndexOf(string_A, string_B): ", stdout);
    fflush(stdout);
    rc = sstr_indexof(str_a, str_b);
    if (rc == SSTR_NPOS)
    {
        fputs("SSTR_NPOS\n", stdout);
    } else {
        fprintf(stdout, "%i\n", (int) rc);
    }
    dspStr("string_A", str_a);
    dspStr("string_B", str_b);
}


void test_sstrSwap(
    sString* str_a,
    sString* str_b
)
{
    sstr_pos rc;
    
    fputs("sstrSwap(string_A, string_B): ", stdout);
    fflush(stdout);
    rc = sstr_swap(str_a, str_b);
    if (rc == SSTR_PASS)
    {
        fputs("SSTR_PASS\n", stdout);
    } else
    if (rc == SSTR_FAIL)
    {
        fputs("SSTR_FAIL\n", stdout);
    } else {
        fputs("!! INVALID RETURN CODE !!\n", stdout);    
    }
    dspStr("string_A", str_a);
    dspStr("string_B", str_b);
}


sstr_rc argCmp(
    sString*    p_src_str,
    const char* p_pat_cstr
)
{
    return ( sstrCmpCstr(p_src_str, p_pat_cstr, strlen(p_pat_cstr)) );
}

void dspStr(
    const char* label,
    sString*    sstr
)
{
    if (sstr != NULL)
    {
        fprintf(stdout, "%s(%s)\n", label, sstr->chars);
    } else {
        fprintf(stdout, "%s -> NULL\n", label);
    }
}

void chkArgs(
    int argc,
    int reqargc
)
{
    if (argc != reqargc)
    {
        syntax_exit();
    }
}
