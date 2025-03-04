/* clang-format off */
#ifndef SHEER_H
#define SHEER_H

#include <CoreServices/CoreServices.h>


/* EVENT SOURCES. ALL SHOULD CONFORM TO GCD */
#define _SHEER_SENTINEL                -1
#define SHEER_FSEVENTS                  0
#define SHEER_NOTIFY                    1 
#define SHEER_SCNETWORK_REACHABILITY    2 
#define SHEER_IOKIT_POWER               3 
#define SHEER_IOKIT_DEVICE              4 
#define SHEER_KQUEUE                    5

#define MINIMUM_BUFFER_SIZE             1

/* FSEVENT CONSTANTS */
#define FSEVENTS_EVENT_ID_SINCE_NOW     kFSEventStreamEventIdSinceNow

/* CREATE FLAGS */
#define CF_FSEVENTS_IGNORE_SELF         kFSEventStreamCreateFlagIgnoreSelf
#define CF_FSEVENTS_MARK_SELF           kFSEventStreamCreateFlagMarkSelf
#define CF_FSEVENTS_WATCH_ROOT          kFSEventStreamCreateFlagWatchRoot
#define CF_FSEVENTS_FILE_EVENTS         kFSEventStreamCreateFlagFileEvents
#define CF_FSEVENTS_FULL_HISTORY        kFSEventStreamCreateFlagFullHistory

/* EVENT FLAGS */
#define F_FSEVENTS_FILE_CREATE          kFSEventStreamEventFlagItemCreated
#define F_FSEVENTS_FILE_DELETE          kFSEventStreamEventFlagItemRemoved

/* DEFAULT FSEVENT */
#define DEFAULT_FSEVENT_LATENCY         1.0

/* clang-format on */

typedef struct {
  /* Stream create flags. */
  uint32_t cflags;
  /* Directory to watch. */
  char *directory;
  /* Event flags. */
  uint32_t flags;
} sheer_fsevent_t;

typedef struct {
  char *notify_key;
} sheer_notify_event_t;

typedef struct {

} sheer_iokit_t;

typedef struct {
  /* The event source API. */
  int source;
  /* Source-specific variables. */
  union {
    sheer_fsevent_t fsevent;
    sheer_notify_event_t notify;
  };
  /* The command or script to execute. */
  char *action;
} sheer_event_t;

typedef struct {
  sheer_event_t *events;
  size_t size;
} sheer_event_buffer_t;

typedef struct {
  sheer_fsevent_t *fsevents;
  size_t size;
} sheer_fsevent_buffer_t;

/* TODO: Figure out how to handle this. */
typedef struct {
  /* Source API of the key event. */
  uint32_t ksource;
  /* The key event. */
  void *key;
} sheer_gate_t;

sheer_event_buffer_t sheer_init_buffer(size_t buffer_size);

void sheer_free_buffer(sheer_event_buffer_t *buffer);

int sheer_add_event(sheer_event_buffer_t *buffer, sheer_event_t event);

void sheer_init(sheer_event_buffer_t *buffer);

#endif
