#include <serial.h>
#include <my_stdio.h>
#include <stdint.h>
// Correctly define va_list as void* to work with built-ins
typedef void* va_list;

// Redefine the macros for variable arguments.
// The built-in functions handle the specific architecture details.
#define va_start(ap, last) (__builtin_va_start(ap, last))
#define va_arg(ap, type) (__builtin_va_arg(ap, type))
#define va_end(ap) (__builtin_va_end(ap))

// Define NULL, which is typically in <stddef.h> or other headers
#define NULL ((void*)0)

// Helper function: Get the length of a string
static int my_strlen(const char *s) {
    int len = 0;
    while (*s != '\0') {
        len++;
        s++;
    }
    return len;
}

// Helper function: Reverse a string in place
static void my_strrev(char* s) {
    int i, j;
    char c;
    for (i = 0, j = my_strlen(s) - 1; i < j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

// Helper function: Convert an integer to a string in a given base
static void my_itobase(uint32_t n, char *s, uint8_t base) {
    int i = 0;
    uint32_t temp = n;

    if (n == 0) {
        s[i++] = '0';
    } else {
        while (temp > 0) {
            uint8_t rem = temp % base;
            s[i++] = (rem > 9) ? (rem - 10 + 'a') : (rem + '0');
            temp /= base;
        }
    }

    s[i] = '\0';
    my_strrev(s);
}

// Helper function: Convert a signed integer to a string (base 10)
static void my_itoa(int n, char *s) {
    int i = 0;
    int sign;

    if ((sign = n) < 0) {
        n = -n;
    }

    do {
        s[i++] = n % 10 + '0';
    } while ((n /= 10) > 0);

    if (sign < 0) {
        s[i++] = '-';
    }
    s[i] = '\0';
    my_strrev(s);
}

// my_printf function implementation
int my_printf(const char *format, ...) {
    va_list args;
    va_start(args, format);

    char temp_s[33]; // Sufficient for a 32-bit binary number
    const char *s_ptr;
    int count = 0;

    while (*format != '\0') {
        if (*format == '%') {
            format++;
            switch (*format) {
                case 'd': // Integer output
                    my_itoa(va_arg(args, int), temp_s);
                    s_ptr = temp_s;
                    while (*s_ptr != '\0') {
                        serial_putc(*s_ptr++);
                        count++;
                    }
                    break;
                case 'x': // Hexadecimal output
                    my_itobase(va_arg(args, uint32_t), temp_s, 16);
                    s_ptr = temp_s;
                    while (*s_ptr != '\0') {
                        serial_putc(*s_ptr++);
                        count++;
                    }
                    break;
                case 'c': // Character output
                    serial_putc(va_arg(args, int));
                    count++;
                    break;
                case 's': // String output
                    s_ptr = va_arg(args, const char *);
                    if (s_ptr == NULL) s_ptr = "(null)";
                    while (*s_ptr != '\0') {
                        serial_putc(*s_ptr++);
                        count++;
                    }
                    break;
                case 'b': // Binary output
                    my_itobase(va_arg(args, uint32_t), temp_s, 2);
                    s_ptr = temp_s;
                    while (*s_ptr != '\0') {
                        serial_putc(*s_ptr++);
                        count++;
                    }
                    break;
                case '%': // Output % character
                    serial_putc('%');
                    count++;
                    break;
                default:
                    // Unsupported format specifier, print it as is
                    serial_putc('%');
                    serial_putc(*format);
                    count += 2;
                    break;
            }
        } else {
            // Regular character, print it directly
            serial_putc(*format);
            count++;
        }
        format++;
    }

    va_end(args);
    return count;
}

