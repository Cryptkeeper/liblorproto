#ifndef LIGHTORAMA_RESULT_H
#define LIGHTORAMA_RESULT_H

typedef int LorResult;

#define LorOK                ((LorResult) (0))
#define LorErrInvalidArg     ((LorResult) (-1))
#define LorErrOutOfBuffer    ((LorResult) (-2))
#define LorErrInvalidChannel ((LorResult) (-3))
#define LorErrUnknownModel   ((LorResult) (-4))

#endif//LIGHTORAMA_RESULT_H
