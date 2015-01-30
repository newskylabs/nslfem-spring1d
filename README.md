
# nslfem-spring1d

*[nslfem-spring1d]* is a simple C++ implementation of the [Finite
 Element Method (FEM)] applied to simple one-dimensional assemblages
 of linear spring elements as described in the second chapter
 *Introduction to the Stiffness (Displacement) Method* of the book *[A
 First Course in the Finite Element Method, Third
 Edition](https://openlibrary.org/books/OL6796268M/A_first_course_in_the_finite_element_method)*
 by [Darul L. Logan].


## Installation


### Download

Clone the sources of *[nslfem-spring1d]* from *github.com*:

```sh
git clone https://github.com/newskylabs/nslfem-spring1d.git
```


### Building

Change into the base directory of the source tree and type `make` to
build the executable:

```sh
cd nslfem-spring1d
make
```


## Examples

To run all examples use `make examples`:

```sh
make examples
```

The examples can also be run one by one either with

```sh
make example-2-1
make example-2-2
make example-2-3 
make example-2-4
```

or by using the executable together with the respective example file:

```sh
bin/nslfem-spring1d input-files/example-2-1.fem
bin/nslfem-spring1d input-files/example-2-2.fem
bin/nslfem-spring1d input-files/example-2-3.fem
bin/nslfem-spring1d input-files/example-2-4.fem
```


## Unit tests

In order to run the unit tests the [boost unit test framework] has to
be installed and the environment variable BOOST_ROOT has to be
defined. On my system boost is installed in `/usr/local` (the header
fieles are in `/usr/local/include/boost`; the libs are in
`/usr/local/lib`) and `BOOST_ROOT` is defined in my bash profile as

```sh
export BOOST_ROOT=/usr/local
```

To run the unit tests use

```sh
make test
```

### Cleaning up

You can remove the program binaries and object files by typing

```sh
make clean
```

To also remove the binaries use

```sh
make cleanall
```


# Examples

The following examples have been taken from the the second chapter
*"Introduction to the Stiffness (Displacement) Method"* of the book
*"[A First Course in the Finite Element Method, Third Edition]"* by
[Darul L. Logan]:


## Example 2.1

The input file `input-files/example-2-1.fem`:

```
/**
  Dietrich Bollmann, Kamakura, 2015/01/01

  Example 2.1, page 40.
  "A First Course in the Finite element Method, third edition"
  by Daryl L. Logan
*/

// Nodes:
// node <tag> (d <displacement>)? (f <force>)?

node 1  d 0
node 2  d 0
node 3
node 4       f 5000

// Spring elements:
// spring <tag>  <node 1> <node 2>  <spring constant>

spring 1  1 3  1000
spring 2  3 4  2000
spring 3  4 2  3000

// fin.
```


Running the example:

```
$ bin/nslfem-spring1d input-files/example-2-1.fem 

FEM: Spring Assemblage (1D)

Input files: 

  - input-files/example-2-1.fem

Global displacements:

  - node 1: 0
  - node 2: 0
  - node 3: 0.909091
  - node 4: 1.36364

Global forces:

  - node 1: -909.091
  - node 2: -4090.91
  - node 3: 0
  - node 4: 5000

Local forces at each element:

  - element 1: (-909.091, 909.091)
  - element 2: (-909.091, 909.091)
  - element 3: (4090.91, -4090.91)

fin.
```


## Example 2.2

The input file `input-files/example-2-2.fem`:

```
/**
  Dietrich Bollmann, Kamakura, 2015/01/01

  Example 2.2, page 43.
  "A First Course in the Finite element Method, third edition"
  by Daryl L. Logan
*/

// Nodes:
// node <tag> (d <displacement>)? (f <force>)?

node 1  d 0
node 2
node 3
node 4
node 5  d 0.02

// Spring elements:
// spring <tag>  <node 1> <node 2>  <spring constant>

spring 1  1 2  200
spring 2  2 3  200
spring 3  3 4  200
spring 4  4 5  200

// fin.
```


Running the example:

```
$ bin/nslfem-spring1d input-files/example-2-2.fem 

FEM: Spring Assemblage (1D)

Input files: 

  - input-files/example-2-2.fem

Global displacements:

  - node 1: 0
  - node 2: 0.005
  - node 3: 0.01
  - node 4: 0.015
  - node 5: 0.02

Global forces:

  - node 1: -1
  - node 2: 0
  - node 3: 0
  - node 4: 0
  - node 5: 1

Local forces at each element:

  - element 1: (-1, 1)
  - element 2: (-1, 1)
  - element 3: (-1, 1)
  - element 4: (-1, 1)

fin.
```


## Example 2.3

The input file `input-files/example-2-3.fem`:

```
/**
  Dietrich Bollmann, Kamakura, 2015/01/01

  Example 2.3, page 46.
  "A First Course in the Finite element Method, third edition"
  by Daryl L. Logan
*/

// Nodes:
// node <tag> (d <displacement>)? (f <force>)?

node 1  d 0
node 2       f 6   // Force: P = 6
node 3  d 0
node 4  d 0

// Spring elements:
// spring <tag>  <node 1> <node 2>  <spring constant>

spring 1  1 2  1   // Spring constant:  k1 = 1
spring 2  2 3  2   // Spring constant:  k2 = 2
spring 3  2 4  3   // Spring constant:  k3 = 3

// fin.
```


Running the example:

```
$ bin/nslfem-spring1d input-files/example-2-3.fem 

FEM: Spring Assemblage (1D)

Input files: 

  - input-files/example-2-3.fem

Global displacements:

  - node 1: 0
  - node 2: 1
  - node 3: 0
  - node 4: 0

Global forces:

  - node 1: -1
  - node 2: 6
  - node 3: -2
  - node 4: -3

Local forces at each element:

  - element 1: (-1, 1)
  - element 2: (2, -2)
  - element 3: (3, -3)

fin.
```


## Example 2.4

The input file `input-files/example-2-4.fem`:

```
/**
  Dietrich Bollmann, Kamakura, 2015/01/01

  Example 2.4, page 54.
  "A First Course in the Finite element Method, third edition"
  by Daryl L. Logan
*/

// Nodes:
// node <tag> (d <displacement>)? (f <force>)?

node 1  d 0
node 2       f 1000

// Spring elements:
// spring <tag>  <node 1> <node 2>  <spring constant>

spring 1  1 2  500

// fin.
```


Running the example:

```
$ bin/nslfem-spring1d input-files/example-2-4.fem 

FEM: Spring Assemblage (1D)

Input files: 

  - input-files/example-2-4.fem

Global displacements:

  - node 1: 0
  - node 2: 2

Global forces:

  - node 1: -1000
  - node 2: 1000

Local forces at each element:

  - element 1: (-1000, 1000)

fin.
```


# Comments etc.

If you have any comments, [please drop me a message]!

*fin.*

*Copyright (c) 2015 [Dietrich Bollmann]*


[nslfem-spring1d]:https://github.com/newskylabs/nslfem-spring1d
[Finite Element Method (FEM)]:http://en.wikipedia.org/wiki/Finite_element_method
[A First Course in the Finite Element Method, Third Edition]:http://openlibrary.org/books/OL6796268M/A_first_course_in_the_finite_element_method
[Darul L. Logan]:http://www.uwplatt.edu/mechanical-engineering/dr-daryl-logan-pe
[boost unit test framework]:http://www.boost.org/doc/libs/1_57_0/libs/test/doc/html/utf.html
[newskylabs]:http://newskylabs.net/
[formgames]:http://formgames.org/
[Dietrich Bollmann]:http://formgames.org/dietrich
[please drop me a message]:http://dietrich.formgames.org/email
