#!/usr/bin/env python

import os
import sys
from omniORB import CORBA
import CosNaming
import tweek
import tweekfacade


def main():
   app_subj_mgr    = None

   # Start the local ORB.
   orb = CORBA.ORB_init(sys.argv, CORBA.ORB_ID)

   try:
      # Get the root POA.  We need to activate it.
      poa = orb.resolve_initial_references("RootPOA")
      poa._get_the_POAManager().activate()

      # Get the reference to the Naming Service.  If we are using omniORB-py,
      # the initial reference information will be provided in the omniORB
      # confiuration file.
      # NOTE: This information could be provided in a 'corbaloc' or 'corbaname'
      # URI constructed from command-line arguments.
      ns_obj = orb.resolve_initial_references("NameService")
      root_ctx = ns_obj._narrow(CosNaming.NamingContext)

      # All Tweek Subject Managers are registered in the 'tweek' context.
      tweek_ctx = [CosNaming.NameComponent("tweek", "context")]

      try:
         # Get the base Tweek context from the root Naming Service context.
         ctx = root_ctx.resolve(tweek_ctx)
         local_ctx = ctx._narrow(CosNaming.NamingContext)

         subj_mgrs = []

         # Ask the Tweek context for all the CosNaming.Binding instances in it.
         (binding_list, binding_iter) = local_ctx.list(100)

         print "Searching for Subject Managers",
         while binding_list != None:
            # Loop over the contents of binding_list, checking each 
            # CosNaming.Binding object to see if it is a Tweek Subject Manager.
            for binding in binding_list:
               print "\b.",
               sys.stdout.flush()

               # Debug output.
#               print binding
#               print "%s/%s" % (binding.binding_name[0].id, binding.binding_name[0].kind)

               # If the name of the current binding object starts with
               # "SubjectManager" we have found a Subject Manager reference.
               if binding.binding_name[0].id.startswith("SubjectManager"):
                  name_comp = binding.binding_name

                  # Query the Tweek context for the CosNaming.NameComponent
                  # in name_comp and try to narrow it to a tweek.SubjectManager
                  # reference.
                  try:
                     obj = local_ctx.resolve(name_comp)
                     mgr = obj._narrow(tweek.SubjectManager)

                     # If mgr is a valid reference to an existent object,
                     # append it to subj_mgrs.
                     if not mgr._non_existent():
                        print "\b+",
                        subj_mgrs.append(mgr)
                  except CosNaming.NamingContext.NotFound, ex:
                     print "Failed to narrow to tweek.SubjectManager:", ex
                  except CORBA.TRANSIENT:
                     pass

            # Continuation and error handling stuff.
            if binding_iter != None:
               (count, binding_list) = binding_iter.next_n(100)

               # If count is greater than 0, then we can continue iterating.
               # Otherwise, we have run out of CosNaming.Binding objects to
               # test.
               if count <= 0:
                  binding_iter.destroy()
                  break

         print "Done"

         for mgr in subj_mgrs:
            mgr_chosen = False

            for item in mgr.getInfo():
#               print '%s --> %s' % (item.key, item.value)
               # This helps avoid having to pick a Subject Manager explicitly,
               # though you could get a reference to someone else's Subject
               # Manager and not even know it.
               if item.key == 'OsgNav' and item.value == 'RemoteNav':
                  app_subj_mgr = mgr
                  mgr_chosen = True

                  print "Picked Subject Manager '%s':" % mgr.getName()
                  for i in mgr.getInfo():
                     print "\t%s --> %s" % (i.key, i.value)

                  break

            if mgr_chosen:
               break

      except CosNaming.NamingContext.NotFound, ex:
         print "Name not found:", ex.why

   except CORBA.ORB.InvalidName, ex:
      print "Invalid name:", ex
      print ex.why

   return app_subj_mgr

def getTweekFacade():
   if 'OMNIORB_CONFIG' not in os.environ:
      print "ERROR: $OMNIORB_CONFIG environment variable is not set!"
      return None

   app_subj_mgr = main()
   if None != app_subj_mgr:
      return tweekfacade.TweekFacade(app_subj_mgr)
   else:
      print "ERROR: No Subject Manager is available!"
      return None

if __name__ == '__main__':
   getTweekFacade()
