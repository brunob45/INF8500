# LineTracking

_(You can view this file best by opening it in VSCode, then hitting CTRL+SHIFT+V)._

This is the multithreaded `LineTracking`.

**IMPORTANT: Put your TP code inside `C:/TEMP`. You must NOT put your TP code inside the `X:` drive**, since one of the programs of a toolchain called Cygwin that SpaceStudio uses fails when it is run on a drive that resides at a network location. **Remember to save/push your code before closing your session to avoid losing your work.**

## `server/` directory

The server is an executable built using MinGW's Posix compatibility libraries and with OpenCV. In itself, it has nothing to do with SpaceStudio ; it just simulates a real server, e.g. a camera on a robot.

## `spacestudio/` directory

Contains the SpaceStudio's `LineTracking` project. Open `spacestudio/LineTracking.spacestudio` with SpaceStudio.

## Instructions

SpaceStudio takes on the very difficult task of making Vivado HLS, Vivado, Linux, Linux drivers, UBoot, custom hardware IPs, QEMU, custom CPU simulators, etc. all work together at the push of a button. While this is appealing, it should be expected that some problems arise. Here is a list of things to do/not to do, and of troubleshooting tips.

### Server

#### Get SpaceStudio's MinGW64 toolchain

Amongst other things, the server needs OpenCV to compile, link and execute. So, before being able to compile the server, we need to download [SpaceStudio's MinGW64, available here](https://github.com/AnthonyD973/MinGW64_SpaceStudio), with all its packages and libraries. Download the ZIP file and unzip it somewhere, e.g. on a USB stick.

#### Simulation

Before running the simulation, **open Git Bash** and set the `MINGW64_SPACESTUDIO_DIR` variable to the location you extracted the MinGW64 toolchain. E.g.:

```bash
export MINGW64_SPACESTUDIO_DIR='/h/mingw64' # If you extracted MinGW64 to H:/mingw64
```

Then start the `start_tcp_server_simulation.sh` script, which is at the root of the TP3 directory:

```bash
./start_tcp_server_simulation.sh
```

Note that you must restart the server every time you rerun the simulation in SpaceStudio. You can close the server by hitting ENTER while in Git Bash.

#### Implementation

Before starting the Zedboard, **open Git Bash** set the `MINGW64_SPACESTUDIO_DIR` variable to the location you extracted the MinGW64 toolchain. E.g.:

```bash
export MINGW64_SPACESTUDIO_DIR='/h/mingw64' # If you extracted MinGW64 to H:/mingw64
```

You should also set the `SERVER_IP_ADDRESS` variable. Find out your IP address by running `ipconfig` in CMD, then run:

```bash
export SERVER_IP_ADDRESS='12.34.56.78' # If the Windows machine's IP is 12.34.56.78
```

Then start the `start_tcp_server_implementation.sh` script, which is at the root of the TP3 directory:

```bash
./start_tcp_server_implementation.sh
```

Note that you must restart the server every time you rerun the implementation on the Zedboard. You can close the server by hitting ENTER while in Git Bash.

### SpaceStudio

#### Creating the Zynq architecture

In SpaceStudio, when you will create the solution to run your code on a Zynq (Zedboard) virtual platform, make sure you select SMP mode, since the `master` and `display` modules require Linux headers to run. This is probably going to save you from insanity :-)

#### Module assignment on a Zynq architecture

If you have activated an architecture that has a Zynq on it, you must always make sure...

1) ...the `zynq0` entity is on the `AMBA_AXIBus_LT0` bus.

2) ...any module you want to run on hardware is on the `zynq0_HP0_AMBA_AXIBus_LT0` bus. Of course, modules you want to run on software must be under the `zynq0` entity.

#### Simulation

When you want to simulate the code, make sure...

1) ...the `display0` and `reception0` modules are in hardware (they use Linux headers to communicate via TCP to the server, and that cannot be done easily when the module runs on the simulated CPU).

2) ...there is a `simulation_timer` on the `AMBA_AXIBus_LT0` bus.

#### Simulation troubleshooting

1) If you encounter the error *`Module <X> is trying to read from device <Y> with an invalid offset <Z>`* during the simulation, then right click the `zynq0_DDR_InterfaceRange0` entity, and edit its `High address` property to be at least 300 KB higher than `<Z>`. Normally, setting `0xBFFFFFFF` should do the trick.

2) If you wish to debug, *only run hardware debugging*. You cannot run Software debugging, nor Hardware/Software co-debugging. Move your bugged modules to hardware if you want to debug them.

3) If you built the simulation with hardware debugging, you can't run the simulation without hardware debugging (which you can run by clicking *Run*, then *Debug*, then *Hardware...*). Clicking the green _Play_ button will not work, even though SpaceStudio gives you the option.

### Implementation

#### Module assignment

When you wish to implement, you must make sure...

1) ...you have a Zynq on the `AMBA_AXIBus_LT0` bus.

2) ...to delete the `simulation_timer`.

3) ...that the `display0` and `reception0` modules are in software, that is, under the `zynq0` entity (this is because these modules do Linux networking stuff, which obviously doesn't synthesize in hardware)

4) ...that your hardware modules are under the `zynq0_HP0_AMBA_AXIBus_LT0` bus.

5) ...that you changed the IP address in `reception.cpp` and `display.cpp` to the Windows machine's IP address (inside the `#else` around line 27 for `reception.cpp` and also inside the `#else` around line 34 for `display.cpp`). It takes ~20 minutes for the Architecture Implementation to run, so it's a bit annoying when you realize you forgot to change this and you have to rerun it again ¯\\\_(°͜ʖ°)_/¯ .

#### Running *Architecture Implementation*

1) Before clicking Architecture Implementation, make sure Vivado HLS and Vivado are enabled by going to *Tools*, then *Preferences*, then, under *SpaceStudio* > *EDA* > *Xilinx - Vivado 2018.3*, make sure the *"EDA is enabled"* and *"High-level synthesis is enabled"* boxes are checked.

2) When you click Architecture Implementation, make sure the popup menu shows: *"Xilinx - Vivado 2018.3* for *Electronic Design Automation (EDA) tool"*, *"ZedBoard Zynq Evaluation and Development Kit"* for *"Board"*, *"Vivado HLS"* for *"High-level synthesis"*, and make sure all the modules are checked. You can leave the default value for *"Project directory"*. Click OK.

### On the Zedboard

#### Changing our MAC address and setting a static IP

Once that the *Architecture Implementation* is done, that you've started the server and that you've started the Zedboard, you can connect to the Zedboard by opening the PuTTY program, and setting these configurations:

- Connection type: Serial
- Serial line: either COM3 or COM4 ; go to *"Gestionnaire de périphériques"*, expand *"Ports (COM et LPT)"* and search for *"USB Serial Port"*.
- Speed: 115200

In the window that opens, you will see any output that the Zedboard prints, and you will be able to type commands to the Zedboard with your keyboard.

##### Decide on a MAC and an IP address

We must assign an IP address manually to the Zedboard (Poly decided that the Zedboard's LAN wouldn't have a DHCP on it). We must also assign a MAC address manually (so that we don't have two Zedboards with the same MAC address in the lab).

Choose an IP address. It must be in the range `132.207.89.10` to `132.207.89.99` and cannot be the same as the IP another student in the lab room chose. **Be careful, the students of INF3995 ("Projet 3") also use these addresses.** Please don't make them sad by using the same IP address as they do while they are in the room.

The MAC address you should use is `00:18:3E:00:00:XY`, where XY are the last two digits of the IP address you chose. For example, if you chose `132.207.89.56` as IP, then your MAC should be `00:18:3E:00:00:56`.

##### Changing the MAC

In the PuTTY window, run:

```bash
ifdown eth0
ifconfig eth0 hw ether <MAC address>
ifup eth0
```

To make sure `eth0`'s MAC was changed, run `ip addr`.

*NOTE: If you restart the Zedboard, you will need to run these commands again.*

##### Changing the IP

To assign the IP address we should edit the `/etc/network/interfaces` file. You can edit it by running the command:

```bash
vi /etc/network/interfaces
```

Once in `vi`, hit ESCAPE then `i` to go into insert mode, then change the file to look like this:

```text
auto lo
iface lo inet loopback
auto eth0 inet static
    address <IP address you chose>
    netmask 255.255.255.0
    gateway 132.207.89.1
    network 132.207.89.0
    dns-nameservers 8.8.8.8
```

Once this is done, hit ESCAPE to go out of insert mode, then write `:wq` to write changes and quit.

We can the apply the new configuration by running:

```bash
ifdown eth0
ifup eth0
```

You can verify that your IP address was assigned by running `ip addr`.

*NOTE: Files you create on the Zedboard are volatile (or something like that), so you will need to edit `/etc/network/interfaces` and run `ifdown eth0; ifup eth0` again if you reboot the Zedboard.*

##### Make sure you have internet connection

Run:

```bash
ping 8.8.8.8
```

If you see some lines like `64 bytes from 8.8.8.8: seq=<X> ttl=<Y> time=<Z>`, then the Zedboard has internet access. Otherwise, you might have missed a step.

#### Running the program

Once you've changed both the MAC and the IP, you can run the `zynq0.core0.arm_a9.elf` executable to observe the results of our SpaceStudio project. Run:

```bash
cd /root
./zynq0.core0.arm_a9.elf
```
