# RFIDMachineACS
Generic Machine RFID access control system built with the Intel Edison

This is an access control device built around the intel Edison designed to control user access to some piece of electronic hardware.

It works by wiring into the on/off switch of some device (3D printer, Lasercutter, soldering iron, etc) where it can enable or disable the switch based on credentials tied to a user's RFID number.

Additionally, it will leave the machine enabled until the on/off switch is turned off, wherein it will require the user to re-authorize themself before the machine will turn on.

This system:
  - works with a local database that is intended to be synced with a cloud database
  - is electrically isolated from the device that it is controlling
  - does not rely on current sensing to keep the machine enabled; instead uses a latching circuit that maintains itself in the on position until the machine is turned off
  - can be used with any machine that has a simple, low-voltage on/off switch
  
# ToBuild

  1 Build circuit in accordance with the Fritzing Diagram
  2 Copy the contents of the opt directory into the opt directory in the root of the Intel Edison
  3 May need to download packages sqlite3 & json-c from Alex-T's Intel Edison repo: http://alextgalileo.altervista.org/edison-package-repo-configuration-instructions.html
  4 Run make in doorapp, and run make in memberdatabase
  5. The executable will be called machineACS

    This code assumes that you have a member database called /opt/memberdatabase/members.db with a table called member with the schema: CREATE TABLE MEMBER(ID INT PRIMARY KEY NOT NULL,FNAME TEXT NOT NULL,SALUTATION TEXT NOT NULL,ACCID INT UNIQUE,ACTIVE BOOLEAN NOT NULL CHECK (ACTIVE IN (0,1)),LASTMOD TEXT,TYPE TEXT, LASTCHECKED INT);

    Where ACCID is the RFID tag number for a user, ACTIVE is whether that user should be allowed into the space, and SALUTATION is the nickname which will be displayed if they are allowed entry.

/opt/libs/doorappMacros.h contains the database name/location, what allowed and denied messages to display, and a few other paramenters.
