# Replace printf_(), etc; by printf(), etc.
# This must be enabled.
set(PRINTF_ALIAS_STANDARD_FUNCTION_NAMES ON)

set(PRINTF_OPTIONS
  # Support decimal notation floating-point conversion (%f, %F)
  PRINTF_SUPPORT_DECIMAL_SPECIFIERS=0
  # Support exponential floating point format conversion specifiers (%e, %E, %g, %G)
  PRINTF_SUPPORT_EXPONENTIAL_SPECIFIERS=0
  # Support the 'I' + bit size integer specifiers (%I8, %I16, %I32, %I64) as in Microsoft Visual C++
  SUPPORT_MSVC_STYLE_INTEGER_SPECIFIERS=0
  # Support the length write-back specifier (%n)
  PRINTF_SUPPORT_WRITEBACK_SPECIFIER=0
  # Support long long integral types (allows for the ll length modifier and affects %p)
  PRINTF_SUPPORT_LONG_LONG=0
)

set(INIH_OPTIONS
  # Linblow: whether to allow libc's FILE io.
  # When disabled, ini_parse_file and ini_parse won't be compiled.
  INI_USE_LIBC_FILE=0
  # Whether ini_handler callback should accept lineno parameter.
  INI_HANDLER_LINENO=0
  INI_ALLOW_MULTILINE=1
  INI_ALLOW_BOM=1
  INI_ALLOW_INLINE_COMMENTS=1
  # Whether to (1) use stack for line buffer, or (0) to use heap (malloc/free).
  INI_USE_STACK=1
  # Maximum line length for any line in INI file (stack or heap).
  # Note that this  must be 3 more than the longest line (due to '\r', '\n', and '\0'). 
  INI_MAX_LINE=200
  # Whether to call the handler at the start of each new section (with name and value NULL).
  INI_CALL_HANDLER_ON_NEW_SECTION=0
  # Whether to allow a name without a value (no '=' or ':' on the line),
  # and call the handler with value NULL in this case. Default is to treat
  # no-value lines as an error.
  INI_ALLOW_NO_VALUE=0
  # Whether to use custom ini_malloc, ini_free, and ini_realloc memory
  # allocation functions (INI_USE_STACK must also be 0). These functions must
  # have the same signatures as malloc/free/realloc and behave in a similar
  # way. ini_realloc is only needed if INI_ALLOW_REALLOC is set. 
  INI_CUSTOM_ALLOCATOR=0
)
