## Overview
Libft is a custom implementation of standard C library functions along with additional utilities designed to be reused across various C projects. This foundational project aims to deepen understanding of core functions in C and create a useful library for future assignments.

## Introduction
Libft replicates several essential standard library functions (such as string manipulation, memory management, etc.) and extends them with additional utility functions. These functions will serve as tools for future projects where using the C standard library might not be allowed or available.

## Compilation and Setup
To compile the library, execute the following steps:

Clone the repository:
```bash
git clone https://github.com/jazzazi007/42_libft.git
```
```bash
cd libft
```
Run the Makefile to generate the libft.a static library:

```bash
make
```
This will create the libft.a file that contains all implemented functions.

Additional Makefile commands:

Clean build files:
```bash
make clean
```
Remove all compiled files and the library:
```bash
make fclean
```
Rebuild the library from scratch:
```bash
make re
```

## Mandatory Functions
Libc Functions
The following functions are re-implemented to behave like their standard C library counterparts, each prefixed with ft_:

Character Checks: ft_isalpha, ft_isdigit, ft_isalnum, ft_isascii, ft_isprint

String Operations: ft_strlen, ft_strlcpy, ft_strlcat

Memory Operations: ft_memset, ft_bzero, ft_memcpy, ft_memmove, ft_memchr, ft_memcmp

String Search: ft_strchr, ft_strrchr, ft_strncmp, ft_strnstr

Conversion Functions: ft_atoi

Character Conversion: ft_toupper, ft_tolower

Memory Allocation: ft_calloc, ft_strdup

Function Descriptions

ft_strlen: Returns the length of a string.

ft_memset: Fills a block of memory with a specified byte.

ft_strchr: Locates a character in a string.

ft_atoi: Converts a string to an integer.

All functions behave identically to their standard C counterparts, with the exception of the ft_ prefix.


## Technical Considerations

* Memory Management: Ensure all dynamically allocated memory is freed to avoid memory leaks.

* Global Variables: Not allowed; use arguments or local declarations instead.
* Static Helper Functions: Declare functions as static if only used within a specific .c file.
* Makefile: Compile using flags -Wall -Wextra -Werror, and include the standard rules: all, clean, fclean, re.
## Testing
Testing your functions is crucial to ensure they behave as expected. Although testing is not required for submission, it is highly recommended to create a set of test programs to validate each function's functionality. This will also assist during the peer-evaluation process.

## License

[MIT](https://choosealicense.com/licenses/mit/)
