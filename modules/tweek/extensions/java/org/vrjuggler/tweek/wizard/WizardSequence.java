/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2007 by Iowa State University
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
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/
package org.vrjuggler.tweek.wizard;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.NoSuchElementException;

/**
 * A WizardSequence is a composite WizardStep. This allows you to group
 * WizardSteps together in such a way that with one decision, you can decide
 * whether or not to visit a whole collection of wizard steps. In this way, you
 * can create optional branches from the root wizard sequence.
 */
public abstract class WizardSequence
   extends AbstractWizardStep
{
   /**
    * Creates a new wizard sequence with no wizard steps. You cannot call this
    * directly since this class is abstract.
    */
   protected WizardSequence()
   {
      mSteps = new ArrayList();
   }

   /**
    * Tests to see if this sequence should be entered and its WizardSteps
    * processed. Implementations of this class should override this to do their
    * own custom conditional tests.
    *
    * @return  true if this sequence should be entered; false if it should be
    *          skipped
    */
   protected abstract boolean shouldEnter();

   /**
    * Adds the given wizard step to the end of this sequence.
    *
    * @param step    the step to add
    */
   public void add(WizardStep step)
   {
      mSteps.add(step);
      step.setParent(this);
   }

   /**
    * Inserts the given wizard step at the given index in this sequence.
    *
    * @param step    the step to add
    * @param idx     the index into the sequence in which to add the step
    */
   public void insert(WizardStep step, int idx)
   {
      mSteps.add(idx, step);
      step.setParent(this);
   }

   /**
    * Removes the given wizard step from this sequence. If the step is not in
    * this sequence, nothing is removed and no error is flagged.
    *
    * @param step    the step to remove
    */
   public void remove(WizardStep step)
   {
      mSteps.remove(step);
      step.setParent(null);
   }

   /**
    * Removes the wizard step at the given index from this sequence.
    *
    * @param idx     the index of the step to remove
    */
   public void remove(int idx)
   {
      WizardStep step = (WizardStep)mSteps.remove(idx);
      step.setParent(null);
   }

   /**
    * Gets the wizard step at the given index in this sequence.
    *
    * @param idx     the index of the step in the sequence
    *
    * @return  the step at the given index
    */
   public WizardStep get(int idx)
   {
      return (WizardStep)mSteps.get(idx);
   }

   /**
    * Gets the index of the given wizard step within this sequence. If the
    * step is in the sequence more than once, it's first index is returned.
    *
    * @param step    the step to search for
    *
    * @return  the index of the step if found, -1 if not found
    */
   public int getIndexOf(WizardStep step)
   {
      return mSteps.indexOf(step);
   }

   /**
    * Gets the number of wizard steps in this sequence.
    *
    * @return  the number of steps
    */
   public int size()
   {
      return mSteps.size();
   }

   /**
    * Gets the forward iterator for this sequence starting at the beginning.
    *
    * @return  a forward iterator at the beginning
    */
   public WizardStepIterator begin()
   {
      return new Itr(0);
   }

   /**
    * Gets the reverse iterator for this sequence starting at the end.
    *
    * @return  a reverse iterator at the end
    */
   public WizardStepIterator end()
   {
      return new Itr(size());
   }

   /**
    * Called when this step is about to be entered. This whiteboard allows
    * information to be passed between WizardSteps.
    */
   public void setMap(Map whiteboard)
   {
      mWhiteBoard = whiteboard;
      for (Iterator itr = mSteps.iterator() ; itr.hasNext() ; )
      {
         ((WizardStep)itr.next()).setMap(whiteboard);
      }
   }

   /**
    * The list of wizard steps in this sequence.
    */
   List mSteps;

   /**
    * Bidirectional iterator for a sequence.
    */
   private class Itr
      implements WizardStepIterator
   {
      /**
       * Index of the last item the iterator returned. This is also the current
       * item.
       */
      int mCursor;

      /**
       * Creates a new iterator to start at the given index.
       */
      public Itr(int start)
      {
         mCursor = start - 1;
      }

      public boolean hasNext()
      {
         return ((mCursor+1) != size());
      }

      public WizardStep next()
      {
         try
         {
            WizardStep next = get(++mCursor);
            return next;
         }
         catch (IndexOutOfBoundsException e)
         {
            throw new NoSuchElementException();
         }
      }

      public boolean hasPrevious()
      {
         return (mCursor != 0);
      }

      public WizardStep previous()
      {
         try
         {
            WizardStep prev = get(--mCursor);
            return prev;
         }
         catch (IndexOutOfBoundsException e)
         {
            throw new NoSuchElementException();
         }
      }
   }
}
