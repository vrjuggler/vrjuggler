/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2010 by Iowa State University
 *
 * Original Authors:
 *   Allen Bierbaum, Christopher Just,
 *   Patrick Hartling, Kevin Meinert,
 *   Carolina Cruz-Neira, Albert Baker
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

package org.vrjuggler.tweek.wizard;

import java.util.ArrayList;
import java.util.List;
import java.util.NoSuchElementException;

/**
 * This is an iterator that can traverse over the entireity of a Wizard. This
 * iterator correctly deals with sequence steps while it iterates over the
 * wizard and enters them only if needed.
 */
public class WizardIterator
   implements WizardStepIterator
{
   /**
    * Creates a new Wizard iterator beginning on the given WizardStepIterator.
    *
    * @param itr     the iterator at the beginning of the wizard
    */
   public WizardIterator(WizardStepIterator itr)
   {
      mIteratorStack = new ArrayList();
      mIteratorStack.add(itr);
   }

   /**
    * Checks if this iterator has a next step.
    *
    * @return true if there are more steps, false otherwise
    */
   public boolean hasNext()
   {
      // wind our way up the stack checking each iterator in turn
      for (int i = mIteratorStack.size() - 1; i >= 0; --i)
      {
         // Check if the next iterator has a next step
         WizardStepIterator itr = (WizardStepIterator)mIteratorStack.get(i);
         if (itr.hasNext())
         {
            return true;
         }
      }

      // none of the iterators on our stack has a next step
      return false;
   }

   /**
    * Moves this iterator to the next WizardStep and returns it.
    *
    * @return  the next WizardStep
    *
    * @throws  NoSuchElementException
    *             if the iteration has no next step
    */
   public WizardStep next()
   {
      // See if there is anything next
      if (! hasNext())
      {
         throw new NoSuchElementException("This iterator has no more elements");
      }

      // Wind our way down the stack looking for an iterator with a next step
      for (int i = mIteratorStack.size() - 1; i >= 0; --i)
      {
         // Move along this iterator until we find a step to stop on. Since any
         // given step may be a sequence and sequences can be skipped, we may
         // have to move over several steps until we find one that we should
         // stop on.
         WizardStepIterator itr = (WizardStepIterator)mIteratorStack.get(i);
         while (itr.hasNext())
         {
            WizardStep step = (WizardStep)itr.next();

            // We handle sequences as a special case
            if (step instanceof WizardSequence)
            {
               WizardSequence sequence = (WizardSequence)step;
               // Check if we should visit this sequence
               if (sequence.shouldEnter())
               {
                  // Push this sequence's iterator on the stack and recurse
                  mIteratorStack.add(sequence.begin());
                  try
                  {
                     return next();
                  }
                  catch (NoSuchElementException e)
                  {
                     // The iterator did not actually contain any useful steps
                     continue;
                  }
               }
               else
               {
                  // We shouldn't visit this sequence, so just continue
                  continue;
               }
            }
            // Just a regular step, return it
            else
            {
               return step;
            }
         }
      }

      // If we made it here, we didn't find a next step
      throw new NoSuchElementException();
   }

   /**
    * Checks if this iterator has a previous step.
    *
    * @return true if there are more steps, false otherwise
    */
   public boolean hasPrevious()
   {
      // wind our way up the stack checking each iterator in turn
      for (int i = mIteratorStack.size() - 1; i >= 0; --i)
      {
         // Check if the next iterator has any previous elements
         WizardStepIterator itr = (WizardStepIterator)mIteratorStack.get(i);
         if (itr.hasPrevious())
         {
            return true;
         }
      }

      // none of the iterators on our stack has a previous step
      return false;
   }

   /**
    * Moves this iterator to the previous WizardStep and returns it.
    *
    * @return  the previous WizardStep
    *
    * @throws  NoSuchElementException
    *             if the iteration has no previous step
    */
   public WizardStep previous()
   {
      // See if there is anything previous
      if (! hasPrevious())
      {
         throw new NoSuchElementException("This iterator has no more elements");
      }

      // Wind our way down the stack looking for an iterator with a prev step
      for (int i = mIteratorStack.size() - 1; i >= 0; --i)
      {
         // Move along this iterator until we find a step to stop on. Since any
         // given step may be a sequence and sequences can be skipped, we may
         // have to move over several steps until we find one that we should
         // stop on.
         WizardStepIterator itr = (WizardStepIterator)mIteratorStack.get(i);
         while (itr.hasPrevious())
         {
            WizardStep step = (WizardStep)itr.previous();

            // We handle sequences as a special case
            if (step instanceof WizardSequence)
            {
               WizardSequence sequence = (WizardSequence)step;
               // Check if we should visit this sequence
               if (sequence.shouldEnter())
               {
                  // Push this sequence's iterator on the stack and recurse
                  mIteratorStack.add(sequence.end());
                  try
                  {
                     return previous();
                  }
                  catch (NoSuchElementException e)
                  {
                     // The iterator did not actually contain any useful steps
                     continue;
                  }
               }
               else
               {
                  // We shouldn't visit this sequence, so just continue
                  continue;
               }
            }
            // Just a regular step, return it
            else
            {
               return step;
            }
         }
      }

      // If we made it here, we didn't find a next step
      throw new NoSuchElementException();
   }

   /**
    * The stack of iterators we've gotten for each embedded sequence.
    */
   private List mIteratorStack;
}
