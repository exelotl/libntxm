
                   libntxm - An XM Player Library for the Nintendo DS
                                                  originally by 0xtob

                            https://github.com/NitrousTracker/libntxm

WHAT'S THIS?
------------

  libntxm (NitroTracker XM library) is a library for playing XM music
  in Nintendo DS homebrew software. It's very CPU friendly, because
  it uses the DS sound hardware directly instead of doing software
  mixing.


WHO SHOULD USE THIS?
--------------------

  This library is intended for use in homebrew games and applications.
  As it does not support the complete XM format yet, other libraries
  may still be preferrable. So, use it only if you can live with the
  limitations listed below.


LIMITATIONS
-----------

  libntxm does not yet support all instrument settings and effects.
  In order to keep the lists short, here are the _unsupported_
  instrument settings and the _supported_ effects:

  Unsupported instrument settings:
    - Envelope loop
    - Fade out
    - Panning
    - Vibrato

  Supported effects:
    - Arpeggio
    - Note cut
    - Note delay
    - Pattern break
    - Pattern delay
    - Portamento
    - Position jump
    - Set loop
    - Set pan
    - Set speed and tempo
    - Set volume
    - Vibrato
    - Volume slide


USAGE
-----

  The provided example explains how to use the library. You need to
  have a custom ARM7 binary. Make sure to link to the respective
  libraries in the arm7 and arm9 makefiles. For testing the example,
  you need to have a file called test.xm on your card.


DEVELOPMENT
-----------

  You want to help by adding new features to the library? Great!

  https://github.com/NitrousTracker/libntxm/issues


VERSION HISTORY
---------------

 v0.2 (2008-04-13)
  - Relicensed under Noncommercial zLib License / GPLv3
  - Now using the same codebase as NitroTracker for easier integration

 v0.1 (2007-04-15)
  - Initial Release


Happy coding!
     - 0xtob

