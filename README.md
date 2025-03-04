# sheer

`sheer` is a library designed for the `shsd` daemon, allowing users to register system events and execute commands or scripts in response.

Basic example using `notify`:
```c 
size_t buffer_size = 1;
sheer_event_buffer_t buffer = sheer_init_buffer(buffer_size);

sheer_event_t event = {
  .source = SHEER_NOTIFY,
  .notify = {
    .notify_key = "com.apple.bluetooth.connection",
  },
  .action =  "echo \"Bluetooth connected/disconnected.\"",
};

int added = sheer_add_event(&buffer, event);

if (!added) {
  printf("Failed to add event. Buffer is full.");
}

sheer_init(&buffer);

dispatch_main();

/* When running, if a user connects or disconnects from Bluetooth, "Bluetooth connected/disconnected." will be echoed. */

```
Note that this is a very bare-bones example using incomplete code.
