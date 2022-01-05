# Passgen-C

Generates and prints a random string between 8 and 2048 characters in length to standard output. Users can choose to include/exclude symbols, uppercase/lowercase letters, and numbers.

#### How to Run:

To run, compile using `make` and run the generated executable named `passgen`.

Use `./passgen -h` to see program description available options.

#### Installation (Only on Linux):

Run the install script using `sudo ./INSTALL` and the executable will be generated and placed in the `/usr/bin` directory.

-  ##### Note: Make sure to not have any similarly named files in that directory, or simply replace 'passgen' in both the Makefile and the Install script with any other name for the program. Modifying the program's help text is optional.
