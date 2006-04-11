__all__ = ["TweekFrame"]

from qt import *
import qtui

class TweekFrame:
   def __init__(self):
      self.mFrame = qtui.QWidgetFactory().create("tweekframe.ui")

   def show(self):
      self.mFrame.show()
