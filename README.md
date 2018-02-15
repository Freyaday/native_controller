# native_controller
A fused-controller XInput wrapper.

This XInput wrapper fuses all connected XInput controllers into a single controller. Physical buttons on the controller are combined via `OR`. Triggers are combined via `max`. Stick values are from whichever controller, with a stick outside the deadzone, has the smallest index.

Because XInput blocks on trying to get data from disconnected controllers, `update()` only queries controllers known to be connected. To check if new controllers have been conncted, use `reconnect_and_update()`. 

Both of these methods update an internal copy of the controller state. It is this state that the various input-querying functions inspect. They do not modify this state, so it should be safe to call them from multiple threads. However, `update()`, `clear()`, and `reconnect_and_update()` all change the internal controller state, and should not be multithreaded nor should they interleave with any of the controller-querying functions. The vibration functions are thin wrappers over the XInput vibration controls and follow the same multithreading restrictions as they do.

Please enjoy. 💜