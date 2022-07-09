## Overview

ArgoSMT is an experimental SMT solver developed for research
purposes. The author of the solver is [Milan
Banković](http:/www.matf.bg.ac.rs/~milan/), a member of [ARGO
Group](http://argo.matf.bg.ac.rs/). The solver is released under the
[GNU GPL licence](http://www.gnu.org/licenses/gpl-3.0.en.html).

The solver currently supports EUF, LRA and LIA theories, as well as
their combinations. Other theory solvers are also planned to be
included in the future.

**IMPORTANT:** This branch has a built-in support for the trail saving
method, as described by [Hickey & Bacchus](https://link.springer.com/chapter/10.1007/978-3-030-51825-7_4).

---

## How to compile

To compile the solver (under Linux), you should run the following commands:

	tar xzvf argosmt-ts.tar.gz
	cd argosmt-ts/
	./configure --enable-hybrid-long-values
	make	

More configuration options can be inspected with:

     ./configure --help

**NOTE:** The trail saving capability is not enabled by default. To enable
it, configure the solver with the `--enable-trail-saving` option:

    ./configure --enable-hybrid-long-values --enable-trail-saving

---

## How to use

To use the solver, it is best to run it in the following way:

	./src/argosmt input_file.smt2
      

The solver accepts inputs in [SMT-LIB
2.0](https://smtlib.cs.uiowa.edu/) format (for supported theories,
only).

Option `-h` provides you with more information about the supported options. 