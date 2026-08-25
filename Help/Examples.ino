// This file contains a heavily annotated code example.
// Take a lot of this with a grain of salt, as most of
// these explanations are for beginners to C++/Arduino.
// I have tried to make this include things you may see
// in the blank starter code/lab examples.
//
// A library is a piece of code that someone else wrote so
// that you don't have to write it yourself.
// A library generally consists of a header file, "my_library.h",
// and a code file, "my_library.ino".
// The header file should contain explanations of the functions
// in the library, and the code file will contain the implementations.
//
// When a tutorial says "Object," they are referring to a class or
// struct, like:
//
// struct my_struct {
// int field_one;
// double field_two;
// };
//
// or:
//
// class my_struct {
// int field_one;
// double field_two;
// };
//
// This is just a way for people to group data together. When you go:
//
// struct my_struct MyVariable;
//
// you construct an instance of my_struct and can refer to it by
// the name MyVariable. This means that you can go:
//
// MyVariable.field_one
//
// or:
//
// MyVariable.field_two
//
// You could imagine representing a spatial vector as a struct:
//
// struct Vec {
// double x;
// double y;
// double z;
// };
//
// and defining a function:
//
// void normalize(struct Vec input);
//
// Instead of taking in x, y, and z separately, the function
// takes in a Vec.

//'ino' files are just .cpp files inder a different name.
//this means they are "compiled" (google)

#define A_PIN A4 //When this gets compiled it will literaly copy paste
                 //"A4" where ever you put A_PIN
                 //the general syntax goes #define NAME TEXT where TEXT
                 //is what you will replace NAME with 

#include "hello.h" 
/*
   the #include will copy pase in all the code in the hello.h
   file. You would do this if you need to define things like pins or functions that need to be shared across files.

   Google "header files for api definition" if you want to learn more.
   The accelerometer also uses I2C

   If you see the itzy bitzy shine a pink light and it might be shorting the bpm and that will break it so don't do that.

   the bmp sensors suck you kinda have to keep trying and make sure everythign is connected

   in the bmp lab if you are not getting serial output double check that all the bpm pins are proplery connected we don't really do any error checking here so it will fail silently

   the pin labeled USB is the 5v

   GENERAL TIPS: 
   if you see in the example code something like:
   int foo;
   double bar;

   Do not try to use these variables yet. They have been defined but not
   initialized, which means they do not have a usable value.

   You must assign them a value before using them:

   foo = 2;
   bar = baz();

   where `baz()` returns a `double`.

   the: "<<" means a "left bit shift" (google)

   the bmp280 sesors use I2C you need a high voltage, ground, clock, and signal line.

   double is generaly 8 bytes and float is 4 they both store numbers like 1.234 ...

   LOOPS:
   if you have a for loop and it is acting strange make sure you initialize your incrmenting variable. by default cpp does not initialize variable so writing
   for (int i; i < 10; i++) {do something}
   will give you garbage
   as opposed to:
   for (int i = 0; i < 10; i++) {do something}

   PRINTING
   if you want you use something like printf("%d" , my_integer);
   you largly can't. You could try sprintf but I have not tested it.
   this means you will have to go: 

   Serial.print("VAR:");
   Serial.println(my_variable);

 *NOTE* print is like: printf("hello"); and println is like: printf("hello \n");

 In the globals.h you may see there is some stuff with: 
#include <RTClib.h>
This gives you the ability to use C esque printf's in arduino


EXTRA — NOT NEEDED TO KNOW:

When you write something like:

Hello greeting;

C++ creates a `Hello` object called `greeting`.

Since `greeting` is an instance (object) of the `Hello` class,
if the class exposes functions, you can call them using:

greeting.my_function();

Here, `greeting` is an instance of the `Hello` class.

This can be seen in the line: 
Adafruit_LIS3DH lis = Adafruit_LIS3DH(); 
Here they just use a function "Adafruit_LIS3DH()" to return a Adafruit_LIS3DH object that exposes methods(functions) you will use. 

lis.setRange(LIS3DH_RANGE_4_G);   
here the method(function) is called  setRange and takes as argument a integer. The LIS3DH_RANGE_4_G is most likley a macro (#define LIS3DH_RANGE_4_G 4)
if you see  int bar = 0xFF; or similar that is a hex number its like binary but is base 16 instead of 2 and you should just google the conversion

if you see anything like   uint8_t, uint32_t these are just like char and unsigned int but they tell you how many bits are in the number.

FOR ADVANCED READERS:
`static` has several meanings in c++.

as a rule for this class, if you see `static`, do not modify it unless
you know why it was made static.

if you put a file-level `static` variable in a header file and include
that header in multiple translation units, each translation unit will
get its own copy of that variable.

if you need one shared variable across multiple translation units,
`static` is not what you want. you would typically declare it with
`extern` in the header and define it in one `.cpp` file. 

ATTN: If you try and do this for this class you need to re evaluate what you are doing

arduino boards and cores support different c++ versions. for this
class, assume roughly c++11.
*/

const int INTERVAL = 1 * 1000 * 1000;
//Const means you cant modify this value

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  while(!Serial);
}

//This function was delared in the hello.h file but defined here by us.
void from_hello() {
  Serial.println("Hi from hello");
}

int timeLast = 0;
int timeNow = 0;
int dt = 0;
int count = 0;

void loop() {
  // Update the global variables for the time in this loop
  timeNow = micros();      // the time we started this loop, microseconds
  dt = timeNow - timeLast; // the time difference since last loop, microseconds
  if (dt >= INTERVAL) {
    count ++;
    timeLast = timeNow;
    Serial.print("\033[2J\033[H");
    Serial.print("secs:");
    Serial.println(count);
    from_hello();
  }
}

