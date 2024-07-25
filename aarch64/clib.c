#include <stdlib.h>
#include <stdarg.h>
#include "swis.h"

void memcpy(char *dst, const char *src, int size)
{
    while (size--)
        *dst++ = *src++;
}


void memset(char *dst, int c, int size)
{
    while (size--)
        *dst++ = c;
}


char *strcpy(char *dst, const char *src)
{
    char *odst = dst;
    while (1)
    {
        int c = *src++;
        *dst++ = c;
        if (!c)
            break;
    }
    return odst;
}


char *strncpy(char *dst, const char *src, size_t n)
{
    char *odst = dst;
    while (n-->0)
    {
        int c = *src++;
        *dst++ = c;
        if (!c)
            break;
    }
    return odst;
}


size_t strlen(const char *s)
{
    const char *os = s;
    while (*s++ != '\0')
        ;
    return s - os - 1;
}


const char *strchr(const char *s, char want)
{
    while (*s != want)
    {
        if (*s == '\0')
            return NULL;
        s++;
    }
    return s;
}


int clock(void)
{
    _kernel_swi_regs regs;
    _kernel_swi(OS_ReadMonotonicTime, NULL, &regs);
    return regs.r[0];
}


int getchar(void)
{
    return os_readc();
}


void __assert_fail(const char *message)
{
    static _kernel_oserror err = {0, "Assertion failed"};
    os_write0("Assertion failed: ");
    os_write0(message);
    os_newline();
    os_generateerror(&err);
}


typedef struct outputter_s {
    int (*write0)(struct outputter_s *out, const char *str);
    int (*writec)(struct outputter_s *out, char c);
    int (*writen)(struct outputter_s *out, const char *str, size_t len);
    int (*newline)(struct outputter_s *out);
    void *private;
} outputter_t;

typedef struct formatparams_s {
    int leading_char;
    int align_left;
    int field_width;
    int param_width;
} formatparams_t;

static int stdout_write0(outputter_t *out, const char *str)
{
    os_write0(str);
    return strlen(str);
}

static int stdout_writen(outputter_t *out, const char *str, size_t len)
{
    os_writen(str, len);
    return len;
}

static int stdout_writec(outputter_t *out, char c)
{
    os_writec(c);
    return 1;
}

static int stdout_newline(outputter_t *out)
{
    os_newline();
    return 1;
}

int printf(const char *format, ...)
{
    va_list args;
    const char *next_nl = strchr(format, '\n');
    int n = 0;
    outputter_t out;
    out.write0 = stdout_write0;
    out.writen = stdout_writen;
    out.writec = stdout_writec;
    out.newline = stdout_newline;

    va_start(args, format);

    while (*format)
    {
        formatparams_t params = {0};
        char c;
        const char *next_percent = strchr(format, '%');
        if (next_percent == NULL && next_nl == NULL)
        {
            /* No more characters left, so we output the remaining string */
            n += out.write0(&out, format);
            break;
        }
        while (format < next_percent || next_nl)
        {
            int len = next_percent - format;
            if ((next_percent && next_nl && next_nl < next_percent) ||
                (next_nl && !next_percent))
            {
                len = next_nl - format;
                n += out.writen(&out, format, len);
                n += out.newline(&out);
                format = next_nl + 1;
                next_nl = strchr(format, '\n');
            }
            else
            {
                n += out.writen(&out, format, len);
                format += len;
                break;
            }
        }
        /* Skip the percent */
        if (next_percent)
            format ++;
        else
            break;

        c = *format++;

        /* Parse the field parameters */
        if (c=='-')
        {
            params.align_left = 1;
            c = *format++;
        }
        if (c==' ' || c=='0')
        {
            params.leading_char = c;
            c = *format++;
        }
        if (c>='1' && c<='9')
        {
            /* Field width */
            params.field_width = c - '0';
            c = *format++;
            while (c>='1' && c<='9')
            {
                params.field_width = (params.field_width * 10) + c - '0';
                c = *format++;
            }
        }
        if (c=='l')
        {
            params.param_width = c;
            c = *format++;
        }

        switch (c)
        {
            case '%':
                n += out.writec(&out, '%');
                break;

            case '\0':
                n += out.writec(&out, '%');
                format--;
                continue;

            case 's':
                {
                    const char *s = va_arg(args, const char *);
                    n += out.write0(&out, s);
                }
                break;

            case 'p':
                n += out.write0(&out, "0x");

            case 'x':
                {
                    char hex[16]; /* FIXME: Should be width for arch */
                    int start = -1;
                    uint32_t value = va_arg(args, uint32_t);
                    int i;
                    for (i=sizeof(hex)-1; i>-1; i--) /* FIXME: widen */
                    {
                        int v = value & 15;
                        hex[i] = "0123456789ABCDEF"[v];
                        value = value >> 4;
                        if (start == -1 && value == 0)
                        {
                            start = i;
                            break;
                        }
                    }
                    if (start == -1)
                        start = 0;
                    n += out.writen(&out, &hex[start], sizeof(hex)-start);
                }
                break;

            case 'i':
            case 'd':
            case 'u':
                {
                    char intstr[21];
                    static uint64_t ints[] = {
                            1,
                            10,
                            100,
                            1000,
                            10000,
                            100000,
                            1000000,
                            10000000,
                            100000000,
                            1000000000,
                            10000000000,
                            100000000000,
                            1000000000000,
                            10000000000000,
                            100000000000000,
                            1000000000000000,
                            10000000000000000,
                            100000000000000000,
                            1000000000000000000,
                            10000000000000000000,
                        };
                    char *p = intstr;
                    int size = 18;
                    uint64_t value;
                    if (c == 'u')
                    {
                        value = va_arg(args, uint64_t);
                    }
                    else
                    {
                        int32_t uvalue = va_arg(args, int64_t);
                        if (uvalue < 0)
                        {
                            uvalue = 0 - uvalue;
                            *p++ = '-';
                        }
                        value = uvalue;
                    }
                    if (value < 10000000000)
                        size = 11;
                    int first = 1;
                    while (size >= 0)
                    {
                        int64_t m = ints[size];
                        if (value >= m)
                        {
                            first = 0;
                            int digit = 1;
                            value -= m;
                            while (value >= m)
                                digit += 1, value -= m;
                            *p++ = '0' + digit;
                        }
                        else
                        {
                            if (!first)
                                *p++ = '0';
                        }
                        size -= 1;
                    }
                    /* Special case 0 */
                    if (p == intstr)
                        *p++ = '0';

                    n += out.writen(&out, intstr, p - intstr);
                }
                break;

            default:
                n += out.writec(&out, '%');
                n += out.writec(&out, c);
                break;
        }
    }

    va_end(args);

    return n;
}
