#ifndef STATE_H

#define STATE_H

typedef enum State {
    STATE_METHOD,
    STATE_URI,
    STATE_PROTOCOL,
    STATE_STATUS_CODE,
    STATE_STATUS_TEXT,
    STATE_HEADER_NAME,
    STATE_HEADER_VALUE,
    STATE_BODY
} State;


#endif /* STATE_H */

