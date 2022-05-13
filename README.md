# ARDUINO + NFC/RFID - quick examples/guide

There are 3 examples that are ordered from simple to more complicated.
If you're somewhat new to coding and or arduino, I'd highly suggest just trying to make example 1 work
before jumping into the more complex ones. I also added some pictures of the settup but clearer instructions for it 
can be found in the link 'Arduino - RFID-NFC - Arduino Tutorial'.

Written by Eyal Rufeisen - e.rufeisen@st.hanze.nl .

## Last notes before jumping into it:

- There is a link in the main folder as well as folder 3 for other examples/tutorials that explain how to set up the arduino and nfc (where and how to connect them properly). I took the original code from there too. So it's a good place to look for some initial help with connectivity and setting things up.

- I personally had a lot of trouble setting up the libraries needed and stuff. You should install the arduino IDE and look for the package manager. In there you can install specifc required libraries like the ones used here (SPI & MFRC522). Hopefully that works well enough for you. Sometimes I got messages that the libraries weren't found and etc, I believe I fixed those issues by moving my installation and IDE libraries folder (can be found in the IDE settings).

- Arduino is very finicky and the IDE is really bad so don't imagine making everything work immediately on the first try. It takes some playing around with but it's very doable.

## Information about the examples:

All the examples make use of the serial monitor (a display of all the messages printed by the code). So if you want to test if something is working you have to upload it to the arduino then run the serial monitor to know what is happening and to see what it prints.

Example 1. Reading the displayed nfc chip and printing the unique ID (UID) of it.
+ Installing proper libraries, reading nfc chip, printing messages

Example 2. Reading displayed nfc and comparing it to a stored UID to check if it matches (prints authorized / not authorized based on which chip was presented)
+ all of example 1 + copying and comparing uid (byte array).

Example 3. This example has a couple of different functions. Reading & Checking if an nfc chip is authorized or not, as well as you can add new chips to the list of authorized ones.
+ all of example 1&2 + working with an array of UIDs + Using a button to (when clicked) save the next presented UID to the authorized UIDs.
