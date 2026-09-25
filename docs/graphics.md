# Graphics / Display

Separate:
- raw VIC device
- display/graphics service
- surfaces
- window server/compositor
- GUI/widget library

## Surface model

A surface is drawable storage.

A window presents a surface through a compositor.

Fullscreen/direct presentation can present a surface directly.

Applications should ideally use the same surface API whether running fullscreen or composited.

## Example layering

    application
       -> graphics/surface API
       -> window server OR direct display presentation
       -> VIC device
       -> hardware

## Exclusive mode

Games/demos can request exclusive display/VIC access and EXCLUSIVE scheduling priority. The GUI can suspend/release physical display ownership and restore afterward.

If a serious game/demo must reboot out of the OS merely to perform adequately, consider that an architectural failure.