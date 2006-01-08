/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2006 by Iowa State University
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
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/
package org.vrjuggler.vrjconfig.customeditors.intersense;

import javax.swing.table.AbstractTableModel;

/**
 * Adaptor which provides the necessary data to a JTable when 
 * given an IntersenseModel.
 *
 * @see AbstractTableModel
 * @see IntersenseModel
 */
public class StationTableModelAdaptor extends AbstractTableModel
{
   /** Model of the entire Intersense tracking device. */
   private IntersenseModel mISenseModel = null;
  
   /**
    * Create an adaptor to give a JTable all data needed 
    * to display a table.
    */
   public StationTableModelAdaptor(IntersenseModel model)
   {
      mISenseModel = model;
   }

   /**
    * Return the number of rows to display in table.
    */
   public int getRowCount()
   {
      return getNumStations();
   }

   /**
    * Return the number of columns to display in table.
    */
   public int getColumnCount()
   {
      return 1;
   }

   /**
    * Return the StationModel for the given row.
    *
    * @see StationModel
    */
   public Object getValueAt(int row, int column)
   {
      return mISenseModel.getStationModels().get(row);
   }
   
   /**
    * Return the number of Intersense stations displayed in table.
    */
   public int getNumStations()
   {
      return mISenseModel.getStationModels().size();
   }
}
