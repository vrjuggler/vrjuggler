
package VjGUI;

import java.awt.*;

public class BorderedPanel extends Panel {

  public static final int NO_BORDER = 0;
  public static final int ETCHED_IN = 1;
  public static final int ETCHED_OUT = 2;
  public static final int RAISED = 3;
  public static final int DEPRESSED = 4;

  int kind, thickness;
  int top, bottom, left, right;
  Insets insets;

  public BorderedPanel (int nx, int ny, int nkind, int nthickness) {
    /* x,y - horizontal and vertical insets.
     * kind - 0 - no visible border
     *        1 - etched in
     *        2 - etched down
     *        3 - raised
     *        4 - depressed
     */
    super();
    left = right = nx;
    top = bottom = ny;
    kind = nkind;
    thickness = nthickness;

    Insets i = super.getInsets();
    insets = new Insets(i.top + top, i.left + left, i.bottom + bottom, i.right + right);
    
  }

  public BorderedPanel (int _left, int _right, int _top, int _bottom,
			int nkind, int nthickness) {
    super();
    left = _left;
    right = _right;
    top = _top;
    bottom = _bottom;
    kind = nkind;
    thickness = nthickness;
   Insets i = super.getInsets();
    insets = new Insets(i.top + top, i.left + left, i.bottom + bottom, i.right + right);
    
  }

  public BorderedPanel (int nx, int ny) {
    super();
    top = bottom = ny;
    left = right = nx;
    kind = 0;
    thickness = 1;
  Insets i = super.getInsets();
    insets = new Insets(i.top + top, i.left + left, i.bottom + bottom, i.right + right);
     
 }

  public void setBorderStyle (int i) {
    kind = i;
  }

/*
  public Insets getInsets () {
    Insets i = super.getInsets();
    i.top += top;
    i.bottom += bottom;
    i.left += left;
    i.right += right;
    return i;
  }
*/
public Insets getInsets() {
  return insets;
}

  public void paint (Graphics g) {
    super.paint(g);
    int i;
    g.setColor(Color.lightGray);

    switch (kind) {
    case 0:
      break;
    case 1:
      g.draw3DRect (0,0, getSize().width-1, getSize().height-1, false);
      g.draw3DRect (1,1,getSize().width-3,getSize().height-3, true);
      break;
    case 2:
      g.draw3DRect (0,0, getSize().width-1, getSize().height-1, true);
      g.draw3DRect (1,1,getSize().width-3,getSize().height-3, false);
      break;
    case 3:
      for (i = 0; i < thickness; i++)
	g.draw3DRect (i,i,getSize().width-(2*i+1),
		      getSize().height-(2*i+1), true);
      break;
    case 4:
      for (i = 0; i < thickness; i++)
	g.draw3DRect (i,i,getSize().width-(2*i+1),
		      getSize().height-(2*i+1), false);
      break;
    }
  }

}


