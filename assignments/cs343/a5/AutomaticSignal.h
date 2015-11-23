#ifndef AUTOMATICSIGNAL_H
#define AUTOMATICSIGNAL_H

#define AUTOMATIC_SIGNAL uCondition blocked

#define WAITUNTIL(pred, before, after) \
    if (!pred) { \
        before; \
        while (!blocked.empty()) { \
            blocked.signal(); \
        } \
        while (!pred) { \
            blocked.wait(); \
        } \
        after; \
    }

#define RETURN(expr...) \
    while (!blocked.empty()) { \
        blocked.signal(); \
    } \
    return expr;

#endif
