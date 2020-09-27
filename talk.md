background-colour: white
foreground-colour: black
padding: 1em
body.font: Optima.Regular
body.size: 24pt
code.size: 0.9em
code.bold.italic.font: Candara.Bold
h1.font: Optima.Bold
h1.size: 58pt
h2.font: Optima.Bold
h2.size: 28pt
vertical-align: middle
padding: 2em
pre.padding: 0.5em
pre.border.type: solid
pre.border.width: 2px
pre.border.colour: slategray
pre.background-colour: gainsboro
logo.bottom-right: GerphG
logo.bottom-right.inset: 1em
logo.bottom-right.width: 4em
logo.bottom-right.height: 4em


# Building for RISC OS, Online
## and what makes it tick

Gerph, November 2020


---
background-colour: #56C1FF
foreground-colour: white
h1.font: Optima.Regular
h1.size: 58pt

# 0. Introduction


November

2020

---
background-colour: white
foreground-colour: black
h1.font: Optima.Bold
h1.size: 42pt
h2.font: Optima.Bold
h2.size: 28pt
vertical-align: top

# Introduction
## How I'll do this talk

* Lots of technology, some of which may be alien to you.
* Questions at the end.
* But, split into sections, with a chance for some questions between them.

---
# Introduction
## What we'll talk about

* Some background.
* What JFPatch-as-a-service is
* How it works
* What powers it...

---
background-colour: #56C1FF
foreground-colour: white
h1.font: Optima.Regular
h1.size: 58pt
vertical-align: middle

# 1. Background

March

2019

---
background-colour: white
foreground-colour: black
h1.font: Optima.Bold
h1.size: 42pt
h2.font: Optima.Bold
h2.size: 28pt
vertical-align: top

# Background
## Who am I?

* A RISC OS architect and engineer, who's been away from the community for about 15 years.
* I used to do a lot of things with RISC OS, which you can read about on my site if you're interested - _gerph.org/riscos_
* I'm not going to talk about that past here.
* I would like to think that I probably know RISC OS in design and execution better than anyone.

---
# Background
## Dear gosh, why?

* What do I want to do with RISC OS and why?
  * Let's make something for me, because I can.

---
# Background
## So, you want to use RISC OS, but…

* Development on RISC OS is tedious
  * The tools aren't great but they only run on RISC OS… and I don't have a RISC OS system \(other than RPCEmu\)
* RISC OS testing is awful
  * Most RISC OS projects do ad-hoc testing, rely on users; no automation
* RISC OS is awful for testing
  * If something goes wrong, you need to hard reboot; no isolation; no security

---
# Background
## How does the real world do things?

* Source control \!
* Cross compiling
* Managed development environments
* Automated testing of changes
* Feature and regression testing
* Fleets of systems available for use

---
# Background
## How can I do this? \(1\)

* Source control:
  * Move things to Git, because CVS is so very painful.
  * \<picture / link>

***`Tech`***: Gitlab, running on my linux server - it's publicly accessible, but most of the 1100-odd projects are private.

---
# Background
## How can I do this? \(2\)

* Cross compiling:
  * Already had the toolchain ported to 32bit Linux and Windows, back in 2005.

***`Tech`***: Port the toolchain to 64bit Linux and to 64bit macOS.

---
# Background
## How can I do this? \(3\)

* Managed environments:
  * How do I get my toolchain? find my libraries? store built components?
  * ***`Tech`***: Artifactory for artifacts, and created some tools for pushing and pulling resources.
* Automation:
  * ***`Tech`***: GitLab CI triggers on every change - pulls resources from Artifactory, builds, pushes result to Artifactory.
  * \<put a diagram here>

---
# Background
## How can I do this? \(4\)

* Managed environments: \(cont'd\)
  * What if I don't want to download my toolchain all the time?
  * ***`Tech`***: Docker RISC OS development environment.
  * \<picture / demo>

---
# Background
## How can I do this? \(4\)

* Feature and regression testing:
  * I need a way to test things…

***`Tech`***: … we'll come to that later …

---
background-colour: #56C1FF
foreground-colour: white
h1.font: Optima.Regular
h1.size: 58pt
vertical-align: middle

# 2. JFPatch-as-a-service

March

2020


---
background-colour: white
foreground-colour: black
h1.font: Optima.Bold
h1.size: 42pt
h2.font: Optima.Bold
h2.size: 28pt
vertical-align: top

# JFPatch-as-a-Service
## Why?

* A friend said to me…
  * “I can't wait until you csa.announce \<this> and confuse the bejesus out of the RISC OS civilians.”
* To which my answer was…
  * “JFPatch as a service would be a doddle to do right now. A service that nobody asked for, or needed.”

---
# JFPatch-as-a-Service
## What is JFPatch?

* It's a pre-processor for the BASIC assembler.
* It has its own file format which describes things to patch, or modules to build.
* It converts these to BASIC files, then runs the BASIC, which does the heavy lifting of assembling.
* It is, itself, written in BASIC.
* It was used to write many of my early assembler modules.

---
# JFPatch-as-a-Service
## What can you build with it?

* Any JFPatch code \(now builds 32bit code\)
* C code that compiles with the Norcroft compiler
* Pascal code \(which will be converted to C and compiled with the Norcroft compiler\)
* BASIC assembler
* Objasm assembler.

***`Tech`***: They're all built for 32bit RISC OS, automatically taken from Artifactory.

---
# JFPatch-as-a-Service
## What might use the service?

* Automated builds can use this:
  * LineEditor \(BASIC assembler\) \(github link\)
  * Nettle \(C application\) \(github link\)
  * CObey \(C module\) \(github link\)
  * ErrorCancel \(ObjAsm\) \(github link\)
  * Pico \(C command line tool\) \(github link\)
  * DDEUtils \(JFPatch module\) \(github link\)

---
# JFPatch-as-a-Service
## How do you use the service? \(1\)

---
# JFPatch-as-a-Service
## How do you use the service? \(2\)

About robuild-client. FIXME

---

background-colour: #56C1FF
foreground-colour: white
h1.font: Optima.Regular
h1.size: 58pt
vertical-align: middle

# 3. How The Service Works

March

2020

---
background-colour: white
foreground-colour: black
h1.font: Optima.Bold
h1.size: 42pt
h2.font: Optima.Bold
h2.size: 28pt
vertical-align: top

# How The Service Works
## What is the service?

* Takes its inspiration from Matthew Godbolt's Compiler explorer.
* \<website image / demo?>
* \<architecture diagram>

***`Tech`***:
  * Infrastructure - AWS SSL, routing and linux server.
  * Front End - Static site, using custom CodeMirror colouring, websockets to talk to back end
  * Back End - Python REST JSON API and WebSockets service
  * Tools - JFPatch tool, compiler, assembler, linker, amu, etc.

---
# How The Service Works
## What runs those services?

---
# How The Service Works
## Wait what?

> “ Wait, RISC OS is running in Docker?\
But Docker runs on Linux?\
You're running RISC OS on Linux then? ”

---
background-colour: #56C1FF
foreground-colour: white
h1.font: Optima.Regular
h1.size: 58pt
vertical-align: middle

# ...

June

2019

---

# 4. RISC OS Pyromaniac

June

2019

---
background-colour: white
foreground-colour: black
h1.font: Optima.Bold
h1.size: 42pt
h2.font: Optima.Bold
h2.size: 28pt
vertical-align: top

# RISC OS Pyromaniac
## How do you test RISC OS software without RISC OS ?

* My RiscPC is in storage.
* It's not good for testing.
* RISC OS was originally run semi-hosted from a BBC, using the BBC as the I/O and RISC OS as the main computer.
* That's what I want to be able to do - I want to be able to drive RISC OS from the CLI of a sane machine.

---
# RISC OS Pyromaniac
## Surely that's easy?

* Surely that's easy? You just run an emulation system until it hits a SWI and then you make the SWI do the I/O thing. Then you run some more?

* Yes. That's exactly what you do.

* The `IfThere` tool ran on June 10th - not well, but it ran.

---
# RISC OS Pyromaniac
## What is RISC OS Pyromaniac?

* Pyromaniac is an alternative implementation of RISC OS for non-ARM systems.
* It is intended for use as a testing and prototyping environment which may be used during development and automated testing.
* Written in a high level language to make that possible.

---
# RISC OS Pyromaniac
## What makes up Pyromaniac? \(1\)

* Written in Python.
* Uses Unicorn \(a QEmu derived package\) for emulating ARM code.
* All other packages are optional.

\<System structure diagram>

---
# RISC OS Pyromaniac
## What makes up Pyromaniac? \(2\)
---
# RISC OS Pyromaniac
## What does it mean?

* A command line only version of RISC OS.
* A RISC OS which runs 32bit ARM binaries, on Windows, macOS, or Linux.
* A reimplementation, which uses none of the code that went before.
* Focused on being able to test software and diagnose issues more easily.

***`Tech`***: RISC OS Pyromaniac, able to run programs on other systems\!

---
# RISC OS Pyromaniac
## Command line only?

* Command line is the primary interface.
* Graphics implementations exist - either 'headless' or using a window on to them - but command line is where it excels.
* For testing, you largely want to be able to exercise things without UI interactions, at least for the lower level tests.

---
# RISC OS Pyromaniac
## No graphics, then?

* The VDU system, and the graphics system work, mostly.
* VDU and graphics are complex so not everything works as it does in RISC OS Classic.
* Not all of it works as documented - after all not all of it is documented\!

---
# RISC OS Pyromaniac
## Graphics demo\!

---
# RISC OS Pyromaniac
## Graphics demo\! (presentation)

***`Tech`***:

* Slide presentation system
* Markdown parser
* FontMap for font remappings.
* WebColours module for colour parsing.
* ImageFileRender for general image rendering, DrawFile for vectors

---
# RISC OS Pyromaniac
## Features - What works?

* System - Runs 32bit modules, utilities and applications.
* Interaction - Interacts with the host as its primary interface
* Video - Pretty good VDU and graphics support GTK/WxWidgets, or snapshots of state.
* Desktop - Not supported
* Sound - SoundChannels mapped through MIDI
* Filesystem - Host filesystem by default, using `,xxx` filename convention.
* Network - Internet module provides limited support for IPv4 and IPv6 networking.
* Compatibility - Many simple programs work, if their support modules are loaded.

---
# RISC OS Pyromaniac
## Features - What doesn't work?

* Desktop - Not supported
* Sound - No wave output
* Graphics: Sprites, Non-paletted modes
* Many other things

---
# RISC OS Pyromaniac
## How do you use it?

* Command line:

```
charles@laputa ~/projects/RO/pyromaniac> ./pyro.py --load-internal-modules --command 'gos'
Supervisor

*fx0

Error: RISC OS 7.14 (02 Aug 2020) [Pyromaniac 0.14 on Darwin/x86_64] (Error number &f7)
*quit
```

---
# RISC OS Pyromaniac
## Configuration

* RISC OS Pyromaniac is highly configurable - over 240 directly configurable options, in 59 groups.
* Configuration can be on the command line or in configuration files.
* Example:
  * `./pyro.py --config modules.rominit_noisy=true --load-internal-modules --command gos`
  * `./pyro.py --config memorymap.rom_base=90000000 --load-internal-modules --command modules`

---
# RISC OS Pyromaniac
## Configuration files

```
%YAML 1.1
---
# Configuration for loading the ROM for RISC OS 5

debug:
  - modules
  - traceregionfunc
  - podules
  - swimisuse

config:
  podule.extensionrom1: ROMs/riscos5
  modules.rominit_noisy: true
  memorymap.rom_base: 0x8800000
  modules.unplug: extrom1:Podule,ParallelDeviceDriver,TaskWindow,SpriteExtend,SystemDevices,...

modules:
  internal: true
```

---

# RISC OS Pyromaniac
## Networking

* Internet module supplied.
  * Supports `AF_INET`, `AF_INET6`, `AF_UNIX`.
  * Many ioctls are supported, mapped to the host system.
* Resolver module provides IPv4 host name resolution \(FIXME: Should be IPv6?\)
* EtherPyromaniac provides a DCI4 driver.
  * Provides a virtual network.

---
# RISC OS Pyromaniac
## FontManager

FIXME: Work out what to say ... UTF-8, host fonts.

---
# RISC OS Pyromaniac
## Draw module

FIXME: Work out what to say - Example Drawfile? \(or have we already done that\).

MORE

---
# RISC OS Pyromaniac
## Tracing and debugging

Tracing SWI arguments:

```
>GCOL 0, 255,192,0
 383f848: SWI     ColourTrans_SetGCOL
          => r0  = &00c0ff00   12648192  colour
             r3  = &00000100        256  flags
             r4  = &00000000          0  action
          <= r0  = &00000000          0  gcol
             r2  = &00000002          2  log2_bpp
             r3  = &00000000          0  corrupted

```

***`Tech`***: OSLib parser and templating system

---
# RISC OS Pyromaniac
## Tracing and debugging

```
$ pyro testcode/bin/word_time_string --debug trace
 700013c: ADR     r1, &07000174             ; -> [&00000000, &00000000,
                                                  &00000000, &00000000]
 7000140: MOV     r0, #&e                   ; #14
 7000144: MOV     r2, #0
 7000148: STRB    r2, [r1]                  ; R2 = &00000000, R1 = &07000174
 700014c: SWI     OS_Word
 7000150: SWI     OS_WriteS
 7000164: MOV     r0, r1                    ; R1 = &07000174
 7000168: SWI     OS_Write0
 700016c: SWI     OS_NewLine
Time string: Sun,06 Sep 2020 08:22:38
 7000170: MOV     pc, lr                    ; R14 = &04107fe0
 4107fe0: SWI     &FEED05
```

---
# RISC OS Pyromaniac
## Tracing and debugging

* Other trace features:
  * Function, memory and SWI traps.
  * Exception and API misuse reports.
* Debug features:
  * Most modules have debug available.
  * Can be enabled at runtime.

MORE

MORE

---
# RISC OS Pyromaniac
## What does it look like?

```

```

---
# RISC OS Pyromaniac
## What does it look like?

```
"""
OS_ReadEscapeState implementation.
"""

from riscos import handlers
import riscos.constants.swis as swis

@handlers.swi.register(swis.OS_ReadEscapeState)
def swi_OS_ReadEscapeState(ro, swin, regs):
    """
    OS_ReadEscapeState

    <= C flag is set if an escape condition has occurred
    """

    regs.cpsr_c = ro.kernel.progenv.escape_condition
```

---
# RISC OS Pyromaniac
## Other technologies\!

***`Tech`***:

* RISC OS Alphabets in Python Codecs
* RISC OS Zip file decoding in Python
* SublimeText syntax for RISC OS command files
* NanoRC syntaxes for some RISC OS file types
* Tool for building hourglass modules
* Tool testing tool
* Miscellaneous toolchain updates

---
# RISC OS Pyromaniac
## RISC OS Testing is awful

* RISC OS Pyromaniac has tests - about a thousand at present.
* Tests take about 18 minutes to run - and run on Linux and macOS in parallel.
* Code coverage hovers at around 65%.

***`Tech`***: Some of the tests exported out to GitHub

---
# RISC OS Pyromaniac
## RISC OS is awful for testing

* Clarification: RISC OS Classic is awful for testing.
* FIXME: Examples?
* JFPatch itself is tested.
* BASIC module has some tests that run programs.

---
# RISC OS Pyromaniac
## What does it run on?

* macOS \(console, GTK, wxWidgets\)
  * Also a dedicated application.
* Linux \(console, GTK, wxWidgets\)
  * Also within a docker container.
* Windows \(console, wxWidgets\) \[native and under Wine, also docker wine-py\]
  * Also a dedicated application.

---
# RISC OS Pyromaniac
## “Releases”?

* Released once a month, just to myself.
    * October's version is 0.16.
    * Releases are a way to stop it being unusably 'half finished'.
    * Releases are a great incentive - I really have achieved a lot this month!
* Long lived development, for example...
  * Font Manager lived on a branch for about 6 months.
  * EasySockets is still on a branch.

---
# RISC OS Pyromaniac
## Future work

* More APIs.
* Better handling of corner cases.
* Back Trace Structures
* Using it for actual testing - that was what it was for!

---
background-colour: #56C1FF
foreground-colour: white
h1.font: Optima.Regular
h1.size: 58pt
vertical-align: middle

# 5. Conclusion

---
background-colour: white
foreground-colour: black
h1.font: Optima.Bold
h1.size: 42pt
h2.font: Optima.Bold
h2.size: 28pt
vertical-align: top

# Conclusion
## References

FIXME: Include a link to where you can find more information.

Create _pyromaniac.riscos.online_ as a resource for this?

---
background-colour: #56C1FF
foreground-colour: white
h1.font: Optima.Regular
h1.size: 58pt
vertical-align: middle

# 6. Questions

