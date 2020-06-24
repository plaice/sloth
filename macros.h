/* some useful C macros */

#define isnull(s) (s == '\0')

#define isnewline(s) (s == '\n')

#define isdigit(s) ((s >= '0') && (s <= '9'))

#define islower(c) (((c) >= 'a') && ((c) <= 'z'))

#define isupper(c) (((c) >= 'A') && ((c) <= 'Z'))

#define isalpha(c) (isupper(c) || islower(c))

#define isalphanum(c) (isupper(c) || islower(c) || isdigit(c))

#define iswhitespace(c) (((c) == ' ') || ((c) == '\t') || ((c) == '\n'))

#define makeupper(c) islower(c) ? (c) - 'a' + 'A' : (c)

#define max(x,y) ((x) > (y)) ? (x) : (y)

#define min(x,y) ((x) < (y)) ? (x) : (y)
