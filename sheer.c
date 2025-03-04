#include <CoreServices/CoreServices.h>
#include <dispatch/dispatch.h>
#include <inttypes.h>
#include <notify.h>
#include <stdio.h>
#include <stdlib.h>

#include "sheer.h"

sheer_event_buffer_t sheer_init_buffer(size_t buffer_size) {

  /* TODO: Fix this function */

  if (buffer_size < MINIMUM_BUFFER_SIZE) {
    /* ERROR OUT */
  };

  sheer_event_buffer_t buffer;
  buffer.events = malloc(sizeof(sheer_event_t) * buffer_size);
  if (!buffer.events) {
    buffer.size = 0;
    return buffer;
  }

  buffer.size = buffer_size;

  for (size_t i = 0; i < buffer_size; i++) {
    buffer.events[i].source = _SHEER_SENTINEL;
  }

  return buffer;
};

void sheer_free_buffer(sheer_event_buffer_t *buffer) {
  if (!buffer || !buffer->events) {
    return;
  }

  free(buffer->events);
  buffer->events = NULL;
  buffer->size = 0;
};

int sheer_add_event(sheer_event_buffer_t *buffer, sheer_event_t event) {
  for (size_t i = 0; i < buffer->size; i++) {
    if (buffer->events[i].source == _SHEER_SENTINEL) {
      buffer->events[i] = event;
      return 1;
    }
  }

  return 0;
};

void sheer_init(sheer_event_buffer_t *buffer) {

  /* INITIALIZE A DISPATCH QUEUE FOR FSEVENTS | NOTIFY | IOKIT */
  dispatch_queue_t queue =
      dispatch_queue_create("com.test.sheer_queue", DISPATCH_QUEUE_CONCURRENT);

  for (size_t i = 0; i < buffer->size; i++) {

    int source = buffer->events[i].source;
    char *action = buffer->events[i].action;

    switch (source) {
    case _SHEER_SENTINEL:
      fprintf(stderr, "unused buffer slot at index %zu. exiting..\n", i);
      /* CLEANUP AND EXIT */
      break;

    case SHEER_FSEVENTS:
      /* BUILD STREAM.. */
      break;

    case SHEER_NOTIFY: {
      int token;
      const char *notify_key = buffer->events[i].notify.notify_key;

      int status =
          notify_register_dispatch(notify_key, &token, queue, ^(int t) {
            system(action);
          });

      if (status != NOTIFY_STATUS_OK) {
        fprintf(stderr, "Failed to register notification %s [STATUS %d]\n",
                notify_key, status);
        /* CLEANUP AND EXIT */
      }
      break;
    }

    case SHEER_SCNETWORK_REACHABILITY:
      /* BUILD ... */
      break;

    case SHEER_IOKIT_POWER:
      /* BUILD ... */
      break;

    case SHEER_IOKIT_DEVICE:
      /* BUILD ... */
      break;

    case SHEER_KQUEUE:
      /* BUILD ... */
      break;

    default:
      fprintf(stderr, "Unknown event source at index %zu: %d\n", i, source);
      break;
    }
  }

  /* OUT OF LOOP -- BUFFER IS NO LONGER NEEDED AS STREAMS ARE REGISTERED */
  sheer_free_buffer(buffer);
}

int main(int argc, char *argv[]) {

  sheer_event_buffer_t buffer = sheer_init_buffer(2);

  // sheer_event_t event = {
  //     .source = SHEER_SRC_FSEVENTS,
  //     .fsevent =
  //         {
  //             .cflags = CF_FSEVENTS_FILE_EVENTS,
  //             .directory = "/Users/user/sheer/",
  //             .flags = F_FSEVENTS_FILE_CREATE | F_FSEVENTS_FILE_DELETE,
  //         },
  //     .action = "",
  // };

  // int added = sheer_add_event(&buffer, event);

  // if (!added) {
  //   printf("failed to add event, buffer is full...");
  // }

  // sheer_event_t nt_event_1 = {
  //     .source = SHEER_NOTIFY,
  //     .notify =
  //         {
  //             .notify_key = "com.apple.bluetooth.connection",
  //         },
  //     .action = "echo \"Bluetooth connected/disconnected.\"",
  // };

  // sheer_event_t nt_event_3 = {
  //     .source = SHEER_NOTIFY,
  //     .notify = {.notify_key = "com.apple.system.config.network_change"},
  //     .action = "echo \"Network change.\"",
  // };

  sheer_event_t event = {
      .source = SHEER_NOTIFY,
      .notify = {.notify_key = "com.apple.system.powersources.percent"},
      .action = "echo \"Battery change.\"",
  };

  int added = sheer_add_event(&buffer, event);

  if (!added) {
    printf("Failed to add event, buffer is full.");
  }

  sheer_init(&buffer);

  /* TODO: Handle this. */
  dispatch_main();

  return EXIT_SUCCESS;
}
