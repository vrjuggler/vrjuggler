Compiling
---------

Running
-------

To run using the example Python module app.ExampleModule, execute the
following (assuming a csh environment):

   % ./extApp -c standalone.jconf

For the above to work, you must have the environment variable $PYTHONPATH
set to include the directory where the PyGMTL extension module is
installed.

If you want to use your own module instead of app.ExampleModule, execute
the following instead:

   % setenv PYTHONPATH <your module dir>
   % ./extApp -s your.module.Here -c standalone.jconf

$Id$
