#include "error.h"
#include "macro.h"
#include <stddef.h>

static const char *s_errorStr[] = {
    str(OK),

    str(INVALID_IDENTIFIER),
    str(INVALID_IDENTIFIER_START),

    str(INVALID_SIZE_QUALIFIER),
    str(INVALID_SIGN_QUALIFIER),
    str(UNEXPECTED_TYPE_TOKEN),
    str(INVALID_TYPENAME),
    str(INVALID_VARIABLE_DECL),
    str(NOT_VARIABLE_DECL),
    str(INVALID_CHAR_LITERAL),
    str(INVALID_INT_LITERAL),
    str(INVALID_FLOAT_LITERAL),

    str(INVALID_LINE)
};

static int s_offsetMap[] = {
    0, // 0x00
    1, // 0x01
    3, // 0x02
    12 // 0x03
};

static int indexLookup(error_t err) {
    int class = err >> 8;
    int index = err & 0xff;
    return s_offsetMap[class] + index;
}

const char *interpStrError(error_t err) {
    return s_errorStr[indexLookup(err)];
}
