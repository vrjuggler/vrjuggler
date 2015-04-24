# VR Juggler C++ Coding Standard #

## Standardization is Important ##

It helps if the standard annoys everyone in some way so everyone feels they are on the same playing field. The proposal here has evolved over many projects, many companies, and literally a total of many weeks spent arguing. It is no particular person's style and is certainly open to local amendments.

### Good Points ###

When a project tries to adhere to common standards a few good things happen:

  * Programmers can go into any code and figure out what's going on
  * New people can get up to speed quickly
  * People new to C++ are spared the need to develop a personal style and defend it to the death
  * People new to C++ are spared making the same mistakes over and over again
  * People make fewer mistakes in consistent environments
  * Programmers have a common enemy

### Bad Points ###

Now the bad:

  * The standard is usually stupid because it was made by someone who doesn't understand C++
  * The standard is usually stupid because it's not what I do
  * Standards reduce creativity
  * Standards are unnecessary as long as people are consistent
  * Standards enforce too much structure
  * People ignore standards anyway

### Discussion ###

The experience of many projects leads to the conclusion that using coding standards makes the project go smoother. Are standards necessary for success? Of course not. But they help, and we need all the help we can get! Be honest, most arguments against a particular standard come from the ego. Few decisions in a reasonable standard really can be said to be technically deficient, just matters of taste. So be flexible, control the ego a bit, and remember any project is fundamentally a team effort.

Since the Juggler team is growing, a coding standard has become an issue that needed to be addressed.  We want the code to be clear to everyone (not just the developer who wrote it) and we need the code to be easily readable when someone is looking at it for the first time.  A coding standard does help for this.  It leads to consistency and understandability.

A coding standard can also help to reduce bugs by enforcing safe coding idioms and debugging methods.

That all said, this coding standard is not at all hard to follow.  It really only enforces a few things compared to other coding standards we have seen.

## Interpretation ##

### Conventions ###

  * The use of the word "shall" in this document requires that any project using this document must comply with the stated standard.
  * The use of the word "should" directs projects in tailoring a project-specific standard, in that the project must include, exclude, or tailor the requirement, as appropriate.
  * The use of the word "may" is similar to "should", in that it designates optional requirements.

### Terminology ###

  * For the sake of simplicity, the use of the word "compiler" means compiler or translator.
  * "C++ Coding Standard" refers to this document whereas "C++ ANSI Standard" refers to the standard C++ language definition.

### Accepting an Idea ###

  1. It's impossible.
  1. Maybe it's possible, but it's weak and uninteresting.
  1. It is true and I told you so.
  1. I thought of it first.
  1. How could it be otherwise.

If you come to object with a negative preconception please keep an open mind. You may still conclude objects are bunk, but there's a road you must follow to accept something different. Allow yourself to travel it for a while.


---


## Names ##

### Make Names Fit ###

Names are the heart of programming. In the past people believed knowing someone's true name gave them magical power over that person. If you can think up the true name for something, you give yourself and the people coming after power over the code. Don't laugh!

A name is the result of a long deep thought process about the ecology it lives in. Only a programmer who understands the system as a whole can create a name that "fits" with the system. If the name is appropriate everything fits together naturally, relationships are clear, meaning is derivable, and reasoning from common human expectations works as expected.

If you find all your names could be Thing and DoIt then you should probably revisit your design.

### Class Names ###

  * Name the class after what it is. If you can't think of what it is that is a clue you have not thought through the design well enough.
  * Compound names of over three words are a clue your design may be confusing various entities in your system. Revisit your design. Try a CRC card session to see if your objects have more responsibilities than they should.
  * Avoid the temptation of bringing the name of the class a class derives from into the derived class's name. A class should stand on its own. It doesn't matter what it derives from.
  * Suffixes are sometimes helpful. For example, if your system uses agents then naming something DownloadAgent conveys real information.

### Method and Function Names ###

  * Usually every method and function performs an action, so the name should make clear what it does: `CheckForErrors()` instead of `ErrorCheck()`, `DumpDataToFile()` instead of `DataFile()`. This will also make functions and data objects more distinguishable.
  * Classes are often nouns. By making function names verbs and following other naming conventions programs can be read more naturally.
  * Suffixes are sometimes useful:
    * `Max` - to mean the maximum value something can have.
    * `Cnt` - the current count of a running count variable.
    * `Key` - key value.
  * For example: `RetryMax` to mean the maximum number of retries, `RetryCnt` to mean the current retry count.
  * Prefixes are sometimes useful:
    * `is` - to ask a question about something. Whenever someone sees Is they will know it's a question.
    * `get` - get a value.
    * `set` - set a value.
  * For example: `isHitRetryLimit()`.

### Include Units in Names ###

If a variable represents time, weight, or some other unit then include the unit in the name so developers can more easily spot problems. For example:

```
uint32 mTimeoutMsecs;
uint32 mMyWeightLbs;
```

Better yet is to make a variable into a class so bad conversions can be caught.

### No All Upper Case Abbreviations ###

When confronted with a situation where you could use an all upper case abbreviation instead use an initial upper case letter followed by all lower case letters. No matter what.

**Justification**

  * People seem to have very different intuitions when making names containing abbreviations. It's best to settle on one strategy so the names are absolutely predictable.
  * Take for example `NetworkABCKey`. Notice how the `C` from `ABC` and `K` from key are confused. Some people don't mind this and others just hate it so you'll find different policies in different code so you never know what to call something.

**Example**

```
class FluidOz	               // NOT FluidOZ
class NetworkAbcKey // NOT NetworkABCKey
```

### Class Names ###

  * Use upper case letters as word separators, lower case for the rest of a word
  * First character in a name is upper case
  * No underbars (`_`)

**Justification**

  * Of all the different naming strategies many people found this one the best compromise.

**Example**

```
class NameOneTwo
class Name
```

### Class Library Names ###

  * Now that name spaces are becoming more widely implemented, name spaces should be used to prevent class name conflicts among libraries from different vendors and groups.
  * When not using name spaces, it's common to prevent class name clashes by prefixing class names with a unique string. Two characters is sufficient, but a longer length is fine.

**Example**

John Johnson's complete data structure library could use `JJ` as a prefix, so classes would be:

```
class JjLinkList
{
};
```

### Method Names ###

  * Use the same rule as for class names - _except_ start with a small character.

**Justification**

  * Of all the different naming strategies many people found this one the best compromise.
  * The small starting character differentiates method names from classes.

**Example**

```
class MyFunkyClass
{
public:
   int doIt();
   void handleError();
};
```

### Class Attribute Names ###

  * Attribute names should be prepended with the character 'm'.
  * After the 'm' use the same rules as for class names.

Justification

  * Often your methods and attribute names will be similar,
  * especially for accessors.

  * Prepending 'm' prevents any conflict with method names,
> > (and local variable names and global names).


Example

```
	class NameOneTwo
	{
	public:
		int						 varAbc();
		int						 errorNumber();
	private:
		int						 mVarAbc;
		int						 mErrorNumber;
		String*					mName;
	}
```



**Method Argument Names**

  * The first character should be lower case.
  * All word beginnings after the first letter should be upper case as with class names.

Justification

  * You can always tell which variables are "passed in".
  * You can use names similar to class names without conflicting with class names.

Example

```
	class NameOneTwo
	{
	public:
		int startYourEngines( Engine& someEngine, Engine& anotherEngine );
	}
```




**Variable Names on the Stack**

  * use all lower case letters
  * use '_' as the word separator._

Justification

  * With this approach the scope of the variable is clear in the code.
  * Now all variables look different and are identifiable in the code.

Example

```
	int NameOneTwo::HandleError( int errorNumber )
	{
		int				error_lookup = OsErr( errorNumber );
		Time			  time_of_error;
		ErrorProcessor error_processor;
	}
```





**Global Variable Names**

  * use the conventions in Global Constants, or in Function Parameters...
  * or use your judgement...

Justification

  * we don't want to decide on this yet...



**Global Constants**

  * Global constants should be all caps with '_' separators._

Justification

It's tradition for global constants to named this way. You must be careful to not conflict with other global #defines and enum labels.

Example

```
	 const int A_GLOBAL_CONSTANT= 5;	 
```




**Static Member Variables**

  * Same as Member Variables

Example

```
	class Test
	{
	public:
	private:
		static StatusInfo mStatus;
	}
```





**Type Names**

  * When possible for types based on native types make a typedef.
  * Typedef names should use the same naming policy as for a class (optionally with the word Type appended).

Justification

  * Of all the different naming strategies many people found this one the best compromise.
  * Types are ''things'' (same as Class) so should use upper case letters.
  * The word "Type" can be appended to make it clear this is not a class.

Example

```
	typedef uint16  ModuleType;
	typedef boost::shared_ptr<System>  SystemPtr;
```





**#define and Macro Names**

  * Put #defines and macros in all upper using '_' separators.
  * NOTE: Enums are preferred to #define constants, since enums are understood by the debugger and are typesafe._

Justification

  * This makes it very clear that the value is not alterable and in the case of macros, makes it clear that you are using a construct that requires care.
  * Some subtle errors can occur when macro names and enum labels use the same name.

Example

```
#define MAX(a,b) blah
#define IS_ERR(err) blah
```



**C Function Names**

  * Same as Method Names

Justification
  * rather than use gnu C standard (all lower case letters with '_' as the word delimiter),
  * use something consistant with the rest of the function naming guidelines..._


```
	  int someFunction()
	  {
	  } 
```



**Enum Types, Names**


> same as Class names...

Example

```
	enum ArrayElement
	{
		Yelt,
		Xelt,
		Zelt
	};
```


---


**Required Methods for a Class**

To be good citizens almost all classes should implement the following methods. If you don't have to define and implement any of the "required" methods they should still be represented in your class definition as comments.

> Default Constructor

  * If your class needs a constructor, make sure to provide one. You need one if during the operation of the class it creates something or does something that needs to be undone when the object dies. This includes creating memory, opening file descriptors, opening transactions etc.

  * If the default constructor is sufficient add a comment indicating that the compiler-generated version will be used.

  * If your default constructor has one or more optional arguments, add a comment indicating that it still functions as the default constructor.


> Virtual Destructor

  * If your class is intended to be derived from by other classes then make the destructor virtual.

> Copy Constructor

  * If your class is copyable, either define a copy constructor and assignment operator or add a comment indicating that the compiler-generated versions will be used.

  * If your class objects should not be copied, make the copy constructor and assignment operator private and don't define bodies for them. If you don't know whether the class objects should be copyable, then assume not unless and until the copy operations are needed.

> Assignment Operator

  * If your class is assignable, either define a assignment operator or add a comment indicating that the compiler-generated versions will be used.

  * If your objects should not be assigned, make the assignment operator private and don't define bodies for them. If you don't know whether the class objects should be assignable, then assume not.

Justification

  * Virtual destructors ensure objects will be completely destructed regardless of inheritance depth. You don't have to use a virtual destructor when:
    * You don't expect a class to have descendants.
    * The overhead of virtualness would be too much.
    * An object must have a certain data layout and size.
  * A default constructor allows an object to be used in an array.
  * The copy constructor and assignment operator ensure an object is always properly constructed.



---

**Brace Placement**

  * Of the three major brace placement strategies one is acceptable:

  * Place brace under and inline with keywords:

```
		  if (condition)		  while (condition)
		  {							{
			  ...						 ...
		  }							}
```

> Justification
  * There are more reasons than psychological for preferring this style.
  * If you use an editor (such as vi) that supports brace matching, the
  * first is a much better style. Why? Let's say you have a large block of code and want to know
  * where the block ends. You move to the first brace hit
  * a key and the editor finds the matching brace. Example:

```
		  if (very_long_condition && second_very_long_condition)
		  {
				...
		  }
		  
		  else if (...)
		  {
				..
		  }
```

> To move from block to block you just need to use cursor down and your brace matching key. No need to move to the end of the line to match a brace then jerk back and forth.

**When Braces are Needed**

> Always...
> All if, while and do statements require braces even if there is only a single statement within the braces. For example:

```
	if (1 == somevalue)
	{
		somevalue = 2;
	}  
```

> Justification

  * It ensures that when someone adds a line of code later there are already braces and they don't forget. It provides a more consistent look. This doesn't affect execution speed. It's easy to do.




**Indentation/Tabs/Space Policy**

  * Indent using 3 spaces for each level.  Never use 

&lt;tab&gt;

 characters (set your editor to use 3 spaces for each 

&lt;tab&gt;

 press).


Justification

  * Tabs aren't used because 8 space indentation severely limits the number of indentation levels one can have. The argument that if this is a problem you have too many indentation levels has some force, but real code can often be three or more levels deep. Changing a tab to be less
  * than 8 spaces is a problem because that setting is usually local. When someone prints the source tabs will be 8 characters and the code will look horrible. Same for people using other editors. Which is why we use spaces...
  * When people using different tab settings the code is impossible to read or print, which is why spaces are preferable to tabs.
  * Nobody can ever agree on the correct number of spaces, so use 3.  In general we have found 3 spaces per indentation level workable.
  * We use 3 for all files so that we don't have to set up our editors differently for each file we edit.  This is really important so that the file indents don't get screwed up... :(




---


**Parens () with Key Words and Functions Policy**

  * Do not put parens next to keywords. Put a space between.
  * Do put parens next to function names.
  * Do not use parens in return statements when it's not necessary.

Justification

  * Keywords are not functions. By putting parens next to keywords keywords and function names are made to look alike.

Example


```
	 if (condition)
	 {
	 }

	 while (condition)
	 {
	 }

	 strcpy( s, s1 );

	 return 1;
```



---


**RCS Keywords, Change Log, and History Policy**

When using RCS directly this policy must change, but when using other source code control systems like CVS that support RCS style keywords:

  * Do not use RCS keywords within files.
  * Do not keep a change history in files.
  * Do not keep author information in files.

Justification

  * The reasoning is your source control system already keeps all this information. There is no reason to clutter up source files with duplicate information that:
    * makes the files larger
    * makes doing diffs difficult as non source code lines change
    * makes the entry into the file dozens of lines lower in the file which makes a search or jump necessary for each file
    * is easily available from the source code control system and does not need embedding in the file
  * When files must be sent to other organizations the comments may contain internal details that should not be exposed to outsiders.


---


**Class Layout**

  * A common class layout is critical from a code comprehension point of view and for automatically generating documentation.
  * C++ programmers, through a set of tools called doxygen, can enjoy the same level generated documentation Java programmers take for granted.

> Class and Method Documentation

  * Please use the following template when creating a new class.

```
	/**		
	 * A one line description of the class.
	 *
	 * #include "XX.h" [[BR]]
	 * -llib 
	 *
	 * A longer description.
	 *  
	 * @see something
	 */
	class MyClass
	{
	public:
		/** my func
		 */
		void func();
	};
```

> see the doxygen website for more commenting details.



---


## namespace ##

**Use of Namespaces**

  * Namespaces are now commonly implemented by compilers. They should be used if your compiler supports them completely.

**Don't Globally Define using**

  * Don't place "using namespace" directive at global scope in a header file. This can cause lots of magic invisible conflicts that are hard to track. Keep using statements to implementation files (if at all)...


---




**Use Header File Guards**

  * Include files should protect against multiple inclusion through the use of guards:

```
#ifndef ClassName_h
#define ClassName_h

#endif 
```


  * The new line after the endif if is rqeuired by some compilers.


---


**A Line Should Not Exceed 78 Characters**

  * Lines should not exceed 78 characters.

Justification

  * Even though with big monitors we stretch windows wide our printers can only print so wide. And we still need to print code.
  * The wider the window the fewer windows we can have on a screen. More windows is better than wider windows.
  * We even view and print diff output correctly on all terminals and printers.



---


## control blocks formatting ##
**If Then Else Formatting**


> Try to put else on a new line  (not "} else {"), this way it is easy to scan down one column and see all of the else blocks.

```
	if (condition)					  // Comment
	{
	} 
	else if (condition)				// Comment
	{
	}
	else									// Comment
	{
	}
```

If you have else if statements then it is usually a good idea to always have an else block for finding unhandled cases. Maybe put a
log message in the else even if there is no corrective action taken.

**Condition Format**

Always put the constant on the left hand side of an equality/inequality comparison. For example:

```
if ( 6 == errorNum ) 
	... 
```

One reason is that if you leave out one of the = signs, the compiler will find the error for you. A second reason is that it puts the
value you are looking for right up front where you can find it instead of buried at the end of your expression. It takes a little time to
get used to this format, but then it really gets useful.


**switch Formatting**

  * Falling through a case statement into the next case statement shall be permitted as long as a comment is included.
  * The default case should always be present and trigger an error if it should not be reached, yet is reached.
  * If you need to create variables put all the code in a block.

Example

```
	switch (...)
	{
		case 1:
			...
		// FALL THROUGH

		case 2:
		{		  
			int v;
			...
		}
		break;

		default:
	}
```


---



## Macros ##

**Replace Macros with Inline Functions**

  * In C++ macros are not needed for code efficiency. Use inlines.

> Example

```
		#define  MAX(x,y)		 (((x) > (y) ? (x) : (y))		  // Get the maximum
```

> The macro above can be replaced for integers with the following inline function with no loss of efficiency:

```
		inline int max( int x, int y )
		{
			return (x > y ? x : y);
		}	
```

> For complete generality:

```
		template <typename T>
		inline T max( const T x, const T y )
		{
			return (x > y ? x : y);
		}	
```


> Justification
  * this allows your compiler to trace/step into the macro.  Good for debugging!



---


**Initialize all Variables**

  * You shall always initialize variables. Always. Every time.

Justification

  * More problems than you can believe are eventually traced back to a pointer or variable left uninitialized. C++ tends to
  * encourage this by spreading initialization to each constructor.


---


## project stuff, docs, etc.. ##
**Don't be Afraid of Small Libraries**

One common enemy of reuse is people not making libraries out of their code. A reusable class may be hiding in a program directory
and will never have the thrill of being shared because the programmer won't factor the class or classes into a library.

One reason for this is because people don't like making small libraries. There's something about small libraries that doesn't feel right.
Get over it. The computer doesn't care how many libraries you have.

If you have code that can be reused and can't be placed in an existing library then make a new library. Libraries don't stay small for
long if people are really thinking about reuse.

If you are afraid of having to update makefiles when libraries are recomposed or added then don't include libraries in your makefiles,
include the idea of services. Base level makefiles define services that are each composed of a set of libraries. Higher level
makefiles specify the services they want. When the libraries for a service change only the lower level makefiles will have to change.


**Tell! When You do Something Tell Everyone**

Let other people know if you have done something reusable. Don't be shy. And don't hide your work to protect your pride. Once
people get in the habit of sharing work everyone gets better.




**Comments on Comments**

**Comments Should Tell a Story**

Consider your comments a story describing the system. Expect your comments to be extracted by a robot and formed into a man
page. Class comments are one part of the story, method signature comments are another part of the story, method arguments another
part, and method implementation yet another part. All these parts should weave together and inform someone else at another point of
time just exactly what you did and why.

**Document Decisions**

Comments should document decisions. At every point where you had a choice of what to do place a comment describing which
choice you made and why. Archeologists will find this the most useful information.




**Interface and Implementation Documentation**

> There are two main audiences for documentation:

  * Class Users
  * Class Implementors

> With a little forethought we can extract both types of documentation directly from source code.

> '''Class Users'''

> Class users need class interface information which when structured correctly can be extracted directly from a header file. When
> filling out the header comment blocks for a class, only include information needed by programmers who use the class. Don't delve
> into algorithm implementation details unless the details are needed by a user of the class. Consider comments in a header file a man
> page in waiting.

> '''Class Implementors'''

> Class implementors require in-depth knowledge of how a class is implemented. This comment type is found in the source file(s)
> implementing a class. Don't worry about interface issues. Header comment blocks in a source file should cover algorithm issues and
> other design decisions. Comment blocks within a method's implementation should explain even more.




**Directory Documentation**

Every directory should have a README file that covers:

  * the purpose of the directory and what it contains
  * a one line comment on each file. A comment can usually be extracted from the NAME attribute of the file header.
  * cover build and install directions
  * direct people to related resources:
    * directories of source
    * online documentation
    * paper documentation
    * design documentation
  * anything else that might help someone

Consider a new person coming in 6 months after every original person on a project has gone. That lone scared explorer should be
able to piece together a picture of the whole project by traversing a source directory tree and reading README files, Makefiles, and
source file headers.


---


**Don't Over Use Operators**

C++ allows the overloading of all kinds of weird operators. Unless you are building a class directly related to math there are very few
operators you should override. Only override an operator when the semantics will be clear to users.

Justification

  * Very few people will have the same intuition as you about what a particular operator will do.


---


## Naming Class Files ##
**Class Definition in One File**

Each class definition should be in its own file where each file is named directly after the class's name:

```
	ClassName.h
```

**Implementation in One File**

In general each class should be implemented in one source file:

```
	ClassName.cpp	// always use cpp.  VC++, gnu make build systems all support this extension (works with cl.exe, g++, and CC)
```


---

**Be Const Correct**

C++ provides the const key word to allow passing as parameters objects that cannot change to indicate when a method doesn't
modify its object. Using const in all the right places is called "const correctness." It's hard at first, but using const really tightens up
your coding style. Const correctness grows on you.


---

## streams ##
**Use Streams**

Programmers transitioning from C to C++ find stream IO strange preferring the familiarity of good old stdio. Printf and gang seem to be
more convenient and are well understood.

**Type Safety**

Stdio is not type safe, which is one of the reasons you are using C++, right? Stream IO is type safe. That's one good reason to use
streams.


**Standard Interface**

When you want to dump an object to a stream there is a standard way of doing it: with the << operator. This is not true of objects
and stdio.

**Interchangeablity of Streams**

One of the more advanced reasons for using streams is that once an object can dump itself to a stream it can dump itself to any
stream. One stream may go to the screen, but another stream may be a serial port or network connection. Good stuff.

**Streams Got Better**

Stream IO is not perfect. It is however a lot better than it used to be. Streams are now standardized, acceptably efficient, more
reliable, and now there's lots of documentation on how to use streams.

**Check Thread Safety**

Some stream implementations are not yet thread safe. Make sure that yours is.



---
