/***************** <Tweek heading BEGIN do not edit this line> ****************
 * Tweek
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 ***************** <Tweek heading END do not edit this line> *****************/

/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2005 by Iowa State University
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

import java.io.Serializable;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.NoSuchElementException;
import java.util.Map;
import java.awt.*;
import javax.swing.*;
import javax.swing.border.*;
import java.awt.event.*;
import info.clearthought.layout.*;

/**
 * Bean implementation of the WizardView interface. This step will allow the
 * user to step their way through a Wizard.
 */
public class WizardViewerBean
   extends JPanel
   implements Serializable, WizardView
{
   /**
    * Creates a new WizardViewer bean with no initial wizard.
    */
   public WizardViewerBean()
   {
      this(new HashMap());
   }

   /**
    * Creates a new WizardViewer bean with no initial wizard.
    *
    * @param whiteBoard WhiteBoard to store data shared between WizardSteps.
    */
   public WizardViewerBean(Map whiteBoard)
   {
      mWhiteBoard = whiteBoard;

      listeners = new ArrayList();

      try
      {
         jbInit();
      }
      catch(Exception e)
      {
         e.printStackTrace();
      }

      setSidebarImage(null);
   }

   /**
    * Gets the wizard step that this view is currently at.
    *
    * @return  the current wizard step
    *
    * @throws  NoSuchElementException
    *             if there is no current step (i.e. the wizard is empty)
    */
   public WizardStep getCurrentWizardStep()
   {
      if (mCurrentStep == null)
      {
         throw new NoSuchElementException();
      }
      return mCurrentStep;
   }

   /**
    * Tells this view to move to the next step in the wizard.
    *
    * @return  true if the move was completed successfully, false otherwise
    *
    * @throws  NoSuchElementException
    *             if the current step is the last step
    */
   public boolean moveNext()
   {
      if (null != mCurrentStep && null != mCurrentStep.getStepLabel())
      {
         mCurrentStep.getStepLabel().setForeground(Color.GRAY);
      }
      // First try to get the next step since this might throw an exception
      WizardStep destStep = mWizardIterator.next();
      if (! visit(destStep))
      {
         // The next step didn't want to be exited. Roll back our changes
         // to the iterator.
         mWizardIterator.previous();
         return false;
      }
      if (null != mCurrentStep.getStepLabel())
      {
         mCurrentStep.getStepLabel().setForeground(Color.YELLOW);
      }
      backBtn.setEnabled(hasPrev());
      if (hasNext())
      {
         nextBtn.setEnabled(true);
         finishBtn.setEnabled(false);
      }
      else
      {
         nextBtn.setEnabled(false);
         finishBtn.setEnabled(true);
      }
      return true;
   }

   /**
    * Tells this view to move to the previous step in the wizard.
    *
    * @return  true if the move was completed successfully, false otherwise
    *
    * @throws  NoSuchElementException
    *             if the current step is the first step
    */
   public boolean movePrev()
   {
      if (null != mCurrentStep && null != mCurrentStep.getStepLabel())
      {
         mCurrentStep.getStepLabel().setForeground(Color.BLACK);
      }
      // First try to get the previous step since this might throw an exception
      WizardStep destStep = mWizardIterator.previous();
      if (! visit(destStep))
      {
         // The previous step didn't want to be exited. Roll back our changes
         // to the iterator.
         mWizardIterator.next();
         return false;
      }
      if (null != mCurrentStep.getStepLabel())
      {
         mCurrentStep.getStepLabel().setForeground(Color.YELLOW);
      }
      backBtn.setEnabled(hasPrev());
      finishBtn.setEnabled(false);
      nextBtn.setEnabled(hasNext());
      return true;
   }

   boolean first = true;

   /**
    * Exits the current step and enters the given step.
    *
    * @return  true if the visit was completed successfully, false if the
    *          current step didn't want to be exited
    */
   private boolean visit(WizardStep nextStep)
   {
      // Only exit the current step if we're actually in a step
      if (mCurrentStep != null)
      {
         // Exit the current step
         if (! mCurrentStep.onExiting())
         {
            // The step didn't want to be exited. Put the iterator back where
            // it was to rollback our changes.
            mWizardIterator.next();
            return false;
         }
         // TODO: don't assume the step is a pane
         WizardPane pane = (WizardPane)mCurrentStep;
         this.remove(pane.getGUI());

         // Revalidate the layout since this is not done automagically for us
         // for some reason. Otherwise, this component does not get repainted
         // with the new wizard pane component contained within.
         revalidate();
         repaint();
         mCurrentStep.onExited();
      }

      // Enter the next step
      mCurrentStep = nextStep;

      // Only enter the next step if there actually is one. We need this test
      // for the case where we're leaving the last step from the user clicking
      // the Finish button and visit(null) is called.
      if (mCurrentStep != null)
      {
         mCurrentStep.setMap(mWhiteBoard);

         mCurrentStep.onEntering();
         // TODO: don't assume the step is a pane
         WizardPane pane = (WizardPane)nextStep;
         this.add(pane.getGUI(), BorderLayout.CENTER);
         mCurrentStep.onEntered();
      }

      return true;
   }

   /**
    * Checks if the wizard has a step previous to the current one.
    *
    * @return  true if there is a previous step; false otherwise
    */
   public boolean hasPrev()
   {
      return mWizardIterator.hasPrevious();
   }

   /**
    * Checks if the wizard has a step after to the current one.
    *
    * @return  true if there is a next step; false otherwise
    */
   public boolean hasNext()
   {
      return mWizardIterator.hasNext();
   }

   /**
    * Sets the wizard that this bean views.
    *
    * @param wizard     the wizard to view
    */
   public void setWizard(Wizard wizard)
   {
      this.wizard = wizard;
      if (wizard != null)
      {
         mWizardIterator = wizard.iterator();
         backBtn.setEnabled(false);
         nextBtn.setEnabled(false);
         finishBtn.setEnabled(false);
         
         for (WizardStepIterator itr = wizard.iterator() ; itr.hasNext() ; )
         {
            WizardStep step = (WizardStep)itr.next();
            mStepsPanel.add(step.getStepLabel());
         }

         // Display the first step if the wizard is not empty
         if (mWizardIterator.hasNext())
         {
            moveNext();
         }

         // Change the wizard title to reflect the new wizard
         titleLbl.setText(wizard.getTitle());
         fireWizardStarted();
      }
   }

   /**
    * Gets the wizard that this bean is viewing.
    *
    * @return  the wizard being viewed
    */
   public Wizard getWizard()
   {
      return wizard;
   }

   /**
    * Sets the sidebar image.
    */
   public void setSidebarImage(ImageIcon img)
   {
      this.mSidebarImage = img;
      imageLbl.setIcon(img);
   }

   /**
    * Gets the sidebar image.
    */
   public ImageIcon getSidebarImage()
   {
      return mSidebarImage;
   }

   /**
    * Adds the given wizard view listener to this wizard view.
    */
   public void addWizardViewListener(WizardViewListener listener)
   {
      listeners.add(listener);
   }

   /**
    * Removes the given wizard view listener from this wizard view.
    */
   public void removeWizardViewListener(WizardViewListener listener)
   {
      listeners.remove(listener);
   }

   /**
    * Notifies all listeners that this view has started.
    */
   protected void fireWizardStarted()
   {
      WizardViewEvent evt = new WizardViewEvent(this);
      Iterator itr = listeners.iterator();
      while (itr.hasNext())
      {
         ((WizardViewListener)itr.next()).wizardStarted(evt);
      }
   }

   /**
    * Notifies all listeners that this view has finished.
    */
   protected void fireWizardFinished()
   {
      WizardViewEvent evt = new WizardViewEvent(this);
      Iterator itr = listeners.iterator();
      while (itr.hasNext())
      {
         ((WizardViewListener)itr.next()).wizardFinished(evt);
      }
   }

   /**
    * Notifies all listeners that this view has been cancelled.
    */
   protected void fireWizardCancelled()
   {
      WizardViewEvent evt = new WizardViewEvent(this);
      Iterator itr = listeners.iterator();
      while (itr.hasNext())
      {
         ((WizardViewListener)itr.next()).wizardCancelled(evt);
      }
   }

   /**
    * The wizard this bean is viewing.
    */
   private Wizard wizard = null;

   /**
    * The iterator being used to traverse the wizard.
    */
   private WizardStepIterator mWizardIterator = null;

   /**
    * Used to share information between wizard steps.
    */
   private Map mWhiteBoard = null;

   /**
    * Cache of the current step being viewed.
    */
   private WizardStep mCurrentStep = null;

   /**
    * The image that amkes up this wizard viewer's sidebar.
    */
   private ImageIcon mSidebarImage = null;

   /**
    * The objects listening to this wizard view.
    */
   private List listeners;

   //---- Begin JBuilder UI elements ----//
   BorderLayout baseLayout = new BorderLayout();
   JPanel buttonPnl = new JPanel();
   FlowLayout buttonLayout = new FlowLayout();
   JButton nextBtn = new JButton();
   JButton backBtn = new JButton();
   JButton finishBtn = new JButton();
   JButton cancelBtn = new JButton();
   JLayeredPane mLayeredPane = new JLayeredPane();
   JPanel mStepsPanel = new JPanel();
   JLabel imageLbl = new JLabel();
   JLabel titleLbl = new JLabel();

   private void jbInit() throws Exception
   {
      double[][] main_size =
      {
         {TableLayout.FILL},
         {115, TableLayout.FILL}
      };
      TableLayout tl = new TableLayout(main_size);
      mLayeredPane.setLayout(tl);

      mStepsPanel.setLayout(new BoxLayout(mStepsPanel, BoxLayout.PAGE_AXIS));
      mStepsPanel.setOpaque(false);
      mLayeredPane.add(mStepsPanel, new TableLayoutConstraints(0, 1, 0, 1,
                              TableLayout.LEFT,
                              TableLayout.TOP));
      mLayeredPane.putLayer(mStepsPanel, JLayeredPane.PALETTE_LAYER.intValue());
      
      mLayeredPane.add(imageLbl, new TableLayoutConstraints(0, 0, 0, 1,
                           TableLayout.LEFT,
                           TableLayout.TOP));
      mLayeredPane.putLayer(imageLbl, JLayeredPane.DEFAULT_LAYER.intValue());
            
      this.setLayout(baseLayout);
      buttonPnl.setLayout(buttonLayout);
      buttonLayout.setAlignment(FlowLayout.RIGHT);
      nextBtn.setText("Next >");
      nextBtn.setEnabled(false);
      nextBtn.addActionListener(new java.awt.event.ActionListener()
      {
         public void actionPerformed(ActionEvent e)
         {
            nextBtn_actionPerformed(e);
         }
      });
      backBtn.setText("< Back");
      backBtn.setEnabled(false);
      backBtn.addActionListener(new java.awt.event.ActionListener()
      {
         public void actionPerformed(ActionEvent e)
         {
            backBtn_actionPerformed(e);
         }
      });
      finishBtn.setText("Finish");
      finishBtn.setEnabled(false);
      finishBtn.addActionListener(new java.awt.event.ActionListener()
      {
         public void actionPerformed(ActionEvent e)
         {
            visit(null);
            fireWizardFinished();
         }
      });
      cancelBtn.setText("Cancel");
      cancelBtn.addActionListener(new java.awt.event.ActionListener()
      {
         public void actionPerformed(ActionEvent e)
         {
            fireWizardCancelled();
         }
      });
      buttonPnl.setBorder(BorderFactory.createEtchedBorder());
      imageLbl.setVerticalAlignment(SwingConstants.TOP);
      this.setPreferredSize(new Dimension(590, 430));
      baseLayout.setHgap(5);
      titleLbl.setFont(new java.awt.Font("Serif", 1, 20));
      titleLbl.setBorder(BorderFactory.createRaisedBevelBorder());
      titleLbl.setHorizontalAlignment(SwingConstants.RIGHT);
      buttonPnl.add(cancelBtn, null);
      buttonPnl.add(backBtn, null);
      this.add(buttonPnl, BorderLayout.SOUTH);
      buttonPnl.add(nextBtn, null);
      buttonPnl.add(finishBtn, null);
      this.add(mLayeredPane/*imageLbl*/, BorderLayout.WEST);
      this.add(titleLbl, BorderLayout.NORTH);
   }

   /**
    * Handles event asking to move to the next step.
    */
   void nextBtn_actionPerformed(ActionEvent e)
   {
      try
      {
         moveNext();
      }
      catch (NoSuchElementException ex)
      {
         ex.printStackTrace();
      }
   }

   /**
    * Handles event asking to move to the previous step.
    */
   void backBtn_actionPerformed(ActionEvent e)
   {
      try
      {
         movePrev();
      }
      catch (NoSuchElementException ex)
      {
         ex.printStackTrace();
      }
   }
}
