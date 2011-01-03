/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2011 by Iowa State University
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

package org.vrjuggler.vrjconfig.commoneditors;

import java.awt.*;
import javax.swing.*;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.util.HashMap;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.Collection;

import info.clearthought.layout.*;

public class KeyboardEditorPanel extends JPanel
{
   private HashMap mKeyMap = new HashMap();

   private JToggleButton A = new JToggleButton();
   private JToggleButton B = new JToggleButton();
   private JToggleButton C = new JToggleButton();
   private JToggleButton D = new JToggleButton();
   private JToggleButton E = new JToggleButton();
   private JToggleButton F = new JToggleButton();
   private JToggleButton G = new JToggleButton();
   private JToggleButton H = new JToggleButton();
   private JToggleButton I = new JToggleButton();
   private JToggleButton J = new JToggleButton();
   private JToggleButton K = new JToggleButton();
   private JToggleButton L = new JToggleButton();
   private JToggleButton M = new JToggleButton();
   private JToggleButton N = new JToggleButton();
   private JToggleButton O = new JToggleButton();
   private JToggleButton P = new JToggleButton();
   private JToggleButton Q = new JToggleButton();
   private JToggleButton R = new JToggleButton();
   private JToggleButton S = new JToggleButton();
   private JToggleButton T = new JToggleButton();
   private JToggleButton U = new JToggleButton();
   private JToggleButton V = new JToggleButton();
   private JToggleButton W = new JToggleButton();
   private JToggleButton X = new JToggleButton();
   private JToggleButton Y = new JToggleButton();
   private JToggleButton Z = new JToggleButton();
   private JToggleButton forward_slash = new JToggleButton();
   private JToggleButton greater = new JToggleButton();
   private JToggleButton less = new JToggleButton();
   private JToggleButton semi = new JToggleButton();

   public KeyboardEditorPanel()
   {
      try
      {
         jbInit();
         setup();
         this.requestFocus();
      }
      catch(Exception ex)
      {
         ex.printStackTrace();
      }
   }

   private class Focuser extends MouseAdapter
   {
      JPanel mPanel = null;

      public Focuser(JPanel panel)
      {
         mPanel = panel;
      }
      public void mousePressed(MouseEvent event)
      {
         mPanel.requestFocus();
      }
   }

   private Focuser mFocuser = new Focuser(this);
   private JToggleButton Control2 = new JToggleButton();
   private JToggleButton LeftClick = new JToggleButton();
   private JToggleButton Win2 = new JToggleButton();
   private JToggleButton Alt2 = new JToggleButton();
   private JToggleButton Space = new JToggleButton();
   private JToggleButton Alt1 = new JToggleButton();
   private JToggleButton Win1 = new JToggleButton();
   private JToggleButton Control1 = new JToggleButton();
   private JToggleButton Shift2 = new JToggleButton();
   private JToggleButton Shift1 = new JToggleButton();
   private JToggleButton Enter1 = new JToggleButton();
   private JToggleButton Quote = new JToggleButton();
   private JToggleButton CapsLock = new JToggleButton();
   private JToggleButton bslash = new JToggleButton();
   private JToggleButton rbracket = new JToggleButton();
   private JToggleButton lbracket = new JToggleButton();
   private JToggleButton Tab = new JToggleButton();
   private JToggleButton BackSpace = new JToggleButton();
   private JToggleButton Equals = new JToggleButton();
   private JToggleButton Dash = new JToggleButton();
   private JToggleButton B0 = new JToggleButton();
   private JToggleButton B9 = new JToggleButton();
   private JToggleButton B8 = new JToggleButton();
   private JToggleButton B7 = new JToggleButton();
   private JToggleButton B6 = new JToggleButton();
   private JToggleButton B5 = new JToggleButton();
   private JToggleButton B4 = new JToggleButton();
   private JToggleButton B3 = new JToggleButton();
   private JToggleButton B2 = new JToggleButton();
   private JToggleButton B1 = new JToggleButton();
   private JToggleButton Tilde = new JToggleButton();
   private JToggleButton F12 = new JToggleButton();
   private JToggleButton F11 = new JToggleButton();
   private JToggleButton F10 = new JToggleButton();
   private JToggleButton F9 = new JToggleButton();
   private JToggleButton F8 = new JToggleButton();
   private JToggleButton F7 = new JToggleButton();
   private JToggleButton F6 = new JToggleButton();
   private JToggleButton F5 = new JToggleButton();
   private JToggleButton F4 = new JToggleButton();
   private JToggleButton F3 = new JToggleButton();
   private JToggleButton F2 = new JToggleButton();
   private JToggleButton F1 = new JToggleButton();
   private JToggleButton Esc = new JToggleButton();

   private void setup()
   {
      this.addKeyListener(new KeyAdapter()
      {
         public void keyPressed(KeyEvent e)
         {
           reset();

           System.out.println(e.getModifiers());
           if(KeyEvent.VK_SHIFT == e.getKeyCode() ||
              KeyEvent.VK_CONTROL == e.getKeyCode() ||
              KeyEvent.VK_ALT == e.getKeyCode())
           {
             return;
           }
           if(e.isShiftDown())
           {
             Shift1.doClick();
             Shift2.doClick();
           }
           if(e.isControlDown())
           {
             Control1.doClick();
             Control2.doClick();
           }
           if(e.isAltDown())
           {
             Alt1.doClick();
             Alt2.doClick();
           }
/*
           else if(KeyEvent.VK_ == e.getKeyCode())
           {
              Control1.doClick();
              Control2.doClick();
           }
*/
           Integer test = new Integer(e.getKeyCode());
           Object obj = mKeyMap.get(test);
           if (null != obj)
           {
             JToggleButton button = (JToggleButton) obj;
             button.doClick();
           }
         }
      });
      this.addMouseListener(mFocuser);
   }

   private void jbInit() throws Exception
   {
      A.setRequestFocusEnabled(false);
      A.setMargin(new Insets(2, 2, 2, 2));
      A.setText("A");

      // Create a TableLayout for the frame
      double border = 10;
      double width = 0.07;
      double height = 0.16;
//    double size[][] =
//       {{ width, width, width, width, width, width, width, width, width, width, width, width, width, width },  // Columns
//        {{ 0.5, 0.5 },  // Columns
//         {0.5, 0.5}}; // Rows
      double size[][] =
         {{-2.0, width, width, width, width, width, width, width, width, width, width, width, width, width, width},
          {-2.0, height, height, height, height, height, height}
         };

      this.setLayout(new TableLayout(size));
      G.setRequestFocusEnabled(false);
      G.setMargin(new Insets(2, 2, 2, 2));
      G.setText("G");
      F.setRequestFocusEnabled(false);
      F.setMargin(new Insets(2, 2, 2, 2));
      F.setText("F");
      E.setRequestFocusEnabled(false);
      E.setMargin(new Insets(2, 2, 2, 2));
      E.setText("E");
      D.setRequestFocusEnabled(false);
      D.setMargin(new Insets(2, 2, 2, 2));
      D.setText("D");
      C.setRequestFocusEnabled(false);
      C.setMargin(new Insets(2, 2, 2, 2));
      C.setText("C");
      B.setRequestFocusEnabled(false);
      B.setMargin(new Insets(2, 2, 2, 2));
      B.setText("B");

      H.setRequestFocusEnabled(false);
      H.setMargin(new Insets(2, 2, 2, 2));
      H.setText("H");
      I.setRequestFocusEnabled(false);
      I.setMargin(new Insets(2, 2, 2, 2));
      I.setText("I");
      J.setRequestFocusEnabled(false);
      J.setMargin(new Insets(2, 2, 2, 2));
      J.setText("J");
      K.setRequestFocusEnabled(false);
      K.setMargin(new Insets(2, 2, 2, 2));
      K.setText("K");
      L.setRequestFocusEnabled(false);
      L.setMargin(new Insets(2, 2, 2, 2));
      L.setText("L");
      M.setRequestFocusEnabled(false);
      M.setMargin(new Insets(2, 2, 2, 2));
      M.setText("M");
      N.setRequestFocusEnabled(false);
      N.setMargin(new Insets(2, 2, 2, 2));
      N.setText("N");
      O.setRequestFocusEnabled(false);
      O.setMargin(new Insets(2, 2, 2, 2));
      O.setText("O");
      P.setRequestFocusEnabled(false);
      P.setMargin(new Insets(2, 2, 2, 2));
      P.setText("P");
      Q.setRequestFocusEnabled(false);
      Q.setMargin(new Insets(2, 2, 2, 2));
      Q.setText("Q");
      R.setRequestFocusEnabled(false);
      R.setMargin(new Insets(2, 2, 2, 2));
      R.setText("R");
      S.setRequestFocusEnabled(false);
      S.setMargin(new Insets(2, 2, 2, 2));
      S.setText("S");
      T.setRequestFocusEnabled(false);
      T.setMargin(new Insets(2, 2, 2, 2));
      T.setText("T");
      U.setRequestFocusEnabled(false);
      U.setMargin(new Insets(2, 2, 2, 2));
      U.setText("U");
      V.setRequestFocusEnabled(false);
      V.setMargin(new Insets(2, 2, 2, 2));
      V.setText("V");
      W.setRequestFocusEnabled(false);
      W.setMargin(new Insets(2, 2, 2, 2));
      W.setText("W");
      X.setRequestFocusEnabled(false);
      X.setMargin(new Insets(2, 2, 2, 2));
      X.setText("X");
      Y.setRequestFocusEnabled(false);
      Y.setMargin(new Insets(2, 2, 2, 2));
      Y.setText("Y");
      Z.setRequestFocusEnabled(false);
      Z.setMargin(new Insets(2, 2, 2, 2));
      Z.setText("Z");
      forward_slash.setRequestFocusEnabled(false);
      forward_slash.setMargin(new Insets(2, 2, 2, 2));
      forward_slash.setText("/");
      greater.setRequestFocusEnabled(false);
      greater.setMargin(new Insets(2, 2, 2, 2));
      greater.setText(">");
      less.setRequestFocusEnabled(false);
      less.setMargin(new Insets(2, 2, 2, 2));
      less.setText("<");
      semi.setRequestFocusEnabled(false);
      semi.setMargin(new Insets(2, 2, 2, 2));
      semi.setText(";");

      Control2.setText("Ctl");
      Control2.setRequestFocusEnabled(false);
      Control2.setMargin(new Insets(2, 2, 2, 2));
      LeftClick.setText("LC");
      LeftClick.setRequestFocusEnabled(false);
      LeftClick.setMargin(new Insets(2, 2, 2, 2));
      Win2.setText("Win");
      Win2.setRequestFocusEnabled(false);
      Win2.setMargin(new Insets(2, 2, 2, 2));
      Alt2.setText("Alt");
      Alt2.setRequestFocusEnabled(false);
      Alt2.setMargin(new Insets(2, 2, 2, 2));
      Space.setRequestFocusEnabled(false);
      Space.setMargin(new Insets(2, 2, 2, 2));
      Space.setText("");
      Alt1.setText("Alt");
      Alt1.setRequestFocusEnabled(false);
      Alt1.setMargin(new Insets(2, 2, 2, 2));
      Win1.setText("Win");
      Win1.setRequestFocusEnabled(false);
      Win1.setMargin(new Insets(2, 2, 2, 2));
      Control1.setText("Ctl");
      Control1.setRequestFocusEnabled(false);
      Control1.setMargin(new Insets(2, 2, 2, 2));
      Shift2.setText("Shift");
      Shift2.setRequestFocusEnabled(false);
      Shift2.setMargin(new Insets(2, 2, 2, 2));
      Shift1.setText("Shift");
      Shift1.setRequestFocusEnabled(false);
      Shift1.setMargin(new Insets(2, 2, 2, 2));
      Enter1.setText("Enter");
      Enter1.setRequestFocusEnabled(false);
      Enter1.setMargin(new Insets(2, 2, 2, 2));
      Quote.setText("\"");
      Quote.setRequestFocusEnabled(false);
      Quote.setMargin(new Insets(2, 2, 2, 2));
      CapsLock.setText("CL");
      CapsLock.setRequestFocusEnabled(false);
      CapsLock.setMargin(new Insets(2, 2, 2, 2));
      bslash.setText("\\");
      bslash.setRequestFocusEnabled(false);
      bslash.setMargin(new Insets(2, 2, 2, 2));
      rbracket.setText("]");
      rbracket.setRequestFocusEnabled(false);
      rbracket.setMargin(new Insets(2, 2, 2, 2));
      lbracket.setText("[");
      lbracket.setRequestFocusEnabled(false);
      lbracket.setMargin(new Insets(2, 2, 2, 2));
      Tab.setText("Tab");
      Tab.setRequestFocusEnabled(false);
      Tab.setMargin(new Insets(2, 2, 2, 2));
      BackSpace.setText("<-");
      BackSpace.setRequestFocusEnabled(false);
      BackSpace.setMargin(new Insets(2, 2, 2, 2));
      Equals.setText("=");
      Equals.setRequestFocusEnabled(false);
      Equals.setMargin(new Insets(2, 2, 2, 2));
      Dash.setText("-");
      Dash.setRequestFocusEnabled(false);
      Dash.setMargin(new Insets(2, 2, 2, 2));
      B0.setText("0");
      B0.setRequestFocusEnabled(false);
      B0.setMargin(new Insets(2, 2, 2, 2));
      B9.setText("9");
      B9.setRequestFocusEnabled(false);
      B9.setMargin(new Insets(2, 2, 2, 2));
      B8.setText("8");
      B8.setRequestFocusEnabled(false);
      B8.setMargin(new Insets(2, 2, 2, 2));
      B7.setText("7");
      B7.setRequestFocusEnabled(false);
      B7.setMargin(new Insets(2, 2, 2, 2));
      B6.setText("6");
      B6.setRequestFocusEnabled(false);
      B6.setMargin(new Insets(2, 2, 2, 2));
      B5.setText("5");
      B5.setRequestFocusEnabled(false);
      B5.setMargin(new Insets(2, 2, 2, 2));
      B4.setText("4");
      B4.setRequestFocusEnabled(false);
      B4.setMargin(new Insets(2, 2, 2, 2));
      B3.setText("3");
      B3.setRequestFocusEnabled(false);
      B3.setMargin(new Insets(2, 2, 2, 2));
      B2.setText("2");
      B2.setRequestFocusEnabled(false);
      B2.setMargin(new Insets(2, 2, 2, 2));
      B1.setText("1");
      B1.setRequestFocusEnabled(false);
      B1.setMargin(new Insets(2, 2, 2, 2));
      Tilde.setText("`");
      Tilde.setRequestFocusEnabled(false);
      Tilde.setMargin(new Insets(2, 2, 2, 2));
      F12.setText("F12");
      F12.setRequestFocusEnabled(false);
      F12.setMargin(new Insets(2, 2, 2, 2));
      F11.setText("F11");
      F11.setRequestFocusEnabled(false);
      F11.setMargin(new Insets(2, 2, 2, 2));
      F10.setText("F10");
      F10.setRequestFocusEnabled(false);
      F10.setMargin(new Insets(2, 2, 2, 2));
      F9.setText("F9");
      F9.setRequestFocusEnabled(false);
      F9.setMargin(new Insets(2, 2, 2, 2));
      F8.setText("F8");
      F8.setRequestFocusEnabled(false);
      F8.setMargin(new Insets(2, 2, 2, 2));
      F7.setText("F7");
      F7.setRequestFocusEnabled(false);
      F7.setMargin(new Insets(2, 2, 2, 2));
      F6.setText("F6");
      F6.setRequestFocusEnabled(false);
      F6.setMargin(new Insets(2, 2, 2, 2));
      F5.setText("F5");
      F5.setRequestFocusEnabled(false);
      F5.setMargin(new Insets(2, 2, 2, 2));
      F4.setText("F4");
      F4.setRequestFocusEnabled(false);
      F4.setMargin(new Insets(2, 2, 2, 2));
      F3.setText("F3");
      F3.setRequestFocusEnabled(false);
      F3.setMargin(new Insets(2, 2, 2, 2));
      F2.setText("F2");
      F2.setRequestFocusEnabled(false);
      F2.setMargin(new Insets(2, 2, 2, 2));
      F1.setText("F1");
      F1.setRequestFocusEnabled(false);
      F1.setMargin(new Insets(2, 2, 2, 2));
      Esc.setText("Esc");
      Esc.setRequestFocusEnabled(false);
      Esc.setMargin(new Insets(2, 2, 2, 2));
      this.add(Esc, "1, 1");
      this.add(F1, "2, 1");
      this.add(F2, "3, 1");
      this.add(F3, "4, 1");
      this.add(F4, "5, 1");
      this.add(F5, "6, 1");
      this.add(F6, "7, 1");
      this.add(F7, "8, 1");
      this.add(F8, "9, 1");
      this.add(F9, "10, 1");
      this.add(F10, "11, 1");
      this.add(F11, "12, 1");
      this.add(F12, "13, 1");
      this.add(Tilde, "1, 2");
      this.add(B1, "2, 2");
      this.add(B2, "3, 2");
      this.add(B3, "4, 2");
      this.add(B4, "5, 2");
      this.add(B5, "6, 2");
      this.add(B6, "7, 2");
      this.add(B7, "8, 2");
      this.add(B8, "9, 2");
      this.add(B9, "10, 2");
      this.add(B0, "11, 2");
      this.add(Dash, "12, 2");
      this.add(Equals, "13, 2");
      this.add(BackSpace, "14, 2");
      this.add(Tab, "1, 3");
      this.add(Q, "2, 3");
      this.add(W, "3, 3");
      this.add(E, "4, 3");
      this.add(R, "5, 3");
      this.add(T, "6, 3");
      this.add(Y, "7, 3");
      this.add(U, "8, 3");
      this.add(I, "9, 3");
      this.add(O, "10, 3");
      this.add(P, "11, 3");
      this.add(lbracket, "12, 3");
      this.add(rbracket, "13, 3");
      this.add(bslash, "14, 3");
      this.add(CapsLock, "1, 4");
      this.add(A, "2, 4");
      this.add(S, "3, 4");
      this.add(D, "4, 4");
      this.add(F, "5, 4");
      this.add(G, "6, 4");
      this.add(H, "7, 4");
      this.add(J, "8 ,4");
      this.add(K, "9, 4");
      this.add(L, "10, 4");
      this.add(semi, "11, 4");
      this.add(Quote, "12, 4");
      this.add(Enter1, "13, 4, 14, 4");
      this.add(Shift1, "1, 5, 2, 5");
      this.add(Z, "3, 5");
      this.add(X, "4, 5");
      this.add(C, "5, 5");
      this.add(V, "6, 5");
      this.add(B, "7, 5");
      this.add(N, "8, 5");
      this.add(M, "9, 5");
      this.add(less, "10, 5");
      this.add(greater, "11, 5");
      this.add(forward_slash, "12, 5");
      this.add(Shift2, "13, 5, 14, 5");
      this.add(Control1, "1, 6, 2, 6");
      this.add(Win1, "3, 6");
      this.add(Alt1, "4, 6");
      this.add(Space, "5, 6, 10, 6");
      this.add(Alt2, "11, 6");
      this.add(Win2, "12, 6");
      this.add(LeftClick, "13, 6");
      this.add(Control2, "14, 6");

      // Row One
      mKeyMap.put(new Integer(KeyEvent.VK_ESCAPE), Esc);
      mKeyMap.put(new Integer(KeyEvent.VK_F1), F1);
      mKeyMap.put(new Integer(KeyEvent.VK_F2), F2);
      mKeyMap.put(new Integer(KeyEvent.VK_F3), F3);
      mKeyMap.put(new Integer(KeyEvent.VK_F4), F4);
      mKeyMap.put(new Integer(KeyEvent.VK_F5), F5);
      mKeyMap.put(new Integer(KeyEvent.VK_F6), F6);
      mKeyMap.put(new Integer(KeyEvent.VK_F7), F7);
      mKeyMap.put(new Integer(KeyEvent.VK_F8), F8);
      mKeyMap.put(new Integer(KeyEvent.VK_F9), F9);
      mKeyMap.put(new Integer(KeyEvent.VK_F10), F10);
      mKeyMap.put(new Integer(KeyEvent.VK_F11), F11);
      mKeyMap.put(new Integer(KeyEvent.VK_F12), F12);

      // Row Two
      mKeyMap.put(new Integer(KeyEvent.VK_BACK_QUOTE), Tilde);
      mKeyMap.put(new Integer(KeyEvent.VK_1), B1);
      mKeyMap.put(new Integer(KeyEvent.VK_2), B2);
      mKeyMap.put(new Integer(KeyEvent.VK_3), B3);
      mKeyMap.put(new Integer(KeyEvent.VK_4), B4);
      mKeyMap.put(new Integer(KeyEvent.VK_5), B5);
      mKeyMap.put(new Integer(KeyEvent.VK_6), B6);
      mKeyMap.put(new Integer(KeyEvent.VK_7), B7);
      mKeyMap.put(new Integer(KeyEvent.VK_8), B8);
      mKeyMap.put(new Integer(KeyEvent.VK_9), B9);
      mKeyMap.put(new Integer(KeyEvent.VK_0), B0);
      mKeyMap.put(new Integer(KeyEvent.VK_MINUS), Dash);
      mKeyMap.put(new Integer(KeyEvent.VK_EQUALS), Equals);
      mKeyMap.put(new Integer(KeyEvent.VK_BACK_SPACE), BackSpace);

      // Row Three
      mKeyMap.put(new Integer(KeyEvent.VK_TAB), Tab);
      mKeyMap.put(new Integer(KeyEvent.VK_Q), Q);
      mKeyMap.put(new Integer(KeyEvent.VK_W), W);
      mKeyMap.put(new Integer(KeyEvent.VK_E), E);
      mKeyMap.put(new Integer(KeyEvent.VK_R), R);
      mKeyMap.put(new Integer(KeyEvent.VK_T), T);
      mKeyMap.put(new Integer(KeyEvent.VK_Y), Y);
      mKeyMap.put(new Integer(KeyEvent.VK_U), U);
      mKeyMap.put(new Integer(KeyEvent.VK_I), I);
      mKeyMap.put(new Integer(KeyEvent.VK_O), O);
      mKeyMap.put(new Integer(KeyEvent.VK_P), P);
      mKeyMap.put(new Integer(KeyEvent.VK_OPEN_BRACKET), lbracket);
      mKeyMap.put(new Integer(KeyEvent.VK_CLOSE_BRACKET), rbracket);
      mKeyMap.put(new Integer(KeyEvent.VK_BACK_SLASH), bslash);

      // Row Four
      mKeyMap.put(new Integer(KeyEvent.VK_CAPS_LOCK), CapsLock);
      mKeyMap.put(new Integer(KeyEvent.VK_A), A);
      mKeyMap.put(new Integer(KeyEvent.VK_S), S);
      mKeyMap.put(new Integer(KeyEvent.VK_D), D);
      mKeyMap.put(new Integer(KeyEvent.VK_F), F);
      mKeyMap.put(new Integer(KeyEvent.VK_G), G);
      mKeyMap.put(new Integer(KeyEvent.VK_H), H);
      mKeyMap.put(new Integer(KeyEvent.VK_J), J);
      mKeyMap.put(new Integer(KeyEvent.VK_K), K);
      mKeyMap.put(new Integer(KeyEvent.VK_L), L);
      mKeyMap.put(new Integer(KeyEvent.VK_SEMICOLON), semi);
      mKeyMap.put(new Integer(KeyEvent.VK_QUOTE), Quote);
      mKeyMap.put(new Integer(KeyEvent.VK_ENTER), Enter1);

      // Row Five
      mKeyMap.put(new Integer(KeyEvent.VK_Z), Z);
      mKeyMap.put(new Integer(KeyEvent.VK_X), X);
      mKeyMap.put(new Integer(KeyEvent.VK_C), C);
      mKeyMap.put(new Integer(KeyEvent.VK_V), V);
      mKeyMap.put(new Integer(KeyEvent.VK_B), B);
      mKeyMap.put(new Integer(KeyEvent.VK_N), N);
      mKeyMap.put(new Integer(KeyEvent.VK_M), M);
      mKeyMap.put(new Integer(KeyEvent.VK_PERIOD), greater);
      mKeyMap.put(new Integer(KeyEvent.VK_COMMA), less);
      mKeyMap.put(new Integer(KeyEvent.VK_SLASH), forward_slash);
      //mKeyMap.put(new Integer(KeyEvent.VK_SHIFT), Shift1);

      // Row Six
      mKeyMap.put(new Integer(KeyEvent.VK_SPACE), Space);
   }

   private void reset()
   {
      Collection buttons = mKeyMap.values();

      for(Iterator itr = buttons.iterator() ; itr.hasNext() ; )
      {
         JToggleButton button = (JToggleButton)itr.next();
         button.setSelected(false);
      }

      Shift1.setSelected(false);
      Shift2.setSelected(false);
      Control1.setSelected(false);
      Control2.setSelected(false);
      Alt1.setSelected(false);
      Alt2.setSelected(false);
   }
}
