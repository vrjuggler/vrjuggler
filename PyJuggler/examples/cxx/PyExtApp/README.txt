Compiling
---------

Running
-------

To run using the example Python module app.ExampleModule, execute the
following (assuming a csh environment):

   % ./extApp -c standalone.jconf

For the above to work, you must have the environment variable $PYTHONPATH
set to include the directory where the PyGMTL extension module is
installed.  It must also include the directory where the application is
being run so that its subdirectory 'app' will be found correctly.  For
example, one might run this from the directory where extApp is compiled:

   % setenv PYTHONPATH /usr/local/lib/python2.2/site-packages:`pwd`

If you want to use your own module instead of app.ExampleModule, execute
the following instead:

   % setenv PYTHONPATH <your module dir>
   % ./extApp -s your.module.Here -c standalone.jconf

$Id$
