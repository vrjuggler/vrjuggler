import string
__all__     = ["vrj", "gadget", "jccl", "gmtl"]
__build__   = 1
__version__ = '0.3.0.%d' % __build__
__date__    = string.join(string.split('$Date$')[1:3], ' ')
__author__  = 'Patrick Hartling <patrick@vrjuggler.org'
__doc__     = '''This is PyJuggler.  For information regarding PyJuggler see:
    http://www.vrjuggler.org/pyjuggler/

For information on VR Juggler see:
    http://www.vrjuggler.org'''
del string
