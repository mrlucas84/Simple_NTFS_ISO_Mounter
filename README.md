#### Simple NTFS ISO Mounter (by CaptainCPS-X, 2013)

https://github.com/CaptainCPS/Simple_NTFS_ISO_Mounter

---

Latest Release: v1.02

---

![Image Alt](http://img811.imageshack.us/img811/5272/ae21.png)

---

This application will look for ISOs in your USB NTFS HDD and allow you
to mount them on your PS3!

### Directories scanned are:

- /*NTFS_HDD*/PS3ISO/
- /*NTFS_HDD*/PSXISO/
- /*NTFS_HDD*/BDISO/
- /*NTFS_HDD*/DVDISO/

### Controls are:

[CROSS] - Select ISO / Game to Mount
[TRIANGLE] - Rescan NTFS drive

Enjoy! Merry Christmas! xD

### NOTES:

This application will ONLY work on "Cobra 7.00 CFW (Mixed w/Rogero 4.46 v1.00)".

Do not connect more than one NTFS drive, it will only use one, plus I don't see
the need of having many drives connected when you just want to mount one game.

Keep reading for more notes...

---

#### THANKS TO:

Thanks to "Estwald" for porting the NTFS library and "Cobra Dev" for 
improving it and releasing the Cobra 7.00 CFW along with source code.

---

#### CHANGELOG:

v1.02 (November 6, 2013)

- When booting the application, any mounted ISO will be automatically un-mounted for security.
- [source] Initial code organization, no functionality was removed from cobra sources included.
- [source] Linked with my personally compiled "libcobra.a" (sce sdk) to allow extra 
functionality (like mounting / un-mounting ISOs / Fake Disc Eject / etc...).

v1.01 (November 4, 2013)

- Fixed support for PSXISO, BDISO and DVDISO, now it should mount them correctly.

v1.00 (November 4, 2013)

- Support for PS3ISO, PSXISO, BDISO and DVDISO directories scanning and listing.
- Initial release.

---

#### NOTES: 

This release is initial, so expect it to be very simple but functional.

More improvements will eventually come, so stay tuned.

Running Iris Manager is causing issues with the mounting of ISOs, avoid
running Iris Manager before booting Simple NTFS ISO Mounter. If you ran 
Iris Manager by accident, just restart your PS3.

PS2ISO is not supported, you must use the internal PS3 HDD and put your ISO(s)
in "/dev_hdd0/PS2ISO/".

---
